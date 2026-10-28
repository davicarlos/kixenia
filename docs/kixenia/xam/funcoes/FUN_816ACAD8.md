# FUN_816ACAD8

**Endereço:** 0x816ACAD8 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos)

## O que faz
Conta presença por jogador (6 entradas {u8 contagem, u32 ID} em 0x81AA07B0); devolve 1 quando um jogador fica mais de 120 quadros, o que zera a inatividade.

## Fatos do decompilado
- Globais: 0x81AA07AC, 0x81AA07B0, 0x81AA07B1

## Chama
- [FUN_81695EC8](FUN_81695EC8.md) — Inatividade: modo de presença

## É chamada por
- [XamNuiHudInterpretFrame (816ACC68)](XamNuiHudInterpretFrame-816ACC68.md)

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): 1. `FUN_816ACAD8` conta presença: 6 entradas de 5 bytes `{u8 contagem, u32 ID}` em 0x81AA07B0. Devolve 1 quando um jogador rastreado continua por **mais de 120 quadros** (4 s); no outro modo (`FUN_81695EC8`), quando há qualquer esqueleto. Nesse caso `FUN_81695EA0` grava a hora em 0x81A9E0A0 (o relógio de inatividade).
