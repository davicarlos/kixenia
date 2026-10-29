# Mapa completo do Kinect no XAM

Todas as **136 exports** do xam ligadas ao Kinect, o que cada uma faz e em que estado estão no Xenia. A análise detalhada de cada área está nas notas ligadas abaixo; aqui fica o mapa.

Relacionado: [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md), [Kinect-Xenia-Edge](kinect-xenia-edge.md), [Kinect-ID](kinect-id.md), [XAM-Fitness](../xam/xam-fitness.md), [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md), [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md), [Kinect-SDK-Windows](kinect-sdk-windows.md).

## Como foi feito

- **REA** (`open_binary` no `xam_17559.exe`, provedor Ghidra 12.1.4, análise automática padrão). O PE não traz os nomes das exports (o PDB `xamc.pdb` não é carregado), então os nomes vêm do `exports.csv` (ordinal → endereço).
- `batch_decompile` em dois lotes de 16 funções: evidências `ev_a432dd00…b933` (10 de 16) e o segundo lote (8 de 16). O REA recusa (`invalid_request`) funções cujo início o Ghidra padrão não reconhece no xam convertido. Essas foram decompiladas pelo projeto Ghidra `xamproj`, que tem o fluxo corrigido pelo script `XamFixFlow`.
- **Imports do kernel**: o registro de cada thunk é `0x01LLOOOO` (biblioteca 1 = xboxkrnl, ordinal). Os do Kinect:

| Thunk | Ordinal | Função do kernel |
|---|---|---|
| 0x81A73134 | 0x30D | PsCamDeviceRequest (câmera) |
| 0x81A73164 | 0x30E | McaDeviceRequest (áudio; motor do modelo 1473) |
| 0x81A73154 | 0x30F | DetroitDeviceRequest (hub; motor do modelo 1414) |
| 0x81A73144 | 0x373 | UsbdGetNatalHardwareVersion |
| 0x81A721F4 / 0x81A72204 / 0x81A72214 | 0x10 / 0x18 / 0x2BC | ExGetXConfigSetting / ExSetXConfigSetting / ExReadModifyWriteXConfigSettingUlong |

- **Estado no Xenia**: lido das declarações `DECLARE_XAM_EXPORT` do branch kixenia-main (2026-10-28). Contagem: 4 ausentes; 98 implementadas; 26 aproximadas; 8 stubs.

## Arquitetura

```
Título (runtime NUI próprio: rastreador na GPU, motor de identidade, motor de inclinação)
  │ exports Xam*Nui*, XMsg 0xFE (app do xam), callbacks registrados
  ▼
xam.xex
  ├─ HUD do sistema: XamNuiHudInterpretFrame, chamado em cada quadro 0x2B002
  ├─ serviço de identidade: repassa ao callback do título (0x2C009)
  ├─ repasse de inclinação: pedidos do sistema → callback do título (0x2B007)
  ├─ telas Nui: NuiHudCheck + tela comum
  └─ fitness (0xFB, Live), voz (XVoiced*)
  ▼
kernel: PsCam (câmera), Mca (áudio), Detroit (hub), XConfig
```

## Mensagens do app 0xFE

**0x2B0xx (despachante `FUN_816F9CB8`, `switch (msg − 0x2B001)`)**

| Mensagem | O que faz |
|---|---|
| 0x2B001 | `FUN_816AC460` |
| 0x2B002 | quadro de esqueleto do título (versão 5) → `XamNuiHudInterpretFrame` |
| 0x2B003 / 0x2B004 | início e fim do título (versões, flags) |
| 0x2B005, 0x2B009, 0x2B00A, 0x2B00B | AutoTilt, StopMovement, ReverseAutoTilt, AdjustTilt → callback do título em 0x81A85C40 |
| 0x2B006, 0x2B00C | status do motor do título → callback em 0x81A85C20 |
| 0x2B007 | registra o motor de inclinação do título (`XamNuiCameraTiltSetCallback`) |
| 0x2B00D | registra quem ouve o status (`XamNuiCameraElevationSetCallback`) |
| 0x2B00E | grava 0x81AAC9A8: o observador de quadros do Kinect Tuner, chamado a cada 0x2B002 antes do HUD; o xam nunca a envia (no Xenia desde 855a12dcc) |

**0x2C0xx (identidade)**: 0x2C006 a 0x2C008 identificar (antes consulta 0x2C013); 0x2C009 registra o callback; 0x2C00C dados de identidade; 0x2C00D resolve o cadastro; 0x2C00E cancelar; 0x2C010 cadastrar para login; 0x2C011 foto do rosto; 0x2C012 apagar cadastro; 0x2C013 dados do cadastro; **0x2C00A** pergunta um valor de 4 bytes ao callback (`FUN_8180DEB0`). O despachante das 0x2C0xx não foi localizado.

**0x2A0xx (biometria)**: 0x2A007 grava e 0x2A008 lê o cache de dados biométricos; 0x2A009 avisa que a biometria foi desligada.

**Telas (0x210xx)**: 0x21008 conquistas/jogos, 0x2100E desligar matriz de microfones, 0x21025 vídeo, 0x21028 Troubleshooter, 0x21030 guia, 0x21032 cartão de jogador, 0x21033 amigos, 0x21034 festa, 0x21035 convite, 0x21036 pedido de amizade, 0x21038 mensagens, 0x2103A a 0x21040 fitness.

## Os dois tipos de motor

`XamNuiCameraGetTiltControllerType` = `UsbdGetNatalHardwareVersion`: **1 = Detroit** (hub com motor, modelo 1414, `045e:02b0`), **2 = Mca** (motor pelo firmware de áudio, modelo 1473), 0 = sem motor (`0x8007048F`).

| Operação | Detroit (0x30F) | Mca (0x30E) |
|---|---|---|
| inclinar (`FUN_8180C700`) | comando 0x33148 (absoluto) ou 0x3314C, ângulo u16 | `{0, ângulo, relativo}` |
| parar | 0x3304 | código 0x24 |
| status (`FUN_8180C850`, 0x48 bytes) | 0xF0410, resposta de 0x45 bytes | código 0x23 |
| flags (`XamNuiCameraSetFlags`) | 0xF0510 | — |
| número de série | 0x770D4, 0x20 bytes | código 0x2B |
| faltas de áudio | — | código 0x20 (`XamVoiceGetMicArrayUnderrunStatus`) |

- O status que os títulos leem (`XamNuiCameraTiltGetStatus`) tem a assinatura **'XtSs'** (0x58745373) e 0x50 bytes. `ElevationGetAngle` calcula o ângulo com atan2 do acelerômetro.
- `SetAngle` desconta 0x81AAC9D0 (o deslocamento de calibração), e `RememberFloor` guarda o plano do chão em 0x81AAC9E0.
- **No Xenia:** desde o commit 6cb02fa57 o tipo é **2** com o Kinect configurado (o motor do 1473, e também a inclinação simulada sem motor), e 0 sem ele. Antes era sempre 1. O Nike e o Kinect Sports não consultam o tipo nem chamam `SetFlags`, e seguiram iguais nas gravações.

## Estado do sensor

- `XamNuiIsDeviceReady` = bit 1 de 0x81AD127C.
- A máquina de estados fica em 0x81AD1274 (`FUN_817F9F38`; eventos 0 inicializar, 6 e 7 atualização do firmware, 0xF desligado, 0x10 a 0x15 outros) e `FUN_817FC750` monta o status.
- **Sensor desligado** (`KinectDisabled`) faz o status responder 0xC000009D.

## Exports

### Inclinação e motor

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 2198 | 0x816FA338 | `XamNuiCameraAdjustTilt` | 0xFE 0x2B00B {u32, float, float} → motor de inclinação do título | ✅ |
| 2178 | 0x816FA3B8 | `XamNuiCameraElevationAutoTilt` | 0xFE 0x2B005 {float} → motor de inclinação do título | ✅ |
| 2177 | 0x816FAF68 | `XamNuiCameraElevationGetAngle` | lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | ✅ |
| 2188 | 0x816FA418 | `XamNuiCameraElevationReverseAutoTilt` | 0xFE 0x2B00A {u32} → motor de inclinação do título | ✅ |
| 2176 | 0x816FA0D0 | `XamNuiCameraElevationSetAngle` | FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | ✅ |
| 2180 | 0x816FA168 | `XamNuiCameraElevationSetCallback` | 0xFE 0x2B00D: registra quem ouve o status (0x81A85C20) | ✅ |
| 2179 | 0x816FA520 | `XamNuiCameraElevationStopMovement` | Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | ✅ |
| 2159 | 0x816FA088 | `XamNuiCameraGetTiltControllerType` | UsbdGetNatalHardwareVersion (kernel 0x373): 1 = Detroit (1414), 2 = Mca (1473), 0 = nenhum | ✅ |
| 2149 | 0x816FA618 | `XamNuiCameraRememberFloor` | guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes) | ✅ |
| 2148 | 0x816FB000 | `XamNuiCameraSetFlags` | Detroit: lê o status e grava flags com o comando 0xF0510; Mca: nada | ✅ |
| 2160 | 0x816FAA60 | `XamNuiCameraTiltGetStatus` | status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | ✅ |
| 2197 | 0x816FA1A8 | `XamNuiCameraTiltReportStatus` | o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| 2157 | 0x816FA128 | `XamNuiCameraTiltSetCallback` | 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |

### Sensor e estado

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 2171 | 0x81964638 | `XamIsNatalPlaybackEnabled` | sempre 0 (mesma função de 0x81964638) | ✅ |
| 2170 | 0x81964638 | `XamIsNuiAutomationEnabled` | sempre 0 (0x81964638 devolve 0) | ✅ |
| 1036 | 0x816C7D88 | `XamIsNuiUIActive` | lê 0x81AA218C (HUD ativo) | ✅ |
| 2137 | 0x817F8DE8 | `XamNatalDeviceAudioCalibrate` | envia MicArrayCalibrate.mec ao sensor (CEMD 0) | ✅ |
| 2142 | 0x816F9ED8 | `XamNuiEnableChatMic` | bit 0x8 de XCONFIG_CONSOLE_NUI + XamVoiceMuteMicArray | ✅ |
| 2272 | 0x816F9B90 | `XamNuiGetCameraIntrinsics` | PsCam 46, seletor 0x100: fx, fy, cx, cy da câmera de profundidade | ✅ focal do plano zero do sensor |
| 2144 | 0x816F9990 | `XamNuiGetDepthCalibration` | lê 0x30 bytes de um registro de configuração (0x3E00) | ✅ |
| 2269 | 0x817F99A0 | `XamNuiGetDeviceSerialNumber` | Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas | ✅ |
| 2181 | 0x817FC990 | `XamNuiGetDeviceStatus` | FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ |
| 2273 | 0x816F9BF0 | `XamNuiGetFanRate` | propriedade 0x838 pelo PsCam 20 (`{1, valor, 0, 0, 0}`); byte baixo do valor = ventilador | 🟡 lê o GET_PARAM 0x838 do firmware, que responde 0 |
| 2270 | 0x816F9A48 | `XamNuiGetLoadedDepthCalibration` | PsCam 44 → dois u32 (A e B da calibração) | ✅ |
| 2274 | 0x817FA2E0 | `XamNuiGetSupportString` | "K:%02d:...:%12s": modo, estado, problema, versão do firmware, série | ✅ |
| 2271 | 0x816F9B38 | `XamNuiGetTrueColorInfo` | PsCam 46: seletor 0 (4 bytes), 1 (tabela de autoexposição de 12 bytes por entrada) | 🟡 |
| 2143 | 0x816F9E90 | `XamNuiIsChatMicEnabled` | !(bit 0x8 de XCONFIG_CONSOLE_NUI) | ✅ |
| 2192 | 0x817F9988 | `XamNuiIsDeviceReady` | bit 1 de 0x81AD127C | ✅ |
| 2183 | 0x817FA270 | `XamNuiNatalCameraUpdateComplete` | evento 7, notificação 0xD; 0x80070002 → Troubleshooter | ✅ |
| 2182 | 0x817FA228 | `XamNuiNatalCameraUpdateStarting` | evento 6, notificação 0xC | ✅ |
| 2193 | 0x817FA170 | `XamNuiSetForceDeviceOff` | bit 31 de XCONFIG_USER_RETAIL_FLAGS; evento 0xF | ✅ |
| 2145 | 0x816F9A50 | `XamNuiStoreDepthCalibration` | grava 0x30 bytes no registro 0x3E00 | ✅ |

### HUD, esqueleto e gestos

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 2174 | 0x816DE4C8 | `ControlpackNuiCursorSetTrackingId` | função não definida pelo Ghidra (prólogo em 0x816DE4C8): lê os dados biométricos do perfil (armazém 0x18) e confere a data dos rostos cadastrados; o nome na tabela do Xenia parece errado | ❌ |
| 2173 | 0x816C7EB8 | `XamNuiHudEnableInputFilter` | com HUD: FUN_81692C48(0x81AA2160, p) | ✅ |
| 1307 | 0x816ACCE8 | `XamNuiHudGetEngagedEnrollmentIndex` | cópia do quadro do HUD: cadastro do engajado, ou −2 | ✅ |
| 2106 | 0x816AC408 | `XamNuiHudGetEngagedTrackingID` | HUD+0x118 | ✅ |
| 2175 | 0x816AC510 | `XamNuiHudGetInitializeFlags` | 0x81AAC990 (flags da 0x2B003) | ✅ |
| 2184 | 0x816AC518 | `XamNuiHudGetVersions` | 0x81AAC998 / 0x81AAC9A0 (versões xam/XTL da 0x2B003) | ✅ |
| 2172 | 0x816ACC68 | `XamNuiHudInterpretFrame` | o HUD do sistema: 2 jogadores, alertas, aceno, gesto do guia; 0x8301000B com UI aberta | ✅ |
| 2158 | 0x816AC4B8 | `XamNuiHudIsEnabled` | HUD existe, HUD+0x50 ≠ 0 e XamNuiIsDeviceReady | ✅ |
| 2108 | 0x816AC428 | `XamNuiHudSetEngagedTrackingID` | HUD+0x118 (sem HUD: 0x80004005) | ✅ |
| 2194 | 0x816F9978 | `XamNuiPlayerEngagementUpdate` | os 2 esqueletos mais próximos → quadro+0xACC (o rastreado ganha 0,3 m) | ✅ |
| 2195 | 0x816F9988 | `XamNuiSkeletonGetBestSkeletonIndex` | o rastreado entre os 2 escolhidos | ✅ |
| 2196 | 0x816F9980 | `XamNuiSkeletonScoreUpdate` | registros de 0x3C: alertas de borda, distância, giro, mãos | ✅ |

### Kinect ID

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 2199 | 0x8180D910 | `XamNuiIdentityAbort` | 0xFE 0x2C00E | ✅ |
| 2162 | 0x8180D970 | `XamNuiIdentityEnrollForSignIn` | 0xFE 0x2C010 {usuário, p2, p4} | ✅ |
| 2140 | 0x8180DF08 | `XamNuiIdentityGetColorTexture` | callback 0x2C011 → cabeçalho de textura (XGSetTextureHeaderEx) | 🟡 |
| 2138 | 0x8180E048 | `XamNuiIdentityGetEnrollmentInfo` | callback 0x2C013 → dois u32 | ✅ |
| 2161 | 0x8180D9E8 | `XamNuiIdentityGetQualityFlags` | 0xFE 0x2C008 (tarefa assíncrona) | 🟡 |
| 2156 | 0x816F9B30 | `XamNuiIdentityGetQualityFlagsMessage` | PsCam 40 com dois u32 | 🟡 |
| 2163 | 0x8180DAC8 | `XamNuiIdentityGetSessionId` | 8 bytes aleatórios, criados na primeira chamada | ✅ |
| 2164 | 0x8180DA50 | `XamNuiIdentityIdentifyWithBiometric` | 0xFE 0x2C007 → callback + tarefa assíncrona do xam | 🟡 pelo vínculo do índice de cadastro, sem o motor de identidade |
| 2139 | 0x8180DFE0 | `XamNuiIdentityUnenroll` | callback 0x2C012 | ✅ |
| 2121 | 0x816B1D88 | `XamReadBiometricData` | XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| 2152 | 0x816DAD28 | `XamUserNuiBind` | tabela de usuários: +0x34 = cadastro, +0x30 = 1 | ✅ |
| 2147 | 0x816DE380 | `XamUserNuiEnableBiometric` | configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| 2154 | 0x816DADA8 | `XamUserNuiGetEnrollmentIndex` | +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| 2153 | 0x816DAE00 | `XamUserNuiGetUserIndex` | o usuário com aquele cadastro | ✅ |
| 2150 | 0x816D8B90 | `XamUserNuiGetUserIndexForBind` | o primeiro perfil conectado sem cadastro | ✅ |
| 2151 | 0x816D8C18 | `XamUserNuiGetUserIndexForSignin` | o primeiro slot de usuário livre | ✅ |
| 2146 | 0x816DAEE0 | `XamUserNuiIsBiometricEnabled` | lê 0x1004004E | ✅ |
| 2155 | 0x816DAE88 | `XamUserNuiUnbind` | apaga a ligação de tipo 5 | ✅ |
| 2122 | 0x816B1E58 | `XamWriteBiometricData` | XMsg 0xFB (Live); tamanho 0 apaga | ✅ |

### Telas

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 1347 | 0x816C5D98 | `XamShowCsvTransitionUI` | tela da Live (não Kinect) | ❌ |
| 1209 | 0x816C5C50 | `XamShowFitnessBodyProfileUI` | UI 0x2103A | ✅ |
| 1273 | 0x816C5BD8 | `XamShowFitnessClearUI` | UI 0x21040 | ⚪ stub |
| 1244 | 0x816C5D20 | `XamShowFitnessWarnAboutPrivacyUI` | UI 0x2103C | 🟡 |
| 1245 | 0x816C5CC8 | `XamShowFitnessWarnAboutTimeUI` | UI 0x2103B | 🟡 |
| 1134 | 0x816C4718 | `XamShowNuiAchievementsUI` | NuiHudCheck + conquistas (0x21008) | ✅ |
| 1135 | 0x816C4010 | `XamShowNuiCommunitySessionsUI` | NuiHudCheck; precisa de Live | ✅ |
| 1147 | 0x816C21C8 | `XamShowNuiControllerRequiredUI` | NuiHudCheck + caixa de mensagem 0x72000 | ✅ |
| 1136 | 0x816C5328 | `XamShowNuiDeviceSelectorUI` | NuiHudCheck + seletor de dispositivo (modo Kinect) | 🟡 |
| 1137 | 0x816C39C8 | `XamShowNuiDirtyDiscErrorUI` | NuiHudCheck + mensagem de disco sujo (0x53, 0x2B) | ⚪ stub |
| 1145 | 0x816C5900 | `XamShowNuiFriendRequestUI` | NuiHudCheck + 0x21036 (Live) | ✅ |
| 1138 | 0x816C3CE8 | `XamShowNuiFriendsUI` | NuiHudCheck + 0x21033 (Live) | ✅ |
| 1139 | 0x816C57B8 | `XamShowNuiGameInviteUI` | NuiHudCheck + 0x21035 (Live) | ✅ |
| 1140 | 0x816C4188 | `XamShowNuiGamerCardUIForXUID` | NuiHudCheck + 0x21032 (Live) | 🟡 |
| 1148 | 0x816C4948 | `XamShowNuiGamesUI` | NuiHudCheck + 0x21008 | 🟡 |
| 854 | 0x816C3590 | `XamShowNuiGuideUI` | NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030 | ✅ |
| 1149 | 0x816C6130 | `XamShowNuiHardwareRequiredUI` | Troubleshooter com flag 0x400000 | ✅ |
| 1235 | 0x816C2350 | `XamShowNuiJoinSessionInProgressUI` | só NuiHudCheck | ✅ |
| 1141 | 0x816C2598 | `XamShowNuiMarketplaceDownloadItemsUI` | 0x78 (não implementado no xam) | ✅ |
| 1142 | 0x816C4D88 | `XamShowNuiMarketplaceUI` | NuiHudCheck + marketplace | 🟡 |
| 1143 | 0x816C5720 | `XamShowNuiMessageBoxUI` | NuiHudCheck + caixa de mensagem com flag 0x2000 | 🟡 |
| 1199 | 0x816C3DF0 | `XamShowNuiMessagesUI` | NuiHudCheck + 0x21038 (Live) | ✅ |
| 1144 | 0x816C3F00 | `XamShowNuiPartyUI` | NuiHudCheck + 0x21034 (com festa ativa) | ✅ |
| 1146 | 0x816C1968 | `XamShowNuiSigninUI` | NuiHudCheck + login (FUN_816C1868) | 🟡 |
| 1131 | 0x816C5F68 | `XamShowNuiTroubleshooterUI` | sem sensor: caixa de mensagem (textos 0x11F/0x4F); com sensor: engaja e abre o Tuner (0x21028), modo 2, 3 ou 5 | ✅ |
| 1266 | 0x816C3B78 | `XamShowNuiVideoRichPresenceUI` | NuiHudCheck + IO 0x21025 | 🟡 |
| 737 | 0x816C29E8 | `XamShowPersonalizationUI` | tela comum (não Kinect) | ❌ |
| 724 | 0x816C24C8 | `XamShowReputationUI` | tela comum (não Kinect) | ❌ |
| 700 | 0x816C3CD8 | `XamShowSigninUI` | tela comum | ✅ |
| 701 | 0x816C1A18 | `XamShowSigninUIEx` | tela comum | 🟡 |
| 702 | 0x816C19C8 | `XamShowSigninUIp` | tela comum | ✅ |

### Voz e microfones

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 783 | 0x816FC1C8 | `XamVoiceClose` | voz (kernel XVoiced*) | ⚪ stub |
| 780 | 0x816FC098 | `XamVoiceCreate` | voz (kernel XVoiced*) | ⚪ stub |
| 1169 | 0x816C3750 | `XamVoiceDisableMicArray` | UI 0x2100E (0x88 bytes) | 🟡 |
| 784 | 0x816FB3C8 | `XamVoiceGetBatteryStatus` | kernel XVoiced* | ✅ |
| 794 | 0x816FB450 | `XamVoiceGetDirectionalData` | kernel XVoicedGetDirectionalData (0x327) | ✅ |
| 988 | 0x816FBCA0 | `XamVoiceGetMicArrayAudio` | kernel XVoiced* | ✅ |
| 1168 | 0x816FBCF8 | `XamVoiceGetMicArrayAudioEx` | kernel XVoiced* | ✅ |
| 1190 | 0x816FBE10 | `XamVoiceGetMicArrayFilenameDesc` | texto localizado 100 | 🟡 |
| 792 | 0x816FBBE8 | `XamVoiceGetMicArrayStatus` | 2 sem matriz; senão o sinal de 0x81AACBF8 | ✅ |
| 1166 | 0x816FBD50 | `XamVoiceGetMicArrayUnderrunStatus` | McaDeviceRequest código 0x20 → falta de dados (bits 0/1) e contagem | ✅ |
| 781 | 0x816FB418 | `XamVoiceHeadsetPresent` | kernel XVoiced* | ⚪ stub |
| 1175 | 0x816FB4D0 | `XamVoiceIsActiveProcess` | kernel XVoiced* | ⚪ stub |
| 1165 | 0x816FBA08 | `XamVoiceMuteMicArray` | kernel XVoiced* | 🟡 |
| 1158 | 0x816FBB28 | `XamVoiceRecordUserPrivileges` | privilégios de voz | ✅ |
| 793 | 0x816FBD58 | `XamVoiceSetAudioCaptureRoutine` | kernel XVoiced* | ✅ |
| 1187 | 0x816FBD60 | `XamVoiceSetMicArrayBeamAngle` | CEMD 5 ao sensor | ✅ |
| 1164 | 0x816FB9C0 | `XamVoiceSetMicArrayIdleUsers` | usuários ociosos da matriz | ✅ |
| 782 | 0x816FB498 | `XamVoiceSubmitPacket` | kernel XVoiced* | ⚪ stub |

### Fitness

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 1210 | 0x8168FA00 | `XamFitnessAddBodyProfileRecord` | 0xFB 0xB0079: grava registro do corpo (pacote + verificação síncrona) | ✅ |
| 1222 | 0x8168FB40 | `XamFitnessAddFitnessEvent` | 0xFB 0xB007A: grava evento de exercício | ✅ |
| 1274 | 0x8168F0E0 | `XamFitnessClearAll` | 0xFB 0xB0084 {usuário, p2} | ✅ |
| 1217 | 0x81698BF8 | `XamFitnessClearBodyProfileRecords` | não fala com a Live: devolve 3 valores em cache (0x10 bytes) | 🟡 |
| 1447 | 0x8168F990 | `XamFitnessContainsFitnessData` | 0xFB 0xB0089 | ✅ |
| 1240 | 0x8168F568 | `XamFitnessConvertByteMetToFloatMet` | byte × 0,1 | ✅ |
| 1250 | 0x8168F5B0 | `XamFitnessConvertFloatMetToByteMet` | (int)(float × 10) | ✅ |
| 1227 | 0x8168FCF0 | `XamFitnessCreateFitnessEventEnumerator` | 0xFB 0xB007F/0xB0009: enumerador de eventos (itens de 0x24) | 🟡 |
| 1219 | 0x8168F328 | `XamFitnessGetAllBodyProfileRecords` | 0xFB 0xB007C | 🟡 |
| 1220 | 0x8168F2C8 | `XamFitnessGetAllTitleSummaries` | 0xFB 0xB007D | 🟡 |
| 1218 | 0x8168F168 | `XamFitnessGetCurrentBodyProfileRecord` | 0xFB 0xB007B (síncrono) | ✅ |
| 1221 | 0x8168F3D8 | `XamFitnessGetOverallSummary` | 0xFB 0xB007E | 🟡 |
| 1275 | 0x8168F688 | `XamFitnessGetPrivacySettings` | configuração de perfil 0x10040000 + privilégio 6 → valor 0 a 6 | ✅ |
| 1316 | 0x8168F8C0 | `XamFitnessGetSuggestedPrivacySettings` | 6, ou 3/4 conforme a região | ✅ |
| 1314 | 0x8168F830 | `XamFitnessGetSyncStatus` | 0xFB 0xB0085 | ✅ |
| 1444 | 0x8168F2E8 | `XamFitnessGetTitleSummaries` | 0xFB 0xB007D | 🟡 |
| 1445 | 0x8168F308 | `XamFitnessGetTitleSummary` | 0xFB 0xB007D (um título) | 🟡 |
| 1228 | 0x8168F4E0 | `XamFitnessInitialize` | 0xFB 0xB0080 (síncrono) | ✅ |
| 1315 | 0x8168F470 | `XamFitnessInitializeForOneUser` | 0xFB 0xB0086 | ✅ |
| 1242 | 0x8168FC78 | `XamFitnessMsgTimeToSystemTime` | segundos desde 2003-01-01 → SYSTEMTIME | ✅ |
| 1243 | 0x8168F5E8 | `XamFitnessSystemTimeToMsgTime` | o inverso | ✅ |
| 1246 | 0x8168F530 | `XamFitnessUnInitialize` | 0xFB 0xB0081 | ✅ |
| 1317 | 0x8168F550 | `XampFitnessLetFireAndForgetsCatchUpInternal` | 0xFB 0xB0088: espera as gravações assíncronas pendentes | ✅ |

### Outras

| Ordinal | Endereço | Função | O que faz | Xenia |
|---|---|---|---|---|
| 652 | 0x816ABA78 | `XNotifyPositionUI` | posição das notificações (não Kinect) | ⚪ stub |

**Legenda:** ✅ implementada · 🟡 aproximada (kSketchy) · ⚪ stub · ❌ ausente no Xenia.

## O que falta, pelo mapa

- **Ausentes que importam:** o fitness (22), as funções de voz que dependem do kernel, as 2 que dependem do sensor (PsCam 20 e 40) e `GetQualityFlags`, que precisa de uma tarefa assíncrona do xam. `GetLoadedDepthCalibration`, `GetCameraIntrinsics` e `IdentifyWithBiometric` foram feitas em 855a12dcc.
- **Stubs:** `GetDeviceStatus` (falta o status real), `IdentityEnrollForSignIn` e o Troubleshooter (que no Xenia abre o Kinect Tuner).
- **Fora do Kinect:** as telas de login, personalização e reputação e `XNotifyPositionUI` entraram na lista só pelo nome.
