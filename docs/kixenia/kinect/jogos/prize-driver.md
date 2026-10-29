# Prize Driver (título 584112AE)

**No HD:** `Prize Driver` (pacote XBLA (`000D0000/`)). **Executável:** 21632 KB de imagem, base 0x82000000, entrada 0x82C2BC60, 52552 funções no `.pdata`, `.text` 0x82370000–0x8332BC40.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

- **Câmera e sensor:** 4 importações (`PsCamDeviceRequest`, `XamNuiCameraRememberFloor`, `XamNuiGetDeviceSerialNumber`, `XamNuiGetDeviceStatus`); 0 funções próprias e 6 do runtime.
- **Inclinação e motor:** 6 importações (`XamNuiCameraElevationGetAngle`, `XamNuiCameraElevationSetAngle`, `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus`, `XamNuiCameraTiltReportStatus`, `XamNuiCameraTiltSetCallback`); 0 funções próprias e 10 do runtime.
- **Kinect ID e biometria:** 6 importações (`XamNuiIdentityGetSessionId`, `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamUserNuiGetEnrollmentIndex`, `XamUserNuiGetUserIndex`, `XamWriteBiometricData`); 0 funções próprias e 10 do runtime.
- **Voz e microfones:** 12 importações (`XAudioCaptureRenderDriverFrame`, `XAudioGetVoiceCategoryVolume`, `XAudioRegisterRenderDriverMECClient`, `XAudioUnregisterRenderDriverMECClient`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayStatus` e mais 4); 0 funções próprias e 13 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`**.
- **Telas Nui do sistema:** 9 importações (`XamShowNuiAchievementsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiDeviceSelectorUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiGuideUI`, `XamShowNuiMarketplaceUI`, `XamShowNuiSigninUI` e mais 1); 0 funções próprias e 0 do runtime.
- **Câmera Xbox Live Vision:** 7 importações (`XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XUsbcamSetView`); 0 funções próprias e 9 do runtime; **faltam/stub: `XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XUsbcamSetView`**.
- **Avatares:** 10 importações (`XamAvatarGetAssets`, `XamAvatarGetAssetsResultSize`, `XamAvatarGetManifestLocalUser`, `XamAvatarGetMetadataRandom`, `XamAvatarGetMetadataSignedOutProfile`, `XamAvatarInitialize`, `XamAvatarLoadAnimation`, `XamAvatarManifestGetBodyType` e mais 2); 0 funções próprias e 9 do runtime; **faltam/stub: `XamAvatarGetMetadataSignedOutProfile`**.
- **Mensagens aos apps do sistema (XMsg):** 6 importações (`XMsgCancelIORequest`, `XMsgCompleteIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`, `XMsgSystemProcessCall`); 0 funções próprias e 54 do runtime.
- **Cache do xam:** 3 importações (`XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`); 0 funções próprias e 1 do runtime.

## Chamadas ao sistema

**423 importações** (182 do `xam.xex`, 241 do `xboxkrnl.exe`), 6340 chamadas diretas no código. Estado no Xenia Edge (kixenia): 293 implementadas, 41 aproximadas, 50 stubs, 38 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 108 funções que chamam o sistema, **108 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **0 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

<details><summary><b>Runtime NUI embutido</b> (108 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82A769B0 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82A76C60 | 92 | `XamNuiCameraElevationGetAngle` | 2 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82A76CC0 | 132 | `XamNuiCameraElevationSetAngle` | 2 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82A76D68 | 76 | `XamNuiCameraTiltGetStatus` | 0 | `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x82A770F0 | 144 | `XamNuiCameraElevationSetAngle` | 3 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82A77180 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraRememberFloor`: guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes); `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82A77358 | 676 | `XamNuiCameraElevationGetAngle` | 2 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82A777E8 | 780 | `XamNuiCameraTiltReportStatus` | 0 | `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82A77BA8 | 148 | `XamNuiCameraTiltSetCallback` | 1 | `XamNuiCameraTiltSetCallback`: 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| 0x82A783F0 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | ✅ |
| 0x82A7CCA8 | 96 | `PsCamDeviceRequest` | 0 |  | ✅ |
| 0x82A89818 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada; `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82A89A38 | 432 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82A8B9C8 | 356 | `XamUserNuiGetUserIndex` | 0 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82A8C7B0 | 388 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82A8C938 | 3200 | `XamNuiIdentityGetSessionId` | 5 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82A99E10 | 996 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82A9E1E0 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | `XamUserNuiGetEnrollmentIndex`: +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| 0x82A9E428 | 336 | `XamUserNuiGetUserIndex` | 1 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82A9E6A8 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 |  | ✅ |
| 0x82A9E7A0 | 488 | `XamReadBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| 0x82A9EDB8 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes; `XamUserNuiEnableBiometric`: configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| 0x82AC48B0 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | `XamNuiGetDeviceSerialNumber`: Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas; `XamVoiceGetMicArrayStatus`: 2 sem matriz; senão o sinal de 0x81AACBF8 | ✅ |
| 0x82AC4978 | 124 | `XamNuiGetDeviceStatus` | 1 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 0x82C29F08 | 40 | `XMsgCancelIORequest` | 11 |  | ✅ |
| 0x82C2C120 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x82C2C198 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0071 ao app 0xFB (Live) | ✅ |
| 0x82C2C210 | 140 | `XMsgStartIORequest` | 1 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x82C2C2A8 | 184 | `XMsgStartIORequest` | 1 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x82C2C410 | 216 | `XMsgStartIORequest` | 2 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x82C385F8 | 56 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2B001 (NUI) | ✅ |
| 0x82C38678 | 40 | `XamNuiGetDeviceStatus` | 4 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 0x82C38E18 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82C39F48 | 60 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C001 ao app 0xFE (xam) | ✅ |
| 0x82C39F88 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C003 ao app 0xFE (xam) | ✅ |
| 0x82C39FD0 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C004 ao app 0xFE (xam) | ✅ |
| 0x82C3A018 | 64 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C005 ao app 0xFE (xam) | ✅ |
| 0x82C3A058 | 56 | `XMsgInProcessCall` | 2 | XMsg: registra o callback de identidade (0x2C009) | ✅ |
| 0x82C3A090 | 76 | `XMsgInProcessCall` | 9 | XMsg: dados de identidade (0x2C00C) | ✅ |
| 0x82C3A0E0 | 68 | `XMsgInProcessCall` | 1 | XMsg: resolve o cadastro (0x2C00D) | ✅ |
| 0x82C3A128 | 80 | `XMsgStartIORequest` | 1 | XMsg: biometria desligada (0x2A009) | ✅ |
| 0x82C3A178 | 112 | `XMsgStartIORequest` | 3 | XMsg: lê o cache biométrico (0x2A008) | ✅ |
| 0x82C3A1E8 | 100 | `XMsgStartIORequest` | 2 | XMsg: grava o cache biométrico (0x2A007) | ✅ |
| 0x82C3A250 | 80 | `XMsgStartIORequest` | 2 | XMsg: 0x2A006 ao app 0xFE (xam) | ✅ |
| 0x82C3A2A0 | 112 | `XMsgStartIORequest` | 1 | XMsg: 0x2A005 ao app 0xFE (xam) | ✅ |
| 0x82C3A310 | 100 | `XMsgStartIORequest` | 1 | XMsg: 0x2A004 ao app 0xFE (xam) | ✅ |
| 0x82C513E8 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x82C95C40 | 124 | `XamAvatarShutdown` | 1 |  | ✅ |
| 0x82C95CC0 | 376 | `XamAvatarInitialize` | 1 |  | ✅ |
| 0x82C95E38 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | ✅ |
| 0x82C95ED0 | 168 | `XamAvatarGetMetadataRandom` | 4 |  | ✅ |
| 0x82C95F78 | 144 | `XamAvatarGetMetadataSignedOutProfile` | 0 |  | ✅ |
| 0x82C960F0 | 448 | `XamAvatarManifestGetBodyType` | 2 |  | ✅ |
| 0x82C96300 | 248 | `XamAvatarGetAssets` | 1 |  | ✅ |
| 0x82C97CE0 | 532 | `XamAvatarManifestGetBodyType` | 1 |  | ✅ |
| 0x82CA08D0 | 220 | `XamAvatarLoadAnimation` | 1 |  | ✅ |
| 0x82CA9578 | 292 | `XUsbcamCreate` | 2 |  | ✅ |
| 0x82CA96A0 | 160 | `XUsbcamDestroy` | 3 |  | ✅ |
| 0x82CA9740 | 92 | `XUsbcamGetState` | 5 |  | ✅ |
| 0x82CA97A0 | 332 | `XUsbcamSetConfig` | 1 |  | ✅ |
| 0x82CA98F0 | 236 | `XUsbcamSetView` | 1 |  | ✅ |
| 0x82CA9A80 | 284 | `XUsbcamSetCaptureMode` | 0 |  | ✅ |
| 0x82CA9BA0 | 240 | `XUsbcamReadFrame` | 0 |  | ✅ |
| 0x82CAF908 | 432 | `XUsbcamSetConfig` | 0 |  | ✅ |
| 0x82CAFAB8 | 428 | `XUsbcamSetConfig` | 1 |  | ✅ |
| 0x82CDBB20 | 200 | `XamVoiceIsActiveProcess` | 3 | `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x82CDCDB8 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced*; `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x82CDD3F8 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x82CDD788 | 600 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x82CDDAE8 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x82CDDBD8 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x82CE0A60 | 688 | `XamVoiceIsActiveProcess` | 3 | `XamVoiceIsActiveProcess`: kernel XVoiced* | ✅ |
| 0x82CE2E88 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x82E79FC0 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x82E7A068 | 104 | `XMsgInProcessCall` | 2 | XMsg: 0x7001B ao app 0xFA | ✅ |
| 0x82E7A7D8 | 352 | `XMsgInProcessCall` | 2 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x82E7A938 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x82E7AA68 | 752 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E7AD58 | 732 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E7B038 | 648 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E7B2C0 | 552 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E7B4E8 | 548 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82E7B780 | 152 | `XMsgInProcessCall` | 2 | XMsg: 0x58035 ao app 0xFC | ✅ |
| 0x82E7B818 | 152 | `XMsgInProcessCall` | 1 | XMsg: 0x58035 ao app 0xFC | ✅ |
| 0x82E7BC20 | 72 | `XMsgInProcessCall` | 1 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x82E7BC68 | 432 | `XMsgStartIORequest` | 2 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x82E7BE18 | 188 | `XMsgStartIORequest` | 1 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x82E7BED8 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x82E7BF88 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x82E7C038 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x82E7C0E8 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x82E7C190 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x82E7C238 | 220 | `XMsgStartIORequest` | 1 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x82E7C318 | 156 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x82E7C3B8 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x82E7C468 | 228 | `XMsgStartIORequest` | 1 | XMsg: 0xB0016 ao app 0xFB (Live) | ✅ |
| 0x82E7C550 | 232 | `XMsgStartIORequest` | 1 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x82E7C638 | 220 | `XMsgStartIORequest` | 1 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x82E7C718 | 192 | `XMsgStartIORequest` | 1 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x82E7C7D8 | 248 | `XMsgStartIORequest` | 2 | XMsg: 0xB001E ao app 0xFB (Live) | ✅ |
| 0x82E7C968 | 68 | `XMsgInProcessCall` | 1 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x82E7CB10 | 252 | `XMsgInProcessCall` | 7 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x82E7E810 | 228 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82E7E8F8 | 676 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x831D6790 | 224 | `XamNuiGetDeviceSerialNumber` | 1 | `XamNuiGetDeviceSerialNumber`: Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas | ✅ |
| 0x831D6978 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | `XamVoiceGetMicArrayAudioEx`: pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Rid… | ✅ |
| 0x831D7838 | 1252 | `XAudioUnregisterRenderDriverMECClient` | 3 |  | ✅ |
| 0x831D7D98 | 1504 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | `XamVoiceGetMicArrayAudioEx`: pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Rid… | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 0 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 17 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 31 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |
| `XUsbcamCreate` | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |
| `XUsbcamSetView` | xboxkrnl 0x220 | 1 | 0 | ❌ |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | 🟡 |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ✅ |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | 🟡 |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ✅ |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 0 | ✅ |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | 🟡 |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 3 | 0 | ✅ |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | 🟡 |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | 🟡 |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 0 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 0 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 0 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 0 | ✅ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ✅ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (71)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAcquireReadWriteLockExclusive` | xboxkrnl 0x7 | 1 | 0 | ✅ |
| `ExAcquireReadWriteLockShared` | xboxkrnl 0x8 | 6 | 0 | ✅ |
| `ExCreateThread` | xboxkrnl 0xD | 5 | 0 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExInitializeReadWriteLock` | xboxkrnl 0x11 | 1 | 0 | ✅ |
| `ExReleaseReadWriteLock` | xboxkrnl 0x16 | 7 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 3 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 1 | 0 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 60 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 2 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 0 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 0 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 0 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 0 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 2 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 0 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 0 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 25 | 0 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 0 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 0 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 0 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 7 | 0 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 104 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 9 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 9 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 1395 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 31 | 0 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 6 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 34 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 45 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 47 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 20 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1420 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 214 | 0 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 9 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1633 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 17 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (60)</summary>

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
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
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
| `XNetLogonGetTitleID` | xam 0x136 | 3 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (59)</summary>

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
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 49 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 11 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 7 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 3 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 11 | 0 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 5 | 0 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 8 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 4 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 14 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 14 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 0 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 49 | 0 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 0 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 3 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 15 | 0 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | 🟡 |
| `XamContentClose` | xam 0x25A | 9 | 0 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 10 | 0 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (31)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 18 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 15 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 2 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 0 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 0 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 17 | 0 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 0 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 3 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 7 | 0 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 2 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 0 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 0 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 17 | 0 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 8 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 9 | 0 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 19 | 0 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 2 | 0 | ✅ |
| `XexLoadImageHeaders` | xboxkrnl 0x19B | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 5 | 0 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
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
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 2 | 0 | 🟡 |
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
| `XGetVideoCapabilities` | xam 0x49E | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 6 | 0 | 🟡 |

</details>

<details><summary><b>Usuários, perfis e Live</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 0 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 0 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowGoldUpgradeUI` | xam 0x6A5 | 1 | 0 | ⚪ stub |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 4 | 0 | 🟡 |
| `XamUserGetAgeGroup` | xam 0x4A5 | 1 | 0 | ✅ |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 0 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 7 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 5 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (25)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 13 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 14 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 3 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 3 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 11 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 7 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 38 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 22 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 21 | 0 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 8 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 0 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 6 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 0 | ✅ |
| `NtProtectVirtualMemory` | xboxkrnl 0xE1 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 11 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 11 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 2 | 0 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeCryptSha256Final` | xboxkrnl 0x312 | 1 | 0 | ✅ |
| `XeCryptSha256Init` | xboxkrnl 0x310 | 1 | 0 | ✅ |
| `XeCryptSha256Update` | xboxkrnl 0x311 | 1 | 0 | ✅ |
| `XeCryptSha384Final` | xboxkrnl 0x318 | 1 | 0 | ❌ |
| `XeCryptSha384Init` | xboxkrnl 0x314 | 1 | 0 | ❌ |
| `XeCryptSha384Update` | xboxkrnl 0x315 | 1 | 0 | ❌ |
| `XeCryptSha512Final` | xboxkrnl 0x324 | 1 | 0 | ✅ |
| `XeCryptSha512Init` | xboxkrnl 0x31F | 1 | 0 | ✅ |
| `XeCryptSha512Update` | xboxkrnl 0x323 | 1 | 0 | ✅ |
| `XeCryptShaFinal` | xboxkrnl 0x191 | 1 | 0 | ✅ |
| `XeCryptShaInit` | xboxkrnl 0x18F | 1 | 0 | ✅ |
| `XeCryptShaUpdate` | xboxkrnl 0x190 | 1 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 0 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 3 | 0 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 0 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 0 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 0 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Avatares</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | 🟡 |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ✅ |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | 🟡 |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ✅ |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 0 | ✅ |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | 🟡 |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 3 | 0 | ✅ |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | 🟡 |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | 🟡 |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ✅ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 17 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 2 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 0 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 2 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Câmera Xbox Live Vision</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XUsbcamCreate` | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |
| `XUsbcamSetView` | xboxkrnl 0x220 | 1 | 0 | ❌ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 17 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 31 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 0 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 4 | 0 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 5 | 0 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 5 | 0 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

**Ausentes no Xenia (38):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetServerToInAddr`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XUsbcamDestroy`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XUsbcamSetView`, `XamShowMessageComposeUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeCryptSha384Final`, `XeCryptSha384Init`, `XeCryptSha384Update`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XUsbcamSetView`, `XamAvatarGetMetadataSignedOutProfile`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess`, `XamVoiceSubmitPacket`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/prizdriv/` (`prizdriv.img`, `prizdriv_imports.json`, `prizdriv_kinect.json`, `prizdriv_fngraph.json`, `c_full/<endereço>.c`: 221 funções).
- Projeto Ghidra: `games/decompilado/prizdriv/ghidra/prizdriv` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
