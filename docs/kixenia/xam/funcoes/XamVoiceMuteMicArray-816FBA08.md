# XamVoiceMuteMicArray

**Endereço:** 0x816FBA08 · **Ordinal:** 1165 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ❌ ausente

## O que faz
kernel XVoiced*

## Fatos do decompilado
- Kernel: KeQuerySystemTime (xboxkrnl 0x84)
- Globais: 0x81AACA50, 0x81AACBE8

## Chama
- Outras: FUN_81983D98, `KeQuerySystemTime (xboxkrnl 0x84)`

## É chamada por
- [XamNuiEnableChatMic (816F9ED8)](XamNuiEnableChatMic-816F9ED8.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2142 | 0x816F9ED8 | `XamNuiEnableChatMic` | bit 0x8 de XCONFIG_CONSOLE_NUI + XamVoiceMuteMicArray | ✅ |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1165 | 0x816FBA08 | `XamVoiceMuteMicArray` | kernel XVoiced* | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiEnableChatMic(ligar)`**: no **`XCONFIG_CONSOLE_NUI`** (categoria 7, item 9, 0x1C bytes) o **bit 3 (0x8) = microfone de chat desligado**; depois `XamVoiceMuteMicArray(!ligar, 1)`. É o mesmo campo que `XamNuiIsChatMicEnabled` lê.
