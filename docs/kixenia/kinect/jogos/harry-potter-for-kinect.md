# Harry Potter for Kinect (título 57520826)

Harry Potter for Kinect, da Eurocom (Warner Bros. Interactive, 2012): o jogador lança feitiços com os braços e a voz pelas cenas dos oito filmes. Executável `default.xex` (14,6 MB, PE original `Potter.exe`, XDK de julho de 2012) extraído do contêiner GOD do HD externo (`Harry Potter for Kinect [RF][Kinect]/Content/0000000000000000/57520826/00007000/96C2EE0D147828C4981F`) para `~/Kixenia/kinect-sdk-re/games/extraidos/harrypotter/` (3,6 GB), com `database.xmplr` (modelos do rastreador), `nuiidentity.bin.be` (identidade), `AvatarAssetPack`, os modelos de voz `nuisp1031` … `nuisp3084` (um por idioma) e os pacotes próprios da Eurocom `filelist.000`/`filelist_boot.000` e `EUROSOUND_*`. O cabeçalho marca `XEX_SYSTEM_SKELETAL_TRACKING_REQUIRED`. Runtime NUI **2.0.21173.0** (`NUI`, `NUISP`, `NUIAUD`, `NUIHNDL`, `XAVATAR`): o mesmo do [Nike-Kinect-Training](nike-kinect-training.md) e do [Kinect-Sports](kinect-sports.md), mais novo que o do [Kinect-Rush](kinect-rush.md) e do [Kinect-Star-Wars](kinect-star-wars.md) (2.0.21076.0) e o do [Kinect-Adventures](kinect-adventures.md) (2.0.11775.7). Por dentro, o motor de inclinação, a voz e o Kinect ID são os mesmos blocos do Rush e do [Kinect-Joy-Ride](kinect-joy-ride.md), em outros endereços; é o jogo analisado que mais se parece com o **Kinect Rush** (voz no próprio executável, câmera e status pelo Kinect Studio primeiro), com avatares a mais.

O motor do jogo (classes `EXKinectManager`, `EXKinectSkeletonManager`, `EXKinectSpeechManager`, `XKinectHandSetup`) usa o esqueleto para os feitiços (`Kinect SpellCasting`), rastreio da cabeça (`Kinect HeadTrack …`), passos para o lado (`Kinect Side Move`), a imagem da câmera (`Kinect Video Capture`, `KinectTexture`) e comandos de voz por gramática (`LoadSpeechGrammar`, `KINECT: Heard %s`).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Rush](kinect-rush.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Sports](kinect-sports.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo e o `XamCache*`). Sem teste com o Kinect real e sem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e rodar | ✅ | 150 s, 4021 quadros, sem travar nem `Host-side crash`. A thread principal segue no laço (controles, `XamNuiGetDeviceStatus` a cada quadro). |
| Runtime NUI | ✅ | `NUI: title runtime 5/52B5 with flags 00000C4B, HUD on` no quadro 125. `XamNuiGetTrueColorInfo` é buscada por ordinal (2 chamadas). |
| Câmera | ✅ | `PsCamDeviceRequest` passa antes por `XamXStudioRequest(0x1004)` a cada quadro; primeiro quadro de profundidade e de cor no quadro 131. |
| Ajuste do sensor | ✅ (sem motor) | −25° no quadro 131 (sem chão guardado), 1° no quadro 154; o chão fica guardado (877 mm) e a busca não recomeça. Nenhuma chamada a `XamNuiCameraTiltGetStatus` nem `RememberFloor` apareceu no log (como no Carnival e no Michael Jackson). |
| Rastreamento | 🟡 | 16 de 42 amostras (38%) com o jogador rastreado, do quadro 679 ao 2574; depois só "0 tracked". |
| Carregamento | ❔ | Só abriu o pacote de inicialização (`FILELIST_BOOT.BIN`/`.000`) e `EUROSOUND_COM`; o pacote principal `FILELIST.BIN` nunca foi aberto em 150 s. Pode estar parado na tela de título ou de "levante a mão"; falta ver na tela. |
| Voz (comandos e feitiços) | — | Reconhecedor no próprio executável (`NUISPEECH`, abre `nuisp1033` e os arquivos `L1033.*`/`ai031033.am`), mas nenhuma função de microfone foi chamada em 150 s: a gramática só deve ligar dentro do jogo. A gravação não tem áudio. |
| Kinect ID (rostos) | ✅ | Abre `NuiIdentityNN`/`NuiIdentityPCA`; `XamCacheOpenFile` e `XamCacheReset` (aproximadas no worktree) respondem; `XamReadBiometricData` uma vez. |
| Avatares | ⚪ | 7 funções `XamAvatar*` importadas, todas stubs; nenhuma chamada em 150 s. |
| Telas Nui do sistema | ⚪ | 9 `XamShowNui*UI` importadas, nenhuma chamada; `…ControllerRequiredUI`, `…FriendsUI` e `…GamerCardUIForXUID` são ausentes. |

Chamadas ausentes que caíram em "undefined extern call": `EtxProducerRegister` (registro de eventos do runtime), `XamXlfsInitializeUploadQueue` e `IoDismountVolumeByFileHandle`; nenhuma é do Kinect.

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 hp_replay 150 ~/Kixenia/kinect-sdk-re/games/extraidos/harrypotter/default.xex
```
(O arquivo de cabeçalho GOD `…/57520826/00007000/96C2EE0D147828C4981F` também abre direto no Xenia.)

## Comparação com os outros jogos

| | Harry Potter | [Kinect-Rush](kinect-rush.md) | [Kinect-Joy-Ride](kinect-joy-ride.md) | [Nike-Kinect-Training](nike-kinect-training.md) / [Kinect-Sports](kinect-sports.md) |
|---|---|---|---|---|
| Runtime NUI | 2.0.21173.0 | 2.0.21076.0 | 2.0.11775.6 | 2.0.21173.0 |
| Microfones | **Sim**, no executável (`XamVoiceGetMicArrayAudioEx` ×2, eco pelo `XAudioRegisterRenderDriverMECClient`) | Sim, no executável | Sim | Nike: sim; Kinect Sports: não |
| Campos do 'XtSs' lidos | +0x24/+0x28, +0x2C, +0x3C, +0x40, +0x48/+0x4C | +0x20..+0x4C | +0x24/+0x28, +0x2C, +0x3C, +0x40, +0x48/+0x4C | as mesmas funções de inclinação |
| Kinect Studio | Câmera (0x1003/0x1004) passa antes pelo `XamXStudioRequest`; o status só se o pedido 1 der certo | Igual | — | — |
| Kinect ID | Sim | Sim | Sim | Sim |
| Avatares | 7 funções (stubs) | Não | Sim | Kinect Sports: sim; Nike: não |

## Motor de inclinação do jogo (como funciona)

É o motor do runtime NUI do [Kinect-Rush](kinect-rush.md) e do [Kinect-Adventures](kinect-adventures.md), aqui entre 0x82A55900 e 0x82A58000. Estado global em 0x82E5D5B0–0x82E5D7A0: estado em `0x82E5D600`, contador de espera em `0x82E5D604`, **'XtSs' em `0x82E5D700`** (0x50 bytes). O leitor do status fica no ponteiro `0x82D18F58`, que começa no thunk de `XamNuiCameraTiltGetStatus` (0x82CF60B4) e só passa para o leitor do Kinect Studio (0x82A55C68) se `XamXStudioRequest(1)` der certo. No Xenia, `XamXStudioRequest` sempre devolve `X_E_FAIL`: o jogo lê o status direto do `XamNuiCameraTiltGetStatus` (o mesmo vale para o [Carnival-Games-Monkey-See-Monkey-Do](carnival-games-monkey-see-monkey-do.md)).

1. **Início** (0x82A55FF0): sem chão guardado (+0x3C = 0) ou com a flag 0x10, inclina para −25°.
2. **Busca** (0x82A571A0): a cada 60 quadros sem chão, sobe um degrau `23 − 10·n` (−17°, −7°, 3°).
3. **Chão** (0x82A56080): vai ao sistema por `XamNuiCameraRememberFloor`.
4. **Pedido de ajuste** (0x82A56258): recusado com +0x40 bit 0; usa `XamNuiCameraElevationGetAngle`.

Campos do 'XtSs' lidos (acessos por ponteiro-base, `hp_acessos_base.py`):

| Campo | Endereço | Funções |
|---|---|---|
| +0x00 tamanho, +0x04 'XtSs' | 0x82E5D700 | gravados antes de cada leitura (0x82A56810, 0x82A571A0) |
| +0x24, +0x28 gravidade | 0x82E5D724 | 0x82A571A0 (ângulo atual) |
| +0x2C flags | 0x82E5D72C | 0x82A55FF0, 0x82A56080, 0x82A56258, 0x82A571A0 |
| +0x3C distância do chão guardado | 0x82E5D73C | 0x82A55910, 0x82A55CB8, 0x82A55FF0, 0x82A56080, 0x82A56258, 0x82A56500, 0x82A569F0 … |
| +0x40 bit 0 (ocupado) | 0x82E5D740 | 0x82A56258 |
| +0x48, +0x4C gravidade do chão | 0x82E5D748 | 0x82A55A30 |

Não lê +0x30..+0x38 nem +0x44 (como o Joy Ride).

## Teste com o Kinect real: pendente

O que verificar ao vivo (`live_test.sh` na sessão principal):
- **Tela**: se passa da tela de título/"levante a mão" e abre o pacote principal `FILELIST.BIN` (na gravação não abriu).
- **Rastreamento e feitiços**: braços, passos laterais (`Kinect Side Move`), cabeça (`Kinect HeadTrack`); o cabeçalho exige esqueleto.
- **Motor**: −25° → ângulo do jogador; não deve repetir a busca; aviso de sensor movido.
- **Voz**: quando a gramática ligar, `XamVoiceGetMicArrayAudioEx` ×2 e `XAudioRegisterRenderDriverMECClient` devem ser chamadas; conferir se os feitiços/comandos por voz são reconhecidos.
- **Câmera colorida**: `Kinect Video Capture`/`KinectTexture` (imagem do jogador na tela).
- **Avatares**: as 7 `XamAvatar*` são stubs; ver se o jogo depende delas.

## Pendências
- `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`: ausentes; nenhuma chamada vista.
- `EtxProducerRegister`/`…Log`/`…Unregister`, `XamXlfs*UploadQueue*`: ausentes, chamadas ao abrir, não afetam o Kinect.
- `XamAvatar*` (7): stubs.

## Arquivos da análise
- Extração: `~/Kixenia/kinect-sdk-re/games/extraidos/harrypotter/` (log `extraidos/hp.dump.log`); cópia do XEX em `games/harrypotter.xex`.
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/harrypotter.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/hpvmx` (**PowerPC:BE:64:VMX128-32addr**, sem análise automática).
- Log do Xenia: `~/Kixenia/kinect-tools/logs/hp_replay.log`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/hp/c_full/<endereço>.c` (62 funções, uma com `halt_baddata`: 0x82C32DF8), com os thunks trocados pelos nomes das importações.
- Scripts: `hp_imports.py`, `hp_funcs.py`, `hp_runtime.py`, `hp_merge.py`, `hp_acessos_base.py`, `gen_harrypotter.py` (`games/decompilado/scripts/`), `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x82246200, 36 088 funções; `.text` 0x82290000–0x82CF6E94.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/hp/`.**

**310 importações** (108 do `xam.xex`, 202 do `xboxkrnl.exe`), 5233 chamadas diretas no código, 108 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 217 implementadas, 26 aproximadas, 40 stubs, 26 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Estado do sensor

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82AB10E8 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | Se o conjunto de microfones existe (`XamVoiceGetMicArrayStatus` ≠ 2), lê o número de série do sensor. | ✅ |
| 0x82AB11B0 | 124 | `XamNuiGetDeviceStatus` | 1 | A cada 60 s, lê `XamNuiGetDeviceStatus` e guarda o bit 3 (sensor presente/pronto). | ✅ |
| 0x82C32A40 | 40 | `XamNuiGetDeviceStatus` | 4 |  | ✅ |
| 0x82C32DF8 | 28 | `XamShowNuiDirtyDiscErrorUI` | 0 |  | ⚠️ |

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A41DC0 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A55910 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: traduz o código de saída, para o motor (`ElevationStopMovement`) nos erros 4 e 0x19 e informa o status ao sistema (0x2B00C). | ✅ |
| 0x82A55BC0 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`). | ✅ |
| 0x82A55C68 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'**: tenta `XamXStudioRequest(0x1005)` e, se o Studio não responder (0x80070057), chama `XamNuiCameraTiltGetStatus`. Só é instalado no ponteiro 0x82D18F58 se `XamXStudioRequest(1)` der certo; senão o ponteiro fica no thunk de `XamNuiCameraTiltGetStatus` (0x82CF60B4). | ✅ |
| 0x82A55FF0 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | ✅ |
| 0x82A56080 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão do quadro de esqueleto e o guarda no sistema (`XamNuiCameraRememberFloor`); com o motor parado e erro, encerra o pedido (0x2B006/0x2B00C). | ✅ |
| 0x82A56258 | 676 | `XamNuiCameraElevationGetAngle` | 1 | **Pedido de ajuste automático** (distância, altura, flags). +0x40 bit 0 do 'XtSs' = ocupado (devolve 5). Lê o ângulo atual (`XamNuiCameraElevationGetAngle`); flag 0x20: só confere. | ✅ |
| 0x82A56500 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (0x2B005…0x2B00B): AutoTilt, StopMovement, ReverseAutoTilt e AdjustTilt do sistema; responde com `XamNuiCameraTiltReportStatus`. | ✅ |
| 0x82A568C0 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x82A56500)`), e, se `XamXStudioRequest(1)` der certo, troca o leitor de status por 0x82A55C68. | ✅ |
| 0x82A571A0 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: lê o 'XtSs' (+0x24/+0x28 para o ângulo atual); na busca sobe degraus `23 − 10·n` a cada 60 quadros sem chão; erro do motor encerra com 0x1F. | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A4EC38 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | Sessão do Kinect ID: lê o banco de identidade (0xAF000 bytes), o identificador da sessão (`XamNuiIdentityGetSessionId`) e o usuário de cada esqueleto (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82A4EE58 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82A50DE8 | 356 | `XamUserNuiGetUserIndex` | 0 | Para cada esqueleto, pergunta ao sistema qual usuário está associado (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82A51BD0 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82A51D58 | 3200 | `XamNuiIdentityGetSessionId` | 5 |  | ✅ |
| 0x82A5E420 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82A627F0 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário: `XamUserNuiGetEnrollmentIndex`. | ✅ |
| 0x82A62A38 | 336 | `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82A62CB8 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: abre até 256 arquivos com `XamCacheOpenFile`, `XamCacheReset(0x18)` e fecha. As três já existem no worktree kinect-sistema (cache sempre vazio). | ✅ |
| 0x82A62DB0 | 488 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`). | ✅ |
| 0x82A63420 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê, mescla e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | ✅ |

#### Avatares

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A887F0 | 124 | `XamAvatarShutdown` | 2 |  | ✅ |
| 0x82A88870 | 376 | `XamAvatarInitialize` | 1 | Inicia os avatares: reserva 3 MB e chama `XamAvatarInitialize(4, …)`. | ✅ |
| 0x82A89240 | 248 | `XamAvatarGetAssets` | 1 |  | ✅ |
| 0x82A89338 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | ✅ |
| 0x82A893D0 | 168 | `XamAvatarGetMetadataRandom` | 1 |  | ✅ |
| 0x82A93568 | 220 | `XamAvatarLoadAnimation` | 1 |  | ✅ |

#### Microfones e reconhecimento de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82BE2750 | 224 | `XamNuiGetDeviceSerialNumber` | 1 |  | ✅ |
| 0x82BE2938 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | **Laço de áudio dos microfones**: lê os quadros de `XamVoiceGetMicArrayAudioEx`, confere `…UnderrunStatus` e manda o áudio de referência ao cancelador de eco (`XAudioCaptureRenderDriverFrame`). | ✅ |
| 0x82BE37F8 | 1252 | `XAudioUnregisterRenderDriverMECClient` | 4 | Desliga o áudio dos microfones e o cancelador de eco (`XAudioUnregisterRenderDriverMECClient`). | ✅ |
| 0x82BE3E00 | 1504 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | **Abre o áudio dos microfones** para o reconhecedor de voz: aloca os buffers (0x6000, 0x1B000, 0x4800…), pede dois fluxos `XamVoiceGetMicArrayAudioEx` e registra o cliente de cancelamento de eco (`XAudioRegisterRenderDriverMECClient`). | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x8262E160 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x8262E1D8 | 140 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8295A780 | 144 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8295A810 | 168 | `XMsgStartIORequestEx` | 2 |  | ✅ |
| 0x8295A8B8 | 104 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82C35748 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82C35788 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82C357D0 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82C35818 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82C35858 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82C35890 | 76 | `XMsgInProcessCall` | 9 |  | ✅ |
| 0x82C358E0 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82C35928 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82C35978 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x82C359E8 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82C35A50 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82C35AA0 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82C35B10 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82C35E98 | 40 | `XMsgCancelIORequest` | 1 |  | ✅ |
| 0x82C65FF8 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82C661A0 | 252 | `XMsgInProcessCall` | 5 |  | ✅ |
| 0x82C662D0 | 352 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82C66430 | 384 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82C665B0 | 312 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82C666E8 | 320 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82C66828 | 320 | `XMsgInProcessCall` | 1 |  | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82914D98 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 11643 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 14 | 11803 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 9 | 8 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 0 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
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
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 4016 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 5 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (62)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 22 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 1 | 1 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 58 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 1 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 1 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 7 | 9 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 2565 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 2 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 25 | 4 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 2539 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 2539 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 20 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 9 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 105 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 10 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 1 | 1 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 1 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 4 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 7 | 13 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 6 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 36 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 39 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 40 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 1 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 18 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 7 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1842 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 195 | 354 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 8 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1973 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 3 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 8 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (58)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 3 | ⚪ stub |
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoDismountVolume` | xboxkrnl 0x3B | 2 | 0 | ❌ |
| `IoDismountVolumeByFileHandle` | xboxkrnl 0x3C | 1 | 0 | ❌ |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 28 | 110 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 4 | 56 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 6 | 20 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 2 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 111 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 4 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 10 | 4 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 3 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 3 | 6 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 5 | 4 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 4 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 10 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 10 | 9 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 0 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 28 | 28 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 0 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 10 | 28 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 9 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 10 | 6 | 🟡 |
| `XamContentResolve` ⭐ | xam 0x268 | 1 | 0 | 🟡 |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 2 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4023 | ⚪ stub |
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
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4024 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 3 | 5 | 🟡 |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 11 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 14 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 3 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 4 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 3 | 2 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 29 | 25 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 16 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 20 | 5 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 14 | 7 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 15 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 4 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 16 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 2542 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 2 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 18 | 12832 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 4 | 2 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 5 | 5 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 5 | 10 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserCheckPrivilege` | xam 0x212 | 1 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 15 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 3 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 3 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (14)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 160 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 2 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 1 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 2 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 32 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 4 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 5 | 1 | ✅ |
| `XamFree` | xam 0x1EC | 10 | 1 | ✅ |

</details>

<details><summary><b>Rede</b> (13)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetGetBroadcastVersionStatus` ⭐ | xam 0x4C | 1 | 1 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 1 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 9 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 1 | ✅ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 11643 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 4016 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 3 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Avatares</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 0 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 8 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 4 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 14 | 11803 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 9 | 8 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 2 | 1 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 1 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 1 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 5 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 4 | 10 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 5 | 701 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 5 | 9 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 16052 | 🟡 |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike, nem o Kinect Sports, nem o Kinect Adventures).

**Ausentes no Xenia (26):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XamShowFriendsUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.
