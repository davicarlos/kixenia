# FUN_816FA6F0

**Endereço:** 0x816FA6F0 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor)

## O que faz
Monta uma matriz de rotação a partir de um eixo e um ângulo; usada para endireitar a inclinação do sensor no referencial do corpo.

## Chama
- Outras: FUN_816FA1E0

## É chamada por
- [FUN_81808DE0](FUN_81808DE0.md) — Referencial do corpo
- e mais 2 funções fora desta base (vizinhas): FUN_81A65968, FUN_81A66108

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Normaliza o vetor "para cima" do quadro (a normal do chão), faz o produto vetorial com a constante em 0x816507B0 (o eixo Y) e calcula o ângulo com `acos` (`FUN_8172FDA0`). Com isso monta uma rotação (`FUN_816FA6F0`) que **endireita a inclinação do sensor**. Se o produto vetorial é zero, usa a identidade. O detector do Xenia projeta na normal do chão, o que é equivalente.
