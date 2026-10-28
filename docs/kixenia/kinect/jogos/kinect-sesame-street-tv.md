# Kinect Sesame Street TV (título 4D5309DA)

Kinect Sesame Street TV, da Microsoft Studios com a Sesame Workshop (2012): episódios interativos da Vila Sésamo em que a criança responde com o corpo e com a voz. Projeto interno **Brooklyn** (`BrooklynX-Retail.pdb`), com a interface em Lua (Havok Script, arquivos `.lub`) e os vídeos dos episódios em pacotes `bundle.dat`. Dois discos em contêiner GOD (SVOD), mesmo título 4D5309DA, no HD externo: `<HD>/Xbox 360/Kinect Sesame Street TV [RF][Kinect][DVD1|DVD2]/`. Extraídos com `xenia-vfs-dump` em `~/Kixenia/kinect-sdk-re/games/extraidos/sesame/` (DVD1, 6,8 GB) e `sesame-dvd2/`.

Runtime NUI **2.0.21119** (`NUI: title runtime 5/527F with flags 0000080B`): entre o do [Kinect-Rush](kinect-rush.md) e do [Kinect-Star-Wars](kinect-star-wars.md) (2.0.21076) e o do [Nike-Kinect-Training](nike-kinect-training.md) e do [Kinect-Sports](kinect-sports.md) (2.0.21173). **Parece mais com o Kinect Rush**: das 35 funções que chamam o Kinect (fora as mensagens XMsg), 29 são iguais às do Rush (mesmas chamadas e mesmo tamanho), 23 às do Star Wars, 10 às do [Kinect-Joy-Ride](kinect-joy-ride.md) e 8 às do [Kinect-Adventures](kinect-adventures.md).

Relacionado: [Kinect-Rush](kinect-rush.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Os dois discos

| | DVD1 | DVD2 |
|---|---|---|
| Executável | `default.xex` (21,2 MB) | `default.xex` do mesmo tamanho: só **297 bytes do cabeçalho** mudam (assinatura e hashes, e o número do disco, 1 → 2). O código é o mesmo. |
| Episódios | `Media/S01/E01`–`E04` e os segmentos deles em `Media/Seg` (introduções, `Murphy…`, `Liss…`, `Ewar…`, `OIV…`, `Passive…`; 5,9 GB) | `Media/S01/E05`–`E08` e os segmentos deles |
| Iguais nos dois | `Media/App` (interface: `appfrontend`, `appgesture`, `appgrammar`, `appaudio`…), `Media/Cmn` (figurinhas, fantasias do Murphy), `database.xmplr` (modelos do rastreador), `nuiidentity.bin.be` (identidade), `nuisp1033`/`2057`/`3081` (voz em inglês dos EUA, do Reino Unido e da Austrália), `nxeart`, `$SystemUpdate` (31 arquivos, 95 MB: atualização do sistema com os apps Kinect, `natalsu.xex`, `nuihud.xex`, `BiometricSetup.xex`…) e o pacote de conteúdo `Content/…/58480870/00000002` (**"More for Kids"**, 108 MB, aberto ao iniciar) | |

Ou seja, o DVD2 é a segunda metade da temporada (episódios 5 a 8); a análise do executável vale para os dois.

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema`, DVD1 extraído). Sem captura de tela: o que aparece na tela não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir | ✅ | "Kinect Sesame TV DVD". 150 s sem travar e **sem `Host-side crash`**; as `RtlRaiseException(0x406D1388)` são só nomes de thread. |
| Runtime NUI | ✅ | `5/527F`, flags 0x80B (as mesmas do Joy Ride). `XamNuiGetTrueColorInfo` é buscada por ordinal em execução e chamada 2 vezes (calibração de cor). |
| Interface (frontend) | ✅? | Carrega `appbootsequence` no quadro 0, a gramática de voz (`appgrammar`) no 9, `appcommon` no 76 e **`appfrontend` no 108**; depois disso não abre mais nada até o fim (quadro 4714, ~31 quadros/s). Deve ter parado no menu/tela inicial esperando a criança, mas sem imagem não dá para confirmar. |
| Rastreamento | 🟡 | 19 de 58 amostras (33%) com o jogador rastreado (Rush 33%, Joy Ride 36%). |
| Inclinação | ✅ (sem motor) | Pede −25° no quadro 6 (sem chão guardado) e 0° no 120; guarda o chão e não recomeça a busca, como o Rush. |
| Voz | 🟡 | Reconhecedor no próprio executável (`NuiSpeech`, `SpeechEngine.lub`): abre `nuisp1033` (tenta `UPDATE:` e cai para `GAME:`) e os arquivos `L1033.*`, `ai031033.am`, `lsr1033.lxa`, `r1033sr.lxa`. Pede `XamVoiceGetMicArrayAudioEx` uma vez (quadro 8); `XAudioRegisterRenderDriverMECClient` não foi chamada. A gravação não tem áudio. |
| Kinect ID (rostos) | ✅ | Abre `NuiIdentityDbVersion.txt`, `NuiIdentityNN.bin.be`, `NuiIdentityPCA.bin.be`; `XamCacheOpenFile` e `XamCacheReset(0x18)` já existem no Xenia (sem "undefined extern call") e lê os rostos do perfil (`XamReadBiometricData`). |
| Avatares, fitness | — | Nenhuma `XamAvatar*` nem `XamFitness*` importada; as mensagens `XamFitness…: 0x%x` do runtime existem, mas nada foi buscado por ordinal em 150 s. |
| Funções ausentes chamadas | 🟡 | "undefined extern call" só para `XamXlfsInitializeUploadQueue`, `EtxProducerRegister`/`Unregister`, `VdRegisterHDCPNotification` e `IoDismountVolumeByFileHandle`: nenhuma do Kinect. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 ses-replay-n3 150 ~/Kixenia/kinect-sdk-re/games/extraidos/sesame/default.xex
```
Também abre direto do contêiner: `".../Kinect Sesame Street TV [RF][Kinect][DVD1]/Content/0000000000000000/4D5309DA/00007000/DFDD95590027BDEC0BC3"`.

## O que o jogo pede do Kinect

| Recurso | Usa? | Como |
|---|---|---|
| Câmera (PsCam) | Sim | Um ponto só (0x826A2728), depois do Kinect Studio (`XamXStudioRequest(0x1003)`); 8911 pedidos em 150 s. Usa a **imagem colorida**: o "Diário" guarda fotos e vídeos da criança (`JournalPhotoAndVideo*`, `IDS_EPISODE_DETAILS_BUTTON_YOUR_PHOTOS`) e há shaders de recorte do jogador (`g_SamplerSegmentation`, `g_BodyPartBasePlayerMaskScale`). |
| Motor de inclinação | Sim | O do Rush (−25°, depois degraus até achar o chão; ±27°), callback 0x8269F6A8. O runtime também expõe `NuiCameraElevationGetAngle`/`StopMovement` como funções próprias. |
| Status 'XtSs' | Sim | Em **0x8320B6E0** (0x50 bytes), lido pelo Kinect Studio (0x1005) ou `XamNuiCameraTiltGetStatus`. Campos lidos (`ses_acessos_base.py`): +0x24/+0x28 (gravidade: ângulo atual), +0x2C (flags do motor; 0x10 recomeça a busca), +0x3C (**chão guardado**, lido como float em 9 funções), +0x40 (bit 0: ocupado), +0x48/+0x4C (gravidade do chão). O mesmo conjunto do Rush e do Star Wars. |
| Esqueleto | Sim | Rastreamento completo; `database.xmplr` e `HeadPosition.xmplr` (posição da cabeça). |
| Identidade (Kinect ID) | Sim | Motor de identidade em 0x826B2688; lê e grava os rostos do perfil. |
| Microfones e voz | Sim | Microfones com cancelamento de eco (MEC) no executável, como no Joy Ride e no Rush, e reconhecimento de voz com gramáticas (`appgrammar`). |
| Avatares | Não | |
| Fitness | Não | (só as mensagens do runtime) |

## Teste com o Kinect real: pendente

O que verificar ao vivo (worktree kinect-sistema, `live_test.sh`, na sessão principal):
1. **Frontend e engajamento**: se o menu aparece e se a criança consegue escolher um episódio com a mão/gesto (cursor do runtime NUI); se o jogo pede para entrar no quadro.
2. **Episódio**: carregar o E01 (DVD1) e ver se os vídeos (`bundle.dat` em `Media/Seg`) tocam e se os trechos interativos reagem ao corpo (pular, apontar, contar).
3. **Voz**: se `XamVoiceGetMicArrayAudioEx` devolve áudio, se `XAudioRegisterRenderDriverMECClient` é chamada e se o jogo reconhece respostas faladas (en-US).
4. **Câmera colorida**: se as fotos do Diário e o recorte do jogador aparecem certos (cor, alinhamento com a profundidade; `XamNuiGetTrueColorInfo` é aproximada no Xenia).
5. **Motor**: se o sensor inclina (−25° e volta) e guarda o chão uma vez só.
6. **DVD2**: abrir o contêiner do DVD2 e conferir que os episódios 5–8 carregam (o jogo pode pedir a troca de disco).

## Pendências
- `XamShowNui*` ausentes importadas: `AchievementsUI`, `CommunitySessionsUI`, `ControllerRequiredUI`, `FriendRequestUI`, `FriendsUI`, `GameInviteUI`, `GamerCardUIForXUID`, `MarketplaceUI`, `PartyUI`; nenhuma chamada em 150 s.
- Muitas ausentes de rede e sistema (`NetDll_Xnp*`, `NetDll_XHttp*`, `Io*`, `Vd*`, `XamBackgroundDownload*`): vêm de bibliotecas ligadas ao executável e não afetam o Kinect.

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/sesame_4D5309DA.img` (Ghidra XEXLoaderWV, `DumpImage.java`); XEX em `games/sesame_4D5309DA.xex`.
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/sesvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática).
- Seções: `.pdata` 0x82232E00 (54 643 funções), `.text` 0x822A0000–0x83185694.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/ses/c_full/<endereço>.c` (35 funções), com os thunks trocados pelos nomes das importações.
- Log da execução: `~/Kixenia/kinect-tools/logs/ses-replay-n3.log`.
- Scripts (`games/decompilado/scripts/`): `ses_imports.py`, `ses_funcs.py`, `ses_runtime.py`, `ses_c.py`, `ses_compara.py`, `ses_acessos_base.py`, `gen_sesame.py`; `DumpImage.java` e `FnGraph.java` em `~/Kixenia/kinect-tools/ghidra/`.

## Chamadas ao sistema (análise de 2026-10-26)

**545 importações** (285 do `xam.xex`, 260 do `xboxkrnl.exe`), 8418 chamadas diretas no código, 142 vistas em execução. Estado no Xenia Edge (worktree kinect-sistema): 322 implementadas, 40 aproximadas, 53 stubs, 129 ausentes, 1 sem marca.

**Como foi feito:** igual ao [Kinect-Rush](kinect-rush.md): imagem plana pelo XEXLoaderWV sem análise; importações e thunks da listagem do log do Xenia; chamadas = `bl`/`b` para cada thunk; a função que contém cada chamada vem da `.pdata`; decompilação pelo Ghidra 12.1.4 (VMX128). Colunas *Rush* e *Adventures*: `=` mesma função (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho.

### Funções do jogo que falam com o Kinect

| Função | Tamanho | Chama | Chamada por | O que faz | Rush | Adventures |
|---|---|---|---|---|---|---|
| 0x8269E650 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 |  | = 0x82963668 | = 0x82d2b3c8 |
| 0x8269E900 | 92 | `XamNuiCameraElevationGetAngle` | 0 | Função pública `NuiCameraElevationGetAngle`: confere os ponteiros e chama `XamNuiCameraElevationGetAngle`. No Rush ela vem embutida no pedido de ajuste. | ≈ 0x82963fb0 (676) | ≈ 0x82d2bcf0 (680) |
| 0x8269E960 | 132 | `XamNuiCameraElevationSetAngle` | 3 |  | = 0x82963918 | — |
| 0x8269EA90 | 144 | `XamNuiCameraElevationStopMovement` | 0 | Função pública `NuiCameraElevationStopMovement`: recusa com o bit 0 de +0x40 (ocupado, E_ACCESSDENIED) ou com um pedido em andamento (0x8301000B). | ≈ 0x82964ef8 (2436) | — |
| 0x8269EBA0 | 76 | `XamNuiCameraTiltGetStatus` | 0 | Lê o status 'XtSs': tenta `XamXStudioRequest(0x1005)` e só se falhar chama `XamNuiCameraTiltGetStatus`. | = 0x829639c0 | ≈ 0x82d2c490 (144) |
| 0x8269EFB0 | 144 | `XamNuiCameraElevationSetAngle` | 3 |  | = 0x82963d48 | = 0x82d2ba88 |
| 0x8269F040 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro de esqueleto: valida o plano do chão e o guarda no sistema (`XamNuiCameraRememberFloor`). | = 0x82963dd8 | = 0x82d2bb18 |
| 0x8269F218 | 676 | `XamNuiCameraElevationGetAngle` | 2 |  | = 0x82963fb0 | ≈ 0x82d2bcf0 (680) |
| 0x8269F6A8 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam; responde com o status (`XamNuiCameraTiltReportStatus`). | = 0x82964258 | = 0x82d2c180 |
| 0x8269FA68 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback do motor (`XamNuiCameraTiltSetCallback(0x8269F6A8)`), depois de `XamXStudioRequest(1)`. | = 0x82964618 | ≈ 0x82d2c520 (80) |
| 0x826A0420 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | Preenche o cabeçalho do 'XtSs' (0x50, 'XtSs' em 0x8320B6E0) e lê o status a cada quadro; usa +0x24/+0x28 (ângulo atual) e +0x2C. | = 0x82964ef8 | — |
| 0x826A2728 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta `XamXStudioRequest(0x1003)` (Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | = 0x82950a60 | = 0x82d1f408 |
| 0x826B0370 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | = 0x8295b5d8 | ≈ 0x82d2f5c8 (392) |
| 0x826B0590 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8295b7f8 | — |
| 0x826B2520 | 356 | `XamUserNuiGetUserIndex` | 1 |  | = 0x8295d788 | ≈ 0x82d30e18 (300) |
| 0x826B3798 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8295e570 | — |
| 0x826B3920 | 3200 | `XamNuiIdentityGetSessionId` | 5 |  | = 0x8295e6f8 | — |
| 0x826C4C40 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8296cea0 | — |
| 0x826C9518 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 |  | = 0x82971270 | = 0x82d30650 |
| 0x826C9760 | 336 | `XamUserNuiGetUserIndex` | 1 |  | = 0x829714b8 | ≈ 0x82d30e18 (300) |
| 0x826C99E0 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Limpeza do cache do Kinect ID ao iniciar o motor de identidade: `XamCacheOpenFile`, `XamCacheReset(0x18)`, `XamCacheCloseFile`. | = 0x82971738 | ≈ 0x82d331a8 (244) |
| 0x826C9AD8 | 488 | `XamReadBiometricData` | 1 |  | = 0x82971830 | ≈ 0x82d332a0 (288) |
| 0x826C9E10 | 708 | `XamReadBiometricData` | 0 | Lê os rostos cadastrados do perfil (`XamReadBiometricData`, buffer de 0x32000 bytes) e confere o formato; maior que a do Rush (708 contra 488 bytes). | ≈ 0x82971830 (488) | ≈ 0x82d332a0 (288) |
| 0x826CA420 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Grava os rostos (`XamWriteBiometricData`) e liga a biometria do perfil (`XamUserNuiEnableBiometric`). | = 0x82971e48 | ≈ 0x82d31938 (568) |
| 0x826FA270 | 224 | `XamNuiGetDeviceSerialNumber` | 1 |  | = 0x82eba480 | — |
| 0x826FA4D8 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | Callback de áudio do MEC, a cada quadro do driver de som: confere o underrun, lê `XamVoiceGetMicArrayAudioEx` e captura o que sai dos alto-falantes (`XAudioCaptureRenderDriverFrame`). | = 0x82eba668 | — |
| 0x826FB488 | 1252 | `XAudioUnregisterRenderDriverMECClient` | 4 |  | = 0x82ebb528 | — |
| 0x826FBA90 | 1504 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | **Liga os microfones e o MEC** (cancelamento de eco): com os bits 1/2 do pedido, carrega os coeficientes de calibração, abre `XamVoiceGetMicArrayAudioEx` e registra o cliente no driver de som (`XAudioRegisterRenderDriverMECClient`). | = 0x82ebba88 | — |
| 0x82729698 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 |  | = 0x82ed3608 | — |
| 0x82729760 | 124 | `XamNuiGetDeviceStatus` | 1 | A cada 60 s lê o estado do sensor (`XamNuiGetDeviceStatus`) e guarda o bit 3 das flags (superaquecimento). | = 0x82ed36d0 | ≈ 0x831ae010 (40) |
| 0x828E8240 | 56 | `XamShowNuiMessageBoxUI` | 1 |  | — | — |
| 0x828E8288 | 28 | `XamShowNuiDirtyDiscErrorUI` | 1 |  | — | = 0x831ae988 |
| 0x828E82E0 | 68 | `XamShowVoiceMailUI` | 0 |  | — | — |
| 0x828ED7F0 | 40 | `XamNuiGetDeviceStatus` | 7 |  | = 0x829473f0 | = 0x831ae010 |
| 0x8290CD90 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | = 0x82d474c0 | ≈ 0x82d93ed8 (224) |

### Importações do Kinect, NUI, voz e mensagens

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8911 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 2 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 1 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 80 | 3504 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 280 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 41 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 13 | 0 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | — (alta freq.) | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | — (alta freq.) | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | — (alta freq.) | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 20 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 3 | 1 | ✅ |
| `XamShowGameVoiceChannelUI` | xam 0x3C5 | 2 | 0 | ❌ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiCommunitySessionsUI` | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendRequestUI` | xam 0x479 | 1 | 0 | ❌ |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGameInviteUI` | xam 0x473 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 1 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Rede</b> (117)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` ⭐ | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` ⭐ | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` ⭐ | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` ⭐ | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` ⭐ | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASend` ⭐ | xam 0x17 | 1 | 0 | ❌ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` ⭐ | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` ⭐ | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XHttpCloseHandle` ⭐ | xam 0xCC | 1 | 0 | ❌ |
| `NetDll_XHttpConnect` ⭐ | xam 0xCD | 1 | 0 | ❌ |
| `NetDll_XHttpCrackUrl` ⭐ | xam 0xDC | 1 | 0 | ✅ |
| `NetDll_XHttpCrackUrlW` ⭐ | xam 0xDB | 1 | 0 | ❌ |
| `NetDll_XHttpCreateUrl` ⭐ | xam 0xDD | 1 | 0 | ❌ |
| `NetDll_XHttpCreateUrlW` ⭐ | xam 0xDE | 1 | 0 | ❌ |
| `NetDll_XHttpDoWork` ⭐ | xam 0xD8 | 1 | 0 | ❌ |
| `NetDll_XHttpOpen` ⭐ | xam 0xCB | 1 | 0 | ⚪ stub |
| `NetDll_XHttpOpenRequest` ⭐ | xam 0xCF | 1 | 0 | ❌ |
| `NetDll_XHttpOpenRequestUsingMemory` ⭐ | xam 0xD0 | 1 | 0 | ❌ |
| `NetDll_XHttpQueryAuthSchemes` ⭐ | xam 0xDA | 1 | 0 | ❌ |
| `NetDll_XHttpQueryHeaders` ⭐ | xam 0xD3 | 1 | 0 | ❌ |
| `NetDll_XHttpQueryOption` ⭐ | xam 0xD6 | 1 | 0 | ❌ |
| `NetDll_XHttpReadData` ⭐ | xam 0xD4 | 1 | 0 | ❌ |
| `NetDll_XHttpReceiveResponse` ⭐ | xam 0xD2 | 1 | 0 | ❌ |
| `NetDll_XHttpSendRequest` ⭐ | xam 0xD1 | 1 | 0 | ❌ |
| `NetDll_XHttpSetCredentials` ⭐ | xam 0xD9 | 1 | 0 | ❌ |
| `NetDll_XHttpSetOption` ⭐ | xam 0xD7 | 1 | 0 | ❌ |
| `NetDll_XHttpSetStatusCallback` ⭐ | xam 0xCE | 1 | 0 | ❌ |
| `NetDll_XHttpShutdown` ⭐ | xam 0xCA | 1 | 0 | ⚪ stub |
| `NetDll_XHttpStartup` ⭐ | xam 0xC9 | 1 | 0 | ⚪ stub |
| `NetDll_XHttpWriteData` ⭐ | xam 0xD5 | 1 | 0 | ❌ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetCreateKey` ⭐ | xam 0x36 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetBroadcastVersionStatus` ⭐ | xam 0x4C | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` ⭐ | xam 0x4A | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 2 | ✅ |
| `NetDll_XNetGetOpt` | xam 0x4E | 1 | 0 | 🟡 |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 4559 | ✅ |
| `NetDll_XNetInAddrToServer` ⭐ | xam 0x3D | 1 | 0 | ❌ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosGetListenStats` | xam 0x4D | 1 | 0 | ❌ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 1 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetOpt` ⭐ | xam 0x4F | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetTsAddrToInAddr` ⭐ | xam 0x3B | 1 | 0 | ❌ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpCapture` ⭐ | xam 0x6C | 1 | 0 | ❌ |
| `NetDll_XnpConfig` ⭐ | xam 0x68 | 1 | 0 | ❌ |
| `NetDll_XnpConfigUPnP` ⭐ | xam 0x67 | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptRecv` ⭐ | xam 0x6F | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptSetCallbacks` ⭐ | xam 0x6D | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptXmit` ⭐ | xam 0x6E | 1 | 0 | ❌ |
| `NetDll_XnpGetActiveSocketList` ⭐ | xam 0x7A | 1 | 0 | ❌ |
| `NetDll_XnpGetConfigStatus` ⭐ | xam 0x69 | 1 | 0 | ❌ |
| `NetDll_XnpGetVlanXboxName` ⭐ | xam 0x78 | 1 | 0 | ❌ |
| `NetDll_XnpLoadConfigParams` ⭐ | xam 0x65 | 1 | 0 | ❌ |
| `NetDll_XnpLoadMachineAccount` ⭐ | xam 0x6A | 1 | 0 | ❌ |
| `NetDll_XnpLogonClearQEvent` ⭐ | xam 0x74 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQFlags` ⭐ | xam 0x71 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQVals` ⭐ | xam 0x75 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetPState` ⭐ | xam 0x77 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQEvent` ⭐ | xam 0x73 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQFlags` ⭐ | xam 0x72 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQVals` ⭐ | xam 0x76 | 1 | 0 | ❌ |
| `NetDll_XnpNoteSystemTime` ⭐ | xam 0x7B | 1 | 0 | ❌ |
| `NetDll_XnpRegisterKeyForCallerType` ⭐ | xam 0x7C | 1 | 0 | ❌ |
| `NetDll_XnpSaveConfigParams` ⭐ | xam 0x66 | 1 | 0 | ❌ |
| `NetDll_XnpSaveMachineAccount` ⭐ | xam 0x6B | 1 | 0 | ❌ |
| `NetDll_XnpSetVlanXboxName` ⭐ | xam 0x79 | 1 | 0 | ❌ |
| `NetDll_XnpUnregisterKeyForCallerType` ⭐ | xam 0x7D | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` ⭐ | xam 0xA | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XNetLogonGetMachineID` ⭐ | xam 0x135 | 55 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 38 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (79)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscGetCacheElementCount` ⭐ | xboxkrnl 0x20 | 1 | 0 | ⚪ stub |
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 1 | ⚪ stub |
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoDismountVolume` | xboxkrnl 0x3B | 3 | 0 | ❌ |
| `IoDismountVolumeByFileHandle` | xboxkrnl 0x3C | 1 | 0 | ❌ |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 37 | 98 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 5 | 119 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 8 | 27 | ✅ |
| `NtCreateIoCompletion` | xboxkrnl 0xD3 | 1 | 1 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 2 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 343 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 4 | 4 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 4 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 14 | 5 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 1 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 3 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 3 | 10 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 4 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 9 | 4 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 4 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtRemoveIoCompletion` | xboxkrnl 0xF4 | 1 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 13 | 0 | ✅ |
| `NtSetIoCompletion` | xboxkrnl 0xF8 | 1 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 11 | 9 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 3 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 4 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 5 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 56 | 31913 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByName` ⭐ | xboxkrnl 0x10D | 3 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 2 | 4 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 21 | 31913 | ✅ |
| `ObTranslateSymbolicLink` ⭐ | xboxkrnl 0x113 | 1 | 0 | ❌ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamBackgroundDownloadItemToContentData` ⭐ | xam 0x9CE | 1 | 0 | ❌ |
| `XamBackgroundDownloadSelectDevice` ⭐ | xam 0x9DC | 1 | 0 | ❌ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentAggregateCreateEnumerator` ⭐ | xam 0x279 | 3 | 1 | ⚪ stub |
| `XamContentClose` | xam 0x25A | 12 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 2 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 6 | 1 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 2 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDefaultDevice` ⭐ | xam 0x284 | 2 | 0 | ❌ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 19 | ✅ |
| `XamContentGetDeviceName` | xam 0x25F | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 2 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentGetThumbnail` ⭐ | xam 0x261 | 1 | 0 | ✅ |
| `XamContentInstall` ⭐ | xam 0x26A | 1 | 0 | ❌ |
| `XamContentLaunchImage` | xam 0x263 | 1 | 0 | ❌ |
| `XamContentOpenFile` | xam 0x269 | 10 | 6 | 🟡 |
| `XamContentResolve` ⭐ | xam 0x268 | 2 | 0 | 🟡 |
| `XamContentResolveInternal` ⭐ | xam 0x276 | 1 | 0 | 🟡 |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (75)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAcquireReadWriteLockExclusive` ⭐ | xboxkrnl 0x7 | 3 | 1 | ✅ |
| `ExCreateThread` | xboxkrnl 0xD | 4 | 34 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExInitializeReadWriteLock` ⭐ | xboxkrnl 0x11 | 2 | 1 | ✅ |
| `ExReleaseReadWriteLock` ⭐ | xboxkrnl 0x16 | 3 | 1 | ✅ |
| `ExSemaphoreObjectType` ⭐ | xboxkrnl 0x17 | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 6 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `ExTimerObjectType` ⭐ | xboxkrnl 0x1C | 0 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 2 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 14 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 10 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 1 | 1 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 110 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeEvent` ⭐ | xboxkrnl 0x70 | 1 | 0 | ✅ |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 1 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 8 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 4 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 6 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 17 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 10441 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 31 | 30391 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3481 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3481 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 32 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 73 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` ⭐ | xboxkrnl 0x9C | 1 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 108 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 4 | 10 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 3 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 2 | 1 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 25 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 17 | 145 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 2 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 8 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 33 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 60 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 58 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 1 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 20 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` ⭐ | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 9 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 2354 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 350 | 416 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 9 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 2700 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 2 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 6 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 20 | 0 | ✅ |
| `XexUnloadImageAndExitThread` ⭐ | xboxkrnl 0x1A2 | 1 | 0 | ❌ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (45)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamQueryLiveHiveA` ⭐ | xam 0x70C | 1 | 0 | ⚪ stub |
| `XamQueryLiveHiveW` ⭐ | xam 0x70D | 2 | 0 | ⚪ stub |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowCustomMessageComposeUI` ⭐ | xam 0x2E5 | 1 | 0 | ❌ |
| `XamShowCustomPlayerListUI` | xam 0x2E6 | 1 | 0 | ❌ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowGoldUpgradeUI` | xam 0x6A5 | 1 | 0 | ⚪ stub |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMarketplaceUI` | xam 0x2C7 | 2 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowOptionalMediaUpdateRequiredUI` ⭐ | xam 0x7D6 | 2 | 0 | ❌ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowPlayersUI` | xam 0x2C8 | 1 | 0 | ❌ |
| `XamShowQuickChatUI` ⭐ | xam 0x2C2 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamShowSigninUIp` ⭐ | xam 0x2BE | 1 | 0 | ✅ |
| `XamShowWordRegisterUI` ⭐ | xam 0x3C0 | 1 | 0 | ❌ |
| `XamUserAreUsersFriends` | xam 0x213 | 1 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 2 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetAge` ⭐ | xam 0x4A4 | 1 | 0 | ❌ |
| `XamUserGetAgeGroup` ⭐ | xam 0x4A5 | 1 | 0 | ✅ |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetMembershipTierFromXUID` ⭐ | xam 0x217 | 3 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` ⭐ | xam 0x218 | 10 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 36484 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 35 | 1 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 2 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (38)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableHDCP` ⭐ | xboxkrnl 0x271 | 1 | 0 | ❌ |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetClosedCaptionReadyStatus` ⭐ | xboxkrnl 0x1B8 | 1 | 0 | ❌ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4716 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 1 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 1 | 🟡 |
| `VdQuerySystemCommandBuffer` ⭐ | xboxkrnl 0x1C8 | 1 | 0 | ❌ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRegisterHDCPNotification` ⭐ | xboxkrnl 0x272 | 1 | 0 | ❌ |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4717 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSendClosedCaptionData` ⭐ | xboxkrnl 0x1CF | 1 | 0 | ❌ |
| `VdSetCGMSState` ⭐ | xboxkrnl 0x2D6 | 1 | 0 | ❌ |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 3 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetMacrovisionOption` ⭐ | xboxkrnl 0x1D7 | 1 | 0 | ❌ |
| `VdSetSCMSState` ⭐ | xboxkrnl 0x2D7 | 1 | 0 | ❌ |
| `VdSetSystemCommandBuffer` ⭐ | xboxkrnl 0x1D8 | 1 | 0 | ❌ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoCapabilities` | xam 0x49E | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 11 | 13 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 3 | 0 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (36)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 24 | 18 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 15 | 3 | ✅ |
| `HalGetCurrentAVPack` ⭐ | xboxkrnl 0x22 | 6 | 1 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 4 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 0 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 1 | ✅ |
| `XamGetAppliesTo` ⭐ | xam 0x4C0 | 1 | 0 | ❌ |
| `XamGetCurrentTitleId` | xam 0x1CF | 21 | 1 | ✅ |
| `XamGetDashBackstackData` ⭐ | xam 0x7DC | 1 | 1 | ✅ |
| `XamGetDashBackstackNodesCount` ⭐ | xam 0x7DD | 2 | 0 | ❌ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetLanguage` ⭐ | xam 0x3D2 | 2 | 1 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 1 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 18 | 1 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 9 | 3485 | ✅ |
| `XamGetToken` ⭐ | xam 0x4BE | 1 | 0 | ❌ |
| `XamIsUIActive` | xam 0x2EC | 4 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 3 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 0 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 3 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 0 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 20 | 20552 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 9 | 2 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 8 | 9 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 2 | 2 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 9 | 12 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 5 | 2 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 3 | 1 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (34)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 21 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 18 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 6 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 5 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 10 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 81 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 2 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 4 | 3 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 40 | 33 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 5 | 0 | ✅ |
| `RtlLowerChar` ⭐ | xboxkrnl 0x132 | 2 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 21 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 5 | 4 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 2 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `RtlUpperChar` ⭐ | xboxkrnl 0x14A | 2 | 0 | ✅ |
| `XamReadString` ⭐ | xam 0x2F6 | 1 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 26 | 5 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 15 | 0 | ✅ |
| `_vscwprintf` ⭐ | xboxkrnl 0x14F | 2 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 6 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 9 | 0 | ✅ |
| `swprintf` ⭐ | xboxkrnl 0x13E | 8 | 0 | ✅ |
| `vsprintf` ⭐ | xboxkrnl 0x14E | 1 | 0 | ✅ |
| `vswprintf` ⭐ | xboxkrnl 0x151 | 2 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptAesCbc` ⭐ | xboxkrnl 0x15B | 1 | 0 | ✅ |
| `XeCryptAesKey` ⭐ | xboxkrnl 0x159 | 1 | 0 | ✅ |
| `XeCryptBnQwBeSigVerify` ⭐ | xboxkrnl 0x166 | 1 | 0 | ⚪ stub |
| `XeCryptRandom` | xboxkrnl 0x18A | 4 | 3 | ⚪ stub |
| `XeCryptRotSumSha` ⭐ | xboxkrnl 0x18E | 1 | 0 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 8 | 1 | ✅ |
| `XeCryptSha256Final` ⭐ | xboxkrnl 0x312 | 2 | 38 | ✅ |
| `XeCryptSha256Init` ⭐ | xboxkrnl 0x310 | 2 | 38 | ✅ |
| `XeCryptSha256Update` ⭐ | xboxkrnl 0x311 | 2 | 38 | ✅ |
| `XeCryptSha384Final` ⭐ | xboxkrnl 0x318 | 1 | 0 | ❌ |
| `XeCryptSha384Init` ⭐ | xboxkrnl 0x314 | 1 | 0 | ❌ |
| `XeCryptSha384Update` ⭐ | xboxkrnl 0x315 | 1 | 0 | ❌ |
| `XeCryptSha512Final` ⭐ | xboxkrnl 0x324 | 1 | 0 | ✅ |
| `XeCryptSha512Init` ⭐ | xboxkrnl 0x31F | 1 | 0 | ✅ |
| `XeCryptSha512Update` ⭐ | xboxkrnl 0x323 | 1 | 0 | ✅ |
| `XeCryptShaFinal` ⭐ | xboxkrnl 0x191 | 3 | 0 | ✅ |
| `XeCryptShaInit` ⭐ | xboxkrnl 0x18F | 5 | 0 | ✅ |
| `XeCryptShaUpdate` ⭐ | xboxkrnl 0x190 | 15 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 1 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 2 | 0 | ✅ |
| `XeKeysGetKey` ⭐ | xboxkrnl 0x244 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Memória</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 769 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 6 | 8 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 18 | 1 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 7 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 12 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 426 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 15 | 217 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 15 | 86 | ✅ |
| `NtProtectVirtualMemory` ⭐ | xboxkrnl 0xE1 | 2 | 8 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 252 | 1 | ✅ |
| `XamAllocEx` ⭐ | xam 0x1EB | 3 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 273 | 0 | ✅ |
| `XamFreeToken` ⭐ | xam 0x4BF | 3 | 0 | ❌ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiCommunitySessionsUI` | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendRequestUI` ⭐ | xam 0x479 | 1 | 0 | ❌ |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGameInviteUI` ⭐ | xam 0x473 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiMessageBoxUI` ⭐ | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8911 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 20 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Áudio</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 4 | 3 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSetSpeakerConfig` ⭐ | xboxkrnl 0x200 | 4 | 0 | ❌ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XGetAudioFlags` ⭐ | xam 0x3CA | 3 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Outras</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAppUnloadStack` ⭐ | xam 0x246 | 1 | 0 | ❌ |
| `XamBackgroundDownloadBindItems` ⭐ | xam 0x9D6 | 1 | 0 | ❌ |
| `XamBackgroundDownloadGetActiveItem` ⭐ | xam 0x9C6 | 3 | 0 | ❌ |
| `XamBackgroundDownloadItemAdd` ⭐ | xam 0x9C9 | 1 | 0 | ❌ |
| `XamBackgroundDownloadItemGetHistoryStatus` ⭐ | xam 0x9D1 | 1 | 0 | ❌ |
| `XamBackgroundDownloadItemGetStatus` ⭐ | xam 0x9CF | 1 | 0 | ❌ |
| `XamBackgroundDownloadItemMakeFirst` ⭐ | xam 0x9CB | 2 | 0 | ❌ |
| `XamBackgroundDownloadSetMode` ⭐ | xam 0x9C4 | 2 | 0 | ❌ |
| `XamFeatureEnabled` ⭐ | xam 0x200 | 1 | 0 | ✅ |
| `XamIsOptionalMediaUpdateInstalled` ⭐ | xam 0x7D5 | 1 | 0 | ❌ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 13 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 16 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 8 | 1 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 7 | ✅ |
| `XamNotifyCreateListenerInternal` ⭐ | xam 0x291 | 1 | 0 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 2 | 1 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Voz e microfones</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamShowGameVoiceChannelUI` ⭐ | xam 0x3C5 | 2 | 0 | ❌ |
| `XamShowVoiceMailUI` ⭐ | xam 0x2C3 | 1 | 0 | ❌ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 1 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 2 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 1 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 80 | 3504 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 280 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 41 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 13 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 3 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 2 | 4558 | ✅ |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 2 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 13 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 736 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 3 | 10 | ✅ |

</details>

⭐ = nenhum dos jogos comparados (Adventures, Joy Ride, Rush, Star Wars) importa.

**Ausentes no Xenia (129):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XHttpCloseHandle`, `NetDll_XHttpConnect`, `NetDll_XHttpCrackUrlW`, `NetDll_XHttpCreateUrl`, `NetDll_XHttpCreateUrlW`, `NetDll_XHttpDoWork`, `NetDll_XHttpOpenRequest`, `NetDll_XHttpOpenRequestUsingMemory`, `NetDll_XHttpQueryAuthSchemes`, `NetDll_XHttpQueryHeaders`, `NetDll_XHttpQueryOption`, `NetDll_XHttpReadData`, `NetDll_XHttpReceiveResponse`, `NetDll_XHttpSendRequest`, `NetDll_XHttpSetCredentials`, `NetDll_XHttpSetOption`, `NetDll_XHttpSetStatusCallback`, `NetDll_XHttpWriteData`, `NetDll_XNetInAddrToServer`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetServerToInAddr`, `NetDll_XNetSetOpt`, `NetDll_XNetTsAddrToInAddr`, `NetDll_XnpCapture`, `NetDll_XnpConfig`, `NetDll_XnpConfigUPnP`, `NetDll_XnpEthernetInterceptRecv`, `NetDll_XnpEthernetInterceptSetCallbacks`, `NetDll_XnpEthernetInterceptXmit`, `NetDll_XnpGetActiveSocketList`, `NetDll_XnpGetConfigStatus`, `NetDll_XnpGetVlanXboxName`, `NetDll_XnpLoadConfigParams`, `NetDll_XnpLoadMachineAccount`, `NetDll_XnpLogonClearQEvent`, `NetDll_XnpLogonGetQFlags`, `NetDll_XnpLogonGetQVals`, `NetDll_XnpLogonGetStatus`, `NetDll_XnpLogonSetPState`, `NetDll_XnpLogonSetQEvent`, `NetDll_XnpLogonSetQFlags`, `NetDll_XnpLogonSetQVals`, `NetDll_XnpNoteSystemTime`, `NetDll_XnpRegisterKeyForCallerType`, `NetDll_XnpSaveConfigParams`, `NetDll_XnpSaveMachineAccount`, `NetDll_XnpSetVlanXboxName`, `NetDll_XnpUnregisterKeyForCallerType`, `NtWriteFileGather`, `ObIsTitleObject`, `ObTranslateSymbolicLink`, `RtlCaptureContext`, `RtlUnwind`, `VdEnableHDCP`, `VdGetClosedCaptionReadyStatus`, `VdQuerySystemCommandBuffer`, `VdRegisterHDCPNotification`, `VdSendClosedCaptionData`, `VdSetCGMSState`, `VdSetMacrovisionOption`, `VdSetSCMSState`, `VdSetSystemCommandBuffer`, `XAudioSetSpeakerConfig`, `XamAppUnloadStack`, `XamBackgroundDownloadBindItems`, `XamBackgroundDownloadGetActiveItem`, `XamBackgroundDownloadItemAdd`, `XamBackgroundDownloadItemGetHistoryStatus`, `XamBackgroundDownloadItemGetStatus`, `XamBackgroundDownloadItemMakeFirst`, `XamBackgroundDownloadItemToContentData`, `XamBackgroundDownloadSelectDevice`, `XamBackgroundDownloadSetMode`, `XamContentGetDefaultDevice`, `XamContentInstall`, `XamContentLaunchImage`, `XamFreeToken`, `XamGetAppliesTo`, `XamGetDashBackstackNodesCount`, `XamGetToken`, `XamIsOptionalMediaUpdateInstalled`, `XamShowCustomMessageComposeUI`, `XamShowCustomPlayerListUI`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGameVoiceChannelUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowNuiAchievementsUI`, `XamShowNuiCommunitySessionsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendRequestUI`, `XamShowNuiFriendsUI`, `XamShowNuiGameInviteUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamShowNuiPartyUI`, `XamShowOptionalMediaUpdateRequiredUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamShowQuickChatUI`, `XamShowVoiceMailUI`, `XamShowWordRegisterUI`, `XamUserGetAge`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeCryptSha384Final`, `XeCryptSha384Init`, `XeCryptSha384Update`, `XeKeysConsoleSignatureVerification`, `XexUnloadImageAndExitThread`, `__C_specific_handler`.
