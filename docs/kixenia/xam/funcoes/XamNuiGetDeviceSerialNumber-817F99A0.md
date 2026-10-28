# XamNuiGetDeviceSerialNumber

**Endereço:** 0x817F99A0 · **Ordinal:** 2269 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ✅ implementada

## O que faz
Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas

## Fatos do decompilado
- Kernel: DetroitDeviceRequest (xboxkrnl 0x30F), KeWaitForSingleObject (xboxkrnl 0xB0), McaDeviceRequest (xboxkrnl 0x30E)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2269 | 0x817F99A0 | `XamNuiGetDeviceSerialNumber` | Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - **Número de série** (`XamNuiGetDeviceSerialNumber(buf, 0x11)`): lido do descritor USB (91ac58116).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiGetDeviceSerialNumber(buf, tamanho)`:** o Nike passa 0x11; o xam copia 12 caracteres e um NUL.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiGetDeviceSerialNumber` | Nike | implementada (91ac58116) |
