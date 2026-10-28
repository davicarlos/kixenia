# XamNuiSkeletonGetBestSkeletonIndex

**Endereço:** 0x816F9988 · **Ordinal:** 2195 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** ✅ implementada

## O que faz
o rastreado entre os 2 escolhidos

## É chamada por
- [FUN_816AC688](FUN_816AC688.md) — HUD: interpretar quadro

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2195 | 0x816F9988 | `XamNuiSkeletonGetBestSkeletonIndex` | o rastreado entre os 2 escolhidos | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamNuiSkeletonGetBestSkeletonIndex`.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiSkeletonGetBestSkeletonIndex`:** o primeiro ID escolhido cujo esqueleto está rastreado; senão, −1.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): O export (ordinal 2196, 0x816F9980) é um desvio para o corpo em **0x8198B628**. `XamNuiSkeletonGetBestSkeletonIndex` (0x816F9988) desvia para 0x8198B170.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - Se o engajado sumiu: `XamNuiSkeletonGetBestSkeletonIndex`, `FUN_817FD2D0(1)` e o ID engajado é zerado.
