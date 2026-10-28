# XamNuiCameraGetTiltControllerType

**Endereço:** 0x816FA088 · **Ordinal:** 2159 · **Área:** [Inclinação e motor](README.md#inclinação-e-motor) · **Xenia:** ⚪ stub

## O que faz
UsbdGetNatalHardwareVersion (kernel 0x373): 1 = Detroit (1414), 2 = Mca (1473), 0 = nenhum

## Fatos do decompilado
- Kernel: UsbdGetNatalHardwareVersion (xboxkrnl 0x373)

## Chama
- [UsbdGetNatalHardwareVersion (81A73144)](UsbdGetNatalHardwareVersion-81A73144.md) — UsbdGetNatalHardwareVersion (xboxkrnl 0x373)

## É chamada por
- [XamNuiCameraAdjustTilt (816FA338)](XamNuiCameraAdjustTilt-816FA338.md)
- [XamNuiCameraElevationAutoTilt (816FA3B8)](XamNuiCameraElevationAutoTilt-816FA3B8.md)
- [XamNuiCameraElevationReverseAutoTilt (816FA418)](XamNuiCameraElevationReverseAutoTilt-816FA418.md)
- [XamNuiCameraElevationStopMovement (816FA520)](XamNuiCameraElevationStopMovement-816FA520.md)
- [XamNuiCameraRememberFloor (816FA618)](XamNuiCameraRememberFloor-816FA618.md)
- [FUN_8180C700](FUN_8180C700.md) — Motor: inclinar
- [FUN_8180C850](FUN_8180C850.md) — Motor: ler status
- e mais 6 funções fora desta base (vizinhas): FUN_8180CB48, FUN_8180CD08, FUN_8180D1B0, FUN_81983490, FUN_819835B0, FUN_81983970

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): `XamNuiCameraGetTiltControllerType` = `UsbdGetNatalHardwareVersion`: **1 = Detroit** (hub com motor, modelo 1414, `045e:02b0`), **2 = Mca** (motor pelo firmware de áudio, modelo 1473), 0 = sem motor (`0x8007048F`).
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2159 | 0x816FA088 | `XamNuiCameraGetTiltControllerType` | UsbdGetNatalHardwareVersion (kernel 0x373): 1 = Detroit (1414), 2 = Mca (1473), 0 = nenhum | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### Inclinação (todas exigem `XamNuiCameraGetTiltControllerType` ≠ 0, senão `0x8007048F`)
