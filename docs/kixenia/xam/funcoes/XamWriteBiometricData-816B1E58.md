# XamWriteBiometricData

**Endereço:** 0x816B1E58 · **Ordinal:** 2122 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
XMsg 0xFB (Live); tamanho 0 apaga

## Fatos do decompilado
- Mensagens XMsg: app 0xFB msg 0xB0077

## Chama
- Outras: XMsgStartIORequest (816A2F80), RtlGetLastError (81721AC0)

## É chamada por
- [XamUserNuiEnableBiometric (816DE380)](XamUserNuiEnableBiometric-816DE380.md)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamReadBiometricData / XamWriteBiometricData | 2121 / 2122 | 0x816B1D88 / 0x816B1E58 | dados biométricos via XMsg 0xFB / 0xB0076 (Live) |
- [Kinect-ID](../../kinect/kinect-id.md): - **cópia do perfil na Live**, acessada por `XamReadBiometricData` e `XamWriteBiometricData`.
- [Kinect-ID](../../kinect/kinect-id.md): | `XamWriteBiometricData(xuid, &versão, buf, &tam, ov)` | | grava na Live; o XAM manda tamanho 0 para apagar |
- [Kinect-ID](../../kinect/kinect-id.md): - **Ao cadastrar** (`82F6AD68`): lê a Live, soma 1 à versão e monta os dados novos. Depois chama `0x2A007`, `XamUserNuiEnableBiometric(usuário, 1)` e `XamWriteBiometricData`.
- [Kinect-ID](../../kinect/kinect-id.md): - `XamReadBiometricData` e `XamWriteBiometricData`: a cópia "Live" fica em `content/<xuid>/FFFE07D1/NuiBiometric.bin`.
