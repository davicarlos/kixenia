# ExGetXConfigSetting

**Endereço:** 0x81A721F4 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `ExGetXConfigSetting (xboxkrnl 0x10)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: ExGetXConfigSetting (xboxkrnl 0x10)

## É chamada por
- e mais 69 funções fora desta base (vizinhas): FUN_81672348, XamSetClosedCaptioningState (81679A38), XamUserContentRestrictionGetRating (81688040), FUN_8168B000, FUN_8168E5F0, FUN_8168E668, XamSetInactivityTimeFromConfig (816960E0), FUN_816967A8, XGetLanguage (81698E60), XTLGetLanguageV2 (81698F38), XamGetLanguage (81699010), FUN_8169D448, FUN_8169DEE0, FUN_8169ED78, XamGetLocaleEx (816A1AB8), XamGetCountry (816A1C20), FUN_816A6F00, FUN_816A7058, FUN_816AB810, FUN_816AECF0, XamUserIsUnsafeProgrammingAllowed (816AF590), XamFamilyAddParentalExemptionToList (816AF6A8), XamFamilyIsParentalValidationRequired (816AF7B0), FUN_816B2BA8, FUN_816B3BE0, FUN_816B3E88, FUN_816B6FC8, FUN_816B70C8, FUN_816BE518, FUN_816BEF30 …

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x81A721F4 / 0x81A72204 / 0x81A72214 | 0x10 / 0x18 / 0x2BC | ExGetXConfigSetting / ExSetXConfigSetting / ExReadModifyWriteXConfigSettingUlong |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
