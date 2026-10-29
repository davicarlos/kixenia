# Dragon Ball Z for Kinect PAL (título 4E4D0860)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `Dragon Ball Z for Kinect PAL-GHOST_XBOX_360_RGH` (`default.xex` solto). **Executável:** 9216 KB de imagem, base 0x82000000, entrada 0x826AE9B8, 22606 funções no `.pdata`, `.text` 0x820f0000–0x82749F64.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Bateria ao vivo (Kinect real, log nível 3, `Dragon_Ball_Z_Kinect.log`): rodou o tempo todo sem travar; 0 de 0 amostras com jogador rastreado.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 6 importações (`XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`); 0 funções próprias e 9 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`**.
- **Mensagens aos apps do sistema (XMsg):** 5 importações (`XMsgCancelIORequest`, `XMsgCompleteIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`); 2 funções próprias (0x826B5E00, 0x826BF120) e 39 do runtime; 8072 chamadas vistas em execução.

## Chamadas ao sistema

**276 importações** (108 do `xam.xex`, 168 do `xboxkrnl.exe`), 887 chamadas diretas no código, 89 vistas em execução. Estado no Xenia Edge (kixenia): 184 implementadas, 22 aproximadas, 41 stubs, 28 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 48 funções que chamam o sistema, **46 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **2 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x826B5E00 | 264 | `XMsgStartIORequest` | 0 | XMsg: 0xB0041 ao app 0xFB (Live) | ✅ |
| 0x826BF120 | 212 | `XMsgStartIORequest` | 0 | XMsg: 0xB0060 ao app 0xFB (Live) | ✅ |

<details><summary><b>Runtime NUI embutido</b> (46 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x824BB3D0 | 432 | `XMsgStartIORequest` | 1 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x824BB580 | 188 | `XMsgStartIORequest` | 0 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x824BB640 | 176 | `XMsgStartIORequest` | 0 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x824BB6F0 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x824BB7A0 | 172 | `XMsgStartIORequest` | 0 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x824BB850 | 168 | `XMsgStartIORequest` | 0 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x824BB8F8 | 168 | `XMsgStartIORequest` | 0 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x824BB9A0 | 220 | `XMsgStartIORequest` | 0 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x824BBA80 | 156 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x824BBB20 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x824BBBD0 | 228 | `XMsgStartIORequest` | 0 | XMsg: 0xB0016 ao app 0xFB (Live) | ✅ |
| 0x824BBCB8 | 232 | `XMsgStartIORequest` | 0 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x824BBDA0 | 280 | `XMsgStartIORequest` | 0 | XMsg: 0xB0065 ao app 0xFB (Live) | ✅ |
| 0x824BBEB8 | 220 | `XMsgStartIORequest` | 0 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x824BBF98 | 164 | `XMsgStartIORequest` | 0 | XMsg: 0xB0026 ao app 0xFB (Live) | ✅ |
| 0x824BC040 | 192 | `XMsgStartIORequest` | 1 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x824BC1D0 | 68 | `XMsgInProcessCall` | 0 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x824BC378 | 252 | `XMsgInProcessCall` | 3 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x824C0868 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x826AF4C0 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x826AF538 | 140 | `XMsgStartIORequest` | 1 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x826AF5C8 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x826AF728 | 124 | `XMsgStartIORequest` | 0 | XMsg: 0xB0020 ao app 0xFB (Live) | ✅ |
| 0x826AF7B8 | 216 | `XMsgStartIORequest` | 0 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x826B03B8 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |
| 0x826B5D50 | 40 | `XMsgCancelIORequest` | 3 |  | ✅ |
| 0x826B5D78 | 136 | `XMsgStartIORequest` | 0 | XMsg: 0xB0041 ao app 0xFB (Live) | ✅ |
| 0x826BECB0 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x826BEE10 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x826BEF40 | 476 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826BF288 | 248 | `XMsgStartIORequest` | 0 | XMsg: 0xB001F ao app 0xFB (Live) | ✅ |
| 0x826BF380 | 248 | `XMsgStartIORequest` | 0 | XMsg: 0xB001E ao app 0xFB (Live) | ✅ |
| 0x826BF4D8 | 304 | `XMsgStartIORequest` | 0 | XMsg: 0xB001D ao app 0xFB (Live) | ✅ |
| 0x826BF608 | 72 | `XMsgInProcessCall` | 1 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x826BF650 | 88 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x826C2EC8 | 200 | `XamVoiceIsActiveProcess` | 3 | `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x826C4160 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced*; `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x826C47A0 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x826C4B30 | 600 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x826C4E90 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x826C4F80 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x826C7168 | 688 | `XamVoiceIsActiveProcess` | 3 | `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x826C9590 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x826CDF28 | 100 | `XMsgInProcessCall` | 0 | XMsg: 0x70009 ao app 0xFA | ✅ |
| 0x826CDF90 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x826CE038 | 104 | `XMsgInProcessCall` | 1 | XMsg: 0x7001B ao app 0xFA | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 9 | 8066 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 28 | 4 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 2 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (52)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 28 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 4 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 4 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 6 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 53 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 6 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 13 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 6 | 2 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 4 | 24195 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 23 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 3 | 23 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 17 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 1 | 1 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 1 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 3 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 6 | 18 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 2 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 7 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 13 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 14 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 16 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 21 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 6 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 6 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 79 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 13 | 119 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 2 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 83 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 15 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 3 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (46)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XHttpCloseHandle` | xam 0xCC | 1 | 0 | ❌ |
| `NetDll_XHttpConnect` | xam 0xCD | 1 | 0 | ❌ |
| `NetDll_XHttpCrackUrl` | xam 0xDC | 1 | 0 | ✅ |
| `NetDll_XHttpDoWork` | xam 0xD8 | 1 | 0 | ❌ |
| `NetDll_XHttpOpen` | xam 0xCB | 1 | 0 | ⚪ stub |
| `NetDll_XHttpOpenRequest` | xam 0xCF | 1 | 0 | ❌ |
| `NetDll_XHttpQueryHeaders` | xam 0xD3 | 1 | 0 | ❌ |
| `NetDll_XHttpReadData` | xam 0xD4 | 1 | 0 | ❌ |
| `NetDll_XHttpReceiveResponse` | xam 0xD2 | 1 | 0 | ❌ |
| `NetDll_XHttpSendRequest` | xam 0xD1 | 1 | 0 | ❌ |
| `NetDll_XHttpSetStatusCallback` | xam 0xCE | 1 | 0 | ❌ |
| `NetDll_XHttpShutdown` | xam 0xCA | 1 | 0 | ⚪ stub |
| `NetDll_XHttpStartup` | xam 0xC9 | 1 | 0 | ⚪ stub |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 8066 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 1 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 1 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
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
| `XNetLogonGetTitleID` | xam 0x136 | 1 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (45)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 10 | 15 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 4 | 11 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 1 | 22 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 45 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 151 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 4 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 5 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 286 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 2 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 1 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 5 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 3 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 24 | 4240 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 6 | 4241 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamContentClose` | xam 0x25A | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 2 | ⚪ stub |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 8092 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 8093 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (23)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 13 | 2 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 10 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 1 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 1 | 1 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 7 | 4 | ✅ |
| `XamGetToken` | xam 0x4BE | 1 | 0 | ❌ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 3 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 4 | 3 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 4 | 9 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (19)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 14 | 16 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 6 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 6 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 5 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamQueryLiveHiveA` | xam 0x70C | 1 | 0 | ⚪ stub |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 15 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 1 | ✅ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 0 | ⚪ stub |
| `XamUserGetName` | xam 0x20E | 1 | 5 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 3 | 2 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 2 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 3 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 12 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 1 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 1 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 3 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 11 | 6 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 6 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 5 | 1 | ✅ |
| `XamFree` | xam 0x1EC | 7 | 0 | ✅ |
| `XamFreeToken` | xam 0x4BF | 1 | 0 | ❌ |

</details>

<details><summary><b>Áudio</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 0 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

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

<details><summary><b>Tarefas, enumeração e notificações</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 8 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamEnumerate` | xam 0x250 | 1 | 1 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 11 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 1 | 🟡 |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 9 | 8066 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 28 | 4 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 2 | ✅ |

</details>

<details><summary><b>Outras</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XampXAuthGetTitleBuffer` | xam 0x4EE | 1 | 0 | ⚪ stub |
| `XampXAuthShutdown` | xam 0x4BD | 1 | 0 | ⚪ stub |
| `XampXAuthStartup` | xam 0x4BC | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 32264 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptSha` | xboxkrnl 0x192 | 5 | 0 | ✅ |

</details>

**Ausentes no Xenia (28):** `IoCheckShareAccess`, `IoCompleteRequest`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_XHttpCloseHandle`, `NetDll_XHttpConnect`, `NetDll_XHttpDoWork`, `NetDll_XHttpOpenRequest`, `NetDll_XHttpQueryHeaders`, `NetDll_XHttpReadData`, `NetDll_XHttpReceiveResponse`, `NetDll_XHttpSendRequest`, `NetDll_XHttpSetStatusCallback`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XamFreeToken`, `XamGetToken`, `XamShowFriendsUI`, `XamShowGamerCardUIForXUID`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/dragballz/` (`dragballz.img`, `dragballz_imports.json`, `dragballz_kinect.json`, `dragballz_fngraph.json`, `c_full/<endereço>.c`: 72 funções).
- Projeto Ghidra: `games/decompilado/dragballz/ghidra/dragballz` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
- Log da bateria: `~/Kixenia/kinect-tools/bateria/saida-resto5/Dragon_Ball_Z_Kinect.log`.
