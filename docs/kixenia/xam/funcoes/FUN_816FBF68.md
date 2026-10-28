# FUN_816FBF68

**Endereço:** 0x816FBF68 · **Área:** [Sensor e estado](README.md#sensor-e-estado)

## O que faz
Liga ou desliga o objeto de voz (`XamVoiceClose` chama com 0).

## Fatos do decompilado
- Kernel: EtxProducerRegister (xboxkrnl 0x334), EtxProducerUnregister (xboxkrnl 0x335), KeQuerySystemTime (xboxkrnl 0x84)
- Globais: 0x81AACBE8

## Chama
- Outras: FUN_816FBE18, FUN_816FBE88, `KeQuerySystemTime (xboxkrnl 0x84)`, `EtxProducerUnregister (xboxkrnl 0x335)`, `EtxProducerRegister (xboxkrnl 0x334)`

## É chamada por
- [XamVoiceClose (816FC1C8)](XamVoiceClose-816FC1C8.md)
- [FUN_816FC410](FUN_816FC410.md) — Microfones: reiniciar

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceClose` | 0x816FC1C8 | desliga o objeto (`FUN_816FBF68(obj, 0)`) e `XVoicedClose` (0x1E3) |
