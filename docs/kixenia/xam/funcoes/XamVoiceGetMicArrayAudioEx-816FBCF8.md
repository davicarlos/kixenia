# XamVoiceGetMicArrayAudioEx

**Endereço:** 0x816FBCF8 · **Ordinal:** 1168 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ✅ implementada

## O que faz
pede ao driver de áudio a próxima janela de 16 ms da matriz de microfones: o pedido (tamanho em +0, 5 ponteiros de buffer em +4 e 5 tamanhos em +0x18: mix processado de 16 bits e 4 microfones de 32 bits) vai ao McaDeviceRequest com o KEVENT; o tic da captura só é escrito em +0x40 se o tamanho passa de 0x40, e a hora em +0x44/+0x48 só se passa de 0x64 (o Sonic Free Riders usa pedidos de 0x44 bytes; escrever a hora neles corrompia o pedido seguinte)

## Fatos do decompilado
- Códigos de erro: 0x80004005
- Kernel: KeWaitForSingleObject (xboxkrnl 0xB0), McaDeviceRequest (xboxkrnl 0x30E)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1168 | 0x816FBCF8 | `XamVoiceGetMicArrayAudioEx` | kernel XVoiced* | 🟡 |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamVoiceGetMicArrayAudioEx(pedido de 0x68 bytes, KEVENT)`:**
