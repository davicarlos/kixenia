# XamNuiCameraElevationAutoTilt

**Endereço:** 0x816FA3B8 · **Ordinal:** 2178 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ✅ implementada

## O que faz
0xFE 0x2B005 {float} → motor de inclinação do título

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2B005
- Códigos de erro: 0x8007048F

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2178 | 0x816FA3B8 | `XamNuiCameraElevationAutoTilt` | 0xFE 0x2B005 {float} → motor de inclinação do título | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiCameraRememberFloor`:** grava o plano do chão e aciona a inclinação automática. `XamNuiCameraElevationAutoTilt` e `XamNuiCameraAdjustTilt` também existem.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiCameraElevationAutoTilt(float)` | XMsg 0xFE **0x2B005** `{float}`: pede ao sistema a inclinação automática |
