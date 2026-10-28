# XamNuiCameraSetFlags

**Endereço:** 0x816FB000 · **Ordinal:** 2148 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ✅ implementada

## O que faz
Detroit: lê o status e grava flags com o comando 0xF0510; Mca: nada

## Fatos do decompilado
- Códigos de erro: 0x8007048F
- Kernel: DetroitDeviceRequest (xboxkrnl 0x30F)

## Chama
- [XamNuiCameraGetTiltControllerType (816FA088)](XamNuiCameraGetTiltControllerType-816FA088.md)
- [XamNuiCameraTiltGetStatus (816FAA60)](XamNuiCameraTiltGetStatus-816FAA60.md)
- [DetroitDeviceRequest (81A73154)](DetroitDeviceRequest-81A73154.md) — DetroitDeviceRequest (xboxkrnl 0x30F)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | flags (`XamNuiCameraSetFlags`) | 0xF0510 | — |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2148 | 0x816FB000 | `XamNuiCameraSetFlags` | Detroit: lê o status e grava flags com o comando 0xF0510; Mca: nada | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamNuiCameraSetFlags`.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiCameraSetFlags(valor, máscara)`:** lê o status da inclinação e manda `(atual & ~máscara) | (valor & máscara)` ao motor. No controlador tipo 2 só devolve sucesso; sem motor, `0x8007048F`.
