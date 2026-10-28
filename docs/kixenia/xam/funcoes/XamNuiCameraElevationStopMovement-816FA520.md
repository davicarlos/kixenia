# XamNuiCameraElevationStopMovement

**Endereço:** 0x816FA520 · **Ordinal:** 2179 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** 🟡 aproximada

## O que faz
Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2B009
- Códigos de erro: 0x8007048F
- Kernel: DetroitDeviceRequest (xboxkrnl 0x30F), McaDeviceRequest (xboxkrnl 0x30E)

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md)
- [DetroitDeviceRequest (81A73154)](DetroitDeviceRequest-81A73154.md) — DetroitDeviceRequest (xboxkrnl 0x30F)
- [McaDeviceRequest (81A73164)](McaDeviceRequest-81A73164.md) — McaDeviceRequest (xboxkrnl 0x30E)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2179 | 0x816FA520 | `XamNuiCameraElevationStopMovement` | Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | 🟡 |
