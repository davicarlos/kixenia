# Funções do XAM: índice

Base de **243 funções** do `xam.xex` (atualização 17559) citadas nas notas do Kinect, uma nota por função em `XAM/Funcoes/`. Cada nota diz o que a função faz, os fatos tirados do decompilado (mensagens, kernel, erros, globais), quem ela chama e quem a chama (com links), o que as notas já diziam e a confirmação.

- **Confirmação pelo REA:** 154 funções decompiladas pelo REA (Ghidra 12.1.4, análise padrão), com o id da evidência na nota. 54 foram recusadas pelo REA porque o Ghidra padrão não reconhece o início delas no xam convertido; essas vêm do projeto Ghidra com o fluxo corrigido (`XamFixFlow`). 58 funções não existiam no Ghidra e foram criadas no endereço (por exemplo, o ordinal 2174).
- **Ligações:** o grafo de chamadas vem do projeto corrigido (chamadas diretas). Chamadas por ponteiro (vtables, callbacks do título) não aparecem: estão descritas no texto de cada nota.
- **Gerado por:** `~/Kixenia/kinect-sdk-re/xam-nui/fnbase/` (`gen.py` para as notas, `index.py` para o índice, `manual.py` com as explicações, `FnGraph.java` para o grafo no Ghidra). Rodar de novo atualiza as notas.

Relacionado: Funcoes-XAM-Vizinhas-Indice (as 992 funções ligadas à base), [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md), [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md), [Kinect-ID](../../kinect/kinect-id.md), [XAM-Fitness](../xam-fitness.md), [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md), [XAM-Rede-e-Login](../xam-rede-e-login.md).

## HUD, esqueleto e gestos

O HUD do sistema: recebe cada quadro de esqueleto do título (0x2B002), escolhe os jogadores, pontua a posição, detecta o aceno de engajamento e o gesto do guia.

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XamNuiHudGetEngagedTrackingID (816AC408)](XamNuiHudGetEngagedTrackingID-816AC408.md) | 0x816AC408 | HUD+0x118 | ✅ | ✅ implementada |
| [XamNuiHudSetEngagedTrackingID (816AC428)](XamNuiHudSetEngagedTrackingID-816AC428.md) | 0x816AC428 | HUD+0x118 (sem HUD: 0x80004005) | ✅ | ✅ implementada |
| [FUN_816AC460](FUN_816AC460.md) | 0x816AC460 | Mensagem 0x2B001 | ✅ | — |
| [XamNuiHudIsEnabled (816AC4B8)](XamNuiHudIsEnabled-816AC4B8.md) | 0x816AC4B8 | HUD existe, HUD+0x50 ≠ 0 e XamNuiIsDeviceReady | ✅ | ✅ implementada |
| [XamNuiHudGetInitializeFlags (816AC510)](XamNuiHudGetInitializeFlags-816AC510.md) | 0x816AC510 | 0x81AAC990 (flags da 0x2B003) | — | ✅ implementada |
| [XamNuiHudGetVersions (816AC518)](XamNuiHudGetVersions-816AC518.md) | 0x816AC518 | 0x81AAC998 / 0x81AAC9A0 (versões xam/XTL da 0x2B003) | — | ✅ implementada |
| [FUN_816AC688](FUN_816AC688.md) | 0x816AC688 | HUD: interpretar quadro | ✅ | — |
| [FUN_816ACAD8](FUN_816ACAD8.md) | 0x816ACAD8 | HUD: contador de presença | ✅ | — |
| [XamNuiHudInterpretFrame (816ACC68)](XamNuiHudInterpretFrame-816ACC68.md) | 0x816ACC68 | o HUD do sistema: 2 jogadores, alertas, aceno, gesto do guia; 0x8301000B com UI aberta | ✅ | ✅ implementada |
| [XamNuiHudGetEngagedEnrollmentIndex (816ACCE8)](XamNuiHudGetEngagedEnrollmentIndex-816ACCE8.md) | 0x816ACCE8 | cópia do quadro do HUD: cadastro do engajado, ou −2 | Ghidra corrigido | ✅ implementada |
| [XamNuiHudEnableInputFilter (816C7EB8)](XamNuiHudEnableInputFilter-816C7EB8.md) | 0x816C7EB8 | com HUD: FUN_81692C48(0x81AA2160, p) | ✅ | 🟡 aproximada |
| [XamNuiPlayerEngagementUpdate (816F9978)](XamNuiPlayerEngagementUpdate-816F9978.md) | 0x816F9978 | os 2 esqueletos mais próximos → quadro+0xACC (o rastreado ganha 0,3 m) | — | ✅ implementada |
| [XamNuiSkeletonScoreUpdate (816F9980)](XamNuiSkeletonScoreUpdate-816F9980.md) | 0x816F9980 | registros de 0x3C: alertas de borda, distância, giro, mãos | — | ✅ implementada |
| [XamNuiSkeletonGetBestSkeletonIndex (816F9988)](XamNuiSkeletonGetBestSkeletonIndex-816F9988.md) | 0x816F9988 | o rastreado entre os 2 escolhidos | — | ✅ implementada |
| [FUN_816F9CB8](FUN_816F9CB8.md) | 0x816F9CB8 | Despachante NUI (0x2B0xx) | ✅ | — |
| [FUN_817FD108](FUN_817FD108.md) | 0x817FD108 | HUD: cursor | ✅ | — |
| [FUN_817FD258](FUN_817FD258.md) | 0x817FD258 | HUD: barra do aceno | ✅ | — |
| [FUN_817FD2D0](FUN_817FD2D0.md) | 0x817FD2D0 | HUD: perda do engajado | ✅ | — |
| [FUN_81800828](FUN_81800828.md) | 0x81800828 | HUD: barra do guia | ✅ | — |
| [FUN_81808B08](FUN_81808B08.md) | 0x81808B08 | Aceno: por jogador | ✅ | — |
| [FUN_81808C60](FUN_81808C60.md) | 0x81808C60 | Gesto do guia: por quadro | ✅ | — |
| [FUN_81808DE0](FUN_81808DE0.md) | 0x81808DE0 | Referencial do corpo | ✅ | — |
| [FUN_818092C8](FUN_818092C8.md) | 0x818092C8 | Aceno: duas mãos | ✅ | — |
| [FUN_81809558](FUN_81809558.md) | 0x81809558 | Gesto do guia: parâmetros | ✅ | — |
| [FUN_81809738](FUN_81809738.md) | 0x81809738 | Gesto do guia: espera | ✅ | — |
| [FUN_81809820](FUN_81809820.md) | 0x81809820 | Dobra do cotovelo | ✅ | — |
| [FUN_818098E0](FUN_818098E0.md) | 0x818098E0 | Gesto do guia: referencial | ✅ | — |
| [FUN_81809AF0](FUN_81809AF0.md) | 0x81809AF0 | Gesto do guia: pose | ✅ | — |
| [FUN_81809FD0](FUN_81809FD0.md) | 0x81809FD0 | Gesto do guia: por jogador | ✅ | — |
| [FUN_8180A0B0](FUN_8180A0B0.md) | 0x8180A0B0 | Gesto do guia: todos os jogadores | ✅ | — |
| [FUN_8180A388](FUN_8180A388.md) | 0x8180A388 | Aceno: parâmetros | ✅ | — |
| [FUN_8180A5C0](FUN_8180A5C0.md) | 0x8180A5C0 | Aceno: meia-onda | ✅ | — |
| [FUN_8180A6F8](FUN_8180A6F8.md) | 0x8180A6F8 | Aceno: ritmo | ✅ | — |
| [FUN_8180A788](FUN_8180A788.md) | 0x8180A788 | Aceno: mão erguida | ✅ | — |
| [FUN_8180A900](FUN_8180A900.md) | 0x8180A900 | Aceno: checagem da mão | ✅ | — |
| [FUN_8180A988](FUN_8180A988.md) | 0x8180A988 | Aceno: direção | ✅ | — |
| [FUN_8180AAB8](FUN_8180AAB8.md) | 0x8180AAB8 | Aceno: por mão | ✅ | — |
| [FUN_8198B0F0](FUN_8198B0F0.md) | 0x8198B0F0 | Projeção na profundidade | ✅ | — |
| [FUN_8198B170](FUN_8198B170.md) | 0x8198B170 | Melhor esqueleto | ✅ | — |
| [FUN_8198B1E8](FUN_8198B1E8.md) | 0x8198B1E8 | Escolher 2 esqueletos | ✅ | — |
| [FUN_8198B390](FUN_8198B390.md) | 0x8198B390 | Pontuar esqueleto | ✅ | — |
| [FUN_8198B628](FUN_8198B628.md) | 0x8198B628 | Atualizar pontuações | ✅ | — |

## Inclinação e motor

O motor do sensor (Detroit no 1414, Mca no 1473) e o repasse dos pedidos de inclinação do sistema ao motor de inclinação do título.

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XamNuiGetCameraIntrinsics (816F9B90)](XamNuiGetCameraIntrinsics-816F9B90.md) | 0x816F9B90 | PsCam 46, seletor 0x100 | ✅ | ❌ ausente |
| [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md) | 0x816FA088 | UsbdGetNatalHardwareVersion (kernel 0x373): 1 = Detroit (1414), 2 = Mca (1473), 0 = nenhum | ✅ | ⚪ stub |
| [XamNuiCameraElevationSetAngle (816FA0D0)](XamNuiCameraElevationSetAngle-816FA0D0.md) | 0x816FA0D0 | FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | Ghidra corrigido | 🟡 aproximada |
| [XamNuiCameraTiltSetCallback (816FA128)](XamNuiCameraTiltSetCallback-816FA128.md) | 0x816FA128 | 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ | ✅ implementada |
| [XamNuiCameraElevationSetCallback (816FA168)](XamNuiCameraElevationSetCallback-816FA168.md) | 0x816FA168 | 0xFE 0x2B00D: registra quem ouve o status (0x81A85C20) | Ghidra corrigido | ✅ implementada |
| [XamNuiCameraTiltReportStatus (816FA1A8)](XamNuiCameraTiltReportStatus-816FA1A8.md) | 0x816FA1A8 | o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | Ghidra corrigido | ✅ implementada |
| [XamNuiCameraAdjustTilt (816FA338)](XamNuiCameraAdjustTilt-816FA338.md) | 0x816FA338 | 0xFE 0x2B00B {u32, float, float} → motor de inclinação do título. Só o Kinect Tuner chama: AdjustTilt(2.133… | Ghidra corrigido | ✅ implementada |
| [XamNuiCameraElevationAutoTilt (816FA3B8)](XamNuiCameraElevationAutoTilt-816FA3B8.md) | 0x816FA3B8 | 0xFE 0x2B005 {float} → motor de inclinação do título | ✅ | ✅ implementada |
| [XamNuiCameraElevationReverseAutoTilt (816FA418)](XamNuiCameraElevationReverseAutoTilt-816FA418.md) | 0x816FA418 | 0xFE 0x2B00A {u32} → motor de inclinação do título | ✅ | ✅ implementada |
| [XamNuiCameraElevationStopMovement (816FA520)](XamNuiCameraElevationStopMovement-816FA520.md) | 0x816FA520 | Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | ✅ | 🟡 aproximada |
| [XamNuiCameraRememberFloor (816FA618)](XamNuiCameraRememberFloor-816FA618.md) | 0x816FA618 | guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes) | Ghidra corrigido | ⚪ stub |
| [FUN_816FA6F0](FUN_816FA6F0.md) | 0x816FA6F0 | Rotação por eixo e ângulo | ✅ | — |
| [XamNuiCameraTiltGetStatus (816FAA60)](XamNuiCameraTiltGetStatus-816FAA60.md) | 0x816FAA60 | status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | Ghidra corrigido | 🟡 aproximada |
| [XamNuiCameraElevationGetAngle (816FAF68)](XamNuiCameraElevationGetAngle-816FAF68.md) | 0x816FAF68 | lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | Ghidra corrigido | 🟡 aproximada |
| [XamNuiCameraSetFlags (816FB000)](XamNuiCameraSetFlags-816FB000.md) | 0x816FB000 | Detroit: lê o status e grava flags com o comando 0xF0510; Mca: nada | Ghidra corrigido | ✅ implementada |
| [XamNuiNatalCameraUpdateStarting (817FA228)](XamNuiNatalCameraUpdateStarting-817FA228.md) | 0x817FA228 | evento 6, notificação 0xC | Ghidra corrigido | ✅ implementada |
| [XamNuiNatalCameraUpdateComplete (817FA270)](XamNuiNatalCameraUpdateComplete-817FA270.md) | 0x817FA270 | evento 7, notificação 0xD; 0x80070002 → Troubleshooter | ✅ | ✅ implementada |
| [FUN_8180C700](FUN_8180C700.md) | 0x8180C700 | Motor: inclinar | Ghidra corrigido | — |
| [FUN_8180C850](FUN_8180C850.md) | 0x8180C850 | Motor: ler status | Ghidra corrigido | — |

## Sensor e estado

Estado do sensor, configurações do console, calibração de profundidade, exposição (PsCam 46), número de série e firmware.

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XMsgInProcessCall (816A2F88)](XMsgInProcessCall-816A2F88.md) | 0x816A2F88 | XMsg síncrono | ✅ | — |
| [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) | 0x816A3128 | XMsg assíncrono | ✅ | — |
| [XamNuiGetDepthCalibration (816F9990)](XamNuiGetDepthCalibration-816F9990.md) | 0x816F9990 | lê 0x30 bytes de um registro de configuração (0x3E00) | ✅ | ✅ implementada |
| [XamNuiGetLoadedDepthCalibration (816F9A48)](XamNuiGetLoadedDepthCalibration-816F9A48.md) | 0x816F9A48 | PsCam 44 → dois u32 | — | ❌ ausente |
| [XamNuiStoreDepthCalibration (816F9A50)](XamNuiStoreDepthCalibration-816F9A50.md) | 0x816F9A50 | grava 0x30 bytes no registro 0x3E00 | Ghidra corrigido | ✅ implementada |
| [XamNuiGetTrueColorInfo (816F9B38)](XamNuiGetTrueColorInfo-816F9B38.md) | 0x816F9B38 | PsCam 46: seletor 0 (4 bytes), 1 (tabela de autoexposição de 12 bytes por entrada) | Ghidra corrigido | 🟡 aproximada |
| [XamNuiGetFanRate (816F9BF0)](XamNuiGetFanRate-816F9BF0.md) | 0x816F9BF0 | pedido > 2000 → PsCam 20, propriedade 0x838; byte 7 = ventilador | Ghidra corrigido | ❌ ausente |
| [XamNuiIsChatMicEnabled (816F9E90)](XamNuiIsChatMicEnabled-816F9E90.md) | 0x816F9E90 | !(bit 0x8 de XCONFIG_CONSOLE_NUI) | ✅ | ✅ implementada |
| [XamNuiEnableChatMic (816F9ED8)](XamNuiEnableChatMic-816F9ED8.md) | 0x816F9ED8 | bit 0x8 de XCONFIG_CONSOLE_NUI + XamVoiceMuteMicArray | ✅ | ✅ implementada |
| [XamXStudioRequest (816FB3A0)](XamXStudioRequest-816FB3A0.md) | 0x816FB3A0 | Voz: estado do dispositivo | ✅ | — |
| [FUN_816FBF68](FUN_816FBF68.md) | 0x816FBF68 | Voz: ligar/desligar objeto | ✅ | — |
| [FUN_8172FDA0](FUN_8172FDA0.md) | 0x8172FDA0 | acos | ✅ | — |
| [XamNatalDeviceAudioCalibrate (817F8DE8)](XamNatalDeviceAudioCalibrate-817F8DE8.md) | 0x817F8DE8 | envia MicArrayCalibrate.mec ao sensor (CEMD 0) | ✅ | ✅ implementada |
| [XamNuiIsDeviceReady (817F9988)](XamNuiIsDeviceReady-817F9988.md) | 0x817F9988 | bit 1 de 0x81AD127C | ✅ | ✅ implementada |
| [XamNuiGetDeviceSerialNumber (817F99A0)](XamNuiGetDeviceSerialNumber-817F99A0.md) | 0x817F99A0 | Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas | — | ✅ implementada |
| [FUN_817F9F38](FUN_817F9F38.md) | 0x817F9F38 | Sensor: máquina de estados | ✅ | — |
| [XamNuiSetForceDeviceOff (817FA170)](XamNuiSetForceDeviceOff-817FA170.md) | 0x817FA170 | bit 31 de XCONFIG_USER_RETAIL_FLAGS; evento 0xF | ✅ | ✅ implementada |
| [XamNuiGetSupportString (817FA2E0)](XamNuiGetSupportString-817FA2E0.md) | 0x817FA2E0 | "K:%02d:...:%12s": modo, estado, problema, versão do firmware, série | ✅ | 🟡 aproximada |
| [FUN_817FC750](FUN_817FC750.md) | 0x817FC750 | Sensor: montar status | ✅ | — |
| [XamNuiGetDeviceStatus (817FC990)](XamNuiGetDeviceStatus-817FC990.md) | 0x817FC990 | FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ✅ | ⚪ stub |
| [XnpGetXwppRuntimeFilter (81964638)](XnpGetXwppRuntimeFilter-81964638.md) | 0x81964638 | Sempre 0 | ✅ | — |

## Kinect ID

O serviço de identidade: repassa ao callback do título (0x2C009), parâmetros por nome, cache de dados biométricos e ligação perfil ↔ cadastro.

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XamCacheFetchFile (816793F0)](XamCacheFetchFile-816793F0.md) | 0x816793F0 | Armazém de cache: ler | ✅ | — |
| [XamCacheStoreFileByString (81679438)](XamCacheStoreFileByString-81679438.md) | 0x81679438 | Armazém de cache: gravar | ✅ | — |
| [XamCacheFetchFileByString (81679460)](XamCacheFetchFileByString-81679460.md) | 0x81679460 | Armazém de cache: ler por nome | ✅ | — |
| [XamQueryLiveHiveA (8169AD08)](XamQueryLiveHiveA-8169AD08.md) | 0x8169AD08 | Configuração por nome | ✅ | — |
| [XamReadBiometricData (816B1D88)](XamReadBiometricData-816B1D88.md) | 0x816B1D88 | XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | Ghidra corrigido | ✅ implementada |
| [XamWriteBiometricData (816B1E58)](XamWriteBiometricData-816B1E58.md) | 0x816B1E58 | XMsg 0xFB (Live); tamanho 0 apaga | ✅ | ✅ implementada |
| [XamUserNuiGetUserIndexForBind (816D8B90)](XamUserNuiGetUserIndexForBind-816D8B90.md) | 0x816D8B90 | o primeiro perfil conectado sem cadastro | Ghidra corrigido | ✅ implementada |
| [XamUserNuiGetUserIndexForSignin (816D8C18)](XamUserNuiGetUserIndexForSignin-816D8C18.md) | 0x816D8C18 | o primeiro slot de usuário livre | ✅ | ✅ implementada |
| [FUN_816D8F50](FUN_816D8F50.md) | 0x816D8F50 | Usuário: apagar ligação | ✅ | — |
| [XamUserNuiBind (816DAD28)](XamUserNuiBind-816DAD28.md) | 0x816DAD28 | tabela de usuários: +0x34 = cadastro, +0x30 = 1 | ✅ | ✅ implementada |
| [XamUserNuiGetEnrollmentIndex (816DADA8)](XamUserNuiGetEnrollmentIndex-816DADA8.md) | 0x816DADA8 | +0x34 se +0x30, senão −2 / 0x80070525 | ✅ | ✅ implementada |
| [XamUserNuiGetUserIndex (816DAE00)](XamUserNuiGetUserIndex-816DAE00.md) | 0x816DAE00 | o usuário com aquele cadastro | ✅ | ✅ implementada |
| [XamUserNuiUnbind (816DAE88)](XamUserNuiUnbind-816DAE88.md) | 0x816DAE88 | apaga a ligação de tipo 5 | ✅ | ✅ implementada |
| [XamUserNuiIsBiometricEnabled (816DAEE0)](XamUserNuiIsBiometricEnabled-816DAEE0.md) | 0x816DAEE0 | lê 0x1004004E | ✅ | ✅ implementada |
| [XamUserNuiEnableBiometric (816DE380)](XamUserNuiEnableBiometric-816DE380.md) | 0x816DE380 | configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ | ✅ implementada |
| [ControlpackNuiCursorSetTrackingId (816DE4C8)](ControlpackNuiCursorSetTrackingId-816DE4C8.md) | 0x816DE4C8 | Data dos rostos cadastrados | Ghidra corrigido | ❌ ausente |
| [XamNuiIdentityGetQualityFlagsMessage (816F9B30)](XamNuiIdentityGetQualityFlagsMessage-816F9B30.md) | 0x816F9B30 | PsCam 40 com dois u32 | — | ❌ ausente |
| [XamNuiIdentityAbort (8180D910)](XamNuiIdentityAbort-8180D910.md) | 0x8180D910 | 0xFE 0x2C00E | ✅ | ⚪ stub |
| [XamNuiIdentityEnrollForSignIn (8180D970)](XamNuiIdentityEnrollForSignIn-8180D970.md) | 0x8180D970 | 0xFE 0x2C010 {usuário, p2, p4} | ✅ | ⚪ stub |
| [XamNuiIdentityGetQualityFlags (8180D9E8)](XamNuiIdentityGetQualityFlags-8180D9E8.md) | 0x8180D9E8 | 0xFE 0x2C008 (tarefa assíncrona) | Ghidra corrigido | ❌ ausente |
| [XamNuiIdentityIdentifyWithBiometric (8180DA50)](XamNuiIdentityIdentifyWithBiometric-8180DA50.md) | 0x8180DA50 | 0xFE 0x2C007 → callback + tarefa assíncrona do xam | ✅ | ❌ ausente |
| [XamNuiIdentityGetSessionId (8180DAC8)](XamNuiIdentityGetSessionId-8180DAC8.md) | 0x8180DAC8 | 8 bytes aleatórios, criados na primeira chamada | ✅ | ✅ implementada |
| [FUN_8180DB10](FUN_8180DB10.md) | 0x8180DB10 | Identidade: quadro | ✅ | — |
| [FUN_8180DB88](FUN_8180DB88.md) | 0x8180DB88 | Identidade: parâmetro por nome | ✅ | — |
| [FUN_8180DEB0](FUN_8180DEB0.md) | 0x8180DEB0 | Identidade: 0x2C00A | ✅ | — |
| [XamNuiIdentityGetColorTexture (8180DF08)](XamNuiIdentityGetColorTexture-8180DF08.md) | 0x8180DF08 | callback 0x2C011 → cabeçalho de textura (XGSetTextureHeaderEx) | ✅ | 🟡 aproximada |
| [XamNuiIdentityUnenroll (8180DFE0)](XamNuiIdentityUnenroll-8180DFE0.md) | 0x8180DFE0 | callback 0x2C012 | ✅ | ✅ implementada |
| [XamNuiIdentityGetEnrollmentInfo (8180E048)](XamNuiIdentityGetEnrollmentInfo-8180E048.md) | 0x8180E048 | callback 0x2C013 → dois u32 | Ghidra corrigido | ✅ implementada |
| [FUN_8180E3E0](FUN_8180E3E0.md) | 0x8180E3E0 | Identidade: método do serviço | ✅ | — |
| [FUN_8180E750](FUN_8180E750.md) | 0x8180E750 | Identidade: chamar o callback | ✅ | — |
| [FUN_8180EB20](FUN_8180EB20.md) | 0x8180EB20 | Identidade: identificar | ✅ | — |

## Voz e microfones

A matriz de microfones e o chat: XVoiced do kernel, mensagens CEMD ao firmware de áudio, feixe e calibração.

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XamVoiceDisableMicArray (816C3750)](XamVoiceDisableMicArray-816C3750.md) | 0x816C3750 | UI 0x2100E (0x88 bytes) | Ghidra corrigido | ❌ ausente |
| [XamVoiceGetBatteryStatus (816FB3C8)](XamVoiceGetBatteryStatus-816FB3C8.md) | 0x816FB3C8 | kernel XVoiced* | Ghidra corrigido | ❌ ausente |
| [XamVoiceHeadsetPresent (816FB418)](XamVoiceHeadsetPresent-816FB418.md) | 0x816FB418 | kernel XVoiced* | ✅ | ⚪ stub |
| [XamVoiceGetDirectionalData (816FB450)](XamVoiceGetDirectionalData-816FB450.md) | 0x816FB450 | kernel XVoicedGetDirectionalData (0x327) | ✅ | ❌ ausente |
| [XamVoiceSubmitPacket (816FB498)](XamVoiceSubmitPacket-816FB498.md) | 0x816FB498 | kernel XVoiced* | ✅ | ⚪ stub |
| [XamVoiceIsActiveProcess (816FB4D0)](XamVoiceIsActiveProcess-816FB4D0.md) | 0x816FB4D0 | kernel XVoiced* | Ghidra corrigido | ⚪ stub |
| [XamVoiceSetMicArrayIdleUsers (816FB9C0)](XamVoiceSetMicArrayIdleUsers-816FB9C0.md) | 0x816FB9C0 | usuários ociosos da matriz | Ghidra corrigido | ✅ implementada |
| [XamVoiceMuteMicArray (816FBA08)](XamVoiceMuteMicArray-816FBA08.md) | 0x816FBA08 | kernel XVoiced* | ✅ | ❌ ausente |
| [XamVoiceRecordUserPrivileges (816FBB28)](XamVoiceRecordUserPrivileges-816FBB28.md) | 0x816FBB28 | privilégios de voz | ✅ | ❌ ausente |
| [XamVoiceGetMicArrayStatus (816FBBE8)](XamVoiceGetMicArrayStatus-816FBBE8.md) | 0x816FBBE8 | 2 sem matriz; senão o sinal de 0x81AACBF8 | ✅ | ✅ implementada |
| [XamVoiceGetMicArrayAudio (816FBCA0)](XamVoiceGetMicArrayAudio-816FBCA0.md) | 0x816FBCA0 | kernel XVoiced* | Ghidra corrigido | ❌ ausente |
| [XamVoiceGetMicArrayAudioEx (816FBCF8)](XamVoiceGetMicArrayAudioEx-816FBCF8.md) | 0x816FBCF8 | kernel XVoiced* | Ghidra corrigido | 🟡 aproximada |
| [XamVoiceGetMicArrayUnderrunStatus (816FBD50)](XamVoiceGetMicArrayUnderrunStatus-816FBD50.md) | 0x816FBD50 | McaDeviceRequest código 0x20 → falta de dados (bits 0/1) e contagem | — | 🟡 aproximada |
| [XamVoiceSetAudioCaptureRoutine (816FBD58)](XamVoiceSetAudioCaptureRoutine-816FBD58.md) | 0x816FBD58 | kernel XVoiced* | — | ❌ ausente |
| [XamVoiceSetMicArrayBeamAngle (816FBD60)](XamVoiceSetMicArrayBeamAngle-816FBD60.md) | 0x816FBD60 | CEMD 5 ao sensor | Ghidra corrigido | ✅ implementada |
| [XamVoiceGetMicArrayFilenameDesc (816FBE10)](XamVoiceGetMicArrayFilenameDesc-816FBE10.md) | 0x816FBE10 | texto localizado 100 | ✅ | ❌ ausente |
| [XamVoiceCreate (816FC098)](XamVoiceCreate-816FC098.md) | 0x816FC098 | voz (kernel XVoiced*) | ✅ | ⚪ stub |
| [XamVoiceClose (816FC1C8)](XamVoiceClose-816FC1C8.md) | 0x816FC1C8 | voz (kernel XVoiced*) | ✅ | ⚪ stub |
| [FUN_816FC410](FUN_816FC410.md) | 0x816FC410 | Microfones: reiniciar | ✅ | — |
| [FUN_81983268](FUN_81983268.md) | 0x81983268 | CEMD: início | ✅ | — |
| [FUN_819832F0](FUN_819832F0.md) | 0x819832F0 | CEMD: bloco | ✅ | — |
| [FUN_81983380](FUN_81983380.md) | 0x81983380 | CEMD: fim | ✅ | — |
| [FUN_81983E98](FUN_81983E98.md) | 0x81983E98 | CEMD: enviar | ✅ | — |

## Telas

As telas 'Nui': NuiHudCheck e depois a tela comum do sistema (mensagens 0x210xx).

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XNotifyPositionUI (816ABA78)](XNotifyPositionUI-816ABA78.md) | 0x816ABA78 | posição das notificações (não Kinect) | ✅ | ⚪ stub |
| [FUN_816C16C8](FUN_816C16C8.md) | 0x816C16C8 | Telas: resultado do pedido de UI | ✅ | — |
| [FUN_816C1808](FUN_816C1808.md) | 0x816C1808 | NuiHudCheck | ✅ | — |
| [FUN_816C1868](FUN_816C1868.md) | 0x816C1868 | Telas: login | ✅ | — |
| [XamShowNuiSigninUI (816C1968)](XamShowNuiSigninUI-816C1968.md) | 0x816C1968 | NuiHudCheck + login (FUN_816C1868) | ✅ | 🟡 aproximada |
| [XamShowSigninUIp (816C19C8)](XamShowSigninUIp-816C19C8.md) | 0x816C19C8 | tela comum | ✅ | ✅ implementada |
| [XamShowSigninUIEx (816C1A18)](XamShowSigninUIEx-816C1A18.md) | 0x816C1A18 | tela comum | ✅ | 🟡 aproximada |
| [XamShowNuiControllerRequiredUI (816C21C8)](XamShowNuiControllerRequiredUI-816C21C8.md) | 0x816C21C8 | NuiHudCheck + caixa de mensagem 0x72000 | ✅ | ✅ implementada |
| [XamShowNuiJoinSessionInProgressUI (816C2350)](XamShowNuiJoinSessionInProgressUI-816C2350.md) | 0x816C2350 | só NuiHudCheck | Ghidra corrigido | ✅ implementada |
| [XamShowReputationUI (816C24C8)](XamShowReputationUI-816C24C8.md) | 0x816C24C8 | tela comum (não Kinect) | ✅ | ❌ ausente |
| [XamShowNuiMarketplaceDownloadItemsUI (816C2598)](XamShowNuiMarketplaceDownloadItemsUI-816C2598.md) | 0x816C2598 | 0x78 (não implementado no xam) | Ghidra corrigido | ✅ implementada |
| [XamShowPersonalizationUI (816C29E8)](XamShowPersonalizationUI-816C29E8.md) | 0x816C29E8 | tela comum (não Kinect) | — | ❌ ausente |
| [XamShowGuideUI (816C3580)](XamShowGuideUI-816C3580.md) | 0x816C3580 | Guia comum | Ghidra corrigido | — |
| [XamShowNuiGuideUI (816C3590)](XamShowNuiGuideUI-816C3590.md) | 0x816C3590 | NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030 | Ghidra corrigido | ✅ implementada |
| [XamShowNuiDirtyDiscErrorUI (816C39C8)](XamShowNuiDirtyDiscErrorUI-816C39C8.md) | 0x816C39C8 | NuiHudCheck + mensagem de disco sujo (0x53, 0x2B) | Ghidra corrigido | ⚪ stub |
| [XamShowNuiVideoRichPresenceUI (816C3B78)](XamShowNuiVideoRichPresenceUI-816C3B78.md) | 0x816C3B78 | NuiHudCheck + IO 0x21025 | Ghidra corrigido | 🟡 aproximada |
| [XamShowSigninUI (816C3CD8)](XamShowSigninUI-816C3CD8.md) | 0x816C3CD8 | tela comum | Ghidra corrigido | ✅ implementada |
| [XamShowNuiFriendsUI (816C3CE8)](XamShowNuiFriendsUI-816C3CE8.md) | 0x816C3CE8 | NuiHudCheck + 0x21033 (Live) | ✅ | ✅ implementada |
| [XamShowNuiMessagesUI (816C3DF0)](XamShowNuiMessagesUI-816C3DF0.md) | 0x816C3DF0 | NuiHudCheck + 0x21038 (Live) | ✅ | ✅ implementada |
| [XamShowNuiPartyUI (816C3F00)](XamShowNuiPartyUI-816C3F00.md) | 0x816C3F00 | NuiHudCheck + 0x21034 (com festa ativa) | ✅ | ✅ implementada |
| [XamShowNuiCommunitySessionsUI (816C4010)](XamShowNuiCommunitySessionsUI-816C4010.md) | 0x816C4010 | NuiHudCheck; precisa de Live | Ghidra corrigido | ✅ implementada |
| [XamShowNuiGamerCardUIForXUID (816C4188)](XamShowNuiGamerCardUIForXUID-816C4188.md) | 0x816C4188 | NuiHudCheck + 0x21032 (Live) | ✅ | 🟡 aproximada |
| [XamShowNuiAchievementsUI (816C4718)](XamShowNuiAchievementsUI-816C4718.md) | 0x816C4718 | NuiHudCheck + conquistas (0x21008) | ✅ | ✅ implementada |
| [XamShowNuiGamesUI (816C4948)](XamShowNuiGamesUI-816C4948.md) | 0x816C4948 | NuiHudCheck + 0x21008 | ✅ | 🟡 aproximada |
| [XamShowNuiMarketplaceUI (816C4D88)](XamShowNuiMarketplaceUI-816C4D88.md) | 0x816C4D88 | NuiHudCheck + marketplace | Ghidra corrigido | 🟡 aproximada |
| [XamShowNuiDeviceSelectorUI (816C5328)](XamShowNuiDeviceSelectorUI-816C5328.md) | 0x816C5328 | NuiHudCheck + seletor de dispositivo (modo Kinect) | Ghidra corrigido | 🟡 aproximada |
| [XamShowNuiMessageBoxUI (816C5720)](XamShowNuiMessageBoxUI-816C5720.md) | 0x816C5720 | NuiHudCheck + caixa de mensagem com flag 0x2000 | ✅ | 🟡 aproximada |
| [XamShowNuiGameInviteUI (816C57B8)](XamShowNuiGameInviteUI-816C57B8.md) | 0x816C57B8 | NuiHudCheck + 0x21035 (Live) | Ghidra corrigido | ✅ implementada |
| [XamShowNuiFriendRequestUI (816C5900)](XamShowNuiFriendRequestUI-816C5900.md) | 0x816C5900 | NuiHudCheck + 0x21036 (Live) | ✅ | ✅ implementada |
| [FUN_816C5A80](FUN_816C5A80.md) | 0x816C5A80 | Fitness: abrir tela | ✅ | — |
| [XamShowCsvTransitionUI (816C5D98)](XamShowCsvTransitionUI-816C5D98.md) | 0x816C5D98 | tela da Live (não Kinect) | Ghidra corrigido | ❌ ausente |
| [XamShowNuiTroubleshooterUI (816C5F68)](XamShowNuiTroubleshooterUI-816C5F68.md) | 0x816C5F68 | sem sensor: caixa de mensagem (textos 0x11F/0x4F); com sensor: engaja e abre o Tuner (0x21028), modo 2, 3 ou 5 | ✅ | ⚪ stub |
| [XamShowNuiHardwareRequiredUI (816C6130)](XamShowNuiHardwareRequiredUI-816C6130.md) | 0x816C6130 | Troubleshooter com flag 0x400000 | Ghidra corrigido | ✅ implementada |
| [XamIsNuiUIActive (816C7D88)](XamIsNuiUIActive-816C7D88.md) | 0x816C7D88 | lê 0x81AA218C (HUD ativo) | ✅ | ✅ implementada |
| [XamAppRequestLoadEx (816CAC60)](XamAppRequestLoadEx-816CAC60.md) | 0x816CAC60 | Telas: carregar app do sistema | ✅ | — |

## Usuários

Estado de login, XUID e slots de usuário.

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XamUserGetXUID (816D8590)](XamUserGetXUID-816D8590.md) | 0x816D8590 | Usuário: XUID | ✅ | — |
| [XamUserGetAgeGroup (816D9CE0)](XamUserGetAgeGroup-816D9CE0.md) | 0x816D9CE0 | Usuário: região/idade | ✅ | — |
| [XamUserGetSigninState (816DD118)](XamUserGetSigninState-816DD118.md) | 0x816DD118 | Usuário: estado de login | ✅ | — |
| [XamUserGetSigninInfo (816DDB20)](XamUserGetSigninInfo-816DDB20.md) | 0x816DDB20 | Usuário: privilégio | ✅ | — |
| [FUN_816EFD40](FUN_816EFD40.md) | 0x816EFD40 | Usuário: flags do slot | Ghidra corrigido | — |
| [XamContentOpenFile (816EFD80)](XamContentOpenFile-816EFD80.md) | 0x816EFD80 | Usuário: XUID do slot | ✅ | — |
| [FUN_816F4768](FUN_816F4768.md) | 0x816F4768 | XamUserGetSigninState | Ghidra corrigido | — |

## XGraphics

A cópia da biblioteca gráfica dentro do xam (cabeçalhos de textura), usada pela foto do rosto do Kinect ID.

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [XGetVideoMode (816FB0E8)](XGetVideoMode-816FB0E8.md) | 0x816FB0E8 | Modo de vídeo | — | — |
| [FUN_817F6FD0](FUN_817F6FD0.md) | 0x817F6FD0 | XGraphics: bloco do formato | ✅ | — |
| [FUN_817F79C0](FUN_817F79C0.md) | 0x817F79C0 | XGraphics: cabeçalho de textura | ✅ | — |
| [XGSetTextureHeaderEx (817F7C38)](XGSetTextureHeaderEx-817F7C38.md) | 0x817F7C38 | XGraphics: cabeçalho de textura 2D | ✅ | — |
| [XGOffsetResourceAddress (817F7D18)](XGOffsetResourceAddress-817F7D18.md) | 0x817F7D18 | XGraphics: deslocar endereços | ✅ | — |
| [FUN_81837A98](FUN_81837A98.md) | 0x81837A98 | XGraphics: alinhar nível | ✅ | — |
| [FUN_81837BB8](FUN_81837BB8.md) | 0x81837BB8 | XGraphics: tamanhos | ✅ | — |
| [FUN_81838CB0](FUN_81838CB0.md) | 0x81838CB0 | XGraphics: fetch constant | ✅ | — |

## Fitness

O perfil de fitness, que conversa com o serviço da Live (app 0xFB).

*(trecho de código descompilado removido da versão pública)*

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [FUN_8168EF70](FUN_8168EF70.md) | 0x8168EF70 | Fitness: validar usuário da tela | ✅ | — |
| [FUN_8168F068](FUN_8168F068.md) | 0x8168F068 | Fitness: montar pacote | ✅ | — |
| [XamFitnessClearAll (8168F0E0)](XamFitnessClearAll-8168F0E0.md) | 0x8168F0E0 | 0xFB 0xB0084 {usuário, p2} | ✅ | ❌ ausente |
| [XamFitnessGetCurrentBodyProfileRecord (8168F168)](XamFitnessGetCurrentBodyProfileRecord-8168F168.md) | 0x8168F168 | 0xFB 0xB007B (síncrono) | ✅ | ❌ ausente |
| [XamFitnessGetAllTitleSummaries (8168F2C8)](XamFitnessGetAllTitleSummaries-8168F2C8.md) | 0x8168F2C8 | 0xFB 0xB007D | Ghidra corrigido | ❌ ausente |
| [XamFitnessGetTitleSummaries (8168F2E8)](XamFitnessGetTitleSummaries-8168F2E8.md) | 0x8168F2E8 | 0xFB 0xB007D | Ghidra corrigido | ❌ ausente |
| [XamFitnessGetTitleSummary (8168F308)](XamFitnessGetTitleSummary-8168F308.md) | 0x8168F308 | 0xFB 0xB007D (um título) | Ghidra corrigido | ❌ ausente |
| [XamFitnessGetAllBodyProfileRecords (8168F328)](XamFitnessGetAllBodyProfileRecords-8168F328.md) | 0x8168F328 | 0xFB 0xB007C | Ghidra corrigido | ❌ ausente |
| [XamFitnessGetOverallSummary (8168F3D8)](XamFitnessGetOverallSummary-8168F3D8.md) | 0x8168F3D8 | 0xFB 0xB007E | ✅ | ❌ ausente |
| [XamFitnessInitializeForOneUser (8168F470)](XamFitnessInitializeForOneUser-8168F470.md) | 0x8168F470 | 0xFB 0xB0086 | ✅ | ❌ ausente |
| [XamFitnessInitialize (8168F4E0)](XamFitnessInitialize-8168F4E0.md) | 0x8168F4E0 | 0xFB 0xB0080 (síncrono) | Ghidra corrigido | ❌ ausente |
| [XamFitnessUnInitialize (8168F530)](XamFitnessUnInitialize-8168F530.md) | 0x8168F530 | 0xFB 0xB0081 | Ghidra corrigido | ❌ ausente |
| [XampFitnessLetFireAndForgetsCatchUpInternal (8168F550)](XampFitnessLetFireAndForgetsCatchUpInternal-8168F550.md) | 0x8168F550 | 0xFB 0xB0088: espera as gravações assíncronas pendentes | Ghidra corrigido | ❌ ausente |
| [XamFitnessConvertByteMetToFloatMet (8168F568)](XamFitnessConvertByteMetToFloatMet-8168F568.md) | 0x8168F568 | byte × 0,1 | ✅ | ❌ ausente |
| [XamFitnessConvertFloatMetToByteMet (8168F5B0)](XamFitnessConvertFloatMetToByteMet-8168F5B0.md) | 0x8168F5B0 | (int)(float × 10) | ✅ | ❌ ausente |
| [XamFitnessSystemTimeToMsgTime (8168F5E8)](XamFitnessSystemTimeToMsgTime-8168F5E8.md) | 0x8168F5E8 | o inverso | ✅ | ❌ ausente |
| [XamFitnessGetPrivacySettings (8168F688)](XamFitnessGetPrivacySettings-8168F688.md) | 0x8168F688 | configuração de perfil 0x10040000 + privilégio 6 → valor 0 a 6 | ✅ | ❌ ausente |
| [XamFitnessGetSyncStatus (8168F830)](XamFitnessGetSyncStatus-8168F830.md) | 0x8168F830 | 0xFB 0xB0085 | ✅ | ❌ ausente |
| [XamFitnessGetSuggestedPrivacySettings (8168F8C0)](XamFitnessGetSuggestedPrivacySettings-8168F8C0.md) | 0x8168F8C0 | 6, ou 3/4 conforme a região | ✅ | ❌ ausente |
| [XamFitnessContainsFitnessData (8168F990)](XamFitnessContainsFitnessData-8168F990.md) | 0x8168F990 | 0xFB 0xB0089 | ✅ | ❌ ausente |
| [XamFitnessAddBodyProfileRecord (8168FA00)](XamFitnessAddBodyProfileRecord-8168FA00.md) | 0x8168FA00 | 0xFB 0xB0079: grava registro do corpo (pacote + verificação síncrona) | Ghidra corrigido | ❌ ausente |
| [XamFitnessAddFitnessEvent (8168FB40)](XamFitnessAddFitnessEvent-8168FB40.md) | 0x8168FB40 | 0xFB 0xB007A: grava evento de exercício | Ghidra corrigido | ❌ ausente |
| [XamFitnessMsgTimeToSystemTime (8168FC78)](XamFitnessMsgTimeToSystemTime-8168FC78.md) | 0x8168FC78 | segundos desde 2003-01-01 → SYSTEMTIME | ✅ | ❌ ausente |
| [XamFitnessCreateFitnessEventEnumerator (8168FCF0)](XamFitnessCreateFitnessEventEnumerator-8168FCF0.md) | 0x8168FCF0 | 0xFB 0xB007F/0xB0009: enumerador de eventos (itens de 0x24) | ✅ | ❌ ausente |
| [FUN_816920F0](FUN_816920F0.md) | 0x816920F0 | HUD: aplicar filtro de entrada | ✅ | — |
| [FUN_81692C48](FUN_81692C48.md) | 0x81692C48 | HUD: pedido de filtro de entrada | ✅ | — |
| [XamResetInactivity (81695EA0)](XamResetInactivity-81695EA0.md) | 0x81695EA0 | Inatividade: marcar atividade | ✅ | — |
| [FUN_81695EC8](FUN_81695EC8.md) | 0x81695EC8 | Inatividade: modo de presença | ✅ | — |
| [FUN_81698B78](FUN_81698B78.md) | 0x81698B78 | Fitness: ler valores em cache | ✅ | — |
| [XamFitnessClearBodyProfileRecords (81698BF8)](XamFitnessClearBodyProfileRecords-81698BF8.md) | 0x81698BF8 | não fala com a Live: devolve 3 valores em cache (0x10 bytes) | ✅ | ❌ ausente |
| [XamShowFitnessClearUI (816C5BD8)](XamShowFitnessClearUI-816C5BD8.md) | 0x816C5BD8 | UI 0x21040 | ✅ | ❌ ausente |
| [XamShowFitnessBodyProfileUI (816C5C50)](XamShowFitnessBodyProfileUI-816C5C50.md) | 0x816C5C50 | UI 0x2103A | Ghidra corrigido | ❌ ausente |
| [XamShowFitnessWarnAboutTimeUI (816C5CC8)](XamShowFitnessWarnAboutTimeUI-816C5CC8.md) | 0x816C5CC8 | UI 0x2103B | Ghidra corrigido | ❌ ausente |
| [XamShowFitnessWarnAboutPrivacyUI (816C5D20)](XamShowFitnessWarnAboutPrivacyUI-816C5D20.md) | 0x816C5D20 | UI 0x2103C | Ghidra corrigido | ❌ ausente |

## Kernel (importações)

Funções do kernel que o xam chama por thunks.

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) | 0x81A721C4 | KeGetCurrentProcessType (xboxkrnl 0x66) | — | — |
| [ExGetXConfigSetting (81A721F4)](ExGetXConfigSetting-81A721F4.md) | 0x81A721F4 | ExGetXConfigSetting (xboxkrnl 0x10) | — | — |
| [ExSetXConfigSetting (81A72204)](ExSetXConfigSetting-81A72204.md) | 0x81A72204 | ExSetXConfigSetting (xboxkrnl 0x18) | — | — |
| [ExReadModifyWriteXConfigSettingUlong (81A72214)](ExReadModifyWriteXConfigSettingUlong-81A72214.md) | 0x81A72214 | ExReadModifyWriteXConfigSettingUlong (xboxkrnl 0x2BC) | — | — |
| [KeWaitForSingleObject (81A72244)](KeWaitForSingleObject-81A72244.md) | 0x81A72244 | KeWaitForSingleObject (xboxkrnl 0xB0) | — | — |
| [_snprintf (81A722E4)](_snprintf-81A722E4.md) | 0x81A722E4 | _snprintf (xboxkrnl 0x13A) | — | — |
| [PsCamDeviceRequest (81A73134)](PsCamDeviceRequest-81A73134.md) | 0x81A73134 | PsCamDeviceRequest (xboxkrnl 0x30D) | — | — |
| [UsbdGetNatalHardwareVersion (81A73144)](UsbdGetNatalHardwareVersion-81A73144.md) | 0x81A73144 | UsbdGetNatalHardwareVersion (xboxkrnl 0x373) | — | — |
| [DetroitDeviceRequest (81A73154)](DetroitDeviceRequest-81A73154.md) | 0x81A73154 | DetroitDeviceRequest (xboxkrnl 0x30F) | — | — |
| [McaDeviceRequest (81A73164)](McaDeviceRequest-81A73164.md) | 0x81A73164 | McaDeviceRequest (xboxkrnl 0x30E) | — | — |
| [XAudioGetSpeakerConfig (81A73654)](XAudioGetSpeakerConfig-81A73654.md) | 0x81A73654 | XAudioGetSpeakerConfig (xboxkrnl 0x1FF) | — | — |

## Rede

Pontos da pilha de rede citados na nota de rede (ficam dentro de outras funções).

| Função | Endereço | Resumo | REA | Xenia |
|---|---|---|---|---|
| [FUN_8175AC78](FUN_8175AC78.md) | 0x8175AC78 | dentro de outra função | — | — |
| [FUN_8175C540](FUN_8175C540.md) | 0x8175C540 | dentro de outra função | — | — |
| [FUN_81764260](FUN_81764260.md) | 0x81764260 | dentro de outra função | — | — |
| [FUN_817653D0](FUN_817653D0.md) | 0x817653D0 | dentro de outra função | — | — |
| [FUN_817654B0](FUN_817654B0.md) | 0x817654B0 | dentro de outra função | — | — |
| [FUN_81765518](FUN_81765518.md) | 0x81765518 | dentro de outra função | — | — |
| [FUN_81765B88](FUN_81765B88.md) | 0x81765B88 | dentro de outra função | — | — |
| [FUN_81766E80](FUN_81766E80.md) | 0x81766E80 | dentro de outra função | — | — |
| [FUN_8176A460](FUN_8176A460.md) | 0x8176A460 | dentro de outra função | — | — |
| [FUN_81778C90](FUN_81778C90.md) | 0x81778C90 | dentro de outra função | — | — |
| [FUN_8177A3F8](FUN_8177A3F8.md) | 0x8177A3F8 | dentro de outra função | — | — |
| [FUN_8177ABC0](FUN_8177ABC0.md) | 0x8177ABC0 | dentro de outra função | — | — |
