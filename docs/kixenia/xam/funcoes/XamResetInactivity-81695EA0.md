# XamResetInactivity

**Endereço:** 0x81695EA0 · **Ordinal:** 417 · **Área:** [Fitness](README.md#fitness)

## O que faz
Grava a hora atual em 0x81A9E0A0, o relógio de inatividade do console. O HUD chama quando vê um jogador presente, para o console não apagar a tela.

## Fatos do decompilado
- Kernel: KeQueryInterruptTime (xboxkrnl 0x82)
- Globais: 0x81A9E0A0

## Chama
- Outras: `KeQueryInterruptTime (xboxkrnl 0x82)`

## É chamada por
- [XamNuiHudInterpretFrame (816ACC68)](XamNuiHudInterpretFrame-816ACC68.md)
- e mais 8 funções fora desta base (vizinhas): FUN_816C7230, FUN_816C7378, FUN_816C77C8, FUN_816F3C48, FUN_817F96A8, FUN_81810D10, FUN_81812448, FUN_81814CD8

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): 1. `FUN_816ACAD8` conta presença: 6 entradas de 5 bytes `{u8 contagem, u32 ID}` em 0x81AA07B0. Devolve 1 quando um jogador rastreado continua por **mais de 120 quadros** (4 s); no outro modo (`FUN_81695EC8`), quando há qualquer esqueleto. Nesse caso `FUN_81695EA0` grava a hora em 0x81A9E0A0 (o relógio de inatividade).
