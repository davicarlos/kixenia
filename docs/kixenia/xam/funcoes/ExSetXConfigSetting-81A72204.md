# ExSetXConfigSetting

**Endereço:** 0x81A72204 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `ExSetXConfigSetting (xboxkrnl 0x18)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: ExSetXConfigSetting (xboxkrnl 0x18)

## É chamada por
- e mais 24 funções fora desta base (vizinhas): FUN_81672440, FUN_81672568, XamSetClosedCaptioningState (81679A38), FUN_8168E6E0, FUN_81696808, XamSetCountry (816A1C78), FUN_816A6F00, FUN_816A7638, XamFamilyAddParentalExemptionToList (816AF6A8), FUN_816B6FC8, FUN_816DBD50, FUN_816DC598, FUN_816F9C38, FUN_8170CCE8, FUN_8170CDA0, FUN_817492A0, FUN_81763798, FUN_81763890, FUN_81873F60, FUN_81876F98, FUN_81901FE8, FUN_8190F660, FUN_8198BF78, FUN_8198C428

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x81A721F4 / 0x81A72204 / 0x81A72214 | 0x10 / 0x18 / 0x2BC | ExGetXConfigSetting / ExSetXConfigSetting / ExReadModifyWriteXConfigSettingUlong |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
