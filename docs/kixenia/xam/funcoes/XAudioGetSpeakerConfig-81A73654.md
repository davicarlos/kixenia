# XAudioGetSpeakerConfig

**Endereço:** 0x81A73654 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `XAudioGetSpeakerConfig (xboxkrnl 0x1FF)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: XAudioGetSpeakerConfig (xboxkrnl 0x1FF)

## É chamada por
- [XamNatalDeviceAudioCalibrate (817F8DE8)](XamNatalDeviceAudioCalibrate-817F8DE8.md)

## Nas notas
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): | `0x81A73654` | XAudioGetSpeakerConfig (0x1FF): o bit 31 escolhe a calibração de baixa latência |
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): 2. Escolhe o arquivo pelo bit 31 de um valor do kernel (`0x81A73654`):
