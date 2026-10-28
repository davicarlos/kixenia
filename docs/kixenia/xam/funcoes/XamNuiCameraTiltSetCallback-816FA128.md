# XamNuiCameraTiltSetCallback

**Endereço:** 0x816FA128 · **Ordinal:** 2157 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ✅ implementada

## O que faz
0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40)

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2B007

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x2B007 | registra o motor de inclinação do título (`XamNuiCameraTiltSetCallback`) |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2157 | 0x816FA128 | `XamNuiCameraTiltSetCallback` | 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `XamNuiCameraTiltSetCallback` registra o tratador 0x82F53ED0, que atende as mensagens 0x2B005 (inclinação automática), 0x2B00A (reverter) e 0x2B00B (ajustar).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `XamNuiCameraTiltSetCallback` só guarda o callback.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | **0x2B007** | grava 0x81A85C40: `XamNuiCameraTiltSetCallback` |
