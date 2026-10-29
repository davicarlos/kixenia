# 50 cent Blood On The Sand (título 5451083B)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `50 cent Blood On The Sand` (`default.xex` solto). **Executável:** 26368 KB de imagem, base 0x82000000, entrada 0x824458C8, 79235 funções no `.pdata`, `.text` 0x82310000–0x83741214.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 7 importações (`XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamShowVoiceMailUI`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`); 3 funções próprias (0x830C6D98, 0x830C6FA8, 0x8332C120) e 5 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamShowVoiceMailUI`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`**.
- **Mensagens aos apps do sistema (XMsg):** 5 importações (`XMsgCancelIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`, `XMsgSystemProcessCall`); 11 funções próprias (0x830C6D98, 0x832EB310, 0x832EBD80, 0x832ECE00 …) e 60 do runtime.

## Chamadas ao sistema

**334 importações** (149 do `xam.xex`, 185 do `xboxkrnl.exe`), 2090 chamadas diretas no código. Estado no Xenia Edge (kixenia): 246 implementadas, 22 aproximadas, 39 stubs, 26 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 77 funções que chamam o sistema, **64 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **13 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x830C6D98 | 284 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x830C6FA8 | 780 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x832EB310 | 768 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x832EBD80 | 604 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x832ECE00 | 428 | `XMsgStartIORequest` | 2 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x832ECFB0 | 184 | `XMsgStartIORequest` | 1 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x832ED5D8 | 224 | `XMsgStartIORequest` | 2 | XMsg: 0xB0016 ao app 0xFB (Live) | ✅ |
| 0x832ED6B8 | 228 | `XMsgStartIORequest` | 0 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x832ED7A0 | 216 | `XMsgStartIORequest` | 1 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x832ED8D0 | 160 | `XMsgStartIORequest` | 1 | XMsg: 0xB0026 ao app 0xFB (Live) | ✅ |
| 0x832ED9F0 | 188 | `XMsgStartIORequest` | 2 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x8332C120 | 64 | `XamShowVoiceMailUI` | 0 |  | ✅ |
| 0x8332DF70 | 120 | `XMsgStartIORequest` | 0 | XMsg: 0xB0020 ao app 0xFB (Live) | ✅ |

<details><summary><b>Runtime NUI embutido</b> (64 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x830C67A0 | 868 | `XamVoiceHeadsetPresent` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced* | ✅ |
| 0x830C73C0 | 248 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x830C74B8 | 180 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x830CC708 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x831FA8A0 | 316 | `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask` | 3 |  | ✅ |
| 0x83228D20 | 80 | `XMsgStartIORequestEx` | 0 |  | ✅ |
| 0x83228D70 | 124 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x83228E58 | 168 | `XMsgStartIORequest` | 0 | XMsg: 0x70002 ao app 0xFA | ✅ |
| 0x83228F00 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0x7003B ao app 0xFA | ✅ |
| 0x83229018 | 292 | `XMsgInProcessCall` | 0 | XMsg: 0x70025 ao app 0xFA | ✅ |
| 0x83229140 | 152 | `XMsgStartIORequest` | 0 | XMsg: 0x7003D ao app 0xFA | ✅ |
| 0x832291D8 | 428 | `XMsgStartIORequestEx` | 0 | XMsg: 0x7002E ao app 0xFA; 0x7000D ao app 0xFA | ✅ |
| 0x83229388 | 240 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70013 ao app 0xFA | ✅ |
| 0x83229478 | 144 | `XMsgStartIORequest` | 0 | XMsg: 0x7000E ao app 0xFA | ✅ |
| 0x83229508 | 84 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70002 ao app 0xFA | ✅ |
| 0x83229560 | 96 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70056 ao app 0xFA | ✅ |
| 0x832295C0 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70003 ao app 0xFA | ✅ |
| 0x832295F8 | 76 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70004 ao app 0xFA | ✅ |
| 0x83229648 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70005 ao app 0xFA | ✅ |
| 0x83229680 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70006 ao app 0xFA | ✅ |
| 0x832296B8 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70007 ao app 0xFA | ✅ |
| 0x832296F0 | 148 | `XMsgStartIORequest` | 0 | XMsg: 0x70008 ao app 0xFA | ✅ |
| 0x83229788 | 108 | `XMsgInProcessCall` | 0 | XMsg: 0x70029 ao app 0xFA | ✅ |
| 0x832297F8 | 100 | `XMsgInProcessCall` | 0 | XMsg: 0x70009 ao app 0xFA | ✅ |
| 0x83229860 | 100 | `XMsgInProcessCall` | 0 | XMsg: 0x7000B ao app 0xFA | ✅ |
| 0x832298C8 | 136 | `XMsgStartIORequest` | 0 | XMsg: 0x7000C ao app 0xFA | ✅ |
| 0x83229950 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x832299F8 | 168 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70068 ao app 0xFA | ✅ |
| 0x83229AA0 | 104 | `XMsgInProcessCall` | 2 | XMsg: 0x7001B ao app 0xFA | ✅ |
| 0x83229B08 | 156 | `XMsgStartIORequest` | 0 | XMsg: 0x70004 ao app 0xFA | ✅ |
| 0x832EA980 | 68 | `XMsgInProcessCall` | 0 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x832EAB58 | 68 | `XMsgInProcessCall` | 1 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x832EABA0 | 116 | `XMsgInProcessCall` | 0 | XMsg: 0x58007 ao app 0xFC | ✅ |
| 0x832EAD40 | 224 | `XMsgInProcessCall` | 15 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x832EB080 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x832EB1E0 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x832EB610 | 564 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x832EB848 | 384 | `XMsgInProcessCall` | 0 | XMsg: 0x58019 ao app 0xFC | ✅ |
| 0x832EB9C8 | 312 | `XMsgInProcessCall` | 0 | XMsg: 0x58046 ao app 0xFC | ✅ |
| 0x832EBB00 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x5801E ao app 0xFC | ✅ |
| 0x832EBC40 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x58044 ao app 0xFC | ✅ |
| 0x832EBFE0 | 728 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x832EC2B8 | 644 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x832EC540 | 652 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x832EC7D0 | 568 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x832ECA08 | 472 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x832ECBE0 | 544 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x832ED068 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x832ED118 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x832ED1C0 | 168 | `XMsgStartIORequest` | 2 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x832ED268 | 164 | `XMsgStartIORequest` | 0 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x832ED310 | 164 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x832ED3B8 | 216 | `XMsgStartIORequest` | 1 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x832ED490 | 152 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x832ED528 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x832ED878 | 84 | `XMsgInProcessCall` | 0 | XMsg: 0xB0019 ao app 0xFB (Live) | ✅ |
| 0x832EDAB0 | 72 | `XMsgInProcessCall` | 3 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x832EDAF8 | 204 | `XMsgStartIORequest` | 1 | XMsg: 0x58009 ao app 0xFC | ✅ |
| 0x8332DD20 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x8332DDA0 | 136 | `XMsgStartIORequest` | 5 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x8332DE30 | 140 | `XMsgStartIORequest` | 1 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x8332DFF8 | 220 | `XMsgStartIORequest` | 2 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x8332E928 | 100 | `XMsgStartIORequest` | 0 | XMsg: 0x2000B ao app 0xFE (xam) | ✅ |
| 0x8332E990 | 132 | `XMsgStartIORequest` | 0 | XMsg: 0x2000D ao app 0xFE (xam) | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 2 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 17 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 40 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 6 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 7 | 0 | ✅ |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

### Todas as importações, por área

<details><summary><b>Rede</b> (66)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASend` | xam 0x17 | 1 | 0 | ❌ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetCreateKey` | xam 0x36 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetBroadcastVersionStatus` | xam 0x4C | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` | xam 0x4A | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetOpt` | xam 0x4E | 1 | 0 | 🟡 |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToServer` | xam 0x3D | 1 | 0 | ❌ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosGetListenStats` | xam 0x4D | 1 | 0 | ❌ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetOpt` | xam 0x4F | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetTsAddrToInAddr` | xam 0x3B | 1 | 0 | ❌ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
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
| `XNetLogonGetMachineID` | xam 0x135 | 2 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 6 | 0 | ✅ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (62)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 0 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExSemaphoreObjectType` | xboxkrnl 0x17 | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `ExTimerObjectType` | xboxkrnl 0x1C | 0 | 0 | ✅ |
| `InterlockedFlushSList` | xboxkrnl 0x2B | 10 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 22 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 116 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 12 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 10 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 1 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 71 | 0 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 0 | ✅ |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 1 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 2 | 0 | ✅ |
| `KeRaiseIrqlToDpcLevel` | xboxkrnl 0x85 | 117 | 0 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 2 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 144 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 1 | 0 | ✅ |
| `KeResumeThread` | xboxkrnl 0x92 | 1 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 3 | 0 | ✅ |
| `KeSetDisableBoostThread` | xboxkrnl 0x9C | 1 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 7 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 11 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 5 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 20 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 14 | 0 | ✅ |
| `KeTryToAcquireSpinLockAtRaisedIrql` | xboxkrnl 0xAE | 2 | 0 | 🟡 |
| `KeUnlockL2` | xboxkrnl 0x6C | 2 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 1 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 5 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 9 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 11 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 3 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 232 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 42 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 254 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 4 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 114 | 0 | ✅ |
| `XexUnloadImageAndExitThread` | xboxkrnl 0x1A2 | 1 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (45)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscGetCacheElementCount` | xboxkrnl 0x20 | 1 | 0 | ⚪ stub |
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
| `NtClose` | xboxkrnl 0xCF | 11 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 2 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 3 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 8 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 4 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 10 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 3 | 0 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 25 | 0 | ✅ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByName` | xboxkrnl 0x10D | 3 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 10 | 0 | ✅ |
| `XamContentClose` | xam 0x25A | 1 | 0 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceName` | xam 0x25F | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 2 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentGetThumbnail` | xam 0x261 | 1 | 0 | ✅ |
| `XamContentInstall` | xam 0x26A | 1 | 0 | ❌ |
| `XamContentLaunchImage` | xam 0x263 | 1 | 0 | ❌ |
| `XamContentResolve` | xam 0x268 | 2 | 0 | 🟡 |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (34)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowPlayersUI` | xam 0x2C8 | 1 | 0 | ❌ |
| `XamShowQuickChatUI` | xam 0x2C2 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamShowSigninUIp` | xam 0x2BE | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 5 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 6 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 0 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (29)</summary>

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
| `VdQuerySystemCommandBuffer` | xboxkrnl 0x1C8 | 1 | 0 | ❌ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 3 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBuffer` | xboxkrnl 0x1D8 | 1 | 0 | ❌ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 2 | 0 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 3 | 0 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (21)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 20 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 2 | 0 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 3 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 7 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 0 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 2 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 3 | 0 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 3 | 0 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 2 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (20)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioQueryDriverPerformance` | xboxkrnl 0x1FD | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 2 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMADisableContext` | xboxkrnl 0x228 | 2 | 0 | ✅ |
| `XMAEnableContext` | xboxkrnl 0x227 | 3 | 0 | ✅ |
| `XMAGetInputBufferReadOffset` | xboxkrnl 0x238 | 1 | 0 | ✅ |
| `XMAGetOutputBufferReadOffset` | xboxkrnl 0x22B | 1 | 0 | ✅ |
| `XMAGetOutputBufferWriteOffset` | xboxkrnl 0x229 | 1 | 0 | ✅ |
| `XMAInitializeContext` | xboxkrnl 0x225 | 1 | 0 | ✅ |
| `XMAIsInputBuffer0Valid` | xboxkrnl 0x22F | 1 | 0 | ✅ |
| `XMAIsInputBuffer1Valid` | xboxkrnl 0x231 | 1 | 0 | ✅ |
| `XMAIsOutputBufferValid` | xboxkrnl 0x22D | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |
| `XMASetInputBuffer0Valid` | xboxkrnl 0x22E | 1 | 0 | ✅ |
| `XMASetInputBuffer1Valid` | xboxkrnl 0x230 | 1 | 0 | ✅ |
| `XMASetInputBufferReadOffset` | xboxkrnl 0x237 | 1 | 0 | ✅ |
| `XMASetOutputBufferReadOffset` | xboxkrnl 0x22A | 2 | 0 | ✅ |
| `XMASetOutputBufferValid` | xboxkrnl 0x22C | 3 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 9 | 0 | ✅ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 10 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 2 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 23 | 0 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 4 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 4 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 2 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_vsnprintf` | xboxkrnl 0x14D | 5 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 9 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 3 | 0 | ✅ |
| `MmAllocatePhysicalMemory` | xboxkrnl 0xB9 | 1 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 7 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 7 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 3 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 14 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 11 | 0 | ✅ |
| `NtProtectVirtualMemory` | xboxkrnl 0xE1 | 2 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 18 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 22 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 3 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 9 | 0 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 4 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 3 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 2 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 17 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 40 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 6 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 7 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 2 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 2 | 0 | ✅ |

</details>

<details><summary><b>Outras</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `KfLowerIrql` | xboxkrnl 0xB3 | 145 | 0 | ✅ |

</details>

**Ausentes no Xenia (26):** `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetInAddrToServer`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetServerToInAddr`, `NetDll_XNetSetOpt`, `NetDll_XNetTsAddrToInAddr`, `RtlCaptureContext`, `RtlUnwind`, `VdQuerySystemCommandBuffer`, `VdSetSystemCommandBuffer`, `XamContentInstall`, `XamContentLaunchImage`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamShowQuickChatUI`, `XamShowVoiceMailUI`, `XexUnloadImageAndExitThread`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamShowVoiceMailUI`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/50centbloo/` (`50centbloo.img`, `50centbloo_imports.json`, `50centbloo_kinect.json`, `50centbloo_fngraph.json`, `c_full/<endereço>.c`: 123 funções).
- Projeto Ghidra: `games/decompilado/50centbloo/ghidra/50centbloo` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
