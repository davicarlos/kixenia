# XamVoiceGetDirectionalData

**Endereço:** 0x816FB450 · **Ordinal:** 794 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ❌ ausente

## O que faz
kernel XVoicedGetDirectionalData (0x327)

## Fatos do decompilado
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66), XVoicedGetDirectionalData (xboxkrnl 0x327)

## Chama
- [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) — KeGetCurrentProcessType (xboxkrnl 0x66)
- Outras: `XVoicedGetDirectionalData (xboxkrnl 0x327)`

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_81A47380

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 794 | 0x816FB450 | `XamVoiceGetDirectionalData` | kernel XVoicedGetDirectionalData (0x327) | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceGetDirectionalData(p1, p2)` | 0x816FB450 | `KeGetCurrentProcessType` e repasse a **`XVoicedGetDirectionalData`** (kernel 0x327). A direção do som é calculada no kernel/xvoiced, não no xam |
