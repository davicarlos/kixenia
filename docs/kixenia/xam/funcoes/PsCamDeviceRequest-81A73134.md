# PsCamDeviceRequest

**Endereço:** 0x81A73134 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `PsCamDeviceRequest (xboxkrnl 0x30D)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: PsCamDeviceRequest (xboxkrnl 0x30D)

## É chamada por
- [XamNuiGetTrueColorInfo (816F9B38)](XamNuiGetTrueColorInfo-816F9B38.md)
- [XamNuiGetCameraIntrinsics (816F9B90)](XamNuiGetCameraIntrinsics-816F9B90.md)
- [XamNuiGetSupportString (817FA2E0)](XamNuiGetSupportString-817FA2E0.md)
- [FUN_8180C700](FUN_8180C700.md) — Motor: inclinar
- e mais 13 funções fora desta base (vizinhas): FUN_817FA820, FUN_817FCA90, FUN_817FCAE8, FUN_817FCB40, FUN_817FCC80, FUN_8180CEA0, FUN_8180D0C0, FUN_8180D518, FUN_819837A8, FUN_81983DD0, FUN_81984E18, FUN_81984EB8, FUN_81985178

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x81A73134 | 0x30D | PsCamDeviceRequest (câmera) |
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): | `0x81A73134` | PsCamDeviceRequest (0x30D) |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
