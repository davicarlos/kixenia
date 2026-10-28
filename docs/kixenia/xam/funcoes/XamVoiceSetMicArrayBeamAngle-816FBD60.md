# XamVoiceSetMicArrayBeamAngle

**Endereço:** 0x816FBD60 · **Ordinal:** 1187 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ✅ implementada

## O que faz
CEMD 5 ao sensor

## Fatos do decompilado
- Códigos de erro: 0x80070057
- Globais: 0x815FC24C

## Chama
- [FUN_81983E98](FUN_81983E98.md) — CEMD: enviar

## Nas notas
- [Kinect-SDK-Windows](../../kinect/kinect-sdk-windows.md): - **Feixe e fonte de som:** de **−50° a +50°**. É o mesmo leque dos 11 feixes da matriz `Lin4el` e a provável faixa de `XamVoiceSetMicArrayBeamAngle`.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1187 | 0x816FBD60 | `XamVoiceSetMicArrayBeamAngle` | CEMD 5 ao sensor | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamVoiceSetMicArrayBeamAngle` envia o CEMD 5 ao sensor; `kinect_audio_calibration_path` e `XamNatalDeviceAudioCalibrate` enviam a calibração da matriz (CEMD 0). Ver [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md).
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): 1. **Canal processado do microfone (canal 1):** quase nunca chega; o firmware `audios.bin` pode mostrar o comando que o liga. Junto, o processamento de áudio do `KinectAudio10_x64.dll` e `XamVoiceSetMicArrayBeamAngle`, que o xam envia ao firmware como pacote CEMD 5.
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): | `XamVoiceSetMicArrayBeamAngle(float)` (0x816FBD60) | `{CEMD, 0x05000000 (id 5 em big-endian), ângulo como float little-endian}`, 0x10 bytes | ângulo **em radianos** entre **−0,873 e +0,873 (±50°)** (0x815FC248 / 0x815FC244) ou o valor especial **FLT_MAX** (0x815FC24C, provavelmente feixe automático); senão `0x80070057` |
