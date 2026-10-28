# XamNuiGetTrueColorInfo

**Endereço:** 0x816F9B38 · **Ordinal:** 2271 · **Área:** [Sensor e estado](README.md#sensor-e-estado) · **Xenia:** 🟡 aproximada

## O que faz
PsCam 46: seletor 0 (4 bytes), 1 (tabela de autoexposição de 12 bytes por entrada)

## Fatos do decompilado
- Kernel: PsCamDeviceRequest (xboxkrnl 0x30D)

## Chama
- [PsCamDeviceRequest (81A73134)](PsCamDeviceRequest-81A73134.md) — PsCamDeviceRequest (xboxkrnl 0x30D)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2271 | 0x816F9B38 | `XamNuiGetTrueColorInfo` | PsCam 46: seletor 0 (4 bytes), 1 (tabela de autoexposição de 12 bytes por entrada) | 🟡 |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): Para o Nike e o Kinect Sports, nenhuma função do Kinect que eles chamam falta: `XamNuiGetTrueColorInfo` foi implementada em 6500fff7d com a tabela de exposição que os próprios jogos usam. O que falta serve a testes ao vivo, a outros sensores e jogos, e ao painel.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiGetTrueColorInfo` (0x8DF, busca dinâmica) | os dois | **não implementada de propósito**: é o PsCam código 46 (0x2E) com um seletor, e a resposta do driver não é conhecida. Os jogos seguem sem ela, como num painel antigo |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### XamNuiGetTrueColorInfo e XamNuiGetCameraIntrinsics
