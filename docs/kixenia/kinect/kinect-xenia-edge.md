# Kinect v1 no Xenia Edge (Linux)

Objetivo: Kinect para Xbox 360 (v1, modelo 1473) funcionando no Xenia Edge no Linux, sem dependências externas. Câmera, motor, LED, acelerômetro, microfones, voz e Kinect ID.

Relacionado: [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md), [Kinect-Mapa-XAM](kinect-mapa-xam.md), [Funcoes-XAM-Indice](../xam/funcoes/README.md), [Kinect-ID](kinect-id.md), [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md), [Nike-Kinect-Training](jogos/nike-kinect-training.md), [Kinect-Adventures](jogos/kinect-adventures.md), [Ferramentas-Ghidra-REA](../ghidra-e-rea.md).

## Onde está

- **Código:** worktree `~/Kixenia/xenia-edge-kinect`, branch `kinect-linux`, **58 commits** sobre o Edge (último: `277a3de5b`, 2026-10-25), todos enviados ao fork.
- **Remoto:** fork `davicarlos/xenia-edge`. Não há PR: a política do upstream (has207) proíbe PR de IA, e o PR tem de ser um commit só, com descrição do próprio autor.
- **Base:** PR xenia-canary#1274 do knuckleslee (Kinect por libusb, só Windows), portado para Linux.
- **Upstream:** o Edge foi mesclado em 2026-10-23 (commit a899be008).
- **Documentação para o usuário:** `docs/kinect.md` no repositório.
- **Compilação:** `cmake --build build --config Release -j 4 --target xenia_edge`. Nunca passar de `-j 6`: com `-j 20` o PC travou.

## Como usar

```bash
./build/bin/Linux/Release/xenia_edge --allow_kinect_camera=true --allow_nui_initialization=true \
  --kinect_audio_firmware_path="$HOME/Downloads/SystemUpdate_17559/extracted/\$SystemUpdate/FFFE07DF00000001" \
  "<jogo>/DEFAULT.xex"
```

| Opção | Para quê |
|---|---|
| `allow_kinect_camera` | usar o sensor conectado |
| `allow_nui_initialization` | o console emulado informa um Kinect configurado |
| `kinect_audio_firmware_path` | firmware de áudio da Microsoft (`audios.bin` ou o pacote do system update) |
| `kinect_microphone_gain` | ganho dos microfones; o padrão 8 é necessário para a voz |
| `kinect_audio_calibration_path` | calibração da matriz de microfones (`MicArrayCalibrate.data`) enviada ao sensor |
| `kinect_auto_tilt` | inclinação automática pelo esqueleto (ligada por padrão) |
| `kinect_elevation` | ângulo calibrado salvo, que os jogos usam ao iniciar |
| `kinect_mirror` | espelhar as imagens |
| `kinect_record_path` / `kinect_replay_path` | gravar ou reproduzir as imagens do sensor |
| `kinect_dump_depth_path` | salvar o quadro de profundidade a cada 2 s |
| `kinect_guide_gesture` | o sistema abre o guia com o gesto do guia (ligada por padrão) |
| `kinect_chat_microphone` | `[Console]`: o chat de voz pode usar os microfones do sensor (bit 0x8 de `XCONFIG_CONSOLE_NUI`) |
| `kinect_disabled` | `[Console]`: sensor desligado nas configurações (`KinectDisabled`), como `XamNuiSetForceDeviceOff` deixa |

No Linux é preciso uma regra udev com `TAG+="uaccess"` para os IDs `045e:02ae`, `02ad`, `02b0`, `02c2`, `02be` e `02bf`. O driver `gspca_kinect` é solto e devolvido automaticamente.

## O que funciona

### Câmera (profundidade e cor)
- `PsCamDeviceRequest` com o Kinect real por libusb (usbfs), a 30 fps. Commits deb4780c9 e 8e593d48f.
- O pacote Ob/NUI necessário para os quadros chegarem ao título:
  - `ObOpenObjectByPointer` mantém o tipo criado;
  - `KeInitializeMutant`;
  - `XamContentOpenFile` (para o `Database.xmplr`);
  - mensagens de identidade e de quadro de esqueleto.
- O stub antigo do `PsCamDeviceRequest`, que tapava o verdadeiro, foi removido.
- **Correção de GPU decisiva para o rastreamento** (da9870cc1):
  - O problema: retângulos de resolve vindos do interpretador de shader da CPU precisam chamar `ProvideResolveOutputForGpuWrite`. Isso pode fechar a submissão aberta por `IssueCopy`, e as cópias do classificador iam parar num command buffer fechado.
  - A correção: `BeginSubmission(true)` depois do *provide*.
- Outros ajustes de GPU: resolve sem vértices em vf0, busca de textura no interpretador e draws multipasso com memexport.
- **Ajustes de câmera (códigos 35 e 36):**
  - formato mapeado; o 36 agora devolve as configurações em vez de lixo da pilha (49c29f1c0);
  - detalhes em [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md).
- **Número de série** (`XamNuiGetDeviceSerialNumber(buf, 0x11)`): lido do descritor USB (91ac58116).

### Dispositivo de áudio: firmware, motor, LED, acelerômetro, microfones (c316870c7)
- **Firmware:**
  - o dispositivo `045e:02ad` começa num bootloader (magic 0x06022009, resposta 0x0A6FE000);
  - o envio é o comando 3, em páginas de 16 KB, e a execução é o comando 4;
  - o firmware é o `audios.bin` 1.2.810.0, lido direto do pacote `FFFE07DF00000001` do system update;
  - ele se perde quando falta energia, e o Xenia o envia de novo.
- **Comandos do firmware:**
  - LED: 0x10 (1 apagado, 2 verde piscando, 3 verde, 4 vermelho). Fica verde fixo enquanto o jogo usa o sensor e volta a piscar ao fechar.
  - Status: 0x8032, um bloco de 104 bytes. O acelerômetro está nas words 4 a 6, a inclinação na 7, e os flags na 1 (bits 0 e 1 = em movimento).
  - Inclinação: 0x803B, limitada a ±30°. Para parar, mira-se o ângulo atual, porque 0x803A falha com o motor em movimento.
- **Microfones:**
  - isócrono IN na EP 0x82, pacotes de 524 bytes;
  - o canal 1 é o sinal processado de 16 bits;
  - os canais 2 a 9 são metades dos 4 microfones, em int32.
- **Voz** (848670276): as amostras USB chegam cerca de 8 vezes mais baixas do que os jogos esperam, por isso o ganho 8.
  - O formato big-endian está certo: o conversor do Nike em 0x82F3A9C8 corta em 0x79999998.
  - O caminho no Nike: worker 0x82F353F0 → MEC do próprio título → saída de 16 bits → reconhecedor, que só recebe quando o objeto de fala +0x34 == 3.
- **Referência de eco** (cbc151162): `XAudioCaptureRenderDriverFrame` devolve o quadro que o jogo tocou por último. Com isso a voz funciona mesmo com a música do jogo tocando.

### Inclinação automática e calibração (cbc151162)
- **Algoritmo:**
  - escolhe o jogador rastreado mais próximo;
  - calcula o topo (cabeça + 0,15 m) e a base (pés − 0,05 m) em graus, contra os ±21,5° do campo de visão, com margem de 2°;
  - se o corpo não cabe, centraliza; se é alto demais, prioriza os pés;
  - quando cabe, salva o ângulo em `kinect_elevation`.
- **Folga:** só inclina depois de 2 s seguidos fora da imagem. Já enquadrado, aceita +2° além da borda (pulos e agachamentos).
- **Calibração:** `XCONFIG_CONSOLE_NUI` (0x1C bytes) informa o sensor calibrado: flag 0x2, altura de 1800 e o ângulo. Com isso os jogos vão direto ao ângulo, sem procurar o chão a −25°.
- O slider de inclinação do Kinect Tuner também salva o ângulo.

### Engajamento e esqueleto (cbc151162)
- `XamNuiSkeletonGetBestSkeletonIndex`.
- `XamNuiPlayerEngagementUpdate`: pontuação do xam = √(x²+z²), −0,3 para quem já está rastreado. Os IDs escolhidos vão em quadro+0xACC.
- `XamNuiCameraSetFlags`.

### Kinect ID (faaaa59af e 46a0badf5)
- O jogo cadastra os rostos e os guarda como dados biométricos do perfil.
- O cache da sessão (XMsg 0x2A007/0x2A008) fica no `NUIController`.
- A cópia "Live" fica em `content/<xuid>/FFFE07D1/NuiBiometric.bin`.
- `XamUserNuiEnableBiometric` grava a configuração 0x1004004E; desligar apaga os dados. `XamUserNuiIsBiometricEnabled` lê essa configuração.
- Detalhes em [Kinect-ID](kinect-id.md).

### Mensagens de identidade (2026-10-24)
- 0x2C00C op 1 copia os dados do Kinect ID (6445d2a1f).
- 0x2C00D responde "ninguém" (7ae9c41d5).
- Os ajustes de câmera têm os nomes corrigidos (eb3843a7c).

- `XamVoiceSetMicArrayIdleUsers` (35019b0a4).

### Kinect Guide (4c55c90b3, 51bb12181)
- `XamShowNuiGuideUI` e `XamShowGuideUI` abrem o menu do botão Guide, com `XN_SYS_UI` enquanto ele está aberto.

### Feixe e calibração de áudio (7bc21371c)
- `XamVoiceSetMicArrayBeamAngle` envia o CEMD 5 ao sensor; `kinect_audio_calibration_path` e `XamNatalDeviceAudioCalibrate` enviam a calibração da matriz (CEMD 0). Ver [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md).

### Pontuação de jogadores (00134e151)
- `XamNuiSkeletonScoreUpdate`: alertas de posição, distância, giro e mãos para até 2 jogadores (ver [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md)).

### HUD do sistema (e19c98e7e)
- **`XamNuiHudInterpretFrame`** faz o que o HUD do xam faz com cada quadro (`FUN_816AC688`):
  - escolhe os 2 esqueletos rastreados (quadro+0xACC);
  - pontua os jogadores (`XamNuiSkeletonScoreUpdate`) e entrega como "só posição" quem tem alerta;
  - solta o engajado que sumiu;
  - detecta o **aceno de engajamento** (4 meias-ondas com a mão erguida; parâmetros de `FUN_8180A388`) e o **gesto do guia**;
  - devolve `0x8301000B` com uma UI do sistema aberta.
- Também: `XamNuiHudGetEngagedEnrollmentIndex` (−2 sem engajado), `XamNuiHudEnableInputFilter`, as telas Nui de conquistas, Marketplace, cartão de jogador e "controle necessário", e a **calibração de profundidade** (`XamNuiGet/StoreDepthCalibration`, 0x30 bytes em `kinect_depth_calibration.bin` na raiz de armazenamento; no console, o registro 0x3E00).
- Aceno testado com mão sintética: engaja em 2,4 s a 1 Hz; lento, mão baixa e mão parada não engajam.

### Funções NUI restantes (4251108a5)
- **Perfil ↔ Kinect ID:** `XamUserNuiBind`/`Unbind`, `GetUserIndex`, `GetUserIndexForBind` e `GetEnrollmentIndex` usam a mesma tabela (no xam, 0x81A82FF4: +0x30 ligado, +0x34 cadastro).
- **Identidade:** o callback da 0x2C009 é guardado; `GetEnrollmentInfo` (0x2C013) e `Unenroll` (0x2C012) o chamam no thread do título.
- **Configurações:** `XamNuiEnableChatMic`/`IsChatMicEnabled` (bit 0x8 de `XCONFIG_CONSOLE_NUI`, opção `kinect_chat_microphone`) e `XamNuiSetForceDeviceOff` (`KinectDisabled`, opção `kinect_disabled`; `0xD0000184` se já está assim).
- `XamNuiGetSupportString` (código `K:…` do Troubleshooter), `XamNuiNatalCameraUpdateStarting/Complete` (0x80070002 abre o Troubleshooter).
- As telas Nui da Live (amigos, festa, convite, mensagens, sessões, vídeo) respondem como o xam sem Live; Games abre as conquistas.

### Texturas, foto do rosto, inclinação e exposição (6500fff7d)
- **`XGSetTextureHeaderEx` e `XGOffsetResourceAddress`** portados da cópia da XGraphics dentro do xam (`FUN_817F79C0`, `FUN_81838CB0`, `FUN_81837BB8`, `FUN_81837A98`); testados: 640×480 ARGB = 1.228.800 bytes, DXT1 256×256 = 32 KB + 32 KB de mips.
- **`XamNuiIdentityGetColorTexture`**: pede 0x2C011 ao callback de identidade e monta a textura da foto do rosto.
- **Inclinação pelo título:** o título registra o próprio motor de inclinação (`XamNuiCameraTiltSetCallback`, 0x2B007; Nike em 0x82F53ED0). `AutoTilt` (0x2B005), `ReverseAutoTilt` (0x2B00A) e `AdjustTilt` (0x2B00B) vão para ele, como no despachante do xam (`FUN_816F9CB8`); `TiltReportStatus` (0x2B006/0x2B00C) vai para quem chamou `ElevationSetCallback` (0x2B00D).
- **`XamNuiGetTrueColorInfo`** (PsCam 46): a tabela de autoexposição `{brilho, quadros/s, exposição}` que os jogos usam (60 Hz, ou 50 Hz com `use_50Hz_mode`). O Nike e o Kinect Sports leem os dois seletores.

### Inclinação automática: jogador maior que a imagem (a2aeea0c8)
- Um "jogador" que ocupa mais que o campo de visão inteiro (perto demais, ou esqueleto errado) deixava o sensor apontado para o chão (−26° no Nike, que então perdia o jogador). Agora o motor fica parado nesse caso.

### Gesto do guia pelo sistema (a56454014)
- **Descoberta:** no console quem vê o gesto é o sistema. O despachante 0x2B0xx do xam passa **cada quadro publicado pelo título (0x2B002)** por `XamNuiHudInterpretFrame` (e pelo serviço de identidade). O Xenia só guardava o quadro; agora chama o HUD.
- O Kinect Sports **não** detecta o gesto: chama `XamShowNuiGuideUI` só pela opção "Guide" do menu de pausa e pelo START.
- O gesto é ignorado com o guia aberto. Verificado com uma gravação cuja profundidade tem o braço desenhado a 45° (`armsim`): o rastreador do jogo mediu 43° e o guia abriu.

### Tipo do motor (6cb02fa57)
- `XamNuiCameraGetTiltControllerType` = `UsbdGetNatalHardwareVersion` no xam: **1 = Detroit** (hub, modelo 1414), **2 = Mca** (motor pelo firmware de áudio, modelo 1473). O Xenia agora informa **2** com o Kinect configurado (antes, sempre 1). Nike e Kinect Sports não consultam o tipo.

### Regras exatas do gesto do guia (277a3de5b)
- As janelas de `FUN_81809AF0` (valores de `FUN_81809558`): **braço esquerdo** 25°–65° para fora a partir de caído, ±20° do plano do corpo, cotovelo < 35°; **braço direito** caído (±20°), cotovelo < 110°, dispensado sem a mão direita rastreada; **tronco** < 9° para a frente ou para trás. Com o flag 4/8 do quadro: braço esquerdo até 95°, cotovelo < 40°, sem braço direito nem tronco.
- Teste isolado com esqueleto montado: 9 casos certos. Numa gravação real em pé o tronco fica entre 8° e 10°: o gesto exige corpo bem reto e braço direito solto.

### Revisão e exports que faltavam (855a12dcc, 2026-10-25)
Revisão do código do Kinect com o xam decompilado ao lado. Correções:
- **Código de suporte** (`XamNuiGetSupportString`): um argumento a mais deslocava a versão do firmware. Agora sai `…:01.02.0810.00:<série>`.
- **Gesto do guia, pose relaxada** (flag 4/8): não olha mais o braço direito, como no xam. O teste isolado ganhou 3 casos e passa nos 12.
- **O Guide abre fora do mutex do HUD:** o `InterpretFrame` só devolve se o gesto foi feito.
- **Corridas:** o ângulo do motor e os flags da câmera são atômicos; a inclinação automática segura o `motor_mutex_` o tempo todo; os pedidos PsCam 35/36 (ajustes da câmera) têm lock próprio.
- **Checagens de ponteiro nulo:** `GetDepthCalibration`, 0x2B001 e 0x2B003.
- **`XamNuiCameraSetFlags`** com o motor do tipo 2 agora guarda os flags.
- **0x2B003:** sem o bit 31, zera o jogador engajado (`FUN_816AD130`). Os "Todo" do log saíram.

Implementadas:
- `XamNuiIdentityIdentifyWithBiometric`: o usuário vinculado ao índice de cadastro do esqueleto no último quadro (0xFE se ninguém). Completa o overlapped e devolve 0x800703E5, que o nuihud.xex espera. `XamNuiIdentityAbort` responde sucesso.
- `XamHudGetUserIndex` / `XamHudSetUserIndex` (0xFF = ninguém).
- `XamNuiGetLoadedDepthCalibration` e o **PsCam 44**: A e B dos primeiros 8 bytes de `kinect_depth_calibration.bin`, ou 1,0 e 0 sem calibração.
- `XamNuiGetCameraIntrinsics`: fx, fy, cx, cy da **câmera de profundidade** a 640×480: a focal vem do plano zero da calibração do sensor (`distância de referência / (2 × tamanho do pixel)` = 575,8 no sensor daqui; nominal 571,26), e o centro é 320 × 240. A ordem vem da matriz do Dash.FieldCalibration.lex (`FUN_9BD22C38`); até 2026-10-28 devolvia os nominais da câmera de cor (531,15). Ver [Kinect-ER-xam, 16g](kinect-er-xam.md#16g-pscam-20-e-as-intrínsecas-2026-10-28).
- `XamNuiCameraRememberFloor`: o plano vem **por valor**, dois floats em cada registrador (r3 = normal x, y; r4 = normal z e a altura, ~854 no Nike, provavelmente mm). Exige motor do tipo 1 ou 2, como o xam, e registra o plano no log. A inclinação automática do Xenia acha o chão sozinha.
- `XamPackageManagerFindPackageContainingIndexedXEX`: 0x80070057 sem saída; senão "não encontrado", porque não há pacotes do sistema.
- **0x2B00E:** guarda o observador de quadros, que é chamado com cada quadro antes do HUD, como em `FUN_816F9FC0`; a 0x2B004 o apaga.

Ficaram de fora: PsCam 19, 34 e 41, que só apps do sistema usam (natalsu e FieldCalibration), e os TODOs herdados do upstream (status sem sensor, leituras pendentes ao parar).

### Confirmação pelo REA (2026-10-25)
Cada afirmação desta seção foi conferida no `xam_17559.exe` com o REA (`batch_decompile`, evidência `ev_0886cc005ca51d07f81cbc5e658ee6c668cf30522c96f0e34872c5ede5c460d1`). Nas funções que o REA recusa, a conferência é pelo projeto Ghidra corrigido.

| Commit | Função do xam | O que foi conferido | Fonte | OK |
|---|---|---|---|---|
| `e19c98e7e` | `XamNuiHudInterpretFrame` (0x816ACC68) | devolve 0x8301000B com UI do sistema aberta (REA escreve 0xffffffff8301000b) | REA | ✅ |
| `e19c98e7e` | `XamNuiHudInterpretFrame` (0x816ACC68) | chama o interpretador do HUD (FUN_816AC688) | REA | ✅ |
| `e19c98e7e` | `FUN_816AC688` (0x816AC688) | pontua os jogadores (ScoreUpdate) | REA | ✅ |
| `e19c98e7e` | `FUN_816AC688` (0x816AC688) | roda o aceno (FUN_81808B08) | REA | ✅ |
| `e19c98e7e` | `FUN_816AC688` (0x816AC688) | roda o gesto do guia (FUN_81808C60) | REA | ✅ |
| `e19c98e7e` | `XamNuiHudGetEngagedEnrollmentIndex` (0x816ACCE8) | sem engajado devolve −2 | Ghidra corrigido | ✅ |
| `e19c98e7e` | `XamNuiHudEnableInputFilter` (0x816C7EB8) | repassa ao objeto do HUD (FUN_81692C48) | REA | ✅ |
| `e19c98e7e` | `XamNuiStoreDepthCalibration` (0x816F9A50) | grava no registro 0x3E00 | Ghidra corrigido | ✅ |
| `e19c98e7e` | `XamNuiGetDepthCalibration` (0x816F9990) | lê do registro 0x3E00 | REA | ✅ |
| `e19c98e7e` | `XamShowNuiAchievementsUI` (0x816C4718) | NuiHudCheck + conquistas (0x21008) | REA | ✅ |
| `e19c98e7e` | `XamShowNuiControllerRequiredUI` (0x816C21C8) | caixa de mensagem 0x72000 | REA | ✅ |
| `4251108a5` | `XamUserNuiBind` (0x816DAD28) | grava o cadastro em +0x34 da tabela de usuários | REA | ✅ |
| `4251108a5` | `XamUserNuiUnbind` (0x816DAE88) | apaga a ligação de tipo 5 (FUN_816D8F50) | REA | ✅ |
| `4251108a5` | `XamUserNuiGetUserIndex` (0x816DAE00) | sem usuário: 0x80070525 (REA escreve 0xffffffff80070525) | REA | ✅ |
| `4251108a5` | `XamUserNuiGetUserIndexForBind` (0x816D8B90) | percorre os 4 slots de usuário | Ghidra corrigido | ✅ |
| `4251108a5` | `XamNuiIdentityGetEnrollmentInfo` (0x8180E048) | pergunta ao callback com 0x2C013 | Ghidra corrigido | ✅ |
| `4251108a5` | `XamNuiIdentityUnenroll` (0x8180DFE0) | pede 0x2C012 ao callback | REA | ✅ |
| `4251108a5` | `XamNuiEnableChatMic` (0x816F9ED8) | bit 0x8 de XCONFIG_CONSOLE_NUI | REA | ✅ |
| `4251108a5` | `XamNuiSetForceDeviceOff` (0x817FA170) | já no estado pedido: 0xD0000184 | REA | ✅ |
| `4251108a5` | `XamNuiGetSupportString` (0x817FA2E0) | formata o código de suporte (0x8160C2B4) | REA | ✅ |
| `4251108a5` | `XamNuiNatalCameraUpdateComplete` (0x817FA270) | 0x80070002 abre o Troubleshooter | REA | ✅ |
| `4251108a5` | `XamNuiCameraElevationAutoTilt` (0x816FA3B8) | manda 0x2B005 | REA | ✅ |
| `4251108a5` | `XamNuiCameraElevationReverseAutoTilt` (0x816FA418) | manda 0x2B00A | REA | ✅ |
| `4251108a5` | `XamNuiCameraAdjustTilt` (0x816FA338) | manda 0x2B00B | Ghidra corrigido | ✅ |
| `4251108a5` | `XamNuiCameraElevationSetCallback` (0x816FA168) | manda 0x2B00D | Ghidra corrigido | ✅ |
| `6500fff7d` | `XGSetTextureHeaderEx` (0x817F7C38) | monta o cabeçalho (FUN_817F79C0) | REA | ✅ |
| `6500fff7d` | `XGOffsetResourceAddress` (0x817F7D18) | desloca base (+0x20) e buffers (+0x18) | REA | ✅ |
| `6500fff7d` | `XamNuiIdentityGetColorTexture` (0x8180DF08) | pede 0x2C011 ao callback | REA | ✅ |
| `6500fff7d` | `XamNuiIdentityGetColorTexture` (0x8180DF08) | monta a textura com XGSetTextureHeaderEx | REA | ✅ |
| `6500fff7d` | `XamNuiCameraTiltSetCallback` (0x816FA128) | registra o motor do título (0x2B007) | REA | ✅ |
| `6500fff7d` | `XamNuiCameraTiltReportStatus` (0x816FA1A8) | status 0x2B006 / 0x2B00C | Ghidra corrigido | ✅ |
| `6500fff7d` | `Despachante 0x2B0xx` (0x816F9CB8) | switch (msg − 0x2B001) | REA | ✅ |
| `6500fff7d` | `XamNuiGetTrueColorInfo` (0x816F9B38) | PsCam código 46 (0x2E) | Ghidra corrigido | ✅ |
| `a56454014` | `Despachante 0x2B0xx` (0x816F9CB8) | 0x2B002 → XamNuiHudInterpretFrame | REA | ✅ |
| `a56454014` | `Despachante 0x2B0xx` (0x816F9CB8) | 0x2B002 → serviço de identidade | REA | ✅ |
| `6cb02fa57` | `XamNuiCameraGetTiltControllerType` (0x816FA088) | tipo pelo kernel (UsbdGetNatalHardwareVersion) | REA | ✅ |
| `6cb02fa57` | `XamNuiCameraGetTiltControllerType` (0x816FA088) | 1 = Detroit, 2 = Mca | REA | ✅ |
| `277a3de5b` | `FUN_81809558 (parâmetros)` (0x81809558) | carrega as janelas 205/245 (REA mostra 0x434D0000 = 205 e 0x43750000 = 245) | REA | ✅ |
| `277a3de5b` | `FUN_81809AF0 (pose)` (0x81809AF0) | converte graus por π/180 (REA mostra 0.017453292) | REA | ✅ |
| `277a3de5b` | `FUN_81809AF0 (pose)` (0x81809AF0) | tronco (0x81AD21A8) | REA | ✅ |

### Interface
- **Kinect Tuner** (`XamShowNuiTroubleshooterUI`, e também pelo menu do botão Guide):
  - prévias de profundidade e cor, espelhadas;
  - esqueletos do jogo;
  - painel do motor com slider;
  - barra de nível do microfone.
- `XamShowNuiMessageBoxUI` e `XamShowNuiDeviceSelectorUI`.

## Testes feitos

| Teste | Resultado |
|---|---|
| Kinect Sports Ultimate (4D5308C9), ao vivo | o jogo reage aos movimentos (o jogo foi apagado depois) |
| Nike+ Kinect Training (4D53090F), ao vivo | menu pelo braço estendido para o lado, voz ("Xbox" + comandos), LED, motor |
| Inclinação automática, ao vivo | inclinou sozinho, parou e salvou −1° |
| Voz com a música do jogo | reconhece |
| Gravação × ao vivo (`~/kinect_rec_aovivo2`) | rastreamento igual: cerca de 92% das articulações nos quadros rastreados |
| Trechos longos sem esqueleto na gravação | é a sala vazia na gravação, não falha (medido na profundidade) |
| Dados do Kinect ID | leitura do arquivo do perfil ok; o Nike aguenta dados inválidos |
| Depois do merge do upstream | compila e roda |
| Kinect Sports com a gravação, depois da 0x2C00D |  nenhuma mensagem não implementada; 92% das articulações |
| Kinect Sports com a gravação (2026-10-24, depois de tudo) | roda; 92% das articulações nos quadros rastreados; só falta `IoDismountVolumeByFileHandle` (não é do Kinect) |
| Teste do Kinect ID ao vivo no Nike (2026-10-24) | **não gravou**: o Nike cadastra sozinho após 200 amostras do rosto (`0x82F5E040`) e nem ao vivo nem com gravação chegou a gravar; pendente |
| Gesto do guia ao vivo no Kinect Sports (2026-10-24) | não abriu: o Xenia não passava os quadros ao HUD (corrigido em a56454014) e o braço passava pela pose em menos de 1 s |
| Gesto do guia por simulação (`kinect_sim_guia`) | abriu o guia após 3 s (a56454014); com as regras exatas (277a3de5b) o quadro de base reprova (braço direito dobrado, tronco 14°) |
| Diagnóstico "o esqueleto some" (2026-10-25) | USB sem perda (6.600 + 6.600 quadros), ritmo igual; a gravação ao vivo reproduzida dá o **mesmo** rastreamento (12% × 12%): nos trechos sem esqueleto **não há ninguém** na imagem |
| Tipo do motor 2 (6cb02fa57) | Nike 973 e Kinect Sports 1145 quadros rastreados nas gravações, sem mudança de comportamento |
| Firmware travado depois de `kill -9` | o motor recusou comandos e o Kinect Sports travou ao carregar; resolvido tirando o Kinect da tomada. **Corrigido por software em 87e867055** (2026-10-28): o endpoint de comandos do firmware de áudio fica travado (bulk para 0x01 dá timeout) com a resposta do último comando pendente; `clear_halt` destrava mas devolve a resposta velha; `libusb_reset_device` resolve sem reiniciar o firmware. Na abertura, lê o estado e reseta se não responde |
| **Bateria ao vivo de 2026-10-28** (3 min por jogo, 33 jogos, roteiro `scratchpad/live/rodar.sh`) | **Passaram** (sem crash, Kinect aberto): Kinect Sports Ultimate, Nike, Adventures (5 avatares), Joy Ride, Rush, Star Wars (xex e GoD), Dance Central 3, Dance Paradise (4 avatares), Fighters Uncaged, Just Dance 4 e 2015, PowerUp Heroes (9 avatares), Black Eyed Peas, Hip Hop Dance, UFC Trainer, Your Shape, Zumba Core/World Party/Kids, Harry Potter, Rabbids, Motion Explosion (14 avatares), Rapala, Sesame Street, Crossboard 7, Fable, Doodle Jump, Fruit Ninja, Carnival, Adrenalin, Kinect Sports Season Two (GoD). **Travou:** Michael Jackson (GoD): `d:\Common\Font\base.fnt` não existe no pacote → ponteiro nulo em 0x82FCFF00; conferir o pacote |
| Crash do Kinect Sports ao montar o avatar do jogador (2026-10-28) | `allow_avatar_initialization=false` fazia o `XamAvatarInitialize` falhar; o jogo não trata e lê uma tabela nula (0x82558F30) quando alguém entra na frente do sensor. Padrão agora `true` (ae5917d1e) |
| Crash do Kinect Adventures ao montar o primeiro avatar (2026-10-28) | estouro de pilha em 0x82A73418: percorre o esqueleto do resultado do GetAssets, que saía vazio. Esqueleto padrão do xam implementado (f0f7c2a9c); veja [Avatares](../avatares/README.md) |
| Jogos em GoD/XBLA não abriam o Kinect ID (`Database.xmplr` dentro da imagem) | Doodle Jump e Michael Jackson travavam num ponteiro nulo, Fruit Ninja/Carnival/Adrenalin/Season Two ficavam num menu; `OpenContentFromGuestFile` agora copia o arquivo da imagem para um temporário (ebdb8cbe8) |
| Motion Explosion: "no room for the component 0020" | uma peça com máscara 0x0208 reservava espaço pelo bit 0x200, fora do pedido 0x3F; a reserva passou a ser `máscara da peça ∧ pedida` |

## Ferramentas de teste (`~/Kixenia/kinect-tools`)
- `live_test.sh <log> [xex] [opções]`: roda um jogo com o sensor real (firmware do pacote do system update, armazenamento em `kinect-tools/storage`).
- `replay_test.sh <gravação> <log> <segundos> [xex]`: reproduz uma gravação num jogo e fecha. `LOGLEVEL` muda o nível de log.
- `diagnostico-kinect.patch`: diagnóstico local (linhas JOINTS/NOSKEL com mãos e ombros, KFRAMES com quadros do USB completos e descartados). **Nunca vai para commit**: antes de cada commit, `git apply -R`, depois aplicar de novo.
- `armsim`: lê e grava quadros de profundidade (11 bits empacotados), mostra a cena em ASCII por faixa de distância, gera imagem colorida (`ppm`) e desenha um braço (`arm`). Foi assim que se fez `~/kinect_sim_guia`.
- `tests/`: testes isolados do aceno (`wave_test.cc`), do detector do guia (`guide_pose_test.cc`) e do cabeçalho de textura (`xg_header_test.cc`).
- `ghidra/`: scripts headless (DecompAt, FnGraph, FindScalar, FindWord, MakeDecomp, ReadF, ReadS, XrefsTo…). Projetos com o fluxo corrigido em `~/Kixenia/kinect-sdk-re/ghidra-projects` (`xamproj`, `ksproj`).
- `verificacao-rea-commits.json`: a conferência pelo REA da tabela acima.
- `xenia-kinect-camera-test`: testa câmera, firmware, motor, LED, microfones e gravação sem jogo.
- **Jogos** no HD de 8 TB (`/dev/sdb1`; se desmontar: `udisksctl mount -b /dev/sdb1 -o ro`): Kinect Sports Ultimate Collection e Nike+ Kinect Training.
- **Gravações:** `~/kinect_rec_aovivo2` (cerca de 2 min, a principal), `~/kinect_rec_guia` e `~/kinect_rec_guia2` (tentativas do gesto do guia; a 2 tem 14 min), `~/kinect_sim_guia` (sintética, braço desenhado a 45°, 540 quadros em loop) e as antigas `kinect_rec_full`, `kinect_rec_deslize`, `kinect_rec_teste`, `kinect_rec1` a `kinect_rec3`.
- **Formato das gravações:** `depth_NNNNN.bin` (640×480, 11 bits empacotados do bit mais alto, 422.400 bytes), `color_NNNNN.bin` (Bayer 640×480, 307.200 bytes), `calibration.bin` (142 bytes); a reprodução fica em loop.

## O que falta (revisado em 2026-10-25, depois da decompilação dos módulos do sistema)

> [!NOTE]
> **Atualização de 2026-10-28**
> No branch `kixenia-main` (repositório kixenia): os 22 `XamFitness*` existem (ver [XAM-Fitness](../xam/xam-fitness.md)), os 8 `XamVoice*` que faltavam foram feitos (commit 7c9a5de9e: `GetMicArrayAudio` usa o mesmo caminho da Ex; `MuteMicArray`, `GetMicArrayFilenameDesc` e `DisableMicArray` são aproximados; `GetDirectionalData` e `GetBatteryStatus` respondem sem dispositivo), e o sistema de avatares foi implementado ([Avatares](../avatares/README.md)). O mapa [Kinect-Mapa-XAM](kinect-mapa-xam.md) foi recontado a partir do código.

Situação do branch:
- Sincronizado com o fork; último commit 5125708b2 (perfil de fitness, ver [XAM-Fitness](../xam/xam-fitness.md)).
- Sem código de depuração commitado: o patch `~/Kixenia/kinect-tools/diagnostico-kinect.patch` fica só local.
- Todas as opções `kinect_*` estão em `docs/kinect.md`.

Para o Nike e o Kinect Sports, nenhuma função do Kinect que eles chamam falta: `XamNuiGetTrueColorInfo` foi implementada em 6500fff7d com a tabela de exposição que os próprios jogos usam. O que falta serve a testes ao vivo, a outros sensores e jogos, e ao painel.

### A. Testes que precisam de você na frente do sensor
1. **Kinect ID no Nike:** não grava. O Nike cadastra sozinho após 200 amostras do rosto (`0x82F5E040`), mas nem ao vivo nem com a gravação chegou a gravar. A baixa porcentagem ao vivo era gente fora da imagem, não falha (diagnóstico de 2026-10-25).
   - **Novo (2026-10-25):** os modelos de identidade **abrem** no Xenia (`nuiidentity:\NuiIdentityNN.bin.be` e `PCA` com status 0), então a falha é anterior à fila de amostras. O caminho completo e os endereços no Nike estão em [Kinect-ID](kinect-id.md); o teste deve registrar o retorno de **0x82F62A28** (flags de qualidade), de **0x82F5B320** (rosto extraído) e as chamadas de **0x82F62730** (amostra guardada).
2. **Gesto do guia:** regras exatas do xam (277a3de5b). Falta ao vivo: braço esquerdo reto a 45° para baixo, **braço direito solto e reto**, corpo ereto, parado 3 s, a uns 2,5 m. No Kinect Sports também dá pela opção "Guide" do menu de pausa.
3. **Inclinação automática durante um treino:** a folga de 2 s e de 2° deve impedir o motor de mexer com pulos e agachamentos.
4. **Ganho do microfone:** no 8, sons altos saturam (provavelmente a TV em outro cômodo); testar 6.
5. **Aceno de engajamento ao vivo:** o HUD agora roda em todo quadro publicado (a56454014), mas o aceno só foi testado com mão sintética (engaja em 2,4 s a 1 Hz).

### B. Implementação, com o comportamento já conhecido
1. **Motor, LED e acelerômetro do modelo 1414** (045e:02b0): protocolo completo na seção 14, em [Kinect-ER-USB-e-Protocolos](kinect-er-usb-e-protocolos.md). Não há sensor desse modelo para testar.
2. **Ajustes de câmera no sensor:** comando 0x95 (0x0106: bits 14 e 1; 0x0009: obturador). Nenhum jogo conhecido pede ajuste manual, e os registradores dos ganhos R/G/B não são conhecidos.
3. **`GetQualityFlags`:** a 0x2C008 roda uma tarefa assíncrona do serviço de identidade do xam (`FUN_8180EB20`). `IdentifyWithBiometric` e `XamHudSet/GetUserIndex` foram feitas em 855a12dcc, sem passar pelo motor de identidade do título.
4. **Intrínsecas reais:** feito em 2026-10-28: `XamNuiGetCameraIntrinsics` calcula a focal da câmera de profundidade a partir do plano zero que o sensor guarda. O que o PsCam 46 com seletor 0x100 responde no console continua sem confirmação direta (precisaria do kernel real).
5. **Kinect ID do painel:** o callback da 0x2C009 é chamado pelas 0x2C011 a 0x2C013 (4251108a5, 6500fff7d). Falta 0x2C00A, 0x2C006 a 0x2C010 e a tabela de cadastros para respostas reais na 0x2C00D.
6. **`RememberFloor`:** só registra o plano no log (855a12dcc); a inclinação automática não usa o chão do título.
7. **TODOs no código:**
   - `NuiInitialized` sem checar se o sensor está pronto (nui.cc);
   - status sem sensor, leituras pendentes ao parar e validação de faixas no PsCam (xboxkrnl_pscam.cc).
8. **Descobertos nos módulos do sistema (2026-10-25)**, nenhum usado pelos dois jogos:
   - PsCam **19** (comando cru à câmera, protocolo PrimeSense), **34** (versão do firmware) e **41** (lido pela calibração): ausentes em `xboxkrnl_pscam.cc` (natalsu.xex, Dash.FieldCalibration.lex);
   - `XamNuiGetLoadedDepthCalibration` (PsCam 44), `XamPackageManagerFindPackageContainingIndexedXEX` e a mensagem 0x2B00E (Guide.NuiTroubleshooter.xex): feitas em 855a12dcc.

### C. Investigação
1. **Canal processado do microfone (canal 1):** quase nunca chega; o firmware `audios.bin` pode mostrar o comando que o liga. Junto, o processamento de áudio do `KinectAudio10_x64.dll` e `XamVoiceSetMicArrayBeamAngle`, que o xam envia ao firmware como pacote CEMD 5.
2. **Firmware:** o que está atrás do I2C (0x0B/0x0C) e da interface 0x258 a 0x261; os comandos do motor 0x8033 a 0x8038.
3. **PsCam:** o código 33; os valores reais do 46 (exposição e intrínsecas), 20 (ventilador) e 40. O 44 é a calibração de profundidade carregada (`0x817FCB40`: A em +4, B em +8), feito em 855a12dcc; os códigos 19, 34 e 41 já foram identificados (item B8).
4. **Esqueleto fantasma** parado no começo da sessão, visto uma vez no Nike e não reproduzido.
6. **Ordinal 2174:** a função é uma checagem de data dos rostos cadastrados; o nome `ControlpackNuiCursorSetTrackingId` na tabela do Xenia parece errado (seção 16g, em [Kinect-ER-Pendencias](kinect-er-pendencias.md)).

### D. Compatibilidade

- **Bateria de 2026-10-28, fora do Kinect:** Black Eyed Peas, Hip Hop Dance e Motion Explosion: 64 `Invalid instruction` VMX128 (opcodes 0x100B630B, 0x13EB630F e parentes, em 0x8348DD20…) sem travar; Star Wars, Zumba Core e Joy Ride (1024 vezes): `PhysicalHeap::AllocRange` não acha faixa contígua com páginas livres (fragmentação do heap físico); Season Two (GoD): `GAME:\Modules\GestureLibraryDLLR.xex` não encontrado na imagem; Fruit Ninja consulta `XamUserGetSigninInfo` em laço (55 mil vezes em 3 min); Michael Jackson (GoD): falta `d:\Common\Font\base.fnt` no pacote.

1. **Compilar e testar no Windows:** o código novo só foi compilado no Linux.
2. **macOS:** sem libusb, não aparece sensor.
3. **Outros jogos Kinect:** no HD só há o Kinect Sports e o Nike; do Kinect Star Wars só existe a part5.rar.
4. **Kinect for Windows** (02bf/02c2): sem teste.
5. **PR no upstream:** a política do has207 exige um commit só e descrição escrita pelo autor. Fica a seu critério.
