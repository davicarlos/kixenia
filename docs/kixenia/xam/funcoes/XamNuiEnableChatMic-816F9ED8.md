# XamNuiEnableChatMic

**Endereço:** 0x816F9ED8 · **Ordinal:** 2142 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ✅ implementada

## O que faz
bit 0x8 de XCONFIG_CONSOLE_NUI + XamVoiceMuteMicArray

## Chama
- [XamVoiceMuteMicArray (816FBA08)](XamVoiceMuteMicArray-816FBA08.md)
- Outras: FUN_816F9C38

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_817F9550

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2142 | 0x816F9ED8 | `XamNuiEnableChatMic` | bit 0x8 de XCONFIG_CONSOLE_NUI + XamVoiceMuteMicArray | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiEnableChatMic(ligar)`**: no **`XCONFIG_CONSOLE_NUI`** (categoria 7, item 9, 0x1C bytes) o **bit 3 (0x8) = microfone de chat desligado**; depois `XamVoiceMuteMicArray(!ligar, 1)`. É o mesmo campo que `XamNuiIsChatMicEnabled` lê.
