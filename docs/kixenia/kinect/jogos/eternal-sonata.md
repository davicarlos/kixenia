# Eternal Sonata (título 4E4D07E4)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `Eternal Sonata [NTSCU]` (pacote GoD (cabeçalho em `Content/…/00007000/`)). **Executável:** 5120 KB de imagem, base 0x82000000, entrada 0x821FE050, 4930 funções no `.pdata`, `.text` 0x82080000–0x82291AA4.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 2 importações (`XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`); 1 funções próprias (0x82251400) e 0 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`**.
- **Mensagens aos apps do sistema (XMsg):** 2 importações (`XMsgInProcessCall`, `XMsgStartIORequest`); 2 funções próprias (0x821FD7D8, 0x822765F0) e 1 do runtime.

## Chamadas ao sistema

**151 importações** (29 do `xam.xex`, 122 do `xboxkrnl.exe`), 779 chamadas diretas no código. Estado no Xenia Edge (kixenia): 115 implementadas, 8 aproximadas, 25 stubs, 2 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 4 funções que chamam o sistema, **1 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **3 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x821FD7D8 | 148 | `XMsgStartIORequest` | 0 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x82251400 | 312 | `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask` | 3 |  | ✅ |
| 0x822765F0 | 108 | `XMsgInProcessCall` | 0 | XMsg: 0x70009 ao app 0xFA | ✅ |

<details><summary><b>Runtime NUI embutido</b> (1 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x821FD758 | 128 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XMsgInProcessCall` | xam 0x1F4 | 1 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 2 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (40)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 1 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 88 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 5 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 4 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 1 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 6 | 0 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 0 | ✅ |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 1 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeRaiseIrqlToDpcLevel` | xboxkrnl 0x85 | 87 | 0 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 2 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 110 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 1 | 0 | ✅ |
| `KeResumeThread` | xboxkrnl 0x92 | 1 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 3 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 7 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 2 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 1 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 4 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 3 | 0 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 1 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 4 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 4 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 5 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 3 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 38 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 10 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 59 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
| `NtClose` | xboxkrnl 0xCF | 6 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 2 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 3 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 2 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 4 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 4 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 2 | 0 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 5 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 5 | 0 | ✅ |
| `XamContentClose` | xam 0x25A | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (25)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 0 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 0 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 2 | 0 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 0 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 0 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 0 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 0 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 0 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 0 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 0 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgPrint` | xboxkrnl 0x3 | 2 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 8 | 0 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 3 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnwind` | xboxkrnl 0x147 | 2 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 8 | 0 | ✅ |
| `vswprintf` | xboxkrnl 0x151 | 1 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (13)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 4 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 6 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 0 | ⚪ stub |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 0 | ⚪ stub |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 3 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 6 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 11 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 6 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 0 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 2 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 1 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgInProcessCall` | xam 0x1F4 | 1 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 2 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Outras</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `KfLowerIrql` | xboxkrnl 0xB3 | 109 | 0 | ✅ |

</details>

**Ausentes no Xenia (2):** `RtlUnwind`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/etersona/` (`etersona.img`, `etersona_imports.json`, `etersona_kinect.json`, `etersona_fngraph.json`, `c_full/<endereço>.c`: 8 funções).
- Projeto Ghidra: `games/decompilado/etersona/ghidra/etersona` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
