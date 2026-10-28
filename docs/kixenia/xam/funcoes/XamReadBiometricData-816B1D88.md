# XamReadBiometricData

**Endereço:** 0x816B1D88 · **Ordinal:** 2121 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB0076

## Chama
- Outras: XMsgStartIORequest (816A2F80), RtlGetLastError (81721AC0)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamReadBiometricData / XamWriteBiometricData | 2121 / 2122 | 0x816B1D88 / 0x816B1E58 | dados biométricos via XMsg 0xFB / 0xB0076 (Live) |
- [Kinect-ID](../../kinect/kinect-id.md): - **cópia do perfil na Live**, acessada por `XamReadBiometricData` e `XamWriteBiometricData`.
- [Kinect-ID](../../kinect/kinect-id.md): | `XamReadBiometricData(xuid, &versão, buf, &tam ≤ 0x32000, ov)` | | 0 e versão ≠ 0 = há dados |
- [Kinect-ID](../../kinect/kinect-id.md): - `XamReadBiometricData` e `XamWriteBiometricData`: a cópia "Live" fica em `content/<xuid>/FFFE07D1/NuiBiometric.bin`.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2121 | 0x816B1D88 | `XamReadBiometricData` | XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
