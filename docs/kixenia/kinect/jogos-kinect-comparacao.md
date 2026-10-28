# Jogos Kinect: comparação do que chamam

Os dois jogos Kinect que temos, lado a lado: o que cada um chama do sistema e as funções do **runtime NUI** que os dois trazem embutido.

Relacionado: [Nike-Kinect-Training](jogos/nike-kinect-training.md), [Kinect-Sports](jogos/kinect-sports.md), [Kinect-Mapa-XAM](kinect-mapa-xam.md), [Funcoes-XAM-Indice](../xam/funcoes/README.md), [Kinect-Xenia-Edge](kinect-xenia-edge.md).

## Números

| | Nike+ Kinect Training (4D53090F) | Kinect Sports (4D5308C9) |
|---|---|---|
| Importações | 391 (163 xam, 228 kernel) | 376 (159 xam, 217 kernel) |
| Chamadas diretas | 3695 | 4224 |
| Importações do Kinect/NUI | 41 | 35 |
| Funções que chamam o Kinect | 94 | 74 |
| Importações em comum | 328 | |

## O runtime NUI é o mesmo nos dois

**68 funções** que falam com o sistema são idênticas nos dois jogos: mesmo tamanho, mesmas importações e mesmas mensagens, só em endereços diferentes. É a biblioteca NUI da Microsoft (o XDK do Kinect), ligada estaticamente em cada jogo. Isso vale para o motor de inclinação, o motor de identidade, os dados biométricos, as mensagens NUI ao xam, o chat de voz e os serviços da Live.

| Nike | Kinect Sports | Tamanho | O que faz |
|---|---|---|---|
| 0x82640698 | 0x82D283E0 | 352 | XMsgInProcessCall |
| 0x826407F8 | 0x82D28540 | 304 | XMsgInProcessCall |
| 0x82640928 | 0x82D28670 | 752 | XMsgStartIORequest |
| 0x8264D990 | 0x82D2A130 | 252 | XMsgInProcessCall |
| 0x82A46BC0 | 0x82D29DE8 | 72 | XMsgInProcessCall |
| 0x82A50930 | 0x82D29E30 | 248 | envia 0xB001E |
| 0x82A779B0 | 0x82D28F38 | 432 | envia 0xB0010 |
| 0x82A77B60 | 0x82D290E8 | 188 | envia 0xB0018 |
| 0x82A77C20 | 0x82D291A8 | 176 | envia 0xB0011 |
| 0x82A77CD0 | 0x82D29258 | 172 | envia 0xB0012 |
| 0x82A77D80 | 0x82D29308 | 172 | envia 0xB0012 |
| 0x82A77E30 | 0x82D293B8 | 168 | envia 0xB0013 |
| 0x82A77ED8 | 0x82D29460 | 168 | envia 0xB0013 |
| 0x82A77F80 | 0x82D29508 | 220 | envia 0xB001A |
| 0x82A78060 | 0x82D295E8 | 156 | envia 0xB0014 |
| 0x82A78100 | 0x82D29688 | 176 | envia 0xB0015 |
| 0x82A781B0 | 0x82D29820 | 232 | envia 0xB001C |
| 0x82A78298 | 0x82D29908 | 220 | envia 0xB001B |
| 0x82A78378 | 0x82D299E8 | 192 | envia 0xB0025 |
| 0x82ADE890 | 0x82D27BE0 | 168 | XMsgStartIORequestEx |
| 0x82ADE938 | 0x82D27C88 | 104 | XMsgInProcessCall |
| 0x82D49F18 | 0x82C34980 | 200 | Chat de voz: abaixa o som do jogo durante o bate-papo (`XamVoiceIsActiveProcess`). |
| 0x82D4B1B0 | 0x82C35C18 | 928 | Chat de voz: fone presente e voz ativa. |
| 0x82D4B7F0 | 0x82C36258 | 316 | Chat de voz: fecha o objeto de voz. |
| 0x82D4BB80 | 0x82C365E8 | 600 | Chat de voz: envia pacotes de voz (`XamVoiceSubmitPacket`). |
| 0x82D4BEE0 | 0x82C36948 | 240 | Chat de voz: fecha o objeto de voz. |
| 0x82D4BFD0 | 0x82C36A38 | 192 | Chat de voz: cria e fecha o objeto de voz. |
| 0x82D4EF48 | 0x82C398C0 | 688 | Chat de voz: estado da voz. |
| 0x82D51370 | 0x82C3BCE8 | 208 | Chat de voz: envia pacotes. |
| 0x82DA4780 | 0x82A74960 | 120 | envia 0xB0008 |
| 0x82DA47F8 | 0x82A74A50 | 140 | envia 0xB0006 |
| 0x82DA4890 | 0x82A74AE8 | 184 | envia 0xB0007 |
| 0x82DA49F8 | 0x82A74C50 | 216 | envia 0xB0021 |
| 0x82DA60E8 | 0x82A77580 | 56 | Mensagem 0x2B001 ao xam (início do runtime NUI). |
| 0x82DA6520 | 0x82A74DE0 | 40 | XMsgCancelIORequest |
| 0x82F476B8 | 0x82A01458 | 96 | O único ponto que chama `PsCamDeviceRequest`: todos os pedidos à câmera (estado, leitura de quadros, ajustes) passam por aqui. |
| 0x82F530F8 | 0x829FF008 | 284 | Motor de inclinação: parar o motor (`ElevationStopMovement`) e informar o status ao sistema (0x2B006/0x2B00C). |
| 0x82F533A8 | 0x829FF318 | 132 | Motor de inclinação: mover para um ângulo (`ElevationSetAngle`). |
| 0x82F53450 | 0x829FF3C0 | 76 | Motor de inclinação: ler o status do motor e do acelerômetro (`TiltGetStatus`, 'XtSs'). |
| 0x82F537D8 | 0x829FF748 | 144 | Motor de inclinação: mover para um ângulo (segunda variante). |
| 0x82F53868 | 0x829FF7D8 | 324 | Motor de inclinação: guardar o plano do chão (`RememberFloor`, cerca de 25 vezes por segundo) e informar o status. |
| 0x82F53A40 | 0x829FF9B0 | 532 | Motor de inclinação: ler o ângulo atual (`ElevationGetAngle`) e decidir o próximo passo. |
| 0x82F53ED0 | 0x829FFE40 | 780 | **Callback do motor de inclinação** registrado no sistema: recebe do xam AutoTilt (0x2B005), StopMovement (0x2B009), ReverseAutoTilt (0x2B00A) e AdjustTilt (0x2B00B) e responde com o status (0x2B006/0x2B00C). |
| 0x82F54290 | 0x82A00200 | 148 | Registra o callback do motor de inclinação no xam (`XamNuiCameraTiltSetCallback`, 0x2B007). |
| 0x82F59850 | 0x82A0E018 | 540 | Motor de identidade: abre uma sessão (`IdentityGetSessionId`) e acha o perfil de um cadastro (`XamUserNuiGetUserIndex`). |
| 0x82F59A70 | 0x82A0E238 | 432 | Motor de identidade: usa o ID de sessão nos pedidos. |
| 0x82F5BA00 | 0x82A101C8 | 356 | Motor de identidade: acha o perfil ligado a um cadastro (`XamUserNuiGetUserIndex`). |
| 0x82F5CA88 | 0x82A10FB0 | 388 | Motor de identidade: usa o ID de sessão. |
| 0x82F5CC10 | 0x82A11138 | 3200 | Motor de identidade: a função principal (3,2 KB), chamada por 5 lugares; trata o resultado das identificações. |
| 0x82F65D68 | 0x82A1B170 | 996 | Motor de identidade: usa o ID de sessão. |
| 0x82F6A138 | 0x82A1F540 | 392 | Kinect ID: carrega os rostos cadastrados do cache (lê o índice do cadastro, `XamUserNuiGetEnrollmentIndex`). |
| 0x82F6A380 | 0x82A1F788 | 336 | Kinect ID: acha o perfil de um cadastro. |
| 0x82F6A600 | 0x82A1FA08 | 248 | Kinect ID: cache de arquivos do xam (`XamCacheOpen/Close/Reset`). |
| 0x82F6A6F8 | 0x82A1FB00 | 488 | Kinect ID: lê os dados biométricos do perfil (`XamReadBiometricData`) e repassa ao cache (0x2A007). |
| 0x82F6AD68 | 0x82A20170 | 604 | **Kinect ID: grava o cadastro do rosto**: lê a cópia do perfil, soma 1 à versão, monta os dados, grava no cache (0x2A007), liga a biometria (`XamUserNuiEnableBiometric`) e grava no perfil (`XamWriteBiometricData`). Disparada quando há 200 amostras do rosto (0x82F5E040). |
| 0x838C26F0 | 0x82A841A8 | 60 | Mensagem 0x2C001 ao xam (0x10 bytes). |
| 0x838C2730 | 0x82A841E8 | 72 | Mensagem 0x2C003 ao xam (0x18 bytes). |
| 0x838C2778 | 0x82A84230 | 72 | Mensagem 0x2C004 ao xam (0x18 bytes). |
| 0x838C27C0 | 0x82A84278 | 64 | Mensagem 0x2C005 ao xam (0xC bytes). |
| 0x838C2800 | 0x82A842B8 | 56 | **Registra o callback de identidade** no xam (0x2C009). |
| 0x838C2838 | 0x82A842F0 | 76 | Mensagem 0x2C00C: dados de identidade (op 1 converte os dados do Kinect ID; op 2 lê parâmetros por nome como `NuiV1QualDist`). 9 chamadores. |
| 0x838C2888 | 0x82A84340 | 68 | Mensagem 0x2C00D: resolução de identidade (qual cadastro foi reconhecido). |
| 0x838C28D0 | 0x82A84388 | 80 | Mensagem 0x2A009 (biometria desligada). |
| 0x838C2920 | 0x82A843D8 | 112 | Mensagem 0x2A008: lê o cache biométrico do perfil (versão 0 = o xam ainda não tem). |
| 0x838C2990 | 0x82A84448 | 100 | Mensagem 0x2A007: grava o cache biométrico do perfil. |
| 0x838C29F8 | 0x82A844B0 | 80 | Mensagem 0x2A006. |
| 0x838C2A48 | 0x82A84500 | 112 | Mensagem 0x2A005. |
| 0x838C2AB8 | 0x82A84570 | 100 | Mensagem 0x2A004. |

## O que é só de cada jogo

**Só no Nike** (funções que falam com o Kinect): 0x82640C18 (XMsgStartIORequest), 0x82640E58 (XMsgInProcessCall, XMsgStartIORequest), 0x8264D7E8 (XMsgInProcessCall), 0x82A46C08 (XMsgInProcessCall), 0x82ADE5F0 (XMsgStartIORequest), 0x82ADE828 (XMsgInProcessCall), 0x82D9D348 (XMsgStartIORequest), 0x82D9D498 (XamShowSigninUI), 0x82DA6070 (XamShowNuiGuideUI), 0x82DA6078 (XamShowNuiSigninUI), 0x82DA6088 (XamShowNuiGamerCardUIForXUID), 0x82DA6090 (XamShowNuiAchievementsUI), 0x82DA6098 (XamShowNuiDeviceSelectorUI), 0x82DA6120 (XMsgInProcessCall), 0x82DA6138 (XMsgInProcessCall), 0x82DA6150 (XMsgInProcessCall), 0x82DA6168 (XamNuiGetDeviceStatus), 0x82DA6DF0 (XMsgCompleteIORequest, XMsgInProcessCall), 0x82E038E0 (XamNuiGetDeviceSerialNumber, XamVoiceGetMicArrayStatus), 0x82E039A8 (XamNuiGetDeviceStatus), 0x82F34B18 (XamNuiGetDeviceSerialNumber), 0x82F34D00 (XAudioCaptureRenderDriverFrame, XamVoiceGetMicArrayAudioEx, XamVoiceGetMicArrayUnderrunStatus), 0x82F36688 (XamVoiceGetMicArrayAudioEx), 0x82F554F8 (XamShowNuiTroubleshooterUI), 0x82F907D0 (XMsgInProcessCall), 0x8367D4B8 (XMsgStartIORequest).

**Só no Kinect Sports:** 0x829FF2B8 (XamNuiCameraElevationGetAngle), 0x82A749D8 (XMsgStartIORequest), 0x82D28960 (XMsgStartIORequest), 0x82D28C40 (XMsgStartIORequest), 0x82D29738 (XMsgStartIORequest), 0x82D29D50 (XMsgInProcessCall).

**Importações só do Nike:** `ExAcquireReadWriteLockExclusive`, `ExAcquireReadWriteLockShared`, `ExAllocatePool`, `ExInitializeReadWriteLock`, `ExReleaseReadWriteLock`, `FscSetCacheElementCount`, `InterlockedPopEntrySList`, `KeCancelTimer`, `KeInitializeSemaphore`, `KeInitializeTimerEx`, `KeReleaseSemaphore`, `KeSetTimer`, `MicDeviceRequest`, `MmQueryStatistics`, `NetDll_XNetCleanup`, `NetDll_XNetXnAddrToMachineId`, `NetDll_XnpLogonGetStatus`, `NtOpenSymbolicLinkObject`, `NtQuerySymbolicLinkObject`, `NtSignalAndWaitForSingleObjectEx`, `RmcDeviceRequest`, `RtlCopyString`, `RtlDowncaseUnicodeChar`, `XAudioCaptureRenderDriverFrame`, `XAudioGetRenderDriverTic`, `XAudioRegisterRenderDriverMECClient`, `XAudioUnregisterRenderDriverMECClient`, `XGetVideoCapabilities`, `XMsgCompleteIORequest`, `XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XamContentCreateDeviceEnumerator`, `XamContentCreateInternal`, `XamContentDeleteInternal`, `XamContentFlush`, `XamContentGetCreator`, `XamContentSetThumbnail`, `XamGetOverlappedResult`, `XamNuiGetDeviceSerialNumber`, `XamNuiGetDeviceStatus`, `XamParseGamerTileKey`, `XamReadTileToTexture`, `XamShowAchievementsUI`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowMarketplaceDownloadItemsUI`, `XamShowMessageBoxUI`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowNuiAchievementsUI`, `XamUserCreateStatsEnumerator`, `XamUserGetAgeGroup`, `XamUserGetCachedUserFlags`, `XamUserWriteProfileSettings`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayStatus`, `XamVoiceGetMicArrayUnderrunStatus`, `_snwprintf`.

**Importações só do Kinect Sports:** `LDIResetDecompression`, `MmLockAndMapSegmentArray`, `MmUnlockAndUnmapSegmentArray`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSAEventSelect`, `NetDll_WSARecv`, `NetDll_WSAResetEvent`, `NetDll_WSASend`, `NetDll_WSASetEvent`, `NetDll_WSASetLastError`, `NetDll_WSAWaitForMultipleEvents`, `NetDll_XNetQosServiceLookup`, `NetDll_getpeername`, `NetDll_getsockopt`, `NetDll_recvfrom`, `NtCreateIoCompletion`, `NtQueueApcThread`, `NtRemoveIoCompletion`, `NtSetIoCompletion`, `NtYieldExecution`, `XamAvatarGetAssets`, `XamAvatarGetAssetsResultSize`, `XamAvatarGetManifestLocalUser`, `XamAvatarGetMetadataRandom`, `XamAvatarGetMetadataSignedOutProfile`, `XamAvatarInitialize`, `XamAvatarLoadAnimation`, `XamAvatarManifestGetBodyType`, `XamAvatarSetCustomAsset`, `XamAvatarShutdown`, `XamShowDirtyDiscErrorUI`, `XamShowGoldUpgradeUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiMarketplaceUI`, `XamUserGetMembershipTierFromXUID`, `XamUserGetOnlineCountryFromXUID`, `XeCryptSha256Final`, `XeCryptSha256Init`, `XeCryptSha256Update`, `XeCryptSha384Final`, `XeCryptSha384Init`, `XeCryptSha384Update`, `XeCryptSha512Final`, `XeCryptSha512Init`, `XeCryptSha512Update`, `XeCryptShaFinal`, `XeCryptShaInit`, `XeCryptShaUpdate`.

### O que isso mostra
- **Nike:** usa os **microfones do Kinect** para a voz (`XamVoiceGetMicArrayAudioEx`, cancelamento de eco com `XAudioRegisterRenderDriverMECClient` e `XAudioCaptureRenderDriverFrame`), o fone do controle (`MicDeviceRequest`), a **câmera Xbox Live Vision** (`XUsbcam*`), o número de série e o status do sensor, grava conteúdo (perfil de treino) e abre várias telas da Live.
- **Kinect Sports:** usa **avatares** (10 funções `XamAvatar*`), rede direta por sockets (`WSASend`, `WSARecv`, `recvfrom`, portas de conclusão), SHA-1/256/384/512 e as telas Nui de Marketplace e "controle necessário".
- Os dois chamam `XamShowNuiGuideUI` só a partir de um atalho do jogo (Nike 0x82DA6070; Kinect Sports pela opção "Guide" do menu de pausa e pelo START). O gesto do guia é visto pelo **sistema**, no quadro que o jogo publica com 0x2B002 (Nike 0x82DA6120).
