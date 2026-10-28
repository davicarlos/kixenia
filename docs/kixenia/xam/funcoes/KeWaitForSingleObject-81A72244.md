# KeWaitForSingleObject

**Endereço:** 0x81A72244 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `KeWaitForSingleObject (xboxkrnl 0xB0)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: KeWaitForSingleObject (xboxkrnl 0xB0)

## É chamada por
- [FUN_81983268](FUN_81983268.md) — CEMD: início
- [FUN_819832F0](FUN_819832F0.md) — CEMD: bloco
- [FUN_81983380](FUN_81983380.md) — CEMD: fim
- [FUN_81983E98](FUN_81983E98.md) — CEMD: enviar
- e mais 63 funções fora desta base (vizinhas): FUN_8167B718, FUN_816964A8, FUN_8169CAB8, FUN_8169E088, FUN_8169EAB0, FUN_816AF338, XamPartyGetFormation (816AFE78), FUN_816B94A0, FUN_816BA058, FUN_816C8800, FUN_816CAC20, FUN_816CAD78, FUN_816D0DD0, FUN_816EB760, FUN_8170A520, FUN_8170A7C0, FUN_8170B1C0, FUN_81742B70, FUN_817FCC80, FUN_817FCE28, FUN_8180CB48, FUN_8180CD08, FUN_8180D1B0, FUN_8180D518, FUN_8180D768, FUN_8180E490, FUN_81851F38, FUN_818870A0, FUN_818871A8, FUN_81887370 …

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
