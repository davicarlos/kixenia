# XamVoiceGetMicArrayAudio

**Endereço:** 0x816FBCA0 · **Ordinal:** 988 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ✅ implementada

## O que faz
forma antiga da `XamVoiceGetMicArrayAudioEx`: recebe um handle de evento em vez do KEVENT e preenche +0x2C..+0x3C do pedido com 0, 5, 5, 5, 5 antes de passar ao mesmo trabalhador

## Fatos do decompilado
- Kernel: ObReferenceObjectByHandle (xboxkrnl 0x110)
- Globais: 0x815F067C

## Chama
- Outras: FUN_81983128, `ObReferenceObjectByHandle (xboxkrnl 0x110)`

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 988 | 0x816FBCA0 | `XamVoiceGetMicArrayAudio` | kernel XVoiced* | ❌ |
