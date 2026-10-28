# XamFitnessAddBodyProfileRecord

**Endereço:** 0x8168FA00 · **Ordinal:** 1210 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
0xFB 0xB0079: grava registro do corpo (pacote + verificação síncrona)

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB0079
- Códigos de erro: 0x80004005, 0x80070057, 0x800703E5

## Chama
- [FUN_8168F068](FUN_8168F068.md) — Fitness: montar pacote
- [XMsgInProcessCall (816A2F88)](XMsgInProcessCall-816A2F88.md) — XMsg síncrono
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamUserGetSigninState (816DD118)](XamUserGetSigninState-816DD118.md) — Usuário: estado de login
- Outras: XamFree (816E6AB0)

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1210 | XamFitnessAddBodyProfileRecord | 0x8168FA00 | 0xB0079 | pacote `{usuário, handle, tamanho, 1}` + registro do corpo |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1210 | 0x8168FA00 | `XamFitnessAddBodyProfileRecord` | 0xFB 0xB0079: grava registro do corpo (pacote + verificação síncrona) | ❌ |
