# DetroitDeviceRequest

**Endereço:** 0x81A73154 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `DetroitDeviceRequest (xboxkrnl 0x30F)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: DetroitDeviceRequest (xboxkrnl 0x30F)

## É chamada por
- [XamNuiCameraElevationStopMovement (816FA520)](XamNuiCameraElevationStopMovement-816FA520.md)
- [XamNuiCameraSetFlags (816FB000)](XamNuiCameraSetFlags-816FB000.md)
- [FUN_8180C700](FUN_8180C700.md) — Motor: inclinar
- [FUN_8180C850](FUN_8180C850.md) — Motor: ler status
- e mais 12 funções fora desta base (vizinhas): FUN_817FC6E8, FUN_817FCD10, FUN_817FCD60, FUN_817FCE28, FUN_8180CB48, FUN_8180CD08, FUN_8180D1B0, FUN_819835B0, FUN_81983970, FUN_81984B18, FUN_81984C00, FUN_81984CC0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x81A73154 | 0x30F | DetroitDeviceRequest (hub; motor do modelo 1414) |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
