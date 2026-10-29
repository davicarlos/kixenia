# Your Shape Fitness Evolved (título 55530886)

**No HD:** `Your Shape Fitness Evolved` (`default.xex` solto). **Executável:** 21376 KB de imagem, base 0x82000000, entrada 0x826B9EF8, 62021 funções no `.pdata`, `.text` 0x82240000–0x83312AA4.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Bateria ao vivo (Kinect real, log nível 3, `Your_Shape_Fitness_Evolved.log`): rodou o tempo todo sem travar; 0 de 33 amostras com jogador rastreado.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Nome no XEX | | : Your Shape: FE 2012 |
| Runtime NUI | ✅ | iniciado: `5/5187 with flags 0000480B, HUD on` |
| Kinect ID | ✅ | motor de identidade iniciado |

## Como o jogo usa o Kinect

- **Câmera e sensor:** 2 importações (`PsCamDeviceRequest`, `XamNuiCameraRememberFloor`); 0 funções próprias e 2 do runtime; 10715 chamadas vistas em execução.
- **Inclinação e motor:** 6 importações (`XamNuiCameraElevationGetAngle`, `XamNuiCameraElevationSetAngle`, `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus`, `XamNuiCameraTiltReportStatus`, `XamNuiCameraTiltSetCallback`); 0 funções próprias e 10 do runtime; 86 chamadas vistas em execução.
- **Kinect ID e biometria:** 6 importações (`XamNuiIdentityGetSessionId`, `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamUserNuiGetEnrollmentIndex`, `XamUserNuiGetUserIndex`, `XamWriteBiometricData`); 0 funções próprias e 10 do runtime; 2 chamadas vistas em execução.
- **Voz e microfones:** 1 importações (`XAudioGetVoiceCategoryVolume`); 0 funções próprias e 1 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`**.
- **Telas Nui do sistema:** 8 importações (`XamShowNuiControllerRequiredUI`, `XamShowNuiDeviceSelectorUI`, `XamShowNuiGuideUI`, `XamShowNuiHardwareRequiredUI`, `XamShowNuiMarketplaceUI`, `XamShowNuiMessageBoxUI`, `XamShowNuiSigninUI`, `XamShowNuiTroubleshooterUI`); 0 funções próprias e 1 do runtime.
- **Mensagens aos apps do sistema (XMsg):** 4 importações (`XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`, `XMsgSystemProcessCall`); 0 funções próprias e 22 do runtime; 2016 chamadas vistas em execução.
- **Cache do xam:** 3 importações (`XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`); 0 funções próprias e 1 do runtime; 2 chamadas vistas em execução.

## Chamadas ao sistema

**326 importações** (127 do `xam.xex`, 199 do `xboxkrnl.exe`), 3775 chamadas diretas no código, 112 vistas em execução. Estado no Xenia Edge (kixenia): 247 implementadas, 26 aproximadas, 33 stubs, 19 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 46 funções que chamam o sistema, **46 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **0 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

<details><summary><b>Runtime NUI embutido</b> (46 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x826BCD40 | 120 | `XMsgStartIORequest` | 2 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x826BCDB8 | 140 | `XMsgStartIORequest` | 0 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x826BD4E8 | 56 | `XMsgSystemProcessCall` | 3 | XMsg: 0x2B001 (NUI) | ✅ |
| 0x826BD5A8 | 56 | `XamShowNuiMessageBoxUI` | 2 | `XamShowNuiMessageBoxUI`: NuiHudCheck + caixa de mensagem com flag 0x2000 | ✅ |
| 0x827BCB58 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x828108F8 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82810BA8 | 92 | `XamNuiCameraElevationGetAngle` | 2 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82810C08 | 132 | `XamNuiCameraElevationSetAngle` | 2 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82810CB0 | 76 | `XamNuiCameraTiltGetStatus` | 0 | `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x82811038 | 144 | `XamNuiCameraElevationSetAngle` | 3 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x828110C8 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraRememberFloor`: guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes); `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x828112A0 | 676 | `XamNuiCameraElevationGetAngle` | 2 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82811730 | 780 | `XamNuiCameraTiltReportStatus` | 0 | `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82811AF0 | 148 | `XamNuiCameraTiltSetCallback` | 1 | `XamNuiCameraTiltSetCallback`: 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| 0x82812490 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | ✅ |
| 0x82813B30 | 96 | `PsCamDeviceRequest` | 0 |  | ✅ |
| 0x82820908 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada; `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82820AF8 | 432 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82822A88 | 356 | `XamUserNuiGetUserIndex` | 0 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82823870 | 388 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x828239F8 | 3200 | `XamNuiIdentityGetSessionId` | 5 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x8282D1B8 | 996 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82831568 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | `XamUserNuiGetEnrollmentIndex`: +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| 0x828317B0 | 336 | `XamUserNuiGetUserIndex` | 1 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82831A30 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 |  | ✅ |
| 0x82831B28 | 488 | `XamReadBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| 0x82832140 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes; `XamUserNuiEnableBiometric`: configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| 0x82944810 | 60 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C001 ao app 0xFE (xam) | ✅ |
| 0x82944850 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C003 ao app 0xFE (xam) | ✅ |
| 0x82944898 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C004 ao app 0xFE (xam) | ✅ |
| 0x829448E0 | 64 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C005 ao app 0xFE (xam) | ✅ |
| 0x82944920 | 56 | `XMsgInProcessCall` | 2 | XMsg: registra o callback de identidade (0x2C009) | ✅ |
| 0x82944958 | 76 | `XMsgInProcessCall` | 9 | XMsg: dados de identidade (0x2C00C) | ✅ |
| 0x829449A8 | 68 | `XMsgInProcessCall` | 1 | XMsg: resolve o cadastro (0x2C00D) | ✅ |
| 0x829449F0 | 80 | `XMsgStartIORequest` | 1 | XMsg: biometria desligada (0x2A009) | ✅ |
| 0x82944A40 | 112 | `XMsgStartIORequest` | 3 | XMsg: lê o cache biométrico (0x2A008) | ✅ |
| 0x82944AB0 | 100 | `XMsgStartIORequest` | 2 | XMsg: grava o cache biométrico (0x2A007) | ✅ |
| 0x82944B18 | 80 | `XMsgStartIORequest` | 2 | XMsg: 0x2A006 ao app 0xFE (xam) | ✅ |
| 0x82944B68 | 112 | `XMsgStartIORequest` | 1 | XMsg: 0x2A005 ao app 0xFE (xam) | ✅ |
| 0x82944BD8 | 100 | `XMsgStartIORequest` | 1 | XMsg: 0x2A004 ao app 0xFE (xam) | ✅ |
| 0x82B025F8 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x82B02758 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 1 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x82B029A0 | 548 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82B02DF8 | 252 | `XMsgInProcessCall` | 3 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x82B04BA8 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x82B04C50 | 104 | `XMsgInProcessCall` | 1 | XMsg: 0x7001B ao app 0xFA | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 10715 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgInProcessCall` | xam 0x1F4 | 10 | 2009 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 10 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 83 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | 🟡 |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (61)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 34 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 2 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedFlushSList` | xboxkrnl 0x2B | 6 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 10 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 32776 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 5 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 58 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 5 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 46 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 7 | 5981 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 5826 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 15 | 6366 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 1988 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 1988 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 33 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 7 | 112 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` | xboxkrnl 0x9C | 1 | 7 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 72 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 13 | 11 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 7 | 2 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 15 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 23 | 8549 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 15 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 39 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 40 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 2 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 35997 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 25 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 15 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1017 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 79 | 499 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 78 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1467 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 2928 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 3 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (53)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 8 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` | xam 0x20 | 1 | 8 | ✅ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` | xam 0x4A | 1 | 1 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetSystemLinkPort` | xam 0x54 | 1 | 0 | ✅ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` | xam 0xA | 1 | 0 | ✅ |
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
| `XNetLogonGetMachineID` | xam 0x135 | 1 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (48)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 193 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 27 | 50 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 9 | 170 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 44 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 4 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 2 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 25 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 22 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 3 | 143 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 4 | 4 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 1 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 11 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 4 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 25 | 8157 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 2 | 3 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 13 | 8158 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 5 | 2 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 8 | 4 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 5 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 2 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 2289 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 2 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 2 | 1 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 2290 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 4 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 1 | 4 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 14 | 24097 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 10 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 1 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 10 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 1995 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 17 | 16770 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 2 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 4 | 9 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 9 | 9 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 14 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 8 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 1 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 1 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 21 | 44 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 15 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 32 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 5 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 3 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 3 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMADisableContext` | xboxkrnl 0x228 | 3 | 0 | ✅ |
| `XMAEnableContext` | xboxkrnl 0x227 | 5 | 0 | ✅ |
| `XMAGetOutputBufferReadOffset` | xboxkrnl 0x22B | 1 | 0 | ✅ |
| `XMAGetOutputBufferWriteOffset` | xboxkrnl 0x229 | 1 | 0 | ✅ |
| `XMAInitializeContext` | xboxkrnl 0x225 | 1 | 0 | ✅ |
| `XMAIsInputBuffer0Valid` | xboxkrnl 0x22F | 1 | 0 | ✅ |
| `XMAIsInputBuffer1Valid` | xboxkrnl 0x231 | 1 | 0 | ✅ |
| `XMAIsOutputBufferValid` | xboxkrnl 0x22D | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |
| `XMASetInputBuffer0Valid` | xboxkrnl 0x22E | 1 | 0 | ✅ |
| `XMASetInputBuffer1Valid` | xboxkrnl 0x230 | 1 | 0 | ✅ |
| `XMASetOutputBufferReadOffset` | xboxkrnl 0x22A | 1 | 0 | ✅ |
| `XMASetOutputBufferValid` | xboxkrnl 0x22C | 2 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 5 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 1 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 3 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 2 | 2 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 5 | 2945 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 30 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 1164 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 5 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 1568 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 1150 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 5 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 5 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 10715 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 83 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | 🟡 |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 2 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 1 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 2 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgInProcessCall` | xam 0x1F4 | 10 | 2009 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 10 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
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

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 2 | 1 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptSha` | xboxkrnl 0x192 | 5 | 0 | ✅ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |

</details>

**Ausentes no Xenia (19):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_XNetServerToInAddr`, `NetDll_XnpLogonGetStatus`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`.
- Aproximadas e chamadas em execução: `XamCacheOpenFile`, `XamCacheReset`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/yourshapfitn/` (`yourshapfitn.img`, `yourshapfitn_imports.json`, `yourshapfitn_kinect.json`, `yourshapfitn_fngraph.json`, `c_full/<endereço>.c`: 88 funções).
- Projeto Ghidra: `games/decompilado/yourshapfitn/ghidra/yourshapfitn` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
- Log da bateria: `~/Kixenia/kinect-tools/bateria/saida-primeira/Your_Shape_Fitness_Evolved.log`.
