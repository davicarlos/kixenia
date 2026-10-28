# XamNuiGetSupportString

**Endereço:** 0x817FA2E0 · **Ordinal:** 2274 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** 🟡 aproximada

## O que faz
"K:%02d:...:%12s": modo, estado, problema, versão do firmware, série

## Fatos do decompilado
- Kernel: PsCamDeviceRequest (xboxkrnl 0x30D), _snprintf (xboxkrnl 0x13A)
- Globais: 0x81AD1274, 0x81AD1348, 0x81AD134C, 0x81AD1350, 0x81AD1354, 0x81AD1361, 0x81AD1364, 0x81AD136C

## Chama
- [_snprintf (81A722E4)](_snprintf-81A722E4.md) — _snprintf (xboxkrnl 0x13A)
- [PsCamDeviceRequest (81A73134)](PsCamDeviceRequest-81A73134.md) — PsCamDeviceRequest (xboxkrnl 0x30D)
- Outras: `memset` (0x8172D4F0), `memcpy` (0x8172D590), FUN_8180D3C0, FUN_81982DB0, FUN_81983970, FUN_81984C00

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2274 | 0x817FA2E0 | `XamNuiGetSupportString` | "K:%02d:...:%12s": modo, estado, problema, versão do firmware, série | 🟡 |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiGetSupportString(?, buffer)`**: o código de suporte da tela de solução de problemas, `"K:%02d:%02d:%02d:%02d:%02d.%02d.%04d.%02d:%12s"`: modo, sub-estado, estado do dispositivo (0x81AD1274), categoria e código do problema, versões, e o número de série (12 caracteres, `000000000000` sem sensor). A categoria compara as versões do firmware esperadas com as do sensor: 0 = iguais, 6/7/8 = di
