# Carnival Games: Monkey See, Monkey Do (título 54540890)

Carnival Games: Monkey See, Monkey Do, da Cat Daddy Games (2K Play, 2011): minijogos de parque de diversões (tiro ao alvo, arremesso, dunk tank, corrida de cavalos…) com o avatar do jogador. Executável `Default.xex` (5,9 MB, PE original `NatalCarnivalReleaseFinal.exe`, XDK `xdk-main-nov10`) extraído do contêiner GOD do HD externo (`Carnival Games Monkey See Monkey Do [RF][Kinect]/Content/0000000000000000/54540890/00007000/76212774D627380FD00D`) para `~/Kixenia/kinect-sdk-re/games/extraidos/carnival/` (1,6 GB), com `database.xmplr` (modelos do rastreador), `nuiidentity.bin.be` (identidade), `AvatarAssetPack`, `AvatarAwards`, os modelos de voz `nuisp1033`, `nuisp2057`, `nuisp2058`, o pacote `xData.car` (752 MB) e `Data\` (vídeos Bink dos minijogos, sons FMOD `.fsb`). Motor próprio da Cat Daddy (`catcore`), física PhysX. O cabeçalho marca `XEX_SYSTEM_SKELETAL_TRACKING_REQUIRED`. Runtime NUI **2.0.20353.1** (`NUISP`, `NUIHNDL` 2.0.20353.0; `XAVATAR`, `XMIC`): o mesmo do [Michael-Jackson-The-Experience](michael-jackson-the-experience.md), entre o do [Kinect-Adventures](kinect-adventures.md)/[Kinect-Joy-Ride](kinect-joy-ride.md) (2.0.11775) e o do [Kinect-Rush](kinect-rush.md)/[Kinect-Star-Wars](kinect-star-wars.md) (2.0.21076.0). Por dentro, o motor de inclinação, os campos do 'XtSs', a voz no executável e o Kinect ID são os mesmos blocos do [Harry-Potter-for-Kinect](harry-potter-for-kinect.md) e do Rush; parece-se mais com o **Michael Jackson** (mesmo runtime) e com o **Harry Potter** (mesmos campos do 'XtSs', avatares).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Harry-Potter-for-Kinect](harry-potter-for-kinect.md), [Michael-Jackson-The-Experience](michael-jackson-the-experience.md), [Kinect-Rush](kinect-rush.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Sports](kinect-sports.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo e o `XamCache*`). Sem teste com o Kinect real e sem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e rodar | ✅ | 150 s, 3510 quadros, sem `Host-side crash`. Um aviso do watchdog no quadro 673 ("no guest frame presented in 2000 watchdog ticks", thread principal rodando, as outras esperando) durante o carregamento; depois seguiu. |
| Runtime NUI | ✅ | `NUI: title runtime 5/4F81 with flags 0000001B, HUD on` no quadro 30 (o mesmo 5/4F81 do Michael Jackson). |
| Câmera | ✅ | `PsCamDeviceRequest` passa antes por `XamXStudioRequest(0x1004)` (8848 vezes); também 0x1401, 0x1402 e 0x1410 por quadro. |
| Ajuste do sensor | ✅ (sem motor) | −25° no quadro 33, 0° no quadro 98; o jogo não chamou `XamNuiCameraRememberFloor` nem `XamNuiCameraTiltGetStatus` em 150 s (igual ao Michael Jackson); o chão do Xenia fica guardado (861 mm) no fim. |
| Menu | 🟡 | Carrega a interface e a música (`UI.fsb`, `Main_Music.fsb`, `Music.fsb`, `Audience.fsb`, `Ambience.fsb`, `Power_Punch.fsb`) e `show_version_info.txt`: deve estar na tela de título ou no menu; falta ver na tela. |
| Rastreamento | 🟡 | 19 de 54 amostras (35%) com o jogador rastreado, do quadro 1026 ao 2146. |
| Voz (comandos) | — | Reconhecedor no próprio executável (`NuiSpeech Worker`, abre `nuisp1033` e `L1033.*`/`ai031033.am`; há uma "List of voice commands"), mas nenhuma função de microfone foi chamada em 150 s. A gravação não tem áudio. |
| Kinect ID (rostos) | ✅ | `NuiIdentityNN`/`NuiIdentityPCA`, `XamCacheOpenFile`/`XamCacheReset` (aproximadas no worktree), `XamReadBiometricData` uma vez. |
| Avatares | ⚪ | 10 funções `XamAvatar*` importadas, todas stubs, nenhuma chamada em 150 s. O jogo usa o avatar como personagem (`AvatarRider`, `attach_avatar`, `GenericAvatars`); com os stubs deve cair nos avatares genéricos. |
| Telas Nui do sistema | ⚪ | 4 `XamShowNui*UI` importadas (DeviceSelector, Guide, Signin, Troubleshooter), nenhuma chamada. |

Chamadas ausentes que caíram em "undefined extern call": `EtxProducerRegister` e `XamXlfsInitializeUploadQueue`; nenhuma é do Kinect. `RmcDeviceRequest` (ausente) é importada, não chamada.

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 cg_replay 150 ~/Kixenia/kinect-sdk-re/games/extraidos/carnival/Default.xex
```
(O nome é `Default.xex`, com D maiúsculo: com `default.xex` o Xenia falha com `Failed to launch target: C00000BB`. O arquivo de cabeçalho GOD `…/54540890/00007000/76212774D627380FD00D` também abre direto.)

## Comparação com os outros jogos

| | Carnival Games | [Michael-Jackson-The-Experience](michael-jackson-the-experience.md) | [Harry-Potter-for-Kinect](harry-potter-for-kinect.md) | [Kinect-Rush](kinect-rush.md) |
|---|---|---|---|---|
| Runtime NUI | 2.0.20353.1 | 2.0.20353 | 2.0.21173.0 | 2.0.21076.0 |
| Microfones | Sim, no executável (`XamVoiceGetMicArrayAudioEx` ×2, eco pelo `XAudioRegisterRenderDriverMECClient`); não chamadas em 150 s | Pelo XHV2 (`XamVoiceGetMicArrayStatus` em polling) | Sim, no executável | Sim, no executável |
| Campos do 'XtSs' lidos | +0x24/+0x28, +0x2C, +0x3C, +0x40, +0x48/+0x4C | os mesmos | os mesmos | +0x20..+0x4C |
| Kinect Studio | Câmera e status passam antes pelo `XamXStudioRequest` | Igual | Igual | Igual |
| Avatares | 10 funções (stubs) | — | 7 funções (stubs) | Não |

## Motor de inclinação do jogo (como funciona)

É o motor do runtime NUI do [Kinect-Rush](kinect-rush.md) e do [Harry-Potter-for-Kinect](harry-potter-for-kinect.md), aqui entre 0x82B49730 e 0x82B4C000. Estado global em 0x83816CA0–0x83816EA0: estado em `0x83816CF0`, contador de espera em `0x83816CF4`, **'XtSs' em `0x83816DF0`** (0x50 bytes). O leitor do status fica no ponteiro `0x82BF8098`, que começa no thunk de `XamNuiCameraTiltGetStatus` (0x82BABFA8) e só passa para o leitor do Kinect Studio (0x82B49A80) se `XamXStudioRequest(1)` der certo. No Xenia, `XamXStudioRequest` sempre devolve `X_E_FAIL`: o jogo lê o status direto do `XamNuiCameraTiltGetStatus`. (Se um dia o Xenia aceitar o pedido 1, o pedido 0x1005 precisa devolver 0x80070057 para o jogo cair no `TiltGetStatus`.) O mesmo vale para o Harry Potter (ponteiro 0x82D18F58, thunk 0x82CF60B4).

1. **Início** (0x82B49E08): sem chão guardado (+0x3C = 0) ou com a flag 0x10, inclina para −25°.
2. **Busca** (0x82B4B260): a cada 60 quadros sem chão, sobe um degrau `23 − 10·n` (−17°, −7°, 3°).
3. **Chão** (0x82B49E98): vai ao sistema por `XamNuiCameraRememberFloor`.
4. **Pedido de ajuste** (0x82B4A070): recusado com +0x40 bit 0; usa `XamNuiCameraElevationGetAngle`.

Campos do 'XtSs' lidos (acessos por ponteiro-base, `cg_acessos_base.py`):

| Campo | Endereço | Funções |
|---|---|---|
| +0x00 tamanho, +0x04 'XtSs' | 0x83816DF0 | gravados antes de cada leitura (0x82B4A810, 0x82B4B260) |
| +0x24, +0x28 gravidade | 0x83816E14 | 0x82B4B260 (ângulo atual) |
| +0x2C flags | 0x83816E1C | 0x82B49E08, 0x82B49E98, 0x82B4A070, 0x82B4B260 |
| +0x3C distância do chão guardado | 0x83816E2C | 0x82B49730, 0x82B49AD0, 0x82B49E08, 0x82B49E98, 0x82B4A070, 0x82B4A500, 0x82B4AAB0 … |
| +0x40 bit 0 (ocupado) | 0x83816E30 | 0x82B4A070 |
| +0x48, +0x4C gravidade do chão | 0x83816E38 | 0x82B49850 |

Não lê +0x30..+0x38 nem +0x44.

## Teste com o Kinect real: pendente

O que verificar ao vivo (`live_test.sh` na sessão principal):
- **Tela**: se chega ao menu e se o cursor de mão funciona; se o aviso do watchdog no carregamento se repete.
- **Rastreamento**: minijogos com arremesso e soco (`Power_Punch`), o cabeçalho exige esqueleto.
- **Motor**: −25° → 0°; conferir que não repete a busca nem mostra "sensor movido".
- **Voz**: quando os comandos de voz ligarem, `XamVoiceGetMicArrayAudioEx` ×2 e `XAudioRegisterRenderDriverMECClient` devem ser chamadas.
- **Avatares**: com as 10 `XamAvatar*` stubs, ver se o jogo mostra avatares genéricos ou trava ao montar o personagem.

## Pendências
- `XamAvatar*` (10): stubs; o jogo é feito em volta do avatar.
- `EtxProducerRegister`/`…Log`/`…Unregister`, `XamXlfs*UploadQueue*`, `RmcDeviceRequest`: ausentes, não afetam o Kinect.

## Arquivos da análise
- Extração: `~/Kixenia/kinect-sdk-re/games/extraidos/carnival/` (log `extraidos/carnival.dump.log`); cópia do XEX em `games/carnival.xex`.
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/carnival.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/cgvmx` (**PowerPC:BE:64:VMX128-32addr**, sem análise automática).
- Log do Xenia: `~/Kixenia/kinect-tools/logs/cg_replay.log`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/cg/c_full/<endereço>.c` (55 funções, nenhuma com `halt_baddata`), com os thunks trocados pelos nomes das importações.
- Scripts: `cg_imports.py`, `cg_funcs.py`, `cg_runtime.py`, `cg_merge.py`, `cg_acessos_base.py`, `gen_carnival.py` (`games/decompilado/scripts/`), `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x821BF000, 23 199 funções; `.text` 0x821F0000–0x82BAC3B8.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/cg/`.**

**292 importações** (103 do `xam.xex`, 189 do `xboxkrnl.exe`), 2285 chamadas diretas no código, 109 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 219 implementadas, 20 aproximadas, 41 stubs, 11 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Estado do sensor

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A05768 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | Se o conjunto de microfones existe (`XamVoiceGetMicArrayStatus` ≠ 2), lê o número de série do sensor. | ✅ |
| 0x82A05830 | 124 | `XamNuiGetDeviceStatus` | 1 | A cada 60 s, lê `XamNuiGetDeviceStatus` e guarda o bit 3 (sensor presente/pronto). | ✅ |
| 0x82BA7E10 | 40 | `XamNuiGetDeviceStatus` | 2 | Lê `XamNuiGetDeviceStatus` e devolve só o campo de estado. | ✅ |

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82B417A0 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82B49730 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: traduz o código de saída, para o motor (`ElevationStopMovement`) nos erros 4 e 0x19 e informa o status ao sistema (0x2B00C). | ✅ |
| 0x82B499D8 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`). | ✅ |
| 0x82B49A80 | 76 | `XamNuiCameraTiltGetStatus` | 0 | Leitor do status 'XtSs' pelo Kinect Studio: `XamXStudioRequest(0x1005)` e, se o Studio responder 0x80070057, `XamNuiCameraTiltGetStatus`. Só é instalado no ponteiro 0x82BF8098 se `XamXStudioRequest(1)` der certo; senão o ponteiro fica no thunk de `XamNuiCameraTiltGetStatus` (0x82BABFA8). | ✅ |
| 0x82B49E08 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | ✅ |
| 0x82B49E98 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão do quadro de esqueleto e o guarda no sistema (`XamNuiCameraRememberFloor`); com o motor parado e erro, encerra o pedido (0x2B006/0x2B00C). | ✅ |
| 0x82B4A070 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura, flags). +0x40 bit 0 do 'XtSs' = ocupado. Lê o ângulo atual (`XamNuiCameraElevationGetAngle`). | ✅ |
| 0x82B4A500 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (0x2B005…0x2B00B); responde com `XamNuiCameraTiltReportStatus`. | ✅ |
| 0x82B4A8C0 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x82B4A500)`) e, se `XamXStudioRequest(1)` der certo, troca o leitor de status por 0x82B49A80. | ✅ |
| 0x82B4B260 | 2408 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: lê o 'XtSs' (+0x24/+0x28 para o ângulo atual); na busca sobe degraus `23 − 10·n` a cada 60 quadros sem chão; erro do motor encerra. | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82B50C50 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | Sessão do Kinect ID: identificador da sessão (`XamNuiIdentityGetSessionId`) e usuário de cada esqueleto (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82B50E40 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82B52670 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário: `XamUserNuiGetEnrollmentIndex`. | ✅ |
| 0x82B528B8 | 348 | `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82B53160 | 360 | `XamUserNuiGetUserIndex` | 0 |  | ✅ |
| 0x82B53E78 | 472 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82B54398 | 704 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê, mescla e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | ✅ |
| 0x82B54658 | 3216 | `XamNuiIdentityGetSessionId` | 8 |  | ✅ |
| 0x82B55C20 | 260 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: `XamCacheOpenFile`, `XamCacheReset(0x18)` e `XamCacheCloseFile` (as três existem no worktree kinect-sistema, cache sempre vazio). | ✅ |
| 0x82B55D28 | 600 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`). | ✅ |
| 0x82B5D410 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |

#### Avatares

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82300740 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | ✅ |
| 0x823007D8 | 168 | `XamAvatarGetMetadataRandom` | 1 |  | ✅ |
| 0x823008F8 | 124 | `XamAvatarShutdown` | 2 |  | ✅ |
| 0x82300978 | 372 | `XamAvatarInitialize` | 1 | Inicia os avatares: reserva 3 MB e chama `XamAvatarInitialize(4, …)`. | ✅ |
| 0x82308978 | 220 | `XamAvatarLoadAnimation` | 1 |  | ✅ |
| 0x82309EA8 | 248 | `XamAvatarGetAssets` | 1 |  | ✅ |
| 0x82309FA0 | 172 | `XamAvatarGenerateMipMaps` | 2 |  | ✅ |

#### Microfones e reconhecimento de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A1C370 | 1412 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | **Laço de áudio dos microfones**: `XamVoiceGetMicArrayAudioEx`, `…UnderrunStatus` e o áudio de referência ao cancelador de eco (`XAudioCaptureRenderDriverFrame`). | ✅ |
| 0x82A1CD28 | 728 | `XAudioUnregisterRenderDriverMECClient` | 3 | Desliga o áudio dos microfones e o cancelador de eco (`XAudioUnregisterRenderDriverMECClient`). | ✅ |
| 0x82A1D000 | 896 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 1 | **Abre o áudio dos microfones** para o reconhecedor de voz: dois fluxos `XamVoiceGetMicArrayAudioEx` e o cliente de cancelamento de eco (`XAudioRegisterRenderDriverMECClient`). | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82BA8280 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82BA82C0 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82BA8308 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82BA8350 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82BA8390 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82BA83C8 | 76 | `XMsgInProcessCall` | 10 |  | ✅ |
| 0x82BA8418 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82BA8460 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BA84B0 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x82BA8520 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82BA8588 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82BA85D8 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BA8648 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x829A7648 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82242A10 | 104 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x822FAC10 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x822FAC88 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x822FAD00 | 140 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82310BA0 | 244 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x827AAF58 | 40 | `XMsgCancelIORequest` | 1 |  | ✅ |
| 0x82B99E38 | 352 | `XMsgInProcessCall` | 1 |  | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8878 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 9 | 3440 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 9 | 5 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 0 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 1 | 0 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 1 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (65)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 3 | 23 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 3 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedFlushSList` | xboxkrnl 0x2B | 6 | 47844 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 10 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 3 | 1 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 160 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 56 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 1 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 1 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 1 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 4 | 10 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 44 | 9839 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 2 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 11 | 19243 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3276 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3276 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 22 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 4 | 17 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 62 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 10 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 6 | 42 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 5 | 37 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 9 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 14 | 65244 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 16 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 34 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 36 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 150250 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 17 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 4 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 565 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 114 | 258 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 7 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 632 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 1 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 4 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 4 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (42)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 1 | ⚪ stub |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 13 | 32 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 3 | 27 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 48 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 5 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 6 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 2 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 2 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 2 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 1 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 15 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 2 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 1 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 2 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 9 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 4 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 4 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 24 | 26556 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 4 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 12 | 26558 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 7 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 6 | 6 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 3511 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 1 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 1 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 3 | 3 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 3512 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 2 | 6 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 10 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 2 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 18 | 56 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 20 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 8 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 10 | 3 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 14 | 20 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 12 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 1 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 1 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 12 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 2 | 3 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 1 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 15 | 18785 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 4 | 7 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 2 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 3 | 4 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Memória</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 4 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 4476 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 184 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 3757 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 1 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 56 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 1142 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 632 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 1 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8878 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 1 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 0 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 1 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 3 | 0 | ✅ |

</details>

<details><summary><b>Avatares</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 0 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 1 | 0 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Áudio</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | 1 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MicDeviceRequest` | xboxkrnl 0x2B1 | 3 | 0 | ⚪ stub |
| `RmcDeviceRequest` | xboxkrnl 0x2E1 | 4 | 0 | ❌ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 9 | 3440 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 9 | 5 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 10 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 701 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 4 | 9 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 5 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 2 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 2 | 1 | ⚪ stub |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike, nem o Kinect Sports, nem o Kinect Adventures).

**Ausentes no Xenia (11):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `RmcDeviceRequest`, `RtlCaptureContext`, `RtlUnwind`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.
