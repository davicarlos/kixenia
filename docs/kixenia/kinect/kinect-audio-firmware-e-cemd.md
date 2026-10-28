# Áudio do Kinect: pacote de firmware, protocolo CEMD e calibração da matriz

Relacionado: [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 3 e 11), [Kinect-Xenia-Edge](kinect-xenia-edge.md).

## 1. O pacote de firmware `FFFE07DF00000001`

É um PIRS (STFS). Para ler um bloco de dados `n`:

```
deslocamento = 0xC000 + (n + n/170 + (n >= 170 ? 1 : 0)) × 0x1000
```

A partir do bloco 170 entra também a tabela de hash de nível 1. Sem essa correção, os arquivos depois do bloco 170 saem trocados (PNGs no lugar do firmware).

| Arquivo | Tamanho | Início | O que é |
|---|---|---|---|
| `2bl.bin` | 196.608 | 0xCAAC0BF0, v2.1.810, base 0x10000, tamanho 0x13E88, entrada 0x10030 | segundo estágio do carregador de áudio (mesmo formato de cabeçalho do audios.bin) |
| `audios.bin` | 474.624 | 0xCA77F00D, v2.1.810, base 0x80000, entrada 0x80030 | **firmware de áudio "Altair"** (ARM): motor, LED, acelerômetro, microfones |
| `detroit.bin` | 12.426 | `6A 12 12 34 04 5E 02 AF …` | firmware **"Detroit"** (hub e motor), provavelmente 8051 (muitos `12 xx xx` = LCALL); contém `045E 02AF` |
| `NuiCam.bin` | 128.640 | `00000002 00000008 … 01020810` | firmware da câmera **"Cambridge"** (PrimeSense) |
| `NuiCam2.bin` | 145.284 | `00000002 00000009 … 01020810` | outra revisão da câmera |
| `pmc.bin` | 4.393 | `BEBA01C0 …` | "pmc" (provavelmente controle de energia) |
| `fwversions.txt` | 89 | | `cambridge=01.02.0810.00`, `altair=01.02.0810.00`, `detroit=01.02.0810.00`, `pmc=01.02.0810.00` |

O que as strings mostram de cada um:
- **`2bl.bin`:** carregador do Altair com a própria pilha USB (`AudiosFakeMDD`, `Altair\Audios\private\usb\stack\pipe.cpp`, `enum.cpp`, `ufnmdd.cpp`, `descript.cpp`) e as mensagens `UsbCommandHandler_SendData/ReceiveData` e "Boot status: %d".
- **`detroit.bin`:** descritores USB "Microsoft" e uma tabela crescente (provavelmente de gama ou de passos), o que confirma o controlador USB 8051 do motor e do hub. O REA não abre 8051.
- **`NuiCam.bin`, `NuiCam2.bin` e `pmc.bin`:** sem texto legível; provavelmente comprimidos ou criptografados.

Os demais arquivos (`crown*.png`, `milestone*.png`, `xbox360*.png/jpeg`) são imagens.

**Nomes internos dos componentes do Kinect:** Cambridge (câmera), Altair (áudio), Detroit (hub/motor; daí o `DetroitDeviceRequest` do kernel) e PMC.

Os arquivos extraídos ficam em `fw/pkg/` no scratchpad da sessão.

## 2. Protocolo CEMD no firmware Altair

Um pacote CEMD é `{u32 'CEMD' (0x444D4543 little-endian), u32 0, u32 id, carga…}`.
- O despachante é `FUN_84E80`: exige tamanho ≥ 0xC, a assinatura e o word 1 igual a 0, com id de 0 a 6.
- A tabela de saltos fica em `0x85808 + (id − 7) × 4`, com deslocamentos relativos a 0x84F2C.

| id | Carga | Tratador | O que faz |
|---|---|---|---|
| 0 | exatamente **0x64008 bytes** | 0x84F30 | carrega a **calibração da matriz de microfones**. O word 0 é um tamanho limitado a **14.400** (0,9 s a 16 kHz, provavelmente a cauda do eco), gravado em +0x44 e +0x48 do estado. Seguem **4 blocos de 102.400 bytes** (4 × 0x19000 = 0x64000), provavelmente um por microfone, copiados em partes de 25.600 bytes. O estado de processamento (0x500 bytes) é zerado |
| 1 | 0 | 0x85278 | liga: grava 1 em +0x50 do estado |
| 2 | 0 | 0x85284 | desliga: grava 0 em +0x50 |
| 3 | 44 bytes | 0x85290 | configuração por **máscara de campos** no word 0: com o bit 1, grava o word em +0x14; com o bit 2, valida o float em +0x18 contra mínimo e máximo (como o feixe); os demais bits têm campos seguintes |
| 4 | 0 | 0x85644 | reinicia o processamento (zera 0x500 bytes). O laço de áudio (`FUN_808C0`) envia a si mesmo `{CEMD, 0, 4}` |
| 5 | 4 bytes | 0x85580 | **ângulo do feixe**: compara com um mínimo e um máximo (`FUN_42240` / `FUN_42140`, comparações de float) e grava o valor limitado |
| 6 | 0 | 0x85604 | zera o estado de localização e do feixe |

- **O laço de áudio** (`FUN_808C0`) aloca 4 buffers de 0x400 bytes, um por microfone. O processamento interno (`FUN_858C0`) roda sempre, porque `FUN_82E20` devolve 1.
- **A carga grande** chega pelos comandos de transporte 0x133 (início, com o tamanho 0x64014 = 0xC de cabeçalho CEMD + 0x64008), 0x134 (blocos de 0x4000 com o endereço) e 0x135 (fim, 0x64000). Assim faz o libfreenect em `upload_cemd_data`, com o arquivo `cemd_data.bin`.

## 2b. Pacotes dos microfones no firmware

- **Montagem e envio** (`FUN_81220(canal, ?, buffer, amostras)`): grava canal, janela e tamanho no cabeçalho e chama `FUN_81100`, que põe a assinatura **0x80000080** (montada com `mov #0x80` / `orr #0x80000000` em 0x81128) e envia, se a transmissão estiver ligada.
- **Pool:** 30 buffers de 532 bytes (12 de cabeçalho + 512 de dados + 8 de lista), criados em `FUN_811A0`. `FUN_81320` tira um buffer livre; sem buffer, o pacote é descartado.
- **Microfones crus** (`FUN_80740`): para cada um dos 4, dois pacotes de 0x200 bytes (0x80 amostras de 32 bits) nos canais 2k e 2k+1, ou seja, canais **2 a 9**.
- **Saída processada** (`FUN_80300`): um pacote no **canal 1** com 0x100 amostras de 16 bits.
- **Ordem no laço principal** (`FUN_808C0`): primeiro os 8 crus, depois o processamento (`FUN_858C0`, ou `FUN_80474` quando os bits 0x22 do estado estão ligados) e por último o processado.
- **Aquecimento:** `0xF3E64` (o word antes do estado `0xF3E68`) é um contador que o laço liga ao começar e decrementa a cada quadro. Enquanto ele não é zero, nem cru nem processado são enviados. Não é um "mute" do canal processado.
- **Hipótese (não confirmada) para o canal 1 quase nunca chegar no Xenia:** o processado é sempre o último dos 9 pacotes de cada quadro. Se o host não drena o endpoint isócrono rápido o bastante, o pool de 30 esgota, e o último pacote é o que cai. A outra hipótese é a falta da calibração (CEMD 0).

## 2c. Como o CEMD chega ao firmware

O CEMD não tem um comando próprio. Vai pelo **carregador de mensagens** do firmware:

| Comando | Função | O que faz |
|---|---|---|
| 0x133 (arg2 = tamanho) | `FUN_81F20` | aloca uma vez um buffer de 0x64400 bytes e guarda o tamanho; falha se o tamanho passar da capacidade |
| 0x134 (arg1 = bytes, arg2 = deslocamento, depois os dados) | `FUN_82000` | copia o bloco no deslocamento |
| 0x135 | `FUN_81FE0` | marca a mensagem como pronta |

O laço principal (`FUN_80820`) tira a mensagem (`FUN_81FA0`) e, se os bits 0x1, 0x2 e 0x20 do estado `0xF3E68` estiverem desligados, a despacha em `FUN_84E80`. Um ângulo de feixe, então, fica assim: 0x133(16) → 0x134 com 16 bytes `{'CEMD', 0, 5, ângulo}` → 0x135.

## 2d. Testes no sensor (2026-10-24, Kinect 1473 com firmware 1.2.810)

Os testes foram feitos com a `xenia-kinect-camera-test` e um patch local (não commitado, `kinect_cemd_tests.patch` no scratchpad), contando os pacotes por canal a cada segundo.

| Teste | Canal 1 (processado) | Canais 2 a 9 (crus) |
|---|---|---|
| Configuração do Xenia (8 transferências × 16 pacotes) | **11 no primeiro segundo, depois 0** | 64/s cada (uma janela a cada 16 ms), 0 lacunas em 7,6 s |
| Fila 4 vezes maior (32 × 32) | 5, depois 0 | 64/s |
| Tom no endpoint de saída (referência de eco não silenciosa) | 5, depois 0 | 64/s |
| CEMD 6, 5 (feixe 0 rad), 1 e 2 antes de capturar | 11, depois 0, em todos | 64/s |

Conclusões:
- **O transporte de CEMD funciona.** 0x133, 0x134 e 0x135 respondem `{0x0A6FE000, tag, 0}`, e o firmware aceita as mensagens 1, 2, 5 e 6.
- **Hipótese do pool descartada:** os canais crus chegam completos, e uma fila maior não muda nada.
- **Hipótese do sinal de alto-falante descartada.**
- **O firmware para de produzir o canal 1 logo depois de começar**, mesmo com CEMD 1, 2, 5 e 6. Fica a hipótese da **calibração** (CEMD 0): sem ela, o processamento não se mantém. Os poucos pacotes iniciais podem ser restos de uma sessão anterior. Só dá para testar com um `MicArrayCalibrate.data`.

**Leitura do bloco do motor (0x8038, 68 bytes, status 0)**, em words little-endian:

```
0: 0xC13A0000 (float −11,625)   1: 0x00020009   2: 1207   3: 0   4: 0x44 (o próprio tamanho)   5: 0
6: 0x6B1217E8   7 a 11: 0   12: 2076   13: 3681   14: 1203   15: 0xFFFFFFFF   16: 0xFFFFFFFF
```

Os significados não são conhecidos. 1207, 2076, 3681 e 1203 parecem contagens do motor (posição e limites?).

## 3. Quem envia CEMD no Xbox (xam 17559)

O envio genérico é `FUN_81983E98(pacote, tamanho)`: um pedido de dispositivo com código 0xE (`FUN_81A73164`, provavelmente `McaDeviceRequest`), que espera a conclusão.

Os pedidos de dispositivo do xam saem por thunks do kernel:

| Thunk | Kernel (ordinal) |
|---|---|
| `0x81A73164` | **McaDeviceRequest** (0x30E): o CEMD vai por ele, com o código 0xE |
| `0x81A73134` | PsCamDeviceRequest (0x30D) |
| `0x81A73654` | XAudioGetSpeakerConfig (0x1FF): o bit 31 escolhe a calibração de baixa latência |
| `0x81A721C4` | KeGetCurrentProcessType (0x66) |

| Função do xam | Pacote | Quando |
|---|---|---|
| `XamVoiceSetMicArrayBeamAngle(float)` (0x816FBD60) | `{CEMD, 0x05000000 (id 5 em big-endian), ângulo como float little-endian}`, 0x10 bytes | ângulo **em radianos** entre **−0,873 e +0,873 (±50°)** (0x815FC248 / 0x815FC244) ou o valor especial **FLT_MAX** (0x815FC24C, provavelmente feixe automático); senão `0x80070057` |
| `FUN_816FC410` | `{CEMD, 0, 0x06000000}`, 0xC bytes (id 6) | depois de reiniciar os 4 usuários e marcar todos como ociosos (`DAT_81A85C84 = 0xF`) |
| `XamNatalDeviceAudioCalibrate` (0x817F8DE8) | calibração inteira, via `FUN_81983268` (início), `FUN_819832F0` (blocos de 0x4000) e `FUN_81983380` (fim) | ver abaixo |

### Calibração de áudio da matriz

`XamNatalDeviceAudioCalibrate`:
1. Monta o pacote de conteúdo do sistema **`MicArrayCalibrate.mec`** (título 0xFFFE07DF, tipo 0x40000) como `MicArrayCalibrate:`.
2. Escolhe o arquivo pelo bit 31 de um valor do kernel (`0x81A73654`):
   - `MicArrayCalibrate:\MicArrayCalibrate.data`
   - `MicArrayCalibrate:\MicArrayCalibrateLowLatency.data`
3. Envia o arquivo ao firmware em blocos de 0x4000.

É a **calibração de áudio que o console gera** quando se faz o teste de áudio no Kinect Tuner, com tons pelos alto-falantes. É o mesmo conteúdo que o libfreenect chama de `cemd_data.bin`.

## 4. KinectAudio10_x64.dll (Windows): cancelamento de eco e matriz

É o DMO **"KinectAEC"** (`AecDMO.cpp`, `aecapi.c`), que faz no PC o que o Xbox faz no xam e nos jogos. Ele recebe os 4 microfones mais o som dos alto-falantes (loopback).

**Descritores de matriz** (tabela em 0x180153A18): `Lin4el` (0x18006FB18) e `Lin4el_2` (0x18006FD58). São matrizes lineares de 4 elementos, com esta estrutura:

```
{ nome, u32 4 (microfones), u32 0x100 (256 bins), u32 0x0B (11 feixes),
  ptr feixes, ptr frequências, ptr pesos, ptr …}
```

- **Feixes** (0x1800CDDB0): 11 triplas de double {azimute, elevação, distância} = **−50° a +50° em passos de 10°**, elevação 0, distância 1,0 m.
- **Frequências** (0x1800CDEE0): 256 floats de 0 a 7968,75 Hz, em passos de **31,25 Hz**. Isso dá uma FFT de 512 pontos a 16 kHz.
- **Pesos** (0x1800CE2E0, 0x16000 bytes): **11 feixes × 256 bins × 4 microfones × complexo (2 floats)**. São os coeficientes fixos do beamforming.
- **Posição dos microfones:** não é constante no DLL nem no firmware. O DMO a lê do dispositivo (o erro "Invalid MicArray Descriptor"). O valor conhecido do Kinect é x ≈ −113, 36, 76 e 113 mm, mas isso não foi confirmado nesta análise.
- **Parâmetros do cancelamento de eco** (nomes de uma tabela com passo de 0x80 a partir de 0x6EE10; os valores padrão não ficam nela): `ClipThreshGrowthStalk/Dtalk`, `MicAvgDecay`, `ResidualAvgDecay`, `SpkAvgDecay`, `SpkEnergyFirstWhiten`, `SpkEnergyWhitenGrowth`, `SpkShortTermMinPeriod`, `SpkLongTermMinDecay`, `SpkNotNoiseFactor`, `DblTalkEchoFactorDecay/Min`, `DblTalkMinStartupEchoFactor`, `DblTalkNoiseFactor`, `SpkPeakDecay`, `InitialSpkToMicGain`, `InitialSpkToLeakageGain`, `CalcdReverbMinNtaps`, `InitialDblTalkEchoFactor`, `FilterUpdateScale`, `MinSpkToMicDelta`, `TrialUpdateScale`, `MinTrialToSafeRatio`, `SpkNotNoiseAtMicFactor`, `MinSpkToLtMinRatio`, `SpkLtMinAttack`.
- **Registro do Windows:** `Software\Microsoft\scrunch\KINAec\{GUID}`.
- **Mensagens:** "MicArray mode set - 0x%04x", "System mode set - %d" e ganho automático do microfone ("Mic gain increased/decreased", `MicGainBounder`).

## 5. No Xenia (commit 7bc21371c, 2026-10-24)

**Implementado:**
- `UsbAudio::SendAudioMessage`: envia qualquer CEMD por 0x133 (tamanho), 0x134 (blocos de 0x4000, enviados em pedaços de 512 bytes, com o deslocamento) e 0x135.
- `XamVoiceSetMicArrayBeamAngle(float)`: aceita de −0,873 a +0,873 rad ou `FLT_MAX`; fora disso devolve `X_E_INVALIDARG`. Envia o CEMD 5 por `NUIController::SetMicrophoneBeamAngle`; sem sensor, `X_E_FAIL`.
- **`kinect_audio_calibration_path`:** caminho de um `MicArrayCalibrate.data`, que deve ter exatamente 0x64014 bytes e começar com 'CEMD'. É enviado como CEMD 0 quando o dispositivo de áudio abre (`SetMotor`) e por `XamNatalDeviceAudioCalibrate`; sem o arquivo, a resposta é `0x8AC80005`, como no xam.
- `xenia-kinect-camera-test --motor_test --beam_angle=<rad>`.

**Testado no sensor:**
- ângulos 0,3, −0,873 e 0: aceitos;
- calibração com arquivo inválido: recusada com "isn't a Kinect microphone array calibration"; o Nike seguiu com motor, LED e microfones normais.

**Não testado:**
- a calibração verdadeira, porque não há um `MicArrayCalibrate.data`;
- o efeito do feixe no áudio, porque o canal processado não chega sem a calibração;
- jogos chamando `XamVoiceSetMicArrayBeamAngle`, porque nem o Nike nem o Kinect Sports a importam.

**Observação:** o número de série USB deste Kinect 1473 vem como `0000000000000000`.

## 6. O que ainda falta para o Xenia

- Testar com uma calibração verdadeira (`MicArrayCalibrate.data` de um console que fez o ajuste de áudio do Kinect) se o canal processado passa a chegar.
- Se passar, usar o canal 1 no lugar da média dos microfones, que hoje substitui o processado.

O atualizador do sistema que grava esses firmwares no sensor está descrito em natalsu.xex (pacote `NATALDEVICEPACKAGE:\`, pedidos Mca/Detroit/PsCam usados) e o teste de microfones em Dash.NuiTroubleshooter.
