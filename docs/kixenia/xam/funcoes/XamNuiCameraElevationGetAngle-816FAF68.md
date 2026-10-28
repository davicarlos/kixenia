# XamNuiCameraElevationGetAngle

**Endereço:** 0x816FAF68 · **Ordinal:** 2177 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** 🟡 aproximada

## O que faz
lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3)

## Fatos do decompilado
- Globais: 0x815FC218

## Chama
- [XamNuiCameraTiltGetStatus (816FAA60)](XamNuiCameraTiltGetStatus-816FAA60.md)
- Outras: `atan2` (0x8172FE90), FUN_817304F0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2177 | 0x816FAF68 | `XamNuiCameraElevationGetAngle` | lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | 🟡 |
