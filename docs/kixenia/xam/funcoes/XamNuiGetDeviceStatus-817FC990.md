# XamNuiGetDeviceStatus

**Endereço:** 0x817FC990 · **Ordinal:** 2181 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ⚪ stub

## O que faz
FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D

## Fatos do decompilado
- Globais: 0x81AD12DC, 0x81AD12E0

## Chama
- [FUN_817FC750](FUN_817FC750.md) — Sensor: montar status

## É chamada por
- [XamShowNuiTroubleshooterUI (816C5F68)](XamShowNuiTroubleshooterUI-816C5F68.md)
- [FUN_816F9CB8](FUN_816F9CB8.md) — Despachante NUI (0x2B0xx)
- e mais 8 funções fora desta base (vizinhas): FUN_81692040, FUN_81694BD8, XamReportKinectSettingsChangedEvent (816B40D0), FUN_816E0680, FUN_816E06C0, FUN_816E18C0, FUN_817FCA00, FUN_8180C230

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2181 | 0x817FC990 | `XamNuiGetDeviceStatus` | FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ⚪ stub |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiGetDeviceStatus`:** o bit 1 de `status_bits` significa "matriz de microfones presente". O Nike só liga o áudio NUI com esse bit.
