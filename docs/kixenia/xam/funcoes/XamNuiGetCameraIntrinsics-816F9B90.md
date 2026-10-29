# XamNuiGetCameraIntrinsics

**Endereço:** 0x816F9B90 · **Ordinal:** 2272 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ❌ ausente

## O que faz
PsCam 46, seletor 0x100: fx, fy, cx, cy da câmera de profundidade (a matriz 3×3 do Dash.FieldCalibration); no Xenia, a focal vem do plano zero do sensor

## Fatos do decompilado
- Kernel: PsCamDeviceRequest (xboxkrnl 0x30D)

## Chama
- [PsCamDeviceRequest (81A73134)](PsCamDeviceRequest-81A73134.md) — PsCamDeviceRequest (xboxkrnl 0x30D)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2272 | 0x816F9B90 | `XamNuiGetCameraIntrinsics` | PsCam 46, seletor 0x100 | ❌ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): 4. **`XamNuiGetCameraIntrinsics`:** PsCam código 46, seletor 0x100; formato desconhecido e nenhum jogo chama. (`TrueColorInfo` feita em 6500fff7d.)
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiGetCameraIntrinsics` | nenhum dos dois | também usa o código 46, com o seletor 0x100 |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### XamNuiGetTrueColorInfo e XamNuiGetCameraIntrinsics
