# XamFitnessInitialize

**Endereço:** 0x8168F4E0 · **Ordinal:** 1228 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
0xFB 0xB0080 (síncrono)

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB0080
- Códigos de erro: 0x80070057

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1228 | XamFitnessInitialize | 0x8168F4E0 | 0xB0080 | `{ponteiro de saída}` (4 bytes), síncrono; ponteiro nulo → `0x80070057` |
- [XAM-Fitness](../xam-fitness.md): - **Inicialização** (`82F55A08`): aloca 8 blocos e chama `XamFitnessInitialize(&saida)`. Se falhar, registra "XamFitnessInitialize: 0x%x", desliga o fitness (`DAT_83B04539 = 0`) e segue. No Xenia hoje o log mostra `XamFitnessInitialize: 0x80004001`.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1228 | 0x8168F4E0 | `XamFitnessInitialize` | 0xFB 0xB0080 (síncrono) | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - rede e Live: `NetDll_WSARecv`/`WSASend`, `XamFitnessInitialize` (Live 0xB0080; ver [XAM-Fitness](../xam-fitness.md)), `XamXlfs*`;
