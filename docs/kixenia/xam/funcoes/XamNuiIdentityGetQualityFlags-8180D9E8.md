# XamNuiIdentityGetQualityFlags

**Endereço:** 0x8180D9E8 · **Ordinal:** 2161 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ❌ ausente

## O que faz
0xFE 0x2C008 (tarefa assíncrona)

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2C008
- Códigos de erro: 0x80004005

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamNuiHudIsEnabled (816AC4B8)](XamNuiHudIsEnabled-816AC4B8.md)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamNuiIdentityGetQualityFlags / ...Message | 2161 / 2156 | 0x8180D9E8 / 0x816F9B30 | qualidade da imagem do rosto (luz, rosto coberto) |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2161 | 0x8180D9E8 | `XamNuiIdentityGetQualityFlags` | 0xFE 0x2C008 (tarefa assíncrona) | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiIdentityGetQualityFlags(?, overlapped)` | XMsg 0xFE **0x2C008** | exige HUD |
