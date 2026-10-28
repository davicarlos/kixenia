# FUN_817F6FD0

**Endereço:** 0x817F6FD0 · **Área:** [XGraphics](README.md#xgraphics)

## O que faz
Largura e altura do bloco de um formato de textura: 4×4 nos DXT, 4×1 ou 2×1 nos empacotados, 1×1 nos demais.

## É chamada por
- [FUN_817F79C0](FUN_817F79C0.md) — XGraphics: cabeçalho de textura
- [FUN_81837A98](FUN_81837A98.md) — XGraphics: alinhar nível
- [FUN_81837BB8](FUN_81837BB8.md) — XGraphics: tamanhos
- e mais 7 funções fora desta base (vizinhas): FUN_817F72E8, FUN_817F78D8, FUN_81837F38, FUN_81838090, FUN_81838660, FUN_81838B48, FUN_8183F548

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Bits por texel** por formato: tabela em 0x8160C121 (de 2 em 2 bytes). **Blocos** (`FUN_817F6FD0`): 4×4 nos DXT (0x12 a 0x14, 0x31, 0x33 a 0x35, 0x3A a 0x3D), 4×1 em 0x27/0x2C, 2×1 em 0x0B/0x0C/0x28.
