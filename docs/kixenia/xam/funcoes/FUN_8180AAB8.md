# FUN_8180AAB8

**Endereço:** 0x8180AAB8 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos)

## O que faz
O detector de aceno de uma mão: espera 1 s, conta meias-ondas com mão erguida e ritmo estável e devolve 1 ao completar 4.

## Fatos do decompilado
- Globais: 0x815F46E8, 0x81AD2200, 0x81AD221C

## Chama
- [FUN_8180A5C0](FUN_8180A5C0.md) — Aceno: meia-onda
- [FUN_8180A6F8](FUN_8180A6F8.md) — Aceno: ritmo
- [FUN_8180A788](FUN_8180A788.md) — Aceno: mão erguida
- [FUN_8180A900](FUN_8180A900.md) — Aceno: checagem da mão
- [FUN_8180A988](FUN_8180A988.md) — Aceno: direção
- Outras: FUN_8180A518

## É chamada por
- [FUN_818092C8](FUN_818092C8.md) — Aceno: duas mãos

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### Aceno de engajamento (`FUN_818092C8` → `FUN_8180AAB8`, por mão)
