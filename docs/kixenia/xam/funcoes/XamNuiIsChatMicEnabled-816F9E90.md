# XamNuiIsChatMicEnabled

**Endereço:** 0x816F9E90 · **Ordinal:** 2143 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ✅ implementada

## O que faz
!(bit 0x8 de XCONFIG_CONSOLE_NUI)

## Chama
- Outras: FUN_816B2BA8

## É chamada por
- e mais 2 funções fora desta base (vizinhas): XamReportKinectSettingsChangedEvent (816B40D0), FUN_817F9550

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2143 | 0x816F9E90 | `XamNuiIsChatMicEnabled` | !(bit 0x8 de XCONFIG_CONSOLE_NUI) | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiEnableChatMic(ligar)`**: no **`XCONFIG_CONSOLE_NUI`** (categoria 7, item 9, 0x1C bytes) o **bit 3 (0x8) = microfone de chat desligado**; depois `XamVoiceMuteMicArray(!ligar, 1)`. É o mesmo campo que `XamNuiIsChatMicEnabled` lê.
