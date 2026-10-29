# Crossboard 7 (título 4B4E082C)

**No HD:** `Crossboard 7` (pacote GoD (cabeçalho em `Content/…/00007000/`)). **Executável:** 12416 KB de imagem, base 0x82000000, entrada 0x824E4F90, 28020 funções no `.pdata`, `.text` 0x821d0000–0x8291BDE4.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Bateria ao vivo (Kinect real, log nível 3, `Crossboard_7.log`): rodou o tempo todo sem travar; 1 de 24 amostras com jogador rastreado.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Nome no XEX | | : クロスボード７ |
| Runtime NUI | ✅ | iniciado: `5/2DFF with flags 0000000B, HUD on` |
| Kinect ID | ✅ | motor de identidade iniciado |
| Arquivos do pacote | ✅ | abre arquivos de dentro da imagem (Kinect ID, bancos do rastreador) |

## Como o jogo usa o Kinect

- **Câmera e sensor:** 2 importações (`PsCamDeviceRequest`, `XamNuiCameraRememberFloor`); 0 funções próprias e 2 do runtime; 10735 chamadas vistas em execução.
- **Inclinação e motor:** 6 importações (`XamNuiCameraElevationGetAngle`, `XamNuiCameraElevationSetAngle`, `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus`, `XamNuiCameraTiltReportStatus`, `XamNuiCameraTiltSetCallback`); 0 funções próprias e 9 do runtime; 74 chamadas vistas em execução.
- **Kinect ID e biometria:** 6 importações (`XamNuiIdentityGetSessionId`, `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamUserNuiGetEnrollmentIndex`, `XamUserNuiGetUserIndex`, `XamWriteBiometricData`); 0 funções próprias e 9 do runtime; 2 chamadas vistas em execução.
- **Voz e microfones:** 5 importações (`XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`); 0 funções próprias e 7 do runtime; 1 chamadas vistas em execução; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`**.
- **Telas Nui do sistema:** 5 importações (`XamShowNuiDeviceSelectorUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiGuideUI`, `XamShowNuiSigninUI`, `XamShowNuiTroubleshooterUI`); 0 funções próprias e 0 do runtime.
- **Avatares:** 9 importações (`XamAvatarGenerateMipMaps`, `XamAvatarGetAssets`, `XamAvatarGetAssetsResultSize`, `XamAvatarGetManifestLocalUser`, `XamAvatarGetMetadataRandom`, `XamAvatarInitialize`, `XamAvatarLoadAnimation`, `XamAvatarManifestGetBodyType` e mais 1); 0 funções próprias e 7 do runtime; 1 chamadas vistas em execução; **faltam/stub: `XamAvatarGenerateMipMaps`**.
- **Mensagens aos apps do sistema (XMsg):** 4 importações (`XMsgCancelIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgSystemProcessCall`); 0 funções próprias e 32 do runtime; 2553 chamadas vistas em execução.
- **Cache do xam:** 3 importações (`XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`); 0 funções próprias e 1 do runtime; 3 chamadas vistas em execução.

## Chamadas ao sistema

**255 importações** (89 do `xam.xex`, 166 do `xboxkrnl.exe`), 1339 chamadas diretas no código, 111 vistas em execução. Estado no Xenia Edge (kixenia): 181 implementadas, 29 aproximadas, 32 stubs, 12 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 64 funções que chamam o sistema, **64 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **0 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

<details><summary><b>Runtime NUI embutido</b> (64 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x824E3170 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x824E31E8 | 140 | `XMsgStartIORequest` | 1 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x824E3290 | 124 | `XMsgStartIORequest` | 0 | XMsg: 0xB0020 ao app 0xFB (Live) | ✅ |
| 0x824E3E88 | 68 | `XMsgInProcessCall` | 1 | XMsg: 0x21012 ao app 0xFE (xam) | ✅ |
| 0x824E3FF0 | 56 | `XMsgSystemProcessCall` | 3 | XMsg: 0x2B001 (NUI) | ✅ |
| 0x826DC5A0 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x8274CB70 | 220 | `XamAvatarLoadAnimation` | 1 |  | ✅ |
| 0x8274D0C0 | 248 | `XamAvatarGetAssets` | 2 |  | ✅ |
| 0x8274D1B8 | 172 | `XamAvatarGenerateMipMaps` | 1 |  | ✅ |
| 0x8274D268 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | ✅ |
| 0x8274D300 | 168 | `XamAvatarGetMetadataRandom` | 1 |  | ✅ |
| 0x8274E6D0 | 124 | `XamAvatarShutdown` | 2 |  | ✅ |
| 0x8274E750 | 372 | `XamAvatarInitialize` | 1 |  | ✅ |
| 0x82762018 | 96 | `PsCamDeviceRequest` | 0 |  | ✅ |
| 0x8276A690 | 392 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada; `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x8276A818 | 340 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x8276B718 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | `XamUserNuiGetEnrollmentIndex`: +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| 0x8276BEE0 | 300 | `XamUserNuiGetUserIndex` | 0 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x8276CCB8 | 568 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes; `XamUserNuiEnableBiometric`: configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| 0x8276CEF0 | 3868 | `XamNuiIdentityGetSessionId` | 7 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x8276E528 | 244 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 |  | ✅ |
| 0x8276E620 | 288 | `XamReadBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| 0x8276F208 | 672 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x827710E0 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82771390 | 112 | `XamNuiCameraElevationSetAngle` | 2 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82771780 | 144 | `XamNuiCameraElevationSetAngle` | 3 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82771810 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraRememberFloor`: guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes); `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x827719E8 | 680 | `XamNuiCameraElevationGetAngle` | 1 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82771C90 | 780 | `XamNuiCameraTiltReportStatus` | 0 | `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82771FA0 | 144 | `XamNuiCameraTiltGetStatus` | 2 | `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x82772030 | 80 | `XamNuiCameraTiltSetCallback` | 1 | `XamNuiCameraTiltSetCallback`: 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| 0x82772830 | 2456 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x82778798 | 976 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x8278AFC8 | 888 | `XamVoiceHeadsetPresent` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced* | ✅ |
| 0x8278B5E0 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x8278B970 | 600 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x8278BCD0 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x8278BDC0 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x827901E8 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x8289E638 | 244 | `XMsgInProcessCall` | 2 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x8289E838 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x8289E998 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 1 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8289EBE0 | 432 | `XMsgStartIORequest` | 1 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x8289ED90 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x8289EE40 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x8289EEF0 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x8289EF98 | 156 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x8289F038 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x8289F0E8 | 192 | `XMsgStartIORequest` | 1 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x828A0BF8 | 100 | `XMsgInProcessCall` | 0 | XMsg: 0x70009 ao app 0xFA | ✅ |
| 0x828B8DA0 | 40 | `XMsgCancelIORequest` | 1 |  | ✅ |
| 0x828B8DD8 | 60 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C001 ao app 0xFE (xam) | ✅ |
| 0x828B8E18 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C003 ao app 0xFE (xam) | ✅ |
| 0x828B8E60 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C004 ao app 0xFE (xam) | ✅ |
| 0x828B8EA8 | 64 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C005 ao app 0xFE (xam) | ✅ |
| 0x828B8EE8 | 56 | `XMsgInProcessCall` | 2 | XMsg: registra o callback de identidade (0x2C009) | ✅ |
| 0x828B8F20 | 76 | `XMsgInProcessCall` | 9 | XMsg: dados de identidade (0x2C00C) | ✅ |
| 0x828B8F70 | 68 | `XMsgInProcessCall` | 1 | XMsg: resolve o cadastro (0x2C00D) | ✅ |
| 0x828B8FB8 | 80 | `XMsgStartIORequest` | 1 | XMsg: biometria desligada (0x2A009) | ✅ |
| 0x828B9008 | 112 | `XMsgStartIORequest` | 2 | XMsg: lê o cache biométrico (0x2A008) | ✅ |
| 0x828B9078 | 100 | `XMsgStartIORequest` | 2 | XMsg: grava o cache biométrico (0x2A007) | ✅ |
| 0x828B90E0 | 80 | `XMsgStartIORequest` | 2 | XMsg: 0x2A006 ao app 0xFE (xam) | ✅ |
| 0x828B9130 | 112 | `XMsgStartIORequest` | 1 | XMsg: 0x2A005 ao app 0xFE (xam) | ✅ |
| 0x828B91A0 | 100 | `XMsgStartIORequest` | 1 | XMsg: 0x2A004 ao app 0xFE (xam) | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 10735 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 1 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 11 | 2546 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 17 | 5 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | 🟡 |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ✅ |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | 🟡 |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ✅ |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ✅ |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | 🟡 |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 1 | 0 | ✅ |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | 🟡 |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 2 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 71 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 1 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (56)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 19 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 312 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 55 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 1 | 🟡 |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 1 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 2 | 2 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 9 | 2514 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 1470 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 1470 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 17 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 4 | 14 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 62 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 40 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 3 | 39 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 8 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 9 | 81 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 4 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 16 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 38 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 50 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 243825 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 14 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 4 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 222 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 26 | 171 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 2 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 298 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 1 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 2 | 6 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 6 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (38)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 1 | ⚪ stub |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 6 | 214 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 3 | 129 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 1 | 20 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 11 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 13 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 1 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 3 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 1 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 8 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 2 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 3 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 1 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 2 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 3 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 29 | 2087 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 3 | 2 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 13 | 2089 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 2 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 3 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 1 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 4 | 4 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 1562 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 1563 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 7 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (23)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 3 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 1 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 1 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 7 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 9 | 11 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 17 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |
| `vsprintf` | xboxkrnl 0x14E | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (20)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 2 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 4 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 7 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 1 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 2 | 1 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 72403 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 10 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 2 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 5 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (19)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 5 | 3 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 1 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 5 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 2 | 1 | ✅ |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 10 | 13736 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 2 | 3 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 3 | 3 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 505 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 175 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 1 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 8 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 230 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 71 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 3 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 4 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 10735 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 71 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |

</details>

<details><summary><b>Avatares</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | 🟡 |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ✅ |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | 🟡 |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ✅ |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ✅ |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | 🟡 |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 1 | 0 | ✅ |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | 🟡 |

</details>

<details><summary><b>Rede</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 3 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 3 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 1 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 1 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 11 | 2546 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 17 | 5 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |

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
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 3 | 1 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 4 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 4 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 5 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 4690 | 🟡 |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 1 | 0 | ⚪ stub |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

**Ausentes no Xenia (12):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `NetDll_XnpLogonGetStatus`, `RtlCaptureContext`, `RtlUnwind`, `XamShowGamerCardUIForXUID`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XamAvatarGenerateMipMaps`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`.
- Aproximadas e chamadas em execução: `XamCacheOpenFile`, `XamCacheReset`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/cros7/` (`cros7.img`, `cros7_imports.json`, `cros7_kinect.json`, `cros7_fngraph.json`, `c_full/<endereço>.c`: 118 funções).
- Projeto Ghidra: `games/decompilado/cros7/ghidra/cros7` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
- Log da bateria: `~/Kixenia/kinect-tools/bateria/saida-primeira/Crossboard_7.log`.
