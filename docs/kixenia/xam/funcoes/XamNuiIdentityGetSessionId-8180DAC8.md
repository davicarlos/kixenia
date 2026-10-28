# XamNuiIdentityGetSessionId

**Endereço:** 0x8180DAC8 · **Ordinal:** 2163 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
8 bytes aleatórios, criados na primeira chamada

## Fatos do decompilado
- Kernel: XeCryptRandom (xboxkrnl 0x18A)
- Globais: 0x81AD23D0

## Chama
- Outras: `XeCryptRandom (xboxkrnl 0x18A)`

## É chamada por
- [XamUserNuiEnableBiometric (816DE380)](XamUserNuiEnableBiometric-816DE380.md)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamNuiIdentityGetSessionId | 2163 | 0x8180DAC8 | id de sessão aleatório de 8 bytes, criado na primeira chamada |
- [Kinect-ID](../../kinect/kinect-id.md): - `XamNuiIdentityGetSessionId`.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2163 | 0x8180DAC8 | `XamNuiIdentityGetSessionId` | 8 bytes aleatórios, criados na primeira chamada | ✅ |
