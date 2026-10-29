# Sonic Free Riders (título 5345084D)

**No HD:** `Sonic Free Riders [RF][Kinect]` (pacote GoD (cabeçalho em `Content/…/00007000/`)). **Executável:** 33408 KB de imagem, base 0x82000000, entrada 0x824D22F0, 23613 funções no `.pdata`, `.text` 0x82210000–0x82ACC4EC.

Sonic Free Riders (Sega, 2010, título de lançamento do Kinect): corrida em pranchas voadoras controlada com o corpo (inclinar para virar, agachar, braços). Jogo só de Kinect, com reconhecimento de voz: carrega `nuispeech:\speech\en-us\*.lxa` e lê a matriz de microfones a cada quadro (`XamVoiceGetMicArrayAudioEx` com `XamVoiceGetMicArrayUnderrunStatus`).

**Defeito achado e corrigido (2026-10-29):** o jogo guarda **dois pedidos estáticos de janela de áudio de 0x44 bytes**, um atrás do outro, em 0x82AFE1B8 e 0x82AFE1FC (`00000044`, 5 ponteiros de buffer, tamanhos 0x200 e 4× 0x400). O xam só escreve o tic em +0x40 quando o pedido declara mais de 0x40 bytes e a hora em +0x44/+0x48 quando declara mais de 0x64; o `KinectAudio::CompleteRequest` do Xenia escrevia os três sempre, e a hora caía em cima do tamanho e do ponteiro do segundo pedido. A partir daí cada janela de áudio (16 ms) era gravada num endereço lixo: constantes de fetch com tipo errado desde o quadro 6125, shaders com opcodes ALU desconhecidos no 6824 e crash em 0x82497E38 (liberador genérico de buffer do motor, com o cabeçalho do buffer sobrescrito) no 7062, ~160 s depois de abrir. Corrigido em 77342ab64 (o tic e a hora só com espaço declarado): 8 385 quadros limpos na rodada seguinte.

Para passar da tela inicial é preciso o gesto de engajamento (corpo inteiro no quadro, mão direita levantada) e depois o cursor de mão.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [XamVoiceGetMicArrayAudioEx (816FBCF8)](../../xam/funcoes/XamVoiceGetMicArrayAudioEx-816FBCF8.md).

## Estado no Xenia Edge

> [!NOTE]
> **Bateria ao vivo (Kinect real, log nível 3, `Sonic_Free_Riders.log`): rodou o tempo todo sem travar; 46 de 56 amostras com jogador rastreado.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Nome no XEX | | : SONIC FREE RIDERS |
| Runtime NUI | ✅ | iniciado: `5/2DFF with flags 0000001B, HUD on` |
| Microfones | ✅ | o jogo lê a matriz de microfones (`XamVoiceGetMicArrayAudioEx`) |
| Kinect ID | ✅ | motor de identidade iniciado |
| Arquivos do pacote | ✅ | abre arquivos de dentro da imagem (Kinect ID, bancos do rastreador) |

## Como o jogo usa o Kinect

- **Câmera e sensor:** 4 importações (`PsCamDeviceRequest`, `XamNuiCameraRememberFloor`, `XamNuiGetDeviceSerialNumber`, `XamNuiGetDeviceStatus`); 0 funções próprias e 5 do runtime; 15842 chamadas vistas em execução.
- **Inclinação e motor:** 6 importações (`XamNuiCameraElevationGetAngle`, `XamNuiCameraElevationSetAngle`, `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus`, `XamNuiCameraTiltReportStatus`, `XamNuiCameraTiltSetCallback`); 0 funções próprias e 9 do runtime; 54 chamadas vistas em execução.
- **Kinect ID e biometria:** 6 importações (`XamNuiIdentityGetSessionId`, `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamUserNuiGetEnrollmentIndex`, `XamUserNuiGetUserIndex`, `XamWriteBiometricData`); 0 funções próprias e 9 do runtime; 4 chamadas vistas em execução.
- **Voz e microfones:** 11 importações (`XAudioCaptureRenderDriverFrame`, `XAudioGetVoiceCategoryVolume`, `XAudioRegisterRenderDriverMECClient`, `XAudioUnregisterRenderDriverMECClient`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayStatus` e mais 3); 3 funções próprias (0x82924370, 0x82924C50, 0x82924EC8) e 8 do runtime; 29776 chamadas vistas em execução; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`**.
- **Telas Nui do sistema:** 9 importações (`XamShowNuiControllerRequiredUI`, `XamShowNuiDeviceSelectorUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiGuideUI`, `XamShowNuiMessageBoxUI`, `XamShowNuiPartyUI`, `XamShowNuiSigninUI` e mais 1); 0 funções próprias e 1 do runtime.
- **Avatares:** 9 importações (`XamAvatarGenerateMipMaps`, `XamAvatarGetAssets`, `XamAvatarGetAssetsResultSize`, `XamAvatarGetManifestLocalUser`, `XamAvatarGetMetadataRandom`, `XamAvatarInitialize`, `XamAvatarLoadAnimation`, `XamAvatarManifestGetBodyType` e mais 1); 0 funções próprias e 8 do runtime; 1 chamadas vistas em execução; **faltam/stub: `XamAvatarGenerateMipMaps`**.
- **Mensagens aos apps do sistema (XMsg):** 5 importações (`XMsgCancelIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`, `XMsgSystemProcessCall`); 0 funções próprias e 39 do runtime; 3427 chamadas vistas em execução.
- **Cache do xam:** 3 importações (`XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`); 0 funções próprias e 1 do runtime; 3 chamadas vistas em execução.

## Chamadas ao sistema

**312 importações** (111 do `xam.xex`, 201 do `xboxkrnl.exe`), 2557 chamadas diretas no código, 125 vistas em execução. Estado no Xenia Edge (kixenia): 228 implementadas, 31 aproximadas, 36 stubs, 16 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 79 funções que chamam o sistema, **76 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **3 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82924370 | 1280 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | `XamVoiceGetMicArrayAudioEx`: pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Rid… | ✅ |
| 0x82924C50 | 632 | `XAudioUnregisterRenderDriverMECClient` | 3 |  | ✅ |
| 0x82924EC8 | 1212 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 1 | `XamVoiceGetMicArrayAudioEx`: pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Rid… | ✅ |

<details><summary><b>Runtime NUI embutido</b> (76 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x824D03B8 | 40 | `XMsgCancelIORequest` | 1 |  | ✅ |
| 0x824D04B8 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x824D0530 | 120 | `XMsgStartIORequest` | 1 | XMsg: 0xB0071 ao app 0xFB (Live) | ✅ |
| 0x824D05A8 | 140 | `XMsgStartIORequest` | 0 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x824D0640 | 184 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x824D07A8 | 216 | `XMsgStartIORequest` | 1 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x824D0CE8 | 56 | `XamShowNuiMessageBoxUI` | 2 | `XamShowNuiMessageBoxUI`: NuiHudCheck + caixa de mensagem com flag 0x2000 | ✅ |
| 0x824D0D70 | 40 | `XamNuiGetDeviceStatus` | 3 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 0x8270E060 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |
| 0x82750E08 | 152 | `XamAvatarGetManifestLocalUser` | 3 |  | ✅ |
| 0x82750EA0 | 168 | `XamAvatarGetMetadataRandom` | 3 |  | ✅ |
| 0x82750F58 | 444 | `XamAvatarManifestGetBodyType` | 2 |  | ✅ |
| 0x82751808 | 220 | `XamAvatarLoadAnimation` | 1 |  | ✅ |
| 0x827534F0 | 248 | `XamAvatarGetAssets` | 1 |  | ✅ |
| 0x827535E8 | 172 | `XamAvatarGenerateMipMaps` | 1 |  | ✅ |
| 0x82753698 | 124 | `XamAvatarShutdown` | 3 |  | ✅ |
| 0x82753718 | 372 | `XamAvatarInitialize` | 1 |  | ✅ |
| 0x82764988 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82764C38 | 112 | `XamNuiCameraElevationSetAngle` | 2 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x82765028 | 144 | `XamNuiCameraElevationSetAngle` | 3 | `XamNuiCameraElevationSetAngle`: FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 0x827650B8 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | `XamNuiCameraRememberFloor`: guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes); `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82765290 | 680 | `XamNuiCameraElevationGetAngle` | 1 | `XamNuiCameraElevationGetAngle`: lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 0x82765538 | 780 | `XamNuiCameraTiltReportStatus` | 0 | `XamNuiCameraTiltReportStatus`: o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 0x82765848 | 144 | `XamNuiCameraTiltGetStatus` | 2 | `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x827658D8 | 80 | `XamNuiCameraTiltSetCallback` | 1 | `XamNuiCameraTiltSetCallback`: 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| 0x82766230 | 2456 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus` | 1 | `XamNuiCameraElevationStopMovement`: Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título; `XamNuiCameraTiltGetStatus`: status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 0x82769508 | 96 | `PsCamDeviceRequest` | 0 |  | ✅ |
| 0x82772DA0 | 392 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada; `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82772F28 | 340 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82773E28 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | `XamUserNuiGetEnrollmentIndex`: +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| 0x827745F0 | 300 | `XamUserNuiGetUserIndex` | 0 | `XamUserNuiGetUserIndex`: o usuário com aquele cadastro | ✅ |
| 0x82775110 | 568 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes; `XamUserNuiEnableBiometric`: configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| 0x82775348 | 3868 | `XamNuiIdentityGetSessionId` | 7 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x82776980 | 244 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 |  | ✅ |
| 0x82776A78 | 288 | `XamReadBiometricData` | 1 | `XamReadBiometricData`: XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| 0x82777660 | 672 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x8277EFF8 | 976 | `XamNuiIdentityGetSessionId` | 1 | `XamNuiIdentityGetSessionId`: 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 0x8278D170 | 432 | `XMsgStartIORequest` | 1 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x8278D320 | 188 | `XMsgStartIORequest` | 1 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x8278D3E0 | 176 | `XMsgStartIORequest` | 2 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x8278D490 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x8278D540 | 172 | `XMsgStartIORequest` | 2 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x8278D5F0 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x8278D698 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x8278D740 | 156 | `XMsgStartIORequest` | 1 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x8278D7E0 | 176 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x8278D890 | 232 | `XMsgStartIORequest` | 1 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x8278D978 | 192 | `XMsgStartIORequest` | 0 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x8278DA68 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x8278DBC8 | 304 | `XMsgInProcessCall` | 1 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x8278DEF0 | 244 | `XMsgInProcessCall` | 2 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x8278DFE8 | 248 | `XMsgStartIORequest` | 2 | XMsg: 0xB001E ao app 0xFB (Live) | ✅ |
| 0x8278E150 | 72 | `XMsgInProcessCall` | 3 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x828FFE98 | 888 | `XamVoiceHeadsetPresent` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced* | ✅ |
| 0x829004B0 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x82900840 | 600 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x82900BA0 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x82900C90 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x82903F88 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |
| 0x8290F0C0 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | `XamNuiGetDeviceSerialNumber`: Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas; `XamVoiceGetMicArrayStatus`: 2 sem matriz; senão o sinal de 0x81AACBF8 | ✅ |
| 0x8290F188 | 124 | `XamNuiGetDeviceStatus` | 1 | `XamNuiGetDeviceStatus`: FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 0x82A52860 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x82A52908 | 104 | `XMsgInProcessCall` | 2 | XMsg: 0x7001B ao app 0xFA | ✅ |
| 0x82A702B0 | 60 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C001 ao app 0xFE (xam) | ✅ |
| 0x82A702F0 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C003 ao app 0xFE (xam) | ✅ |
| 0x82A70338 | 72 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C004 ao app 0xFE (xam) | ✅ |
| 0x82A70380 | 64 | `XMsgSystemProcessCall` | 1 | XMsg: 0x2C005 ao app 0xFE (xam) | ✅ |
| 0x82A703C0 | 56 | `XMsgInProcessCall` | 2 | XMsg: registra o callback de identidade (0x2C009) | ✅ |
| 0x82A703F8 | 76 | `XMsgInProcessCall` | 9 | XMsg: dados de identidade (0x2C00C) | ✅ |
| 0x82A70448 | 68 | `XMsgInProcessCall` | 1 | XMsg: resolve o cadastro (0x2C00D) | ✅ |
| 0x82A70490 | 80 | `XMsgStartIORequest` | 1 | XMsg: biometria desligada (0x2A009) | ✅ |
| 0x82A704E0 | 112 | `XMsgStartIORequest` | 2 | XMsg: lê o cache biométrico (0x2A008) | ✅ |
| 0x82A70550 | 100 | `XMsgStartIORequest` | 2 | XMsg: grava o cache biométrico (0x2A007) | ✅ |
| 0x82A705B8 | 80 | `XMsgStartIORequest` | 2 | XMsg: 0x2A006 ao app 0xFE (xam) | ✅ |
| 0x82A70608 | 112 | `XMsgStartIORequest` | 1 | XMsg: 0x2A005 ao app 0xFE (xam) | ✅ |
| 0x82A70678 | 100 | `XMsgStartIORequest` | 1 | XMsg: 0x2A004 ao app 0xFE (xam) | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 11057 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 1 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 11 | 3386 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 23 | 38 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 3 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | 🟡 |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ✅ |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | 🟡 |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ✅ |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ✅ |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | 🟡 |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 2 | 0 | ✅ |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | 🟡 |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 2 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 51 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 4785 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 3 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ✅ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 7445 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 22330 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (60)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 5 | 41 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 1 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 4 | 1 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 18378 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 6 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 55 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 1 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 6 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 3 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 5 | 36 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 201 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 13 | 14993 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3367 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3367 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 44 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 29 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 71 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 30 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 1 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 3 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 9 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 10 | 9 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 26 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 42 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 53 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 1 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 4554 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 4532 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 14 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 622 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 120 | 905 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 6 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 695 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 2 | 29 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 2 | 32 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 5 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (54)</summary>

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
| `NtCancelIoFile` | xboxkrnl 0x2B8 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 26 | 177 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 9 | 186 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 156 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 2 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 2 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 0 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 2 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 6 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 3 | 73 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 2 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 2 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 2 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 10 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 4 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 3 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 36 | 8708 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 3 | 1 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 14 | 8711 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 2 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 7 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 2 | 1 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 1 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 6 | 6 | 🟡 |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 7 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 328 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 2 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 2683 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 22 | 56 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 19 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 13 | 15 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |
| `vsprintf` | xboxkrnl 0x14E | 2 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 8387 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 8388 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 7 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (21)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 6 | 45 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 10 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 10 | 42 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 9 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 5 | 4 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 10 | 17031 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 3 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 5 | 24 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 21 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 10 | 8 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Memória</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 1679 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 1 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 1 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 4 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 6 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 1862 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 1621 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 2 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 4 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 2 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 4 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 11 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 0 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 4 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 5 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 11057 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 51 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 4785 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ✅ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 7445 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 22330 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

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
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 2 | 0 | ✅ |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 0 | 🟡 |

</details>

<details><summary><b>Áudio</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 2 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 1 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 11 | 3386 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 23 | 38 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 3 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 8 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 3 | 1 | ⚪ stub |
| `XamEnumerate` | xam 0x250 | 3 | 3 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 4 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 8386 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 1 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 3 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 13 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 736 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 4 | 10 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 2 | 1 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 5 | 0 | ✅ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Outras</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `HidReadKeys` | xboxkrnl 0x1EB | 1 | 8386 | ⚪ stub |

</details>

**Ausentes no Xenia (16):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XamAvatarGenerateMipMaps`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`.
- Aproximadas e chamadas em execução: `XamCacheOpenFile`, `XamCacheReset`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/sonifreeride/` (`sonifreeride.img`, `sonifreeride_imports.json`, `sonifreeride_kinect.json`, `sonifreeride_fngraph.json`, `c_full/<endereço>.c`: 158 funções).
- Projeto Ghidra: `games/decompilado/sonifreeride/ghidra/sonifreeride` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
- Log da bateria: `~/Kixenia/kinect-tools/bateria/saida-sonic2/Sonic_Free_Riders.log`.
