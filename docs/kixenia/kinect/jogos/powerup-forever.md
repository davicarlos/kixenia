# PowerUp Forever (título 58410934)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `PowerUp Forever` (pacote XBLA (`000D0000/`)). **Executável:** 11008 KB de imagem, base 0x82000000, entrada 0x822E5708, 18460 funções no `.pdata`, `.text` 0x82100000–0x827F1FB4.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 6 importações (`XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`); 2 funções próprias (0x827E0C90, 0x827E0EA0) e 5 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`**.
- **Câmera Xbox Live Vision:** 6 importações (`XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`); 2 funções próprias (0x82432C50, 0x82432F08) e 6 do runtime; **faltam/stub: `XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`**.
- **Mensagens aos apps do sistema (XMsg):** 3 importações (`XMsgCancelIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`); 5 funções próprias (0x827D9150, 0x827D9300, 0x827D9880, 0x827D9968 …) e 18 do runtime.

## Chamadas ao sistema

**270 importações** (89 do `xam.xex`, 181 do `xboxkrnl.exe`), 1366 chamadas diretas no código. Estado no Xenia Edge (kixenia): 188 implementadas, 21 aproximadas, 41 stubs, 19 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 36 funções que chamam o sistema, **28 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **8 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82432C50 | 336 | `XUsbcamSetConfig` | 2 |  | ✅ |
| 0x82432F08 | 288 | `XUsbcamSetCaptureMode` | 0 |  | ✅ |
| 0x827D9150 | 428 | `XMsgStartIORequest` | 3 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x827D9300 | 184 | `XMsgStartIORequest` | 2 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x827D9880 | 228 | `XMsgStartIORequest` | 1 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x827D9968 | 188 | `XMsgStartIORequest` | 2 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x827E0C90 | 284 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x827E0EA0 | 780 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |

<details><summary><b>Runtime NUI embutido</b> (28 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x822E5938 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x822E59B0 | 136 | `XMsgStartIORequest` | 0 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x822E5A40 | 140 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x822E5B80 | 220 | `XMsgStartIORequest` | 1 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x822E6C00 | 40 | `XMsgCancelIORequest` | 13 |  | ✅ |
| 0x82432A28 | 292 | `XUsbcamCreate` | 2 |  | ✅ |
| 0x82432B50 | 160 | `XUsbcamDestroy` | 4 |  | ✅ |
| 0x82432BF0 | 92 | `XUsbcamGetState` | 5 |  | ✅ |
| 0x82433028 | 240 | `XUsbcamReadFrame` | 0 |  | ✅ |
| 0x82434A98 | 432 | `XUsbcamSetConfig` | 0 |  | ✅ |
| 0x82434C48 | 432 | `XUsbcamSetConfig` | 1 |  | ✅ |
| 0x82626718 | 316 | `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask` | 3 |  | ✅ |
| 0x827D8F00 | 68 | `XMsgInProcessCall` | 1 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x827D9070 | 224 | `XMsgInProcessCall` | 2 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x827D93B8 | 172 | `XMsgStartIORequest` | 2 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x827D9468 | 168 | `XMsgStartIORequest` | 0 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x827D9510 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x827D95B8 | 164 | `XMsgStartIORequest` | 2 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x827D9660 | 216 | `XMsgStartIORequest` | 1 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x827D9738 | 152 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x827D97D0 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x827D9A58 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x827D9BB8 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x827D9CE8 | 72 | `XMsgInProcessCall` | 2 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x827E0698 | 868 | `XamVoiceHeadsetPresent` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced* | ✅ |
| 0x827E12B8 | 248 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x827E13B0 | 180 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x827E4300 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 5 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 15 | 0 | ✅ |
| `XUsbcamCreate` | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (60)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAcquireReadWriteLockExclusive` | xboxkrnl 0x7 | 1 | 0 | ✅ |
| `ExAcquireReadWriteLockShared` | xboxkrnl 0x8 | 6 | 0 | ✅ |
| `ExCreateThread` | xboxkrnl 0xD | 3 | 0 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExInitializeReadWriteLock` | xboxkrnl 0x11 | 1 | 0 | ✅ |
| `ExReleaseReadWriteLock` | xboxkrnl 0x16 | 7 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 89 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 7 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 4 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 6 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 40 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 2 | 0 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 0 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 2 | 0 | ✅ |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 6 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 7 | 0 | ✅ |
| `KeRaiseIrqlToDpcLevel` | xboxkrnl 0x85 | 89 | 0 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 2 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 112 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 8 | 0 | ✅ |
| `KeResumeThread` | xboxkrnl 0x92 | 1 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 4 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 23 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 2 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 6 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 5 | 0 | ✅ |
| `KeTryToAcquireSpinLockAtRaisedIrql` | xboxkrnl 0xAE | 1 | 0 | 🟡 |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 1 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 19 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 14 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 15 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 5 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 97 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 15 | 0 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 1 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 116 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 4 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (42)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoDismountVolume` | xboxkrnl 0x3B | 2 | 0 | ❌ |
| `IoDismountVolumeByFileHandle` | xboxkrnl 0x3C | 1 | 0 | ❌ |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `NtClose` | xboxkrnl 0xCF | 17 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 3 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 4 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 6 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 4 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 5 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 10 | 0 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 22 | 0 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObReferenceObject` | xboxkrnl 0x10F | 3 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 7 | 0 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamContentClose` | xam 0x25A | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Rede</b> (29)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 0 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 0 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 0 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 0 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 0 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 0 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 0 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 0 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 0 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 0 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 3 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 2 | 0 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Usuários, perfis e Live</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 10 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 2 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 0 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 13 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 12 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 1 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 0 | ⚪ stub |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 4 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 2 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 0 | ⚪ stub |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 4 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 3 | 0 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 3 | 0 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 2 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 19 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 7 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 6 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (13)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 6 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 3 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 6 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 2 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 2 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 4 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 9 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 0 | ⚪ stub |
| `XamEnumerate` | xam 0x250 | 1 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 0 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Voz e microfones</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Câmera Xbox Live Vision</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XUsbcamCreate` | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |

</details>

<details><summary><b>Áudio</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 2 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 5 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 15 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |

</details>

<details><summary><b>Outras</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `KfLowerIrql` | xboxkrnl 0xB3 | 112 | 0 | ✅ |

</details>

**Ausentes no Xenia (19):** `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XUsbcamDestroy`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XamShowFriendsUI`, `XamShowGamerCardUIForXUID`, `XamShowPlayerReviewUI`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/powefore/` (`powefore.img`, `powefore_imports.json`, `powefore_kinect.json`, `powefore_fngraph.json`, `c_full/<endereço>.c`: 95 funções).
- Projeto Ghidra: `games/decompilado/powefore/ghidra/powefore` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
