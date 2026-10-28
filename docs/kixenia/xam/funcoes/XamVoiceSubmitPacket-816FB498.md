# XamVoiceSubmitPacket

**Endereço:** 0x816FB498 · **Ordinal:** 782 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ⚪ stub

## O que faz
kernel XVoiced*

## Fatos do decompilado
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66), XVoicedSubmitPacket (xboxkrnl 0x1E2)

## Chama
- [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) — KeGetCurrentProcessType (xboxkrnl 0x66)
- Outras: `XVoicedSubmitPacket (xboxkrnl 0x1E2)`

## É chamada por
- e mais 2 funções fora desta base (vizinhas): FUN_81A42260, FUN_81A47618

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 782 | 0x816FB498 | `XamVoiceSubmitPacket` | kernel XVoiced* | ⚪ stub |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceSubmitPacket` | 0x816FB498 | repassa a `XVoicedSubmitPacket` (0x1E2) |
