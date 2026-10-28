# XamNuiCameraRememberFloor

**Endereço:** 0x816FA618 · **Ordinal:** 2149 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ⚪ stub

## O que faz
guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes)

## Fatos do decompilado
- Globais: 0x81A85BFC, 0x81AAC9E0, 0x81AAC9E4, 0x81AAC9E8, 0x81AAC9EC, 0x81AAC9F0

## Chama
- [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md)
- [FUN_8180C850](FUN_8180C850.md) — Motor: ler status
- Outras: FUN_816FA478

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2149 | 0x816FA618 | `XamNuiCameraRememberFloor` | guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes) | ⚪ stub |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiCameraRememberFloor`:** grava o plano do chão e aciona a inclinação automática. `XamNuiCameraElevationAutoTilt` e `XamNuiCameraAdjustTilt` também existem.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `XamNuiCameraRememberFloor`: o Kinect Sports chama cerca de 25 vezes por segundo, e o Nike também. Responde sucesso; a inclinação automática é a do Xenia.
