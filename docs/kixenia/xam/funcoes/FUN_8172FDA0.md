# FUN_8172FDA0

**Endereço:** 0x8172FDA0 · **Área:** [Sensor e estado](README.md#sensor-e-estado)

## O que faz
Arco cosseno (biblioteca matemática do xam).

## Chama
- Outras: FUN_8172FCA8

## É chamada por
- [XamNuiCameraTiltGetStatus (816FAA60)](XamNuiCameraTiltGetStatus-816FAA60.md)
- [FUN_81808DE0](FUN_81808DE0.md) — Referencial do corpo
- [FUN_81809820](FUN_81809820.md) — Dobra do cotovelo
- [FUN_818098E0](FUN_818098E0.md) — Gesto do guia: referencial
- [FUN_81809AF0](FUN_81809AF0.md) — Gesto do guia: pose
- [FUN_8198B390](FUN_8198B390.md) — Pontuar esqueleto
- e mais 1 funções fora desta base (vizinhas): FUN_81A66108

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): Normaliza o vetor "para cima" do quadro (a normal do chão), faz o produto vetorial com a constante em 0x816507B0 (o eixo Y) e calcula o ângulo com `acos` (`FUN_8172FDA0`). Com isso monta uma rotação (`FUN_816FA6F0`) que **endireita a inclinação do sensor**. Se o produto vetorial é zero, usa a identidade. O detector do Xenia projeta na normal do chão, o que é equivalente.
