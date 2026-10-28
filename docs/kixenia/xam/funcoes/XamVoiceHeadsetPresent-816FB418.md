# XamVoiceHeadsetPresent

**Endereço:** 0x816FB418 · **Ordinal:** 781 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ⚪ stub

## O que faz
kernel XVoiced*

## Fatos do decompilado
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66), XVoicedHeadsetPresent (xboxkrnl 0x1E1)

## Chama
- [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) — KeGetCurrentProcessType (xboxkrnl 0x66)
- Outras: `XVoicedHeadsetPresent (xboxkrnl 0x1E1)`

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_81A41A48

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 781 | 0x816FB418 | `XamVoiceHeadsetPresent` | kernel XVoiced* | ⚪ stub |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceHeadsetPresent` | 0x816FB418 | repassa a `XVoicedHeadsetPresent` (0x1E1) |
