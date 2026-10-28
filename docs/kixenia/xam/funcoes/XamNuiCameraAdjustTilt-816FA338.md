# XamNuiCameraAdjustTilt

**Endereço:** 0x816FA338 · **Ordinal:** 2198 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ✅ implementada

## O que faz
0xFE 0x2B00B {u32, float, float} → motor de inclinação do título. Só o Kinect Tuner chama: AdjustTilt(2.1336, 1.3716, 0x20) = distância máxima 7 pés, referência 4,5 pés, 0x20 só verificar (cabeça cortada no topo); bits 0–1 = mira +0/150/300 mm; resposta na 0x2B00C (0xE0 bytes). Ver Guide.NuiTroubleshooter.xex

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2B00B
- Códigos de erro: 0x8007048F

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2198 | 0x816FA338 | `XamNuiCameraAdjustTilt` | 0xFE 0x2B00B {u32, float, float} → motor de inclinação do título | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiCameraRememberFloor`:** grava o plano do chão e aciona a inclinação automática. `XamNuiCameraElevationAutoTilt` e `XamNuiCameraAdjustTilt` também existem.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiCameraAdjustTilt(float, float, u32)` | 0xFE **0x2B00B** `{u32, float, float}` |
