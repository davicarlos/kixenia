# XamFitnessClearBodyProfileRecords

**Endereço:** 0x81698BF8 · **Ordinal:** 1217 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
não fala com a Live: devolve 3 valores em cache (0x10 bytes)

## Fatos do decompilado
- Códigos de erro: 0x8000FFFF, 0x80070057
- Globais: 0x81A815C0, 0x81A815C4, 0x81A9F6AC, 0x81A9F6B0

## Chama
- [FUN_81698B78](FUN_81698B78.md) — Fitness: ler valores em cache
- Outras: FUN_816C88B8

## É chamada por
- [FUN_81695EC8](FUN_81695EC8.md) — Inatividade: modo de presença

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1217 | XamFitnessClearBodyProfileRecords | 0x81698BF8 | — | **não fala com a Live**: preenche uma estrutura de 0x10 bytes (`[0] == 0x10`) com 3 valores em cache (`81A815C4`, `81A9F6AC`, `81A815C0`), lidos uma vez por `FUN_81698B78`. O nome do export parece não bater com o que ela faz |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1217 | 0x81698BF8 | `XamFitnessClearBodyProfileRecords` | não fala com a Live: devolve 3 valores em cache (0x10 bytes) | ❌ |
