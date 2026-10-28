# XamNuiIdentityGetQualityFlagsMessage

**Endereço:** 0x816F9B30 · **Ordinal:** 2156 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ❌ ausente

## O que faz
PsCam 40 com dois u32

## Fatos do decompilado
- Códigos de erro: 0x80070057
- Kernel: PsCamDeviceRequest (xboxkrnl 0x30D)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamNuiIdentityGetQualityFlags / ...Message | 2161 / 2156 | 0x8180D9E8 / 0x816F9B30 | qualidade da imagem do rosto (luz, rosto coberto) |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2156 | 0x816F9B30 | `XamNuiIdentityGetQualityFlagsMessage` | PsCam 40 com dois u32 | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiIdentityGetQualityFlagsMessage(p)` | **PsCam código 40** (0x28) | manda os dois u32 de `p` ao sensor |
