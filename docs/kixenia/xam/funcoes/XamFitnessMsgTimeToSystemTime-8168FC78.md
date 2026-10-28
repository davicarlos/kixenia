# XamFitnessMsgTimeToSystemTime

**Endereço:** 0x8168FC78 · **Ordinal:** 1242 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
segundos desde 2003-01-01 → SYSTEMTIME

## Fatos do decompilado
- Códigos de erro: 0x80070057

## Chama
- Outras: FileTimeToSystemTime (8171C3E8)

## É chamada por
- [XamFitnessCreateFitnessEventEnumerator (8168FCF0)](XamFitnessCreateFitnessEventEnumerator-8168FCF0.md)
- e mais 7 funções fora desta base (vizinhas): FUN_8193D538, FUN_8193D658, FUN_8193D810, FUN_8193DB08, FUN_8193DC48, FUN_8193F238, FUN_8193F800

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1242 | XamFitnessMsgTimeToSystemTime | 0x8168FC78 | — | `FILETIME = (MsgTime + 0x2F422C080) × 10⁷`, depois `FileTimeToSystemTime` |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1242 | 0x8168FC78 | `XamFitnessMsgTimeToSystemTime` | segundos desde 2003-01-01 → SYSTEMTIME | ❌ |
