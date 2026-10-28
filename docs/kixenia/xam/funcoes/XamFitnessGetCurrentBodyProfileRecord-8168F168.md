# XamFitnessGetCurrentBodyProfileRecord

**Endereço:** 0x8168F168 · **Ordinal:** 1218 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
0xFB 0xB007B (síncrono)

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB007B
- Códigos de erro: 0x80004005, 0x80070057

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamUserGetSigninState (816DD118)](XamUserGetSigninState-816DD118.md) — Usuário: estado de login

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_8193F460

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1218 | XamFitnessGetCurrentBodyProfileRecord | 0x8168F168 | 0xB007B | `{usuário, buffer, u16 tamanho}` (0xC bytes), síncrono |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1218 | 0x8168F168 | `XamFitnessGetCurrentBodyProfileRecord` | 0xFB 0xB007B (síncrono) | ❌ |
