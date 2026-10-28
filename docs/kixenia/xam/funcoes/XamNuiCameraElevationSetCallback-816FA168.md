# XamNuiCameraElevationSetCallback

**Endereço:** 0x816FA168 · **Ordinal:** 2180 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ✅ implementada

## O que faz
0xFE 0x2B00D: registra quem ouve o status (0x81A85C20)

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2B00D

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x2B00D | registra quem ouve o status (`XamNuiCameraElevationSetCallback`) |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2180 | 0x816FA168 | `XamNuiCameraElevationSetCallback` | 0xFE 0x2B00D: registra quem ouve o status (0x81A85C20) | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiCameraElevationSetCallback(cb)` | 0xFE **0x2B00D** `{cb}`, sem checar o controlador |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | **0x2B00D** | grava 0x81A85C20: `XamNuiCameraElevationSetCallback` |
