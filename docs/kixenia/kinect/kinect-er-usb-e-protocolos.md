# Kinect, engenharia reversa: uSB do 1473, protocolo do PsCam, comandos de firmware e o motor do 1414

Parte de [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 3, 7, 11, 14).

## 3. USB do Kinect para Xbox 360 (modelo 1473)

| Dispositivo | ID | O que é |
|---|---|---|
| Câmera | 045e:02ae | profundidade e cor (protocolo do libfreenect / PR #1274) |
| Áudio | 045e:02ad | bootloader; com o firmware: microfones, motor, LED, acelerômetro |
| Motor (só modelo 1414) | 045e:02b0 | dispositivo separado, não implementado |

### Firmware de áudio `audios.bin`
Notas completas em `~/Kixenia/kinect-sdk-re/audios-firmware-notes.md`.

- **Origem e identificação:** `$SystemUpdate/FFFE07DF00000001`, 474.624 bytes, versão 1.2.810.0, SHA-1 `272b02b5…`.
- **Código:** ARM little-endian, carregado em 0x80000, entrada em 0x80030. O projeto se chama `Altair\Audios` e tem uma pilha USB própria.
- **Cabeçalho:** magic 0xCA77F00D, versão, base, tamanho 0x73E00, entrada.
- **Bootloader:** o comando 3 grava o firmware em páginas de 16 KB e o comando 4 o executa. Ele se perde quando falta energia.
- **Comandos** (EP 0x01): `{0x06022009, tag, arg1, cmd, arg2…}`. A resposta (EP 0x81) é `{0x0A6FE000, tag, status}`. O dispatcher fica em 0xAE780 e há 48 comandos registrados em 0xAEFA0.

| Comando | O que faz |
|---|---|
| 0x10 | LED: 1 apagado, 2 verde piscando, 3 verde, 4 vermelho |
| 0x8032 | estado em 104 bytes: words 4 a 6 = acelerômetro x, y, z (cerca de 819 por g); word 7 = inclinação; word 1 = flags (bits 0 e 1 = em movimento) |
| 0x803B | inclinar para um ângulo, limitado a ±30 depois da calibração |
| 0x803A | parar ou reiniciar o motor; falha com o motor em movimento, por isso para-se mirando o ângulo atual |
| 0x8033 / 0x8034 / 0x8035 | calibração, movimento em ±60 e modo 2 (não testados) |
| 0x8037 / 0x8038 | recebe ou envia um bloco de 68 bytes (parâmetros do motor?) |
| 0x133 / 0x134 | carga de dados de áudio (CEMD) |
| 0x0B–0x0F, 0x258–0x261 | desconhecidos, provavelmente processamento de áudio |

### Motor: máquina de estados (firmware, analisado em 2026-10-24)

Cada comando grava um **modo** (um byte no estado do motor, ponteiro em `DAT_B8188`/`DAT_B8204`), o alvo e o flag de execução, com as interrupções desligadas (`FUN_F2150`).

| Comando | Tratador | Modo | Parâmetros e comportamento |
|---|---|---|---|
| 0x803B | `FUN_B8120` | 5 | alvo = arg2 (já limitado a ±30), subestado 4; só rearma se o modo ou o alvo mudarem. É o ângulo em relação ao horizonte, pelo acelerômetro |
| 0x8034 | `FUN_B81A0` | 1 | alvo = arg2 (±60), mesma estrutura. Provavelmente a posição do motor em contagens |
| 0x8035 | `FUN_B82E0` | 2 | 20 e arg2. Provavelmente movimento por tempo ou velocidade |
| 0x8033 | `FUN_B8280` | 3 | 20 e 900, subestado 7. É a varredura de calibração até os limites; depois dela, os limites do 0x803B passam a ±30 (`0xB7E04`) |
| 0x803A | `FUN_B7E40` | — | só limpa o flag de execução se `FUN_B7D20()` indicar motor parado; senão falha. Por isso o Xenia para o motor mirando o ângulo atual |
| 0x8037 | `FUN_B8920` | — | recebe exatamente 0x44 bytes (senão status 3), e o callback `DAT_B89A0` aplica: gravar o bloco de parâmetros do motor |
| 0x8038 | `FUN_B89C0` | — | aloca e preenche 0x44 bytes (`FUN_B6C00`) e devolve; falha com 0x80 (+30000): ler o bloco de parâmetros ou calibração |

### Microfones
- **Entrada (isócrono IN, EP 0x82):** pacotes de 524 bytes, com cabeçalho de 12 bytes `{0x80000080, canal, tamanho, janela}` e 512 bytes de dados.
  - O canal 1 é o sinal processado de 16 bits, que quase nunca chega. Por isso a média dos microfones o substitui.
  - Os canais 2 a 9 são as metades dos 4 microfones, em int32.
  - A janela avança 3 a cada 16 ms.
- **Saída (OUT, EP 0x02):** 76 bytes com o cabeçalho de sequência do libfreenect.
- **Nível:** as amostras USB chegam cerca de 8 vezes mais baixas do que os jogos esperam, daí o ganho 8.

## 7. Protocolo do PsCamDeviceRequest (xboxkrnl 0x30D)

O jogo fala com a câmera por `PsCamDeviceRequest(pedido)`. Cada pedido tem um cabeçalho de 0x18 bytes:
- +0: código;
- +0x10: contexto;
- +0x14: rotina de conclusão, chamada com (pedido, status, bytes). O valor 0 ou 0xFFFFFFFF significa nenhuma.

Os códigos 14 e 25 escrevem a resposta a partir de +0x08.

Ordem dos códigos no Kinect Sports (igual à do jogo 5345084D do knuckleslee): 0, 27, 35, 24, 33, 13, 14, 25, 5, 36.

| Código | O que é | Como o Xenia responde |
|---|---|---|
| 0 | estado do dispositivo | 0 ausente, 2 transmitindo, 4 pronto |
| 2 | parar a transmissão | completa as leituras pendentes |
| 5 | ler um quadro: `{cabeçalho, stream (2 cor, 4 profundidade), buffer, tamanho, ?, ponteiro do contador de quadros, geração das configurações (só cor)}` (0x30 bytes) | ver abaixo |
| 13 | versão do driver: o jogo usa o registro novo se a versão maior for ≥ 5 e a menor ≠ 0 (`82F4BE38`) | 5.1 |
| 14 | registro (calibração cor ↔ profundidade), 0x10B0 bytes. O Nike (`82F4B840`) só lê os 29 words e os valores do fim; o bloco de 0x100C bytes em +0x7C e o word +0x04 nunca são lidos | os 29 words de registro mais `reference_distance`, `reference_pixel_size`, `frame_width`, `shift_to_depth_pixel_const`, `shift_to_depth_const_offset` e `dcmos_rcmos_distance`, como no `freenect_init_depth_to_rgb` |
| 15 | sincronização de horário | sucesso; o código 25 desliga com 0 |
| 24 | tabela de profundidade: `{cabeçalho com +0x04 = 1, ponteiro, 0x2000 entradas}`. Se falhar, o Nike monta uma tabela identidade (raw = raw) | 8192 entradas, de raw de 11 bits para mm |
| 25 | período dos quadros (µs) | 33333 para cor e profundidade |
| 27 | **iniciar a transmissão**: só é enviado com o estado 4 (pronto). +0x1C = bit 0 sempre ligado, bit 1 = cor em alta qualidade (flag 0x40 da inicialização NUI) (`82F4BF78`) | sucesso |
| 33 | enviado logo depois da tabela de profundidade, com +0x1C = 1; o significado não é conhecido | sucesso |
| 35 e 36 | ajustes de câmera | [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md) |

Sobre a leitura de quadros (código 5):
- **Profundidade:** o jogo aceita **0x96000** bytes (640×480×2, desempacotada) ou **0x67200** bytes (11 bits empacotados). O raw 2047 significa "sem profundidade".
- **Cor:** Bayer 640×480, convertida pelo próprio jogo.
- O ponteiro em +0x28 do pedido (descrição +72) aponta para a **informação do quadro**, de 0x10 bytes: sequência (+0), desconhecido (+4) e **horário em milissegundos, 64 bits** (+8). A biblioteca NUI copia esse horário para os quadros de imagem e de esqueleto. Com horário 0, o Nike não alimenta o menu do Kinect (ver [Nike-Entradas](jogos/nike-entradas.md), seção 4). O Xenia escreve o tempo de atividade do console convidado, em ms. A conclusão do Kinect Sports foi lida em `default.xex` @0x82A05E78.

Sequência no Nike quando o estado é 4: código 27, depois 35 (`82F51310`), depois 24, 33, 13 e 14 (`82F4BE38`), e por fim 25.

## 11. Firmware: comandos que faltavam (analisados em 2026-10-24)

Para analisar, extraí o `audios.bin` do pacote `FFFE07DF00000001`:
- é um PIRS, e a entrada `audios.bin` começa no bloco 49 (0x3D000), com 116 blocos contíguos;
- o SHA-1 confere;
- abri no REA como ARM cru em 0x80000.

| Comando | O que faz |
|---|---|
| 0x0B | **transação I2C**: recebe até 256 bytes. O callback `0xBA658` transforma cada byte num word com 0x100 (início) no primeiro e 0x200 (fim) no último, mais 0x400 (leitura) se o bit 0 do primeiro byte (o bit R/W do endereço I2C) estiver ligado. Executa e espera o status 3; falha com 0x75AF |
| 0x0C | devolve 0x104 bytes: tamanho e dados lidos na transação |
| 0x0D | devolve 256 bytes: lista de registros de 16 e 12 bytes (`FUN_bad64`, enumeração de algo) |
| 0x0E | recebe até 256 bytes e os aplica (`FUN_baeac`); erro + 30000 se > 8 |
| 0x0F | recebe até 0xF4 bytes |
| 0x258 a 0x261 | interface de **acesso a um periférico**, só registrada se `FUN_b5e80()` confirmar o recurso; aloca dois buffers de 0x200. Todos repetem até 20 vezes, esperando de 10 a 105 ms: 0x258 grava até 0x1000 bytes num endereço de 16 bits; 0x25C escreve (endereço, valor); 0x25D lê um bloco de 36 bytes, guardado em cache; 0x25E aplica um parâmetro; 0x25F envia até 8 KB; 0x261 faz uma operação com (u16, u8, u8). Provavelmente uma memória de configuração ou fábrica |

Nenhum jogo usa esses comandos.

## 14. Motor do Kinect modelo 1414 (045e:02b0, do libfreenect `src/tilt.c`)

Transferências de controle de fabricante:

| Pedido | O que faz |
|---|---|
| `0x40, 0x31, wValue = graus × 2` | inclinar (±31°) |
| `0x40, 0x06, wValue = LED` | LED: 0 apagado, 1 verde, 2 vermelho, 3 amarelo, 4 verde piscando, 6 vermelho/amarelo piscando |
| `0xC0, 0x32`, 10 bytes | x, y, z em big-endian int16 nos bytes 2 a 7; inclinação (int8, em meios graus) no byte 8; estado no byte 9 (0 parado, 1 no limite, 4 em movimento) |

Os LEDs são numerados diferente do 1473.
