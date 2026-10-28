# McaDeviceRequest

**Endereço:** 0x81A73164 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `McaDeviceRequest (xboxkrnl 0x30E)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: McaDeviceRequest (xboxkrnl 0x30E)

## É chamada por
- [XamNuiCameraElevationStopMovement (816FA520)](XamNuiCameraElevationStopMovement-816FA520.md)
- [FUN_8180C700](FUN_8180C700.md) — Motor: inclinar
- [FUN_8180C850](FUN_8180C850.md) — Motor: ler status
- [FUN_81983268](FUN_81983268.md) — CEMD: início
- [FUN_819832F0](FUN_819832F0.md) — CEMD: bloco
- [FUN_81983380](FUN_81983380.md) — CEMD: fim
- [FUN_81983E98](FUN_81983E98.md) — CEMD: enviar
- e mais 20 funções fora desta base (vizinhas): FUN_817FC528, FUN_8180CB48, FUN_8180CD08, FUN_8180D1B0, FUN_8180D768, FUN_81982CE0, FUN_81982D28, FUN_81982D70, FUN_81982DB0, FUN_81982E60, FUN_81982EF0, FUN_81982F80, FUN_81983128, FUN_81983408, FUN_81983490, FUN_81983970, FUN_81983BF8, FUN_81983C88, FUN_81983D20, FUN_81983D98

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x81A73164 | 0x30E | McaDeviceRequest (áudio; motor do modelo 1473) |
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): O envio genérico é `FUN_81983E98(pacote, tamanho)`: um pedido de dispositivo com código 0xE (`FUN_81A73164`, provavelmente `McaDeviceRequest`), que espera a conclusão.
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): | `0x81A73164` | **McaDeviceRequest** (0x30E): o CEMD vai por ele, com o código 0xE |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.
