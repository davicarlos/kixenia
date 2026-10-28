# XamFitnessAddFitnessEvent

**Endereço:** 0x8168FB40 · **Ordinal:** 1222 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
0xFB 0xB007A: grava evento de exercício

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB007A
- Códigos de erro: 0x80004005, 0x80070057, 0x800703E5

## Chama
- [FUN_8168F068](FUN_8168F068.md) — Fitness: montar pacote
- [XMsgInProcessCall (816A2F88)](XMsgInProcessCall-816A2F88.md) — XMsg síncrono
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamUserGetSigninState (816DD118)](XamUserGetSigninState-816DD118.md) — Usuário: estado de login
- Outras: XamFree (816E6AB0)

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1222 | XamFitnessAddFitnessEvent | 0x8168FB40 | 0xB007A | pacote como no AddBodyProfileRecord + evento |
- [XAM-Fitness](../xam-fitness.md): Depois chama `XamFitnessAddFitnessEvent`.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1222 | 0x8168FB40 | `XamFitnessAddFitnessEvent` | 0xFB 0xB007A: grava evento de exercício | ❌ |
