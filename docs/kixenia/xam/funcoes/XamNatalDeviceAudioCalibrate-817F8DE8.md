# XamNatalDeviceAudioCalibrate

**Endereço:** 0x817F8DE8 · **Ordinal:** 2137 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ✅ implementada

## O que faz
envia MicArrayCalibrate.mec ao sensor (CEMD 0)

## Fatos do decompilado
- Kernel: NtClose (xboxkrnl 0xCF), XAudioGetSpeakerConfig (xboxkrnl 0x1FF)

## Chama
- [FUN_81983268](FUN_81983268.md) — CEMD: início
- [FUN_819832F0](FUN_819832F0.md) — CEMD: bloco
- [FUN_81983380](FUN_81983380.md) — CEMD: fim
- [XAudioGetSpeakerConfig (81A73654)](XAudioGetSpeakerConfig-81A73654.md) — XAudioGetSpeakerConfig (xboxkrnl 0x1FF)
- Outras: XamContentCreateInternal (816EE478), XamContentClose (816EEB78), `memset` (0x8172D4F0), FUN_8172E278, FUN_81984468, FUN_81984538, `NtClose (xboxkrnl 0xCF)`

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_817F9550

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2137 | 0x817F8DE8 | `XamNatalDeviceAudioCalibrate` | envia MicArrayCalibrate.mec ao sensor (CEMD 0) | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamVoiceSetMicArrayBeamAngle` envia o CEMD 5 ao sensor; `kinect_audio_calibration_path` e `XamNatalDeviceAudioCalibrate` enviam a calibração da matriz (CEMD 0). Ver [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md).
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): | `XamNatalDeviceAudioCalibrate` (0x817F8DE8) | calibração inteira, via `FUN_81983268` (início), `FUN_819832F0` (blocos de 0x4000) e `FUN_81983380` (fim) | ver abaixo |
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): `XamNatalDeviceAudioCalibrate`:
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): - **`kinect_audio_calibration_path`:** caminho de um `MicArrayCalibrate.data`, que deve ter exatamente 0x64014 bytes e começar com 'CEMD'. É enviado como CEMD 0 quando o dispositivo de áudio abre (`SetMotor`) e por `XamNatalDeviceAudioCalibrate`; sem o arquivo, a resposta é `0x8AC80005`, como no xam.
