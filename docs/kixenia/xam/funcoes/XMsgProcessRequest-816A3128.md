# XMsgProcessRequest

**Endereço:** 0x816A3128 · **Ordinal:** 1183 · **Área:** [Sensor e estado](README.md#sensor-e-estado)

## O que faz
Envia uma mensagem a um app do xam (0xFB Live, 0xFE xam) com XOVERLAPPED opcional; base de quase todo o fitness e de StopMovement (0x2B009).

## Fatos do decompilado
- Códigos de erro: 0x800703E5
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66)

## Chama
- [XMsgInProcessCall (816A2F88)](XMsgInProcessCall-816A2F88.md) — XMsg síncrono
- [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) — KeGetCurrentProcessType (xboxkrnl 0x66)
- Outras: XMsgStartIORequestEx (816A2CF0), RtlGetLastError (81721AC0)

## É chamada por
- [XamFitnessClearAll (8168F0E0)](XamFitnessClearAll-8168F0E0.md)
- [XamFitnessGetCurrentBodyProfileRecord (8168F168)](XamFitnessGetCurrentBodyProfileRecord-8168F168.md)
- [XamFitnessGetAllTitleSummaries (8168F2C8)](XamFitnessGetAllTitleSummaries-8168F2C8.md)
- [XamFitnessGetAllBodyProfileRecords (8168F328)](XamFitnessGetAllBodyProfileRecords-8168F328.md)
- [XamFitnessGetOverallSummary (8168F3D8)](XamFitnessGetOverallSummary-8168F3D8.md)
- [XamFitnessInitializeForOneUser (8168F470)](XamFitnessInitializeForOneUser-8168F470.md)
- [XamFitnessInitialize (8168F4E0)](XamFitnessInitialize-8168F4E0.md)
- [XamFitnessGetSyncStatus (8168F830)](XamFitnessGetSyncStatus-8168F830.md)
- [XamFitnessContainsFitnessData (8168F990)](XamFitnessContainsFitnessData-8168F990.md)
- [XamFitnessAddBodyProfileRecord (8168FA00)](XamFitnessAddBodyProfileRecord-8168FA00.md)
- [XamFitnessAddFitnessEvent (8168FB40)](XamFitnessAddFitnessEvent-8168FB40.md)
- [XamNuiCameraTiltSetCallback (816FA128)](XamNuiCameraTiltSetCallback-816FA128.md)
- [XamNuiCameraAdjustTilt (816FA338)](XamNuiCameraAdjustTilt-816FA338.md)
- [XamNuiCameraElevationAutoTilt (816FA3B8)](XamNuiCameraElevationAutoTilt-816FA3B8.md)
- [XamNuiCameraElevationReverseAutoTilt (816FA418)](XamNuiCameraElevationReverseAutoTilt-816FA418.md)
- [XamNuiCameraElevationStopMovement (816FA520)](XamNuiCameraElevationStopMovement-816FA520.md)
- [XamNuiIdentityAbort (8180D910)](XamNuiIdentityAbort-8180D910.md)
- [XamNuiIdentityEnrollForSignIn (8180D970)](XamNuiIdentityEnrollForSignIn-8180D970.md)
- [XamNuiIdentityIdentifyWithBiometric (8180DA50)](XamNuiIdentityIdentifyWithBiometric-8180DA50.md)
- e mais 23 funções fora desta base (vizinhas): XamWriteTile (816707F8), XamReadString (81670898), XamReadTileToTexture (81670CB8), XamWriteGamerTile (81670E08), XamGetUserBalance (8168AAA8), FUN_816A31C8, XNotifyQueueUIEx (816A9DE8), XamPngDecode (816B11C0), XamUserReadUserPreference (816B1F68), XamUserWriteUserPreference (816B2018), XamProfileSaveAccountInfo (816DAFC8), XamProfileDelete (816DB078), FUN_816DED50, FUN_816EE0E8, XamContentIsGameInstalledToHDD (816EE150), FUN_816EE190, FUN_816EF470, XamContentGetOnlineCreator (816EF6D0), XamContentGetLicenseMask (816EF770), XamContentDuplicateFileHandle (816EFA40), XamContentMountInstalledGame (816EFAF0), XamContentQueryLicenseInternal (816EFB90), FUN_816EFC20

## Nas notas
- [XAM-Fitness](../xam-fitness.md): - **Envio:** `FUN_816a3128(0xFB, msg, buffer, tamanho, overlapped)` envia ao serviço da Live (assíncrono, com XOVERLAPPED).
