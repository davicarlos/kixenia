# O que o Nike+ Kinect Training recebe do Xenia e do Kinect

Levantamento feito em 2026-10-25. Fontes:
- as 391 importações do jogo, com os pontos de chamada no código decompilado (`modulos/decomp/Nike/`);
- o estado de cada função no Xenia, lido do código pelo `modulos/xenia_estado.py`;
- os logs das sessões ao vivo (`kinect-tools/logs/aovivo-*.log`).

Relacionado: [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-ER-USB-e-Protocolos](../kinect-er-usb-e-protocolos.md), [XAM-Fitness](../../xam/xam-fitness.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md).

## 1. Do Kinect (passando pelo Xenia)

### Câmera: `PsCamDeviceRequest`
É a única importação de câmera do jogo. Os pedidos chegam nesta ordem: 0, 27, 35, 24, 33, 13, 14, 25, 5 (leituras contínuas) e 36. O 2 vem ao parar.

| Código | O jogo recebe |
|---|---|
| 0 | estado do sensor (ausente, pronto ou transmitindo) |
| 27 | início da transmissão |
| 13 | versão do driver: 5.1 |
| 14 | registro cor↔profundidade (os 29 words e as constantes do fim) |
| 24 | tabela de 8192 entradas, de raw para mm |
| 25 | período dos quadros: 33.333 µs, ou seja, 30 fps |
| 33 | sucesso (significado desconhecido) |
| 35 / 36 | ajustes da câmera de cor ([Kinect-Ajustes-de-Camera](../kinect-ajustes-de-camera.md)) |
| **5** | **quadros**: profundidade 640×480 em words de 16 bits big-endian (0x96000 bytes, raw de 11 bits; o Xenia entrega 0 no lugar do 2047 "sem dado") e cor Bayer 640×480 (0x4B000 bytes), com o contador de quadros em +72 |

### Microfones: pelo xam
- `XamVoiceGetMicArrayAudioEx`: cerca de 76 mil leituras numa sessão.
- `XamVoiceGetMicArrayUnderrunStatus`: cerca de 229 mil consultas.
- Os 4 microfones crus mais a referência de eco alimentam o MEC e o reconhecedor de voz do próprio jogo.

### Motor e acelerômetro: pelo xam
- `XamNuiCameraTiltGetStatus`: cerca de 63 mil consultas (ângulo, acelerômetro, estado do motor).
- `XamNuiCameraElevationSetAngle`, `StopMovement` e `TiltSetCallback`.
- `TiltReportStatus` (82 vezes).
- `RememberFloor`: o plano do chão vai por valor, cerca de 33 mil vezes.

### Estado do sensor
- `XamNuiGetDeviceStatus`: cerca de 110 mil consultas. Bits lidos: 0x1 sensor, 0x2 microfones, 0x4 e 0x40 erros, 0x8 superaquecimento.
- `XamNuiGetDeviceSerialNumber`.
- `XamNuiGetTrueColorInfo`: tabela de exposição.
- `XamNuiGetFanRate` (ordinal 0x8E1): buscada por ordinal; **o Xenia não implementa**.

## 2. Do sistema (xam e kernel emulados)

### Kinect ID e fitness
- **Kinect ID:**
  - `XamUserNuiGetUserIndex` (vínculo cadastro→usuário; o Xenia responde "ninguém", como num console sem cadastro);
  - `XamUserNuiGetEnrollmentIndex`, `XamUserNuiEnableBiometric`, `XamReadBiometricData`;
  - `XamNuiIdentityGetSessionId`.
- **Fitness:** 9 funções `XamFitness*` resolvidas por ordinal (perfil do corpo, eventos de treino, privacidade). O Xenia as implementa desde 5125708b2.

### Mensagens XMsg
| App | Mensagens | O que é |
|---|---|---|
| 0xFE (xam) | 0x2B002 | o jogo **publica** cada quadro de esqueleto para o HUD do sistema (cerca de 29 mil numa sessão) |
| 0xFE | 0x2B003 | inicialização do NUI (flags 0x40006C0B) |
| 0xFE | 0x2C009, 0x2C00C | callback e mensagens do motor de identidade |
| 0xFE | 0x2A005 a 0x2A008 | pedidos de identidade e dados biométricos |
| 0xFA (XMP) | 0x70009, 0x7001A, 0x7001B | estado do tocador de música (cerca de 10 mil vezes cada) |
| 0xFB | 0xB0006 | serviço da Live |
| 0xFC | 0x58006 | — |

### Restante, por área
Contagem das importações e do estado no Xenia:

| Área | Importações | Estado no Xenia |
|---|---|---|
| Rede e Live | 48 | 41 implementadas, 5 ausentes |
| Threads e sincronização | 46 | 33 implementadas, 7 aproximadas |
| Arquivos e kernel | 46 | 34 implementadas, 8 ausentes |
| Vídeo e GPU | 27 | 15 stubs, 4 ausentes |
| Conteúdo e arquivos salvos | 19 | 11 implementadas, 7 stubs |
| Telas do sistema | 14 | 6 implementadas, 6 ausentes (amigos, mensagens, convites) |
| Perfil e usuário | 14 | 10 implementadas |
| Áudio (XAudio) | 14 | 5 ausentes (*ducker*) |
| Entrada (controle) | 4 | implementadas |
| Câmera Live Vision (`XUsbcam*`) | 6 | não usada pelo Kinect |
| Fones (`Mic`/`RmcDeviceRequest`) | 5 | só com fone conectado |

## 3. Dos arquivos do próprio jogo

| Arquivo | Para quê |
|---|---|
| `Database.xmplr` (12,9 MB; 22,6 MB descomprimido) | o modelo do rastreador de esqueleto na GPU |
| `HeadPosition.xmplr` (440 KB) | posição da cabeça (Kinect ID) |
| `NuiIdentity.bin.be` (3,2 MB) e `NuiHeadOrientation.bin.be` (2,6 MB) | modelos do reconhecimento facial |
| `nuisp1031` … `nuisp3084` (0,9 a 1,5 MB cada) | modelos de voz por idioma |
| `Game/Scripts.pak` | os scripts Lua, entre eles o menu do Kinect (`UI/NuiMenu.luc`, `SupportLibs/NuiCursorHelper.luc`) |
| `Game/autoexec.cfg` | alterado por uma sessão anterior: `ShowDebugWindow` e `g_ShowGameGestureDebug` ligados; original em `.orig` |

## 4. O menu pelo Kinect

A lógica fica toda no jogo. Não depende de nenhuma função do xam.
- `Function_829853F0` decide a ativação a partir da mão direita à direita do corpo, com os limites `g_NUIActivation*`.
- Bloqueios: um campo de estado (+0x4A8) e a notificação `kXNotificationSystemNUIPause` (0x6001A), que o Xenia nunca envia.
- **Sonda ao vivo (2026-10-25):**
  - o objeto do menu existe (ponteiros em 0x83A2FE30 → +0x148 → +100) e tem os limites certos (−0,10 / 0 / 0,20 / 0,40);
  - mas as posições das mãos ficam em **zero** e os temporizadores não andam, mesmo com o jogador rastreado;
  - ou seja, o menu não está sendo alimentado com o esqueleto.
- **Causa encontrada (2026-10-25): o horário dos quadros.**
  - Por quadro, o jogo pega o esqueleto vivo (`82999EC0`) e só o copia para os dois quadros do menu (`p1+0x10` e `p1+0x14`, de 4456 bytes cada) se o horário (primeiros 8 bytes) não for 0 nem −1 e tiver mudado (`0x8294AA5C`). O horário é tratado em milissegundos (fator 0,001; o valor inicial é `ts − 33`).
  - Sem a cópia, o caminho do menu (`8294A744` → `82985E48`) vê os 6 esqueletos com estado 0 e nunca chama a atualização do menu.
  - O horário vem da informação de quadro do PsCam (código 5): `{sequência, ?, horário em ms de 64 bits}`. O Xenia só escrevia a sequência, e a biblioteca NUI do jogo carimbava todo quadro de imagem e de esqueleto com 0.
  - **Corrigido** no branch `kinect-sistema` (commit 77f6b0970). Com a gravação, o objeto do menu passa a ter as articulações, a posição das mãos e um histórico de 14 a 18 entradas por mão. Falta o teste ao vivo.
  - Sonda usada: `~/Kixenia/kinect-tools/sonda-menu-nike.patch`.
- **Descartados:** espelhamento da imagem (A/B com `kinect_rec_aovivo2`: 18 × 19 de 43 amostras rastreadas), controle conectado (`--hid=nop`) e a notificação de pausa.
- **Rastreio das mãos (2026-10-25):**
  - Ao vivo, a mão direita ficou rastreada em 30% a 88% dos quadros, conforme a posição do jogador; o tronco, em 98–99%.
  - Com a gravação `kinect_rec_aovivo2` (teste `kinect-tools/ab_gpu.sh`), as mãos ficam em cerca de 85% com a configuração atual, com `render_target_path=accuracy` e com a profundidade de 24 bits exata. A precisão da GPU não muda nada.
  - `snorm16_render_target_full_range=false` deixa o rastreador sem nenhum esqueleto: a opção precisa ficar ligada (é o padrão).
  - `kinect_no_depth_raw=2047` (opção nova, local) não mudou o menu.
  - Em todas as rodadas o objeto do menu recebeu as mãos em zero, mesmo com a mão rastreada em 85% dos quadros. **O rastreio das mãos não é a causa do menu parado.**
