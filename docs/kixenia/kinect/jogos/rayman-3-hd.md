# Rayman 3 HD (título 5841123F)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `Rayman 3 HD` (pacote XBLA (`000D0000/`)). **Executável:** 13184 KB de imagem, base 0x82000000, entrada 0x820E13F8, 21679 funções no `.pdata`, `.text` 0x820e0000–0x8280C550.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 2 importações (`XAudioGetVoiceCategoryVolume`, `XamShowVoiceMailUI`); 0 funções próprias e 2 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XamShowVoiceMailUI`**.
- **Mensagens aos apps do sistema (XMsg):** 4 importações (`XMsgCancelIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`); 0 funções próprias e 79 do runtime.

## Chamadas ao sistema

**364 importações** (176 do `xam.xex`, 188 do `xboxkrnl.exe`), 2102 chamadas diretas no código. Estado no Xenia Edge (kixenia): 237 implementadas, 22 aproximadas, 39 stubs, 65 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 81 funções que chamam o sistema, **81 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **0 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

<details><summary><b>Runtime NUI embutido</b> (81 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x822FC010 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x822FC088 | 120 | `XMsgStartIORequest` | 0 | XMsg: 0xB0071 ao app 0xFB (Live) | ✅ |
| 0x822FC108 | 140 | `XMsgStartIORequest` | 3 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x822FC1A0 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x822FC308 | 124 | `XMsgStartIORequest` | 0 | XMsg: 0xB0020 ao app 0xFB (Live) | ✅ |
| 0x822FC398 | 216 | `XMsgStartIORequest` | 3 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x822FC980 | 68 | `XamShowVoiceMailUI` | 0 |  | ✅ |
| 0x822FCDB0 | 40 | `XMsgCancelIORequest` | 1 |  | ✅ |
| 0x822FD1D8 | 232 | `XMsgStartIORequest` | 0 | XMsg: 0x20021 ao app 0xFE (xam) | ✅ |
| 0x822FD300 | 100 | `XMsgStartIORequest` | 0 | XMsg: 0x2000B ao app 0xFE (xam) | ✅ |
| 0x822FD368 | 132 | `XMsgStartIORequest` | 0 | XMsg: 0x2000D ao app 0xFE (xam) | ✅ |
| 0x82491F88 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x826D78E0 | 68 | `XMsgInProcessCall` | 0 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x826D7AD0 | 68 | `XMsgInProcessCall` | 0 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x826D7B18 | 164 | `XMsgInProcessCall` | 0 | XMsg: 0x58007 ao app 0xFC | ✅ |
| 0x826D7C48 | 84 | `XMsgInProcessCall` | 0 | XMsg: 0x58052 ao app 0xFC | ✅ |
| 0x826D7D78 | 252 | `XMsgInProcessCall` | 17 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x826D7F50 | 100 | `XMsgInProcessCall` | 2 | XMsg: 0x58072 ao app 0xFC | ✅ |
| 0x826D7FB8 | 276 | `XMsgStartIORequest` | 0 | XMsg: 0x5806A ao app 0xFC | ✅ |
| 0x826D80D0 | 260 | `XMsgStartIORequest` | 0 | XMsg: 0x5806B ao app 0xFC | ✅ |
| 0x826D81D8 | 96 | `XMsgStartIORequest` | 0 | XMsg: 0x5806C ao app 0xFC | ✅ |
| 0x826D8238 | 96 | `XMsgStartIORequest` | 0 | XMsg: 0x5806D ao app 0xFC | ✅ |
| 0x826D8298 | 88 | `XMsgStartIORequest` | 0 | XMsg: 0x5806E ao app 0xFC | ✅ |
| 0x826D82F0 | 60 | `XMsgStartIORequest` | 0 | XMsg: 0x5806F ao app 0xFC | ✅ |
| 0x826D83A8 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x826D8508 | 304 | `XMsgInProcessCall` | 0 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x826D8638 | 752 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D8928 | 568 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D8B60 | 384 | `XMsgInProcessCall` | 0 | XMsg: 0x58019 ao app 0xFC | ✅ |
| 0x826D8CE0 | 312 | `XMsgInProcessCall` | 0 | XMsg: 0x58046 ao app 0xFC | ✅ |
| 0x826D8E18 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x5801E ao app 0xFC | ✅ |
| 0x826D8F58 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x58044 ao app 0xFC | ✅ |
| 0x826D9098 | 608 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D92F8 | 732 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D95D8 | 648 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D9860 | 656 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D9AF0 | 572 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D9D30 | 476 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826D9F10 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x826DA158 | 552 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826DA380 | 548 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x826DA5A8 | 432 | `XMsgStartIORequest` | 1 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x826DA758 | 188 | `XMsgStartIORequest` | 0 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x826DA818 | 176 | `XMsgStartIORequest` | 2 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x826DA8C8 | 172 | `XMsgStartIORequest` | 0 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x826DA978 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x826DAA28 | 168 | `XMsgStartIORequest` | 0 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x826DAAD0 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x826DAB78 | 220 | `XMsgStartIORequest` | 0 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x826DAC58 | 156 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x826DACF8 | 176 | `XMsgStartIORequest` | 2 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x826DADA8 | 228 | `XMsgStartIORequest` | 0 | XMsg: 0xB0016 ao app 0xFB (Live) | ✅ |
| 0x826DAE90 | 232 | `XMsgStartIORequest` | 0 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x826DAF78 | 280 | `XMsgStartIORequest` | 0 | XMsg: 0xB0065 ao app 0xFB (Live) | ✅ |
| 0x826DB090 | 220 | `XMsgStartIORequest` | 0 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x826DB170 | 84 | `XMsgInProcessCall` | 0 | XMsg: 0xB0019 ao app 0xFB (Live) | ✅ |
| 0x826DB1C8 | 164 | `XMsgStartIORequest` | 1 | XMsg: 0xB0026 ao app 0xFB (Live) | ✅ |
| 0x826DB2F0 | 192 | `XMsgStartIORequest` | 1 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x827C93E8 | 80 | `XMsgStartIORequestEx` | 0 |  | ✅ |
| 0x827C95F8 | 292 | `XMsgInProcessCall` | 0 | XMsg: 0x70025 ao app 0xFA | ✅ |
| 0x827C9720 | 152 | `XMsgStartIORequest` | 0 | XMsg: 0x7003D ao app 0xFA | ✅ |
| 0x827C97B8 | 164 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70078 ao app 0xFA | ✅ |
| 0x827C9860 | 140 | `XMsgStartIORequest` | 0 | XMsg: 0x7007A ao app 0xFA | ✅ |
| 0x827C98F0 | 136 | `XMsgStartIORequest` | 0 | XMsg: 0x7007B ao app 0xFA | ✅ |
| 0x827C9978 | 116 | `XMsgInProcessCall` | 0 | XMsg: 0x7007C ao app 0xFA | ✅ |
| 0x827C99F0 | 152 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70079 ao app 0xFA | ✅ |
| 0x827C9A88 | 384 | `XMsgInProcessCall` | 0 | XMsg: 0x70014 ao app 0xFA | ✅ |
| 0x827C9C08 | 504 | `XMsgStartIORequest` | 0 | XMsg: 0x7002B ao app 0xFA | ✅ |
| 0x827C9E00 | 396 | `XMsgInProcessCall` | 0 | XMsg: 0x7001C ao app 0xFA | ✅ |
| 0x827C9F90 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0x7002A ao app 0xFA | ✅ |
| 0x827CA048 | 176 | `XMsgStartIORequestEx` | 0 | XMsg: 0x7006F ao app 0xFA | ✅ |
| 0x827CA0F8 | 176 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70075 ao app 0xFA | ✅ |
| 0x827CA1F0 | 144 | `XMsgStartIORequest` | 0 | XMsg: 0x7000E ao app 0xFA | ✅ |
| 0x827CA280 | 148 | `XMsgStartIORequest` | 0 | XMsg: 0x70008 ao app 0xFA | ✅ |
| 0x827CA318 | 108 | `XMsgInProcessCall` | 0 | XMsg: 0x70029 ao app 0xFA | ✅ |
| 0x827CA388 | 100 | `XMsgInProcessCall` | 0 | XMsg: 0x70009 ao app 0xFA | ✅ |
| 0x827CA3F0 | 100 | `XMsgInProcessCall` | 0 | XMsg: 0x7000B ao app 0xFA | ✅ |
| 0x827CA458 | 136 | `XMsgStartIORequest` | 0 | XMsg: 0x7000C ao app 0xFA | ✅ |
| 0x827CA4E0 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x827CA588 | 168 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70068 ao app 0xFA | ✅ |
| 0x827CA630 | 104 | `XMsgInProcessCall` | 2 | XMsg: 0x7001B ao app 0xFA | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 22 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 51 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 7 | 0 | ✅ |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |

### Todas as importações, por área

<details><summary><b>Rede</b> (97)</summary>

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
| `NetDll_XNetDnsReverseLookup` | xam 0x55 | 1 | 0 | ❌ |
| `NetDll_XNetDnsReverseRelease` | xam 0x56 | 1 | 0 | ❌ |
| `NetDll_XNetGetBroadcastVersionStatus` | xam 0x4C | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` | xam 0x4A | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetOpt` | xam 0x4E | 1 | 0 | 🟡 |
| `NetDll_XNetGetSystemLinkPort` | xam 0x53 | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetGetXnAddrPlatform` | xam 0x52 | 1 | 0 | ❌ |
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
| `NetDll_XNetReplaceKey` | xam 0x51 | 1 | 0 | ❌ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetOpt` | xam 0x4F | 1 | 0 | ❌ |
| `NetDll_XNetSetSystemLinkPort` | xam 0x54 | 1 | 0 | ✅ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetTsAddrToInAddr` | xam 0x3B | 1 | 0 | ❌ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpCapture` | xam 0x6C | 1 | 0 | ❌ |
| `NetDll_XnpConfig` | xam 0x68 | 1 | 0 | ❌ |
| `NetDll_XnpConfigUPnP` | xam 0x67 | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptRecv` | xam 0x6F | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptSetCallbacks` | xam 0x6D | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptXmit` | xam 0x6E | 1 | 0 | ❌ |
| `NetDll_XnpGetActiveSocketList` | xam 0x7A | 1 | 0 | ❌ |
| `NetDll_XnpGetConfigStatus` | xam 0x69 | 1 | 0 | ❌ |
| `NetDll_XnpGetVlanXboxName` | xam 0x78 | 1 | 0 | ❌ |
| `NetDll_XnpLoadConfigParams` | xam 0x65 | 1 | 0 | ❌ |
| `NetDll_XnpLoadMachineAccount` | xam 0x6A | 1 | 0 | ❌ |
| `NetDll_XnpLogonClearQEvent` | xam 0x74 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQFlags` | xam 0x71 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQVals` | xam 0x75 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetPState` | xam 0x77 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQEvent` | xam 0x73 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQFlags` | xam 0x72 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQVals` | xam 0x76 | 1 | 0 | ❌ |
| `NetDll_XnpNoteSystemTime` | xam 0x7B | 1 | 0 | ❌ |
| `NetDll_XnpRegisterKeyForCallerType` | xam 0x7C | 1 | 0 | ❌ |
| `NetDll_XnpSaveConfigParams` | xam 0x66 | 1 | 0 | ❌ |
| `NetDll_XnpSaveMachineAccount` | xam 0x6B | 1 | 0 | ❌ |
| `NetDll_XnpSetVlanXboxName` | xam 0x79 | 1 | 0 | ❌ |
| `NetDll_XnpUnregisterKeyForCallerType` | xam 0x7D | 1 | 0 | ❌ |
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
| `XNetLogonGetMachineID` | xam 0x135 | 3 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 7 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (59)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscGetCacheElementCount` | xboxkrnl 0x20 | 1 | 0 | ⚪ stub |
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
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
| `NtClose` | xboxkrnl 0xCF | 28 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 3 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 7 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 13 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 4 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 9 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 11 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 10 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 5 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 33 | 0 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByName` | xboxkrnl 0x10D | 3 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 0 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 9 | 0 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
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

<details><summary><b>Threads, sincronização e tempo</b> (59)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 0 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExSemaphoreObjectType` | xboxkrnl 0x17 | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `ExTimerObjectType` | xboxkrnl 0x1C | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 13 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 10 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 103 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 0 | 🟡 |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 8 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 5 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 7 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 5 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 3 | 0 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` | xboxkrnl 0x9C | 1 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 26 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 2 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 19 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 11 | 0 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 2 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 4 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 10 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 9 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 9 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 6 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 426 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 23 | 0 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 2 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 498 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 6 | 0 | ✅ |
| `XexUnloadImageAndExitThread` | xboxkrnl 0x1A2 | 1 | 0 | ❌ |

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
| `XamShowMarketplaceUI` | xam 0x2C7 | 2 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowPlayersUI` | xam 0x2C8 | 1 | 0 | ❌ |
| `XamShowQuickChatUI` | xam 0x2C2 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamShowSigninUIp` | xam 0x2BE | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 1 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 2 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 2 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 2 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 3 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 8 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 2 | 0 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (29)</summary>

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
| `VdQuerySystemCommandBuffer` | xboxkrnl 0x1C8 | 1 | 0 | ❌ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBuffer` | xboxkrnl 0x1D8 | 1 | 0 | ❌ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 0 | ⚪ stub |
| `XamReadTileToTexture` | xam 0x2F5 | 3 | 0 | ⚪ stub |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (23)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 6 | 0 | ✅ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 10 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 2 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 35 | 0 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 7 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 5 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 2 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `XamReadString` | xam 0x2F6 | 1 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 11 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 6 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 9 | 0 | ✅ |
| `vsprintf` | xboxkrnl 0x14E | 1 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (20)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 20 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 12 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 4 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 3 | 0 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 8 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 7 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 6 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 2 | 0 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 3 | 0 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 3 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 6 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 12 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 6 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 15 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 15 | 0 | ✅ |
| `NtProtectVirtualMemory` | xboxkrnl 0xE1 | 2 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 22 | 0 | ✅ |
| `XamAllocEx` | xam 0x1EB | 3 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 24 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 3 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 8 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 3 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 0 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 0 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 22 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 51 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 7 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 2 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 2 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |

</details>

<details><summary><b>Voz e microfones</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |

</details>

**Ausentes no Xenia (65):** `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetDnsReverseLookup`, `NetDll_XNetDnsReverseRelease`, `NetDll_XNetGetXnAddrPlatform`, `NetDll_XNetInAddrToServer`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetReplaceKey`, `NetDll_XNetServerToInAddr`, `NetDll_XNetSetOpt`, `NetDll_XNetTsAddrToInAddr`, `NetDll_XnpCapture`, `NetDll_XnpConfig`, `NetDll_XnpConfigUPnP`, `NetDll_XnpEthernetInterceptRecv`, `NetDll_XnpEthernetInterceptSetCallbacks`, `NetDll_XnpEthernetInterceptXmit`, `NetDll_XnpGetActiveSocketList`, `NetDll_XnpGetConfigStatus`, `NetDll_XnpGetVlanXboxName`, `NetDll_XnpLoadConfigParams`, `NetDll_XnpLoadMachineAccount`, `NetDll_XnpLogonClearQEvent`, `NetDll_XnpLogonGetQFlags`, `NetDll_XnpLogonGetQVals`, `NetDll_XnpLogonGetStatus`, `NetDll_XnpLogonSetPState`, `NetDll_XnpLogonSetQEvent`, `NetDll_XnpLogonSetQFlags`, `NetDll_XnpLogonSetQVals`, `NetDll_XnpNoteSystemTime`, `NetDll_XnpRegisterKeyForCallerType`, `NetDll_XnpSaveConfigParams`, `NetDll_XnpSaveMachineAccount`, `NetDll_XnpSetVlanXboxName`, `NetDll_XnpUnregisterKeyForCallerType`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `VdQuerySystemCommandBuffer`, `VdSetSystemCommandBuffer`, `XamContentInstall`, `XamContentLaunchImage`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamShowQuickChatUI`, `XamShowVoiceMailUI`, `XeKeysConsoleSignatureVerification`, `XexUnloadImageAndExitThread`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XamShowVoiceMailUI`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/raym3hd/` (`raym3hd.img`, `raym3hd_imports.json`, `raym3hd_kinect.json`, `raym3hd_fngraph.json`, `c_full/<endereço>.c`: 102 funções).
- Projeto Ghidra: `games/decompilado/raym3hd/ghidra/raym3hd` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
