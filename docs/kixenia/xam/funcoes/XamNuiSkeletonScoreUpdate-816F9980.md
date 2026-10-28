# XamNuiSkeletonScoreUpdate

**Endereço:** 0x816F9980 · **Ordinal:** 2196 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** ✅ implementada

## O que faz
registros de 0x3C: alertas de borda, distância, giro, mãos

## Fatos do decompilado
- Globais: 0x815F46E8

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2196 | 0x816F9980 | `XamNuiSkeletonScoreUpdate` | registros de 0x3C: alertas de borda, distância, giro, mãos | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamNuiSkeletonScoreUpdate`: alertas de posição, distância, giro e mãos para até 2 jogadores (ver [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md)).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### Pontuação de jogadores (`XamNuiSkeletonScoreUpdate`, analisada em 2026-10-24)
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): O export (ordinal 2196, 0x816F9980) é um desvio para o corpo em **0x8198B628**. `XamNuiSkeletonGetBestSkeletonIndex` (0x816F9988) desvia para 0x8198B170.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): **`XamNuiSkeletonScoreUpdate(quadro, pontuações)`** mantém até **2 registros** de 0x3C bytes (em +0x00 e +0x3C), um por jogador rastreado:
