# XamVoiceClose

**Endereço:** 0x816FC1C8 · **Ordinal:** 783 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ⚪ stub

## O que faz
voz (kernel XVoiced*)

## Fatos do decompilado
- Kernel: XVoicedClose (xboxkrnl 0x1E3)

## Chama
- [FUN_816FBF68](FUN_816FBF68.md) — Voz: ligar/desligar objeto
- Outras: FUN_816E6B10, `XVoicedClose (xboxkrnl 0x1E3)`

## É chamada por
- [FUN_816FC410](FUN_816FC410.md) — Microfones: reiniciar
- e mais 3 funções fora desta base (vizinhas): FUN_81A42058, FUN_81A425B0, FUN_81A426A0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 783 | 0x816FC1C8 | `XamVoiceClose` | voz (kernel XVoiced*) | ⚪ stub |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceClose` | 0x816FC1C8 | desliga o objeto (`FUN_816FBF68(obj, 0)`) e `XVoicedClose` (0x1E3) |
