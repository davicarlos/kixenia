# XamFitnessCreateFitnessEventEnumerator

**Endereço:** 0x8168FCF0 · **Ordinal:** 1227 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
0xFB 0xB007F/0xB0009: enumerador de eventos (itens de 0x24)

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB0009
- Kernel: NtClose (xboxkrnl 0xCF), ObDereferenceObject (xboxkrnl 0x105)

## Chama
- [XamFitnessSystemTimeToMsgTime (8168F5E8)](XamFitnessSystemTimeToMsgTime-8168F5E8.md)
- [XamFitnessMsgTimeToSystemTime (8168FC78)](XamFitnessMsgTimeToSystemTime-8168FC78.md)
- [XMsgInProcessCall (816A2F88)](XMsgInProcessCall-816A2F88.md) — XMsg síncrono
- [XamUserGetSigninState (816DD118)](XamUserGetSigninState-816DD118.md) — Usuário: estado de login
- Outras: FUN_816E6B10, FUN_816E6B18, XamCreateEnumeratorHandle (816F0F78), XamGetPrivateEnumStructureFromHandle (816F1040), RtlSetLastNTError (81721A88), `memset` (0x8172D4F0), `memcpy` (0x8172D590), `ObDereferenceObject (xboxkrnl 0x105)`, `NtClose (xboxkrnl 0xCF)`

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1227 | XamFitnessCreateFitnessEventEnumerator | 0x8168FCF0 | 0xB007F (via 0xB000B), 0xB0009 | cria um enumerador de eventos a partir de uma consulta (abaixo) |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1227 | 0x8168FCF0 | `XamFitnessCreateFitnessEventEnumerator` | 0xFB 0xB007F/0xB0009: enumerador de eventos (itens de 0x24) | ❌ |
