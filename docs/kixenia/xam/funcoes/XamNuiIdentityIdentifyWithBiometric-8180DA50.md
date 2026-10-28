# XamNuiIdentityIdentifyWithBiometric

**Endereço:** 0x8180DA50 · **Ordinal:** 2164 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ❌ ausente

## O que faz
0xFE 0x2C007 → callback + tarefa assíncrona do xam

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2C007
- Códigos de erro: 0x80004005

## Chama
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamNuiHudIsEnabled (816AC4B8)](XamNuiHudIsEnabled-816AC4B8.md)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): Título → XamNuiIdentityIdentifyWithBiometric / EnrollForSignIn / Abort
- [Kinect-ID](../../kinect/kinect-id.md): | 0x2C006, 0x2C007, 0x2C008 | `XamNuiIdentityIdentifyWithBiometric` (0x2C007, buffer de 0x10) | identificar; antes consulta 0x2C013 |
- [Kinect-ID](../../kinect/kinect-id.md): | XamNuiIdentityIdentifyWithBiometric | 2164 | 0x8180DA50 | envia 0x2C007 com {usuário, p3, p4, 0} |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2164 | 0x8180DA50 | `XamNuiIdentityIdentifyWithBiometric` | 0xFE 0x2C007 → callback + tarefa assíncrona do xam | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiIdentityIdentifyWithBiometric(?, overlapped, a, b)` | XMsg 0xFE **0x2C007** `{?, a, b, 0}` | exige HUD |
