# XamNuiPlayerEngagementUpdate

**Endereço:** 0x816F9978 · **Ordinal:** 2194 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** ✅ implementada

## O que faz
os 2 esqueletos mais próximos → quadro+0xACC (o rastreado ganha 0,3 m)

## Fatos do decompilado
- Globais: 0x815F46E8, 0x815FC24C, 0x816352CC, 0x816352D0

## É chamada por
- [FUN_816AC688](FUN_816AC688.md) — HUD: interpretar quadro

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2194 | 0x816F9978 | `XamNuiPlayerEngagementUpdate` | os 2 esqueletos mais próximos → quadro+0xACC (o rastreado ganha 0,3 m) | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamNuiPlayerEngagementUpdate`: pontuação do xam = √(x²+z²), −0,3 para quem já está rastreado. Os IDs escolhidos vão em quadro+0xACC.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiPlayerEngagementUpdate`:** a pontuação de cada pessoa é √(x²+z²), menos 0,3 se ela já está rastreada, com mínimo 0,1. Os dois mais próximos são escolhidos e os IDs gravados no quadro. A saída tem 7 campos: ID engajado, índice, usuário, cadastro, quantos rastreados e os 2 índices.
