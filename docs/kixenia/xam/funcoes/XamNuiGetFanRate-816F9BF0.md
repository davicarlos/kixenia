# XamNuiGetFanRate

**Endereço:** 0x816F9BF0 · **Ordinal:** 2273 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ❌ ausente

## O que faz
pedido > 2000 → PsCam 20, propriedade 0x838; byte 7 = ventilador

## Chama
- Outras: FUN_819838D0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2273 | 0x816F9BF0 | `XamNuiGetFanRate` | pedido > 2000 → PsCam 20, propriedade 0x838; byte 7 = ventilador | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiGetFanRate(&byte)`**: pedidos acima de 2000 vão pelo **PsCam código 20** (0x14) com o número da propriedade; a **0x838** é o **ventilador** do sensor, e o byte 7 da resposta é a rotação.
