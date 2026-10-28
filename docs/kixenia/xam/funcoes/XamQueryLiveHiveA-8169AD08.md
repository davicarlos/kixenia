# XamQueryLiveHiveA

**Endereço:** 0x8169AD08 · **Ordinal:** 1804 · **Área:** [Kinect ID](README.md#kinect-id)

## O que faz
Lê uma configuração de texto por nome (usada pela op 2 da 0x2C00C antes de cair na tabela de padrões).

## Fatos do decompilado
- Códigos de erro: 0x8000000A
- Kernel: KeEnterCriticalRegion (xboxkrnl 0x5F), KeLeaveCriticalRegion (xboxkrnl 0x7D)

## Chama
- Outras: FUN_81699FD8, FUN_8169AA48, `KeLeaveCriticalRegion (xboxkrnl 0x7D)`, `KeEnterCriticalRegion (xboxkrnl 0x5F)`

## É chamada por
- [FUN_8180DB88](FUN_8180DB88.md) — Identidade: parâmetro por nome
- e mais 27 funções fora desta base (vizinhas): FUN_8168E6E0, FUN_8168E7D0, XamGetLiveHiveValueA (8169AF50), XamNetworkStorageIsEnabledInLiveHive (816A4588), FUN_816C7138, FUN_816E2898, XamXlfsMountUploadQueueInstance (816F53E8), FUN_816F5898, FUN_816F5918, FUN_816F8950, FUN_816FDB28, FUN_816FE620, FUN_816FEC58, FUN_818593A0, FUN_818DBB10, FUN_818E00B0, FUN_8190EF08, FUN_8191F600, FUN_81969B88, FUN_81988D80, FUN_8198F528, FUN_8198F740, FUN_8198FA98, FUN_81996500, FUN_8199F1F0, FUN_81A33B68, FUN_81A6CCC8

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - O nome precisa começar com "Nui" (0x8160DBC4) e ter menos de 0x14 caracteres. O valor é procurado num armazenamento de configurações (`FUN_8169AD08`; armazém 0x1A em `FUN_81679460`/`FUN_81679438`). Se não achar, usa a tabela de padrões em 0x8160DB98 (5 entradas).
