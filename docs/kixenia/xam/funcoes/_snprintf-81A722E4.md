# _snprintf

**Endereço:** 0x81A722E4 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `_snprintf (xboxkrnl 0x13A)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: _snprintf (xboxkrnl 0x13A)

## É chamada por
- [XamNuiGetSupportString (817FA2E0)](XamNuiGetSupportString-817FA2E0.md)
- e mais 194 funções fora desta base (vizinhas): XamAvatarGetInstalledAssetPackageDescription (816756D0), FUN_816764A8, FUN_81676690, FUN_816769D0, FUN_81677EA8, FUN_8167B1D8, FUN_8167B610, FUN_8167BF50, FUN_8167D0E8, FUN_8167D4E8, FUN_8167D748, FUN_8167DD70, FUN_8167E400, FUN_8167EB60, FUN_8167EE50, FUN_8167EFC8, FUN_8167FDA0, FUN_81680078, FUN_81681FB8, FUN_81682230, FUN_81682F20, FUN_816840F0, FUN_816842F8, FUN_81684688, FUN_816857F0, FUN_81685F38, FUN_81686248, FUN_81686978, FUN_8168BEF0, FUN_8168C120 …

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
