# XamCacheFetchFileByString

**Endereço:** 0x81679460 · **Ordinal:** 1151 · **Área:** [Kinect ID](README.md#kinect-id)

## O que faz
Lê um item, por nome, num armazém de cache (0x1A, parâmetros de identidade da op 2 da 0x2C00C).

## Chama
- Outras: FUN_81679398

## É chamada por
- [FUN_8180DB88](FUN_8180DB88.md) — Identidade: parâmetro por nome
- e mais 1 funções fora desta base (vizinhas): FUN_8191D108

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - O nome precisa começar com "Nui" (0x8160DBC4) e ter menos de 0x14 caracteres. O valor é procurado num armazenamento de configurações (`FUN_8169AD08`; armazém 0x1A em `FUN_81679460`/`FUN_81679438`). Se não achar, usa a tabela de padrões em 0x8160DB98 (5 entradas).
