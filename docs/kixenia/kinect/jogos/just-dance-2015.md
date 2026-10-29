# Just Dance 2015 (título 555308CD)

**No HD:** `Just Dance 2015` (`default.xex` solto). **Executável:** 17728 KB de imagem, base 0x82000000, entrada 0x82B7B178, 41023 funções no `.pdata`, `.text` 0x823d0000–0x82FE1C20.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Bateria ao vivo (Kinect real, log nível 3, `Just_Dance_2015.log`): rodou o tempo todo sem travar; 0 de 66 amostras com jogador rastreado.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Nome no XEX | | : Just Dance® 2015 |
| Runtime NUI | ✅ | iniciado: `5/5308 with flags 0000400B, HUD on` |
| Microfones | ✅ | o jogo lê a matriz de microfones (`XamVoiceGetMicArrayAudioEx`) |
| Kinect ID | ✅ | motor de identidade iniciado |

## Como o jogo usa o Kinect

- **Câmera e sensor:** 4 importações (`PsCamDeviceRequest`, `XamNuiCameraRememberFloor`, `XamNuiGetDeviceSerialNumber`, `XamNuiGetDeviceStatus`); 0 funções próprias e 6 do runtime; 9579 chamadas vistas em execução.
- **Inclinação e motor:** 6 importações (`XamNuiCameraElevationGetAngle`, `XamNuiCameraElevationSetAngle`, `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus`, `XamNuiCameraTiltReportStatus`, `XamNuiCameraTiltSetCallback`); 0 funções próprias e 9 do runtime; 66 chamadas vistas em execução.
- **Kinect ID e biometria:** 6 importações (`XamNuiIdentityGetSessionId`, `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamUserNuiGetEnrollmentIndex`, `XamUserNuiGetUserIndex`, `XamWriteBiometricData`); 0 funções próprias e 10 do runtime; 11 chamadas vistas em execução.
- **Voz e microfones:** 7 importações (`XAudioCaptureRenderDriverFrame`, `XAudioGetVoiceCategoryVolume`, `XAudioRegisterRenderDriverMECClient`, `XAudioUnregisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayStatus`, `XamVoiceGetMicArrayUnderrunStatus`); 0 funções próprias e 5 do runtime; 36943 chamadas vistas em execução; **faltam/stub: `XAudioGetVoiceCategoryVolume`**.
- **Telas Nui do sistema:** 8 importações (`XamShowNuiDeviceSelectorUI`, `XamShowNuiFriendRequestUI`, `XamShowNuiFriendsUI`, `XamShowNuiGuideUI`, `XamShowNuiMessageBoxUI`, `XamShowNuiPartyUI`, `XamShowNuiSigninUI`, `XamShowNuiTroubleshooterUI`); 0 funções próprias e 1 do runtime.
- **Mensagens aos apps do sistema (XMsg):** 6 importações (`XMsgCancelIORequest`, `XMsgCompleteIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`, `XMsgSystemProcessCall`); 0 funções próprias e 35 do runtime; 4014 chamadas vistas em execução.
- **Cache do xam:** 3 importações (`XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`); 0 funções próprias e 1 do runtime; 2 chamadas vistas em execução.

## Chamadas ao sistema

**388 importações** (181 do `xam.xex`, 207 do `xboxkrnl.exe`), 3306 chamadas diretas no código, 149 vistas em execução. Estado no Xenia Edge (kixenia): 267 implementadas, 34 aproximadas, 46 stubs, 40 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 65 funções que chamam o sistema, **65 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **0 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

<details><summary><b>Runtime NUI embutido</b> (65 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x828C4350 | 400 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82ADA268 | 96 | `PsCamDeviceRequest` | 0 |  | ✅ |
| 0x82AE9460 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada; `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82AE9680 | 432 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82AEB610 | 356 | `XamUserNuiGetUserIndex` | 0 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82AEC698 | 388 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82AEC820 | 3200 | `XamNuiIdentityGetSessionId` | 5 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82AEF6E8 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82AEF998 | 132 | `XamNuiCameraElevationSetAngle` | 2 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82AEFA40 | 76 | `XamNuiCameraTiltGetStatus` | 0 | `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x82AEFDC8 | 144 | `XamNuiCameraElevationSetAngle` | 3 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82AEFE58 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraRememberFloor`: guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes); `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82AF0030 | 676 | `XamNuiCameraElevationGetAngle` | 1 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82AF02D8 | 780 | `XamNuiCameraTiltReportStatus` | 0 | `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82AF0698 | 148 | `XamNuiCameraTiltSetCallback` | 1 | `XamNuiCameraTiltSetCallback`: 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| 0x82AF0F78 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | ✅ |
| 0x82AF79F0 | 996 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82AFBDA0 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | `XamUserNuiGetEnrollmentIndex`: +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| 0x82AFBFE8 | 336 | `XamUserNuiGetUserIndex` | 1 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82AFC268 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 |  | ✅ |
| 0x82AFC360 | 488 | `XamReadBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| 0x82AFC9D0 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes; `XamUserNuiEnableBiometric`: configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| 0x82B21EB8 | 432 | `XMsgStartIORequest` | 1 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x82B22068 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x82B22118 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x82B221C8 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x82B22270 | 156 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x82B22310 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x82B223C0 | 220 | `XMsgStartIORequest` | 1 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x82B22D48 | 252 | `XMsgInProcessCall` | 6 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x82B22F60 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x82B230C0 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x82B231F0 | 312 | `XMsgInProcessCall` | 1 | XMsg: 0x58046 ao app 0xFC | ✅ |
| 0x82B23328 | 572 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82B23568 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 2 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x82B237B0 | 548 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82B281A0 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x82B7BBB0 | 56 | `XamShowNuiMessageBoxUI` | 1 | `XamShowNuiMessageBoxUI`: NuiHudCheck + caixa de mensagem com flag 0x2000 | ✅ |
| 0x82B7BBF0 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x82B7BC68 | 140 | `XMsgStartIORequest` | 0 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x82B7BD00 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x82B7D5D0 | 40 | `XMsgCancelIORequest` | 10 |  | ✅ |
| 0x82B7E468 | 40 | `XamNuiGetDeviceStatus` | 5 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 0x82B7E648 | 60 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C001 ao app 0xFE (xam) | ✅ |
| 0x82B7E688 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C003 ao app 0xFE (xam) | ✅ |
| 0x82B7E6D0 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C004 ao app 0xFE (xam) | ✅ |
| 0x82B7E718 | 64 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C005 ao app 0xFE (xam) | ✅ |
| 0x82B7E758 | 56 | `XMsgInProcessCall` | 2 | XMsg: registra o callback de identidade (0x2C009) | ✅ |
| 0x82B7E790 | 76 | `XMsgInProcessCall` | 9 | XMsg: dados de identidade (0x2C00C) | ✅ |
| 0x82B7E7E0 | 68 | `XMsgInProcessCall` | 1 | XMsg: resolve o cadastro (0x2C00D) | ✅ |
| 0x82B81C38 | 80 | `XMsgStartIORequest` | 1 | XMsg: biometria desligada (0x2A009) | ✅ |
| 0x82B81C88 | 112 | `XMsgStartIORequest` | 3 | XMsg: lê o cache biométrico (0x2A008) | ✅ |
| 0x82B81CF8 | 100 | `XMsgStartIORequest` | 2 | XMsg: grava o cache biométrico (0x2A007) | ✅ |
| 0x82B81D60 | 80 | `XMsgStartIORequest` | 2 | XMsg: 0x2A006 ao app 0xFE (xam) | ✅ |
| 0x82B81DB0 | 112 | `XMsgStartIORequest` | 1 | XMsg: 0x2A005 ao app 0xFE (xam) | ✅ |
| 0x82B81E20 | 100 | `XMsgStartIORequest` | 1 | XMsg: 0x2A004 ao app 0xFE (xam) | ✅ |
| 0x82CA19E0 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x82CA1A88 | 104 | `XMsgInProcessCall` | 1 | XMsg: 0x7001B ao app 0xFA | ✅ |
| 0x82D08FA0 | 224 | `XamNuiGetDeviceSerialNumber` | 1 | `XamNuiGetDeviceSerialNumber`: Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas | ✅ |
| 0x82D09188 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | `XamVoiceGetMicArrayAudioEx`: pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Rid… | ✅ |
| 0x82D0A048 | 1252 | `XAudioUnregisterRenderDriverMECClient` | 4 |  | ✅ |
| 0x82D0A650 | 1504 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | `XamVoiceGetMicArrayAudioEx`: pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Rid… | ✅ |
| 0x82D21D38 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | `XamNuiGetDeviceSerialNumber`: Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas; `XamVoiceGetMicArrayStatus`: 2 sem matriz; senão o sinal de 0x81AACBF8 | ✅ |
| 0x82D21E00 | 124 | `XamNuiGetDeviceStatus` | 1 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 0x82E998A8 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8906 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 1 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 14 | 4007 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 19 | 6 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 63 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 672 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendRequestUI` | xam 0x479 | 1 | 0 | ✅ |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ✅ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 9237 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 27705 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Rede</b> (66)</summary>

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
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XHttpCloseHandle` | xam 0xCC | 1 | 0 | ❌ |
| `NetDll_XHttpConnect` | xam 0xCD | 1 | 0 | ❌ |
| `NetDll_XHttpCrackUrl` | xam 0xDC | 1 | 0 | ✅ |
| `NetDll_XHttpDoWork` | xam 0xD8 | 1 | 0 | ❌ |
| `NetDll_XHttpOpen` | xam 0xCB | 1 | 1 | ⚪ stub |
| `NetDll_XHttpOpenRequest` | xam 0xCF | 1 | 0 | ❌ |
| `NetDll_XHttpQueryHeaders` | xam 0xD3 | 1 | 0 | ❌ |
| `NetDll_XHttpReadData` | xam 0xD4 | 1 | 0 | ❌ |
| `NetDll_XHttpReceiveResponse` | xam 0xD2 | 1 | 0 | ❌ |
| `NetDll_XHttpSendRequest` | xam 0xD1 | 1 | 0 | ❌ |
| `NetDll_XHttpSetOption` | xam 0xD7 | 1 | 0 | ❌ |
| `NetDll_XHttpSetStatusCallback` | xam 0xCE | 1 | 0 | ❌ |
| `NetDll_XHttpShutdown` | xam 0xCA | 1 | 0 | ⚪ stub |
| `NetDll_XHttpStartup` | xam 0xC9 | 1 | 1 | ⚪ stub |
| `NetDll_XHttpWriteData` | xam 0xD5 | 1 | 0 | ❌ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetCreateKey` | xam 0x36 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` | xam 0x4A | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetSystemLinkPort` | xam 0x54 | 1 | 0 | ✅ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
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
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XNetLogonGetMachineID` | xam 0x135 | 1 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 3 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (64)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 41 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 5 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 5 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 1 | 1 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 27678 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 61 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 1 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 1 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 11 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 14698 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 11884 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 61 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 21 | 35495 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3984 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3984 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 37 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 47 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 101 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 12 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 4 | 3 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 4 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 16 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 18 | 80 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 6 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 29 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 39 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 40 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 108591 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 34 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 7 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 825 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 151 | 18553 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 5 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 931 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 10 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 7351 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 3 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (59)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 1 | ⚪ stub |
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
| `NtClose` | xboxkrnl 0xCF | 30 | 141 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 5 | 68 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 6 | 262 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 15 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 17 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 4 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 10 | 4 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 11 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 3 | 105 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 5 | 4 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 4 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 12 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 11 | 9 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 4 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 44 | 36276 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 4 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 2 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 14 | 36277 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 9 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 1 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceName` | xam 0x25F | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 10 | 6 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (30)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 16 | 58710 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 15 | 4 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 2 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 2 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 17 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetLanguage` | xam 0x3D2 | 1 | 0 | ✅ |
| `XamGetLocaleEx` | xam 0x5D7 | 1 | 5 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 0 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 3 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 3989 | ✅ |
| `XamGetToken` | xam 0x4BE | 1 | 0 | ❌ |
| `XamIsUIActive` | xam 0x2EC | 2 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 18 | 22145 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 6 | 2 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 6 | 13 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 1 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 6 | 17 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 2 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 7313 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 7314 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 3 | 6 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 1 | 4 | ⚪ stub |

</details>

<details><summary><b>Usuários, perfis e Live</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 6 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserCheckPrivilege` | xam 0x212 | 1 | 1 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 28983 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 3 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 1 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 3 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 5 | 1 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 3 | 2 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 18 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 15 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 3 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 4 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 9 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 4 | 2 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 29 | 143 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 25 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 20 | 5 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 7 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 4836 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 40 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 1 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 2 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 529060 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 200 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 10 | 1 | ✅ |
| `XamFree` | xam 0x1EC | 11 | 1 | ✅ |
| `XamFreeToken` | xam 0x4BF | 1 | 0 | ❌ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8906 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 63 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 672 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Áudio</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 1 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 10 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 0 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 2 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 1 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 5 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 2 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendRequestUI` | xam 0x479 | 1 | 0 | ✅ |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ✅ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MicDeviceRequest` | xboxkrnl 0x2B1 | 4 | 0 | ⚪ stub |
| `RmcDeviceRequest` | xboxkrnl 0x2E1 | 5 | 0 | ❌ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 9237 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 27705 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetCapabilitiesEx` | xam 0x2AD | 1 | 21946 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 7314 | ✅ |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputRawState` | xam 0x195 | 1 | 0 | ❌ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 14 | 4007 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 19 | 6 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 1 | 1 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 1 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 1 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Outras</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XampXAuthGetTitleBuffer` | xam 0x4EE | 1 | 0 | ⚪ stub |
| `XampXAuthShutdown` | xam 0x4BD | 1 | 0 | ⚪ stub |
| `XampXAuthStartup` | xam 0x4BC | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 12 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 733 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 3 | 10 | ✅ |

</details>

**Ausentes no Xenia (40):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_XHttpCloseHandle`, `NetDll_XHttpConnect`, `NetDll_XHttpDoWork`, `NetDll_XHttpOpenRequest`, `NetDll_XHttpQueryHeaders`, `NetDll_XHttpReadData`, `NetDll_XHttpReceiveResponse`, `NetDll_XHttpSendRequest`, `NetDll_XHttpSetOption`, `NetDll_XHttpSetStatusCallback`, `NetDll_XHttpWriteData`, `NetDll_XNetServerToInAddr`, `NetDll_XnpLogonGetStatus`, `NtWriteFileGather`, `ObIsTitleObject`, `RmcDeviceRequest`, `RtlCaptureContext`, `RtlUnwind`, `XamFreeToken`, `XamGetToken`, `XamInputRawState`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGamerCardUIForXUID`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`.
- Aproximadas e chamadas em execução: `XamCacheOpenFile`, `XamCacheReset`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/justdanc2015/` (`justdanc2015.img`, `justdanc2015_imports.json`, `justdanc2015_kinect.json`, `justdanc2015_fngraph.json`, `c_full/<endereço>.c`: 133 funções).
- Projeto Ghidra: `games/decompilado/justdanc2015/ghidra/justdanc2015` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
- Log da bateria: `~/Kixenia/kinect-tools/bateria/saida-primeira/Just_Dance_2015.log`.
