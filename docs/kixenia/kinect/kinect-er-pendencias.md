# Kinect, engenharia reversa: hipóteses descartadas, pendências pesquisadas e o que falta decifrar

Parte de [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 10, 16g, 17, 18).

## 10. Hipóteses testadas e descartadas

- **Horário do sensor nos quadros:** o runtime usa o próprio relógio (mftb). O intervalo medido estava certo, perto de 33 ms.
- **Perda de pacotes USB:** medi 30 fps praticamente sem perdas.
- **Formato das amostras do microfone:** little-endian não ajudou; o certo é big-endian, e o problema era o nível (ganho 8).
- **Sinal do eixo z do acelerômetro no chão do Nike:** invertê-lo não mudou nada. A causa era a falta da calibração.
- **Ao vivo pior que a gravação:** a gravação e o ao vivo da mesma sessão dão o mesmo resultado (90,2% e 91,6%). A diferença vinha dos movimentos.
- **Cabeça rastreada em poucos quadros:** é enquadramento. A cabeça ficava cortada no topo da imagem, e a inclinação automática resolve isso.
- **Trechos longos sem esqueleto na gravação:** é a sala vazia na gravação, medido na profundidade com `presence.py`.

## 16g. Pendências pesquisadas com o REA (2026-10-25)

Pesquisa no `xam_17559.exe` com o REA (Ghidra 12.1.4: `search_bytes`, `xrefs`, `search_strings`, `resolve_containing_procedure`, `procedure_pseudo_code`), completada pelo projeto Ghidra com o fluxo corrigido quando o REA não reconhece a função.

### Gesto do guia: as janelas de ângulo (`FUN_81809AF0`, decifrado)
Os ângulos estão em graus e são convertidos por 0x8160AF70 (π/180); ângulo negativo ganha 2π (0x815FC1F8). Para cada braço, o xam mede dois ângulos com `atan2` no referencial do corpo e a dobra do cotovelo com `FUN_81809820(esqueleto, ombro, cotovelo, mão)`.

| Teste | Braço esquerdo (o do gesto) | Braço direito |
|---|---|---|
| plano frontal | **205° a 245°** (225° ± 20° = 45° para fora, a partir do braço caído) | **160° a 200°** (caído ao lado do corpo) |
| plano lateral | **250° a 290°** (apontando para baixo, sem ir para a frente) | 0° a 360° (qualquer) |
| dobra do cotovelo | **< 35°** (braço reto) | < 110° |
| tronco (`acos`, 0x81AD21A8) | **< 9°** | |

- Constantes em `FUN_81809558`: esquerdo 0x81AD21D8 = 35, 0x81AD21DC = 250, 0x81AD21E0 = 290, 0x81AD21E4 = 205, 0x81AD21E8 = 245; direito 0x81AD21AC = 110, 0x81AD21B0 = 0, 0x81AD21B4 = 360, 0x81AD21B8 = 160, 0x81AD21BC = 200.
- Com o flag 4 ou 8 do quadro (`quadro+0xAB8`): esquerdo 205° a 275° / 245° a 295°, dobra < 40°; o braço direito e o tronco deixam de ser exigidos. Sem a mão direita (articulação 11) ou o ombro direito (8) rastreados, o braço direito é dispensado.
- Os flags 0x81AD21C0 e 0x81AD21EC (ambos 1) ligam a exigência de cada braço.
- **No Xenia (277a3de5b):** todas as regras implementadas, num referencial do corpo (para cima = normal do chão, para fora = linha dos ombros, para a frente = produto vetorial). Mão e ombros suavizados; cotovelos sem suavização, como no xam.
- **Testado:** teste isolado com um esqueleto montado (9 casos: 45° e 30° passam; 70°, cotovelo dobrado 50°, braço direito aberto 40°, tronco 12° para a frente ou para trás e braços caídos falham; a inclinação lateral não conta).
- **Na prática:** numa gravação real em pé, o tronco medido fica entre 8° e 10°, na divisa do limite, e o braço direito falhou em 49 de 59 amostras de movimento normal. O gesto exige **corpo bem reto** e **braço direito solto e reto**. A pose sintética (`kinect_sim_guia`) deixou de disparar, porque no quadro de base o braço direito está dobrado (102°) e o tronco inclinado (14°).

### Referencial do corpo do aceno (`FUN_81808DE0`, decifrado)
Normaliza o vetor "para cima" do quadro (a normal do chão), faz o produto vetorial com a constante em 0x816507B0 (o eixo Y) e calcula o ângulo com `acos` (`FUN_8172FDA0`). Com isso monta uma rotação (`FUN_816FA6F0`) que **endireita a inclinação do sensor**. Se o produto vetorial é zero, usa a identidade. O detector do Xenia projeta na normal do chão, o que é equivalente.

### 0x2B00E
O padrão `ori rX,rY,0xB00E` não existe no xam (busca completa: 5,7 MB, todos os blocos). **O xam nunca envia a 0x2B00E**: quem envia é um título ou um aplicativo do sistema.

### Identidade
- O objeto do serviço (0x81AD23F0 → 0x81AD23F8) é usado por `FUN_8180DB10`, que o despachante das 0x2B0xx chama **em cada quadro de esqueleto** (0x2B002). Ela chama o primeiro método da vtable do serviço com o quadro (`FUN_8180E3E0`).
- Mensagem nova: **0x2C00A** (`FUN_8180DEB0`) pergunta ao callback de identidade e devolve um valor de 4 bytes, ou 0 em caso de erro.
- O despachante das 0x2C0xx continua não localizado: o xam não compara as constantes.

### Parâmetros de identidade por nome (op 2 da 0x2C00C, `FUN_8180DB88`)
- O nome precisa começar com "Nui" (0x8160DBC4) e ter menos de 0x14 caracteres. O valor é procurado num armazenamento de configurações (`FUN_8169AD08`; armazém 0x1A em `FUN_81679460`/`FUN_81679438`). Se não achar, usa a tabela de padrões em 0x8160DB98 (5 entradas).

| Nome | Padrão do xam |
|---|---|
| `NuiV1FaceLoRes` e `NuiV1UpFaceLoRes` | `0.0343,0.0397,0.042,0.039,0.037,0.001` |
| `NuiV1FaceHiRes` e `NuiV1UpFaceHiRes` | `0.038,0.0397,0.042,0.039,0.038,0.001` |
| `NuiIdCfgVer` | `2` |

- O Nike e o Kinect Sports pedem **outros nomes**: `NuiV1QualDist`, `QualPose`, `QualClip`, `FaceSample`, `SkFilter`, `ImgFilter`, `LightFilter`, `Outliers`, `GuidePanCfg` e `TitlePanCfg`. Nenhum está na tabela; no console viriam do armazenamento (ajustes baixados). A resposta vazia do Xenia equivale ao console sem esses ajustes, e os jogos usam os próprios padrões.

### Ordinal 2174 (`ControlpackNuiCursorSetTrackingId` na tabela do Xenia)
- 0x816DE4C8 é uma função de verdade (prólogo `mflr r12`) que o Ghidra não tinha definido. O REA confirma: o endereço não pertence a nenhuma função analisada.
- Decompilada com a função criada: aloca 0x32000 bytes, lê os **dados biométricos do perfil** do armazém 0x18 (`FUN_816793F0`), confere até 0x66 registros e compara a data de cada um com a data atual ajustada em meses. Responde se o cadastro está velho.
- **O nome da tabela do Xenia para esse ordinal parece errado**: nada nela trata de cursor nem de ID de rastreamento.

### Por que o esqueleto some ao vivo (diagnóstico de 2026-10-25)
- USB: 6.600 quadros de profundidade e 6.600 de cor completos, **nenhum descartado**. Desempenho: o ritmo de quadros de esqueleto por quadro de vídeo é igual ao vivo e com gravação.
- **Prova:** a gravação `kinect_rec_guia2`, feita ao vivo, reproduzida no mesmo jogo, deu **o mesmo rastreamento** da sessão ao vivo: 12% contra 12%, nos mesmos trechos.
- Nos trechos sem esqueleto, a imagem de profundidade **não tem ninguém** na área vista pelo sensor (só um móvel à esquerda, uma cadeira e o chão). Imagem: `~/Kixenia/kinect_visao_sensor.png`.
- O rastreador não pisca: ou acompanha por muito tempo, ou fica sem candidatos. Esqueletos a 0,3–0,6 m aparecem por instantes quando a pessoa sai.
- Mão (176 quadros rastreados): salto por quadro com mediana de 1,4 cm (esquerda) e 4,5 cm (direita, acenando), p90 de 11 a 15 cm; mão "estimada" em 13% a 21% dos quadros.

## 17. Notas complementares (2026-10-24)

- [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md): o pacote de firmware completo (Cambridge, Altair, Detroit, PMC), o protocolo CEMD do firmware (7 mensagens), a calibração da matriz `MicArrayCalibrate.mec` e o `KinectAudio10` (feixes, FFT, pesos).
- [Kinect-SDK-Windows](kinect-sdk-windows.md): exports e enums com valores do SDK 1.8, cruzados com o Xbox.
- [XAM-Fitness](../xam/xam-fitness.md): as 22 funções `XamFitness*`.
- **`kinectcamera_x64.sys`** é uma ponte fina: quase sem strings, só a transferência isócrona (`KINECTCAMERA_ISOC_TRANSFER`) e alguns IOCTLs `0x22xxxx`. O protocolo de comandos da câmera fica no `Kinect10_x64.dll` e não foi localizado lá; o libfreenect já o documenta.

## 18. O que ainda não foi decifrado

- **PsCam:** os valores reais do código 46 (0x2E: exposição já entendida, seção 16e; intrínsecas, seletor 0x100) e o significado do código 33 (+0x1C = 1); o campo +0x24 do código 5; o conteúdo do bloco de 0x100C bytes do código 14, que os jogos não leem.
- **Firmware:** o dispositivo atrás do I2C (0x0B/0x0C) e da interface 0x258 a 0x261; o conteúdo dos 4 blocos de 102.400 bytes da calibração (CEMD 0) e os campos 3 em diante da máscara do CEMD 3; o bloco de 0x44 bytes do motor (0x8037/0x8038); a confirmação de que os modos 1 e 2 do motor são posição e tempo; os firmwares Detroit (8051, que o REA não abre), Cambridge (`NuiCam.bin`), `2bl.bin` e `pmc.bin`.
- **Microfones:** por que o canal 1 (processado) para logo depois de começar. Testado no sensor em 2026-10-24: não é vazão, nem falta de sinal de alto-falante, nem as CEMD 1, 2, 5 ou 6 (ver [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md), seção 2d). Resta a falta da calibração (CEMD 0), que precisa de um `MicArrayCalibrate.data`.
- **Voz:** o que `XVoicedGetDirectionalData` (kernel 0x327) devolve, porque o xam só repassa.
- **Câmera:** a propriedade 4 e o valor medido; os registradores dos ganhos R/G/B. A escrita pelo comando 0x95 nunca foi testada no sensor.
- **Kinect ID:** o motor facial dentro do jogo; de onde o xam lê os parâmetros por nome (op 2 da 0x2C00C); o serviço do xam que atende a 0x2C00D com cadastros reais.
- **PsCam 20/40/44:** o que o sensor responde à propriedade 0x838 (ventilador), ao código 40 (mensagem das flags de qualidade) e ao 44 (calibração carregada). Ver seção 16d.
- **Gestos:** decifrados na seção 16g (janelas do guia e referencial do aceno).
- **Aplicativos do sistema:** nunca foram decompilados, inclusive o comportamento de `XamShowNuiGuideUI` (o xam chama `XamAppRequestLoadEx(4)`).
