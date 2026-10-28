# XamNuiCameraTiltGetStatus

**Endereço:** 0x816FAA60 · **Ordinal:** 2160 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** 🟡 aproximada

## O que faz
status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23)

## Fatos do decompilado
- Globais: 0x815F46E8, 0x81A85BFC, 0x81AAC9E0, 0x81AAC9E4, 0x81AAC9E8, 0x81AAC9F0

## Chama
- [FUN_816FA6F0](FUN_816FA6F0.md) — Rotação por eixo e ângulo
- [FUN_8172FDA0](FUN_8172FDA0.md) — acos
- [FUN_8180C850](FUN_8180C850.md) — Motor: ler status
- Outras: FUN_816FA478, `memcpy` (0x8172D590), `atan2` (0x8172FE90), FUN_81730234, FUN_817304CC

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): - O status que os títulos leem (`XamNuiCameraTiltGetStatus`) tem a assinatura **'XtSs'** (0x58745373) e 0x50 bytes. `ElevationGetAngle` calcula o ângulo com atan2 do acelerômetro.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2160 | 0x816FAA60 | `XamNuiCameraTiltGetStatus` | status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | 🟡 |
