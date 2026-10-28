# XamNuiSetForceDeviceOff

**Endereço:** 0x817FA170 · **Ordinal:** 2193 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** ✅ implementada

## O que faz
bit 31 de XCONFIG_USER_RETAIL_FLAGS; evento 0xF

## Fatos do decompilado
- Globais: 0x81AD1274

## Chama
- [FUN_817F9F38](FUN_817F9F38.md) — Sensor: máquina de estados
- [FUN_817FC750](FUN_817FC750.md) — Sensor: montar status
- Outras: FUN_817F9BA8, FUN_817F9ED0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2193 | 0x817FA170 | `XamNuiSetForceDeviceOff` | bit 31 de XCONFIG_USER_RETAIL_FLAGS; evento 0xF | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamNuiSetForceDeviceOff(desligar)`**: liga ou desliga o bit **31 de `XCONFIG_USER_RETAIL_FLAGS`** (`KinectDisabled`) com ReadModifyWrite. Se já estava assim, `0xD0000184`. Depois reavalia o estado e, se desligou, manda o evento 0xF à máquina de estados.
