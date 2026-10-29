# FIFA 19 (título 454109F9)

**No HD:** `FIFA 19` (`default.xex` solto). **Executável:** 29312 KB de imagem, base 0x82000000, entrada 0x825B5F70, 75024 funções no `.pdata`, `.text` 0x822f0000–0x83A2B318.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

- **Câmera e sensor:** 1 importações (`XamNuiGetDeviceStatus`); 0 funções próprias e 1 do runtime.
- **Voz e microfones:** 6 importações (`XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`); 0 funções próprias e 9 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`**.
- **Câmera Xbox Live Vision:** 2 importações (`XUsbcamGetState`, `XUsbcamSetConfig`); 0 funções próprias e 4 do runtime; **faltam/stub: `XUsbcamGetState`, `XUsbcamSetConfig`**.
- **Mensagens aos apps do sistema (XMsg):** 5 importações (`XMsgCancelIORequest`, `XMsgCompleteIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`); 2 funções próprias (0x83624D08, 0x83624EE8) e 34 do runtime.

## Chamadas ao sistema

**347 importações** (147 do `xam.xex`, 200 do `xboxkrnl.exe`), 4112 chamadas diretas no código. Estado no Xenia Edge (kixenia): 226 implementadas, 26 aproximadas, 50 stubs, 44 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 48 funções que chamam o sistema, **46 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **2 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x83624D08 | 476 | `XMsgStartIORequest` | 0 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x83624EE8 | 232 | `XMsgStartIORequest` | 0 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |

<details><summary><b>Runtime NUI embutido</b> (46 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x825B3DD0 | 232 | `XMsgStartIORequest` | 1 | XMsg: 0x20021 ao app 0xFE (xam) | ✅ |
| 0x835D91A8 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x836233F8 | 352 | `XMsgInProcessCall` | 2 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x83623558 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x83623688 | 752 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x83623978 | 568 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x83623BB0 | 384 | `XMsgInProcessCall` | 1 | XMsg: 0x58019 ao app 0xFC | ✅ |
| 0x83623D30 | 312 | `XMsgInProcessCall` | 0 | XMsg: 0x58046 ao app 0xFC | ✅ |
| 0x83623E68 | 320 | `XMsgInProcessCall` | 1 | XMsg: 0x5801E ao app 0xFC | ✅ |
| 0x83623FA8 | 320 | `XMsgInProcessCall` | 1 | XMsg: 0x58044 ao app 0xFC | ✅ |
| 0x836240E8 | 572 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x83624328 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 2 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x83624570 | 548 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x83624870 | 72 | `XMsgInProcessCall` | 2 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x83624988 | 68 | `XMsgInProcessCall` | 1 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x83624B30 | 252 | `XMsgInProcessCall` | 11 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x83624FD0 | 176 | `XMsgStartIORequest` | 0 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x83625080 | 172 | `XMsgStartIORequest` | 0 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x83625130 | 168 | `XMsgStartIORequest` | 0 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x836251D8 | 220 | `XMsgStartIORequest` | 0 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x836252B8 | 156 | `XMsgStartIORequest` | 0 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x83625358 | 176 | `XMsgStartIORequest` | 0 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x83625408 | 220 | `XMsgStartIORequest` | 1 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x836254E8 | 192 | `XMsgStartIORequest` | 0 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x83625708 | 248 | `XMsgStartIORequest` | 0 | XMsg: 0xB001E ao app 0xFB (Live) | ✅ |
| 0x83629B18 | 200 | `XamVoiceIsActiveProcess` | 3 | `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x8362ADB0 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced*; `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x8362B3F0 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x8362B780 | 600 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x8362BAE0 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x8362BBD0 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x8362CA20 | 688 | `XamVoiceIsActiveProcess` | 3 | `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x83630E90 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x83636720 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x836367C8 | 104 | `XMsgInProcessCall` | 2 | XMsg: 0x7001B ao app 0xFA | ✅ |
| 0x83636898 | 92 | `XUsbcamGetState` | 1 |  | ✅ |
| 0x836368F8 | 332 | `XUsbcamSetConfig` | 1 |  | ✅ |
| 0x836370D0 | 432 | `XUsbcamSetConfig` | 0 |  | ✅ |
| 0x83637280 | 428 | `XUsbcamSetConfig` | 1 |  | ✅ |
| 0x83688A80 | 152 | `XMsgStartIORequest` | 1 | XMsg: 0x22005 ao app 0xFE (xam) | ✅ |
| 0x83689358 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |
| 0x8368A0F0 | 120 | `XMsgStartIORequest` | 2 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x8368A168 | 140 | `XMsgStartIORequest` | 1 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x8368A200 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x8368A750 | 40 | `XMsgCancelIORequest` | 26 |  | ✅ |
| 0x8368AC78 | 40 | `XamNuiGetDeviceStatus` | 1 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 12 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 21 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

### Todas as importações, por área

<details><summary><b>Rede</b> (64)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XHttpCloseHandle` | xam 0xCC | 1 | 0 | ❌ |
| `NetDll_XHttpConnect` | xam 0xCD | 1 | 0 | ❌ |
| `NetDll_XHttpDoWork` | xam 0xD8 | 1 | 0 | ❌ |
| `NetDll_XHttpOpen` | xam 0xCB | 1 | 0 | ⚪ stub |
| `NetDll_XHttpOpenRequest` | xam 0xCF | 1 | 0 | ❌ |
| `NetDll_XHttpQueryHeaders` | xam 0xD3 | 1 | 0 | ❌ |
| `NetDll_XHttpReadData` | xam 0xD4 | 1 | 0 | ❌ |
| `NetDll_XHttpReceiveResponse` | xam 0xD2 | 1 | 0 | ❌ |
| `NetDll_XHttpSendRequest` | xam 0xD1 | 1 | 0 | ❌ |
| `NetDll_XHttpSetOption` | xam 0xD7 | 1 | 0 | ❌ |
| `NetDll_XHttpSetStatusCallback` | xam 0xCE | 1 | 0 | ❌ |
| `NetDll_XHttpShutdown` | xam 0xCA | 1 | 0 | ⚪ stub |
| `NetDll_XHttpStartup` | xam 0xC9 | 1 | 0 | ⚪ stub |
| `NetDll_XHttpWriteData` | xam 0xD5 | 1 | 0 | ❌ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetCreateKey` | xam 0x36 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` | xam 0xA | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 0 | ✅ |
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
| `XNetLogonGetMachineID` | xam 0x135 | 2 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 4 | 0 | ✅ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (54)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 0 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 7 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 4 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 55 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 0 | 🟡 |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 5 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 3 | 0 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 21 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 7 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 6 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 8 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 9 | 0 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 2 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 9 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 13 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 14 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 6 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1395 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 209 | 0 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 5 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1541 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 13 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (49)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
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
| `NtClose` | xboxkrnl 0xCF | 20 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 4 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 5 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 5 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 6 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 9 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 28 | 0 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObReferenceObject` | xboxkrnl 0x10F | 2 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 8 | 0 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamContentClose` | xam 0x25A | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Usuários, perfis e Live</b> (30)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 10 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowCustomPlayerListUI` | xam 0x2E6 | 1 | 0 | ❌ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMarketplaceUI` | xam 0x2C7 | 2 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 0 | ⚪ stub |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 9 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 0 | 🟡 |
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
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 0 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 16 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 12 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 2 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetLanguage` | xam 0x3D2 | 1 | 0 | ✅ |
| `XamGetLocaleEx` | xam 0x5D7 | 1 | 0 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 0 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 4 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 0 | ✅ |
| `XamGetToken` | xam 0x4BE | 1 | 0 | ❌ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 2 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 2 | 0 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 6 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 5 | 0 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 5 | 0 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 0 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 3 | 0 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMADisableContext` | xboxkrnl 0x228 | 2 | 0 | ✅ |
| `XMAEnableContext` | xboxkrnl 0x227 | 3 | 0 | ✅ |
| `XMAGetOutputBufferWriteOffset` | xboxkrnl 0x229 | 1 | 0 | ✅ |
| `XMAInitializeContext` | xboxkrnl 0x225 | 1 | 0 | ✅ |
| `XMAIsInputBuffer0Valid` | xboxkrnl 0x22F | 1 | 0 | ✅ |
| `XMAIsInputBuffer1Valid` | xboxkrnl 0x231 | 1 | 0 | ✅ |
| `XMAIsOutputBufferValid` | xboxkrnl 0x22D | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |
| `XMASetInputBuffer0` | xboxkrnl 0x232 | 1 | 0 | ✅ |
| `XMASetInputBuffer0Valid` | xboxkrnl 0x22E | 1 | 0 | ✅ |
| `XMASetInputBuffer1` | xboxkrnl 0x233 | 1 | 0 | ✅ |
| `XMASetInputBuffer1Valid` | xboxkrnl 0x230 | 1 | 0 | ✅ |
| `XMASetInputBufferReadOffset` | xboxkrnl 0x237 | 1 | 0 | ✅ |
| `XMASetOutputBufferReadOffset` | xboxkrnl 0x22A | 1 | 0 | ✅ |
| `XMASetOutputBufferValid` | xboxkrnl 0x22C | 2 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (20)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 3 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 20 | 0 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 7 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 10 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 3 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 5 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 16 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 19 | 0 | ✅ |
| `XamFreeToken` | xam 0x4BF | 1 | 0 | ❌ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 25 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 0 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 3 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 1 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 0 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 2 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Voz e microfones</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Criptografia e chaves do console</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |
| `XeKeysGetKey` | xboxkrnl 0x244 | 1 | 0 | 🟡 |
| `XeKeysGetKeyProperties` | xboxkrnl 0x241 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 12 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 21 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |

</details>

<details><summary><b>Outras</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamBackgroundDownloadSetMode` | xam 0x9C4 | 1 | 0 | ❌ |
| `XampXAuthGetTitleBuffer` | xam 0x4EE | 1 | 0 | ⚪ stub |
| `XampXAuthShutdown` | xam 0x4BD | 1 | 0 | ⚪ stub |
| `XampXAuthStartup` | xam 0x4BC | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Câmera Xbox Live Vision</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |

</details>

**Ausentes no Xenia (44):** `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_WSARecv`, `NetDll_XHttpCloseHandle`, `NetDll_XHttpConnect`, `NetDll_XHttpDoWork`, `NetDll_XHttpOpenRequest`, `NetDll_XHttpQueryHeaders`, `NetDll_XHttpReadData`, `NetDll_XHttpReceiveResponse`, `NetDll_XHttpSendRequest`, `NetDll_XHttpSetOption`, `NetDll_XHttpSetStatusCallback`, `NetDll_XHttpWriteData`, `NetDll_XNetServerToInAddr`, `NetDll_XnpLogonGetStatus`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XUsbcamSetConfig`, `XamBackgroundDownloadSetMode`, `XamFreeToken`, `XamGetToken`, `XamShowCustomPlayerListUI`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowPlayerReviewUI`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XUsbcamGetState`, `XUsbcamSetConfig`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/fifa19/` (`fifa19.img`, `fifa19_imports.json`, `fifa19_kinect.json`, `fifa19_fngraph.json`, `c_full/<endereço>.c`: 111 funções).
- Projeto Ghidra: `games/decompilado/fifa19/ghidra/fifa19` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
