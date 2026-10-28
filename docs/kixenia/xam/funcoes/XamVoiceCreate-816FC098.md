# XamVoiceCreate

**Endereço:** 0x816FC098 · **Ordinal:** 780 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ⚪ stub

## O que faz
voz (kernel XVoiced*)

## Fatos do decompilado
- Códigos de erro: 0x8007000E
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66), KeQuerySystemTime (xboxkrnl 0x84)
- Globais: 0x81AACA28, 0x81AACA2C, 0x81AACA30, 0x81AACA34, 0x81AACA38, 0x81AACA3C, 0x81AACA40, 0x81AACA44, 0x81AACA48, 0x81AACA50, 0x81AACA58

## Chama
- [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) — KeGetCurrentProcessType (xboxkrnl 0x66)
- Outras: FUN_816E6B18, `memset` (0x8172D4F0), `KeQuerySystemTime (xboxkrnl 0x84)`

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_81A426A0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 780 | 0x816FC098 | `XamVoiceCreate` | voz (kernel XVoiced*) | ⚪ stub |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceCreate(?, n, &obj)` | 0x816FC098 | um objeto de 0x38 bytes por usuário (tabela em 0x81AACA28) com uma fila de `n` entradas de 0x10 bytes |
