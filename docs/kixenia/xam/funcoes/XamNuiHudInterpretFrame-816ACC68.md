# XamNuiHudInterpretFrame

**Endereço:** 0x816ACC68 · **Ordinal:** 2172 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** ✅ implementada

## O que faz
o HUD do sistema: 2 jogadores, alertas, aceno, gesto do guia; 0x8301000B com UI aberta

## Fatos do decompilado
- Códigos de erro: 0x80004005
- Globais: 0x81AA07D0

## Chama
- [XamResetInactivity (81695EA0)](XamResetInactivity-81695EA0.md) — Inatividade: marcar atividade
- [FUN_816AC688](FUN_816AC688.md) — HUD: interpretar quadro
- [FUN_816ACAD8](FUN_816ACAD8.md) — HUD: contador de presença
- Outras: XamIsUIActive (816C7D78)

## É chamada por
- [FUN_816F9CB8](FUN_816F9CB8.md) — Despachante NUI (0x2B0xx)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): ├─ HUD do sistema: XamNuiHudInterpretFrame, chamado em cada quadro 0x2B002
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 0x2B002 | quadro de esqueleto do título (versão 5) → `XamNuiHudInterpretFrame` |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2172 | 0x816ACC68 | `XamNuiHudInterpretFrame` | o HUD do sistema: 2 jogadores, alertas, aceno, gesto do guia; 0x8301000B com UI aberta | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): 5. **HUD do sistema (e19c98e7e):** nenhum jogo do HD chama `XamNuiHudInterpretFrame`; o aceno foi testado só com mão sintética (engaja em 2,4 s a 1 Hz). O gesto do guia do sistema usa uma pose aproximada (braço esquerdo esticado, 30° a 60° abaixo do ombro, para o lado, 3 s).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): **Quem usa:** nem o Nike nem o Kinect Sports importam `XamNuiSkeletonScoreUpdate`, `XamNuiHudInterpretFrame` ou `XamNuiGetSystemGestureControl`, porque cada um tem o próprio runtime.
