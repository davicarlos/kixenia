# XamCacheFetchFile

**Endereço:** 0x816793F0 · **Ordinal:** 691 · **Área:** [Kinect ID](README.md#kinect-id)

## O que faz
Lê um item do armazém de cache do xam (o armazém 0x18 guarda os dados biométricos de cada perfil): devolve a versão (64 bits) e os dados até o tamanho pedido. Usado pela checagem de data dos rostos (ordinal 2174).

## Chama
- Outras: FUN_81679340

## É chamada por
- e mais 11 funções fora desta base (vizinhas): FUN_81689C00, FUN_816908B0, FUN_816A3AF8, FUN_816F5F18, FUN_816FE8D0, FUN_81733A98, FUN_818D8998, FUN_818DF810, FUN_818E06E8, FUN_818F4E18, FUN_8191D108

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - Decompilada com a função criada: aloca 0x32000 bytes, lê os **dados biométricos do perfil** do armazém 0x18 (`FUN_816793F0`), confere até 0x66 registros e compara a data de cada um com a data atual ajustada em meses. Responde se o cadastro está velho.
