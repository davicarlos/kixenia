# ExReadModifyWriteXConfigSettingUlong

**Endereço:** 0x81A72214 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `ExReadModifyWriteXConfigSettingUlong (xboxkrnl 0x2BC)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: ExReadModifyWriteXConfigSettingUlong (xboxkrnl 0x2BC)

## É chamada por
- e mais 5 funções fora desta base (vizinhas): FUN_8169D448, XamSetCountry (816A1C78), XNotifyUISetOptions (816ABB38), FUN_816E3510, FUN_817F9ED0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x81A721F4 / 0x81A72204 / 0x81A72214 | 0x10 / 0x18 / 0x2BC | ExGetXConfigSetting / ExSetXConfigSetting / ExReadModifyWriteXConfigSettingUlong |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
