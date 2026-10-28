# XamNuiCameraElevationReverseAutoTilt

**Endereço:** 0x816FA418 · **Ordinal:** 2188 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ✅ implementada

## O que faz
0xFE 0x2B00A {u32} → motor de inclinação do título

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2B00A
- Códigos de erro: 0x8007048F

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2188 | 0x816FA418 | `XamNuiCameraElevationReverseAutoTilt` | 0xFE 0x2B00A {u32} → motor de inclinação do título | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiCameraElevationReverseAutoTilt(u32)` | 0xFE **0x2B00A** `{u32}`: desfaz a última inclinação automática |
