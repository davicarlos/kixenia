# XamUserNuiEnableBiometric

**Endereço:** 0x816DE380 · **Ordinal:** 2147 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2A009
- Kernel: EtxProducerRegister (xboxkrnl 0x334), EtxProducerUnregister (xboxkrnl 0x335)

## Chama
- [XamWriteBiometricData (816B1E58)](XamWriteBiometricData-816B1E58.md)
- [XamNuiIdentityGetSessionId (8180DAC8)](XamNuiIdentityGetSessionId-8180DAC8.md)
- [FUN_8180DEB0](FUN_8180DEB0.md) — Identidade: 0x2C00A
- Outras: XMsgSystemProcessCall (816A2FF8), XamUserWriteProfileSettings (816B1968), FUN_816D8510, FUN_816DB630, XMemSet (81722A30), `EtxProducerUnregister (xboxkrnl 0x335)`, `EtxProducerRegister (xboxkrnl 0x334)`

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | 0x2A009 | `XamUserNuiEnableBiometric` ao desligar | avisar que a biometria foi desligada (0x18 bytes, zerados) |
- [Kinect-ID](../../kinect/kinect-id.md): | XamUserNuiEnableBiometric | 2147 | 0x816DE380 | grava a configuração de perfil **0x1004004E**; ao desligar, apaga os dados biométricos e envia 0x2A009 |
- [Kinect-ID](../../kinect/kinect-id.md): - **Ao cadastrar** (`82F6AD68`): lê a Live, soma 1 à versão e monta os dados novos. Depois chama `0x2A007`, `XamUserNuiEnableBiometric(usuário, 1)` e `XamWriteBiometricData`.
- [Kinect-ID](../../kinect/kinect-id.md): - `XamUserNuiEnableBiometric` grava a configuração de perfil 0x1004004E; desligar apaga os dados. `XamUserNuiIsBiometricEnabled` lê essa configuração.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2147 | 0x816DE380 | `XamUserNuiEnableBiometric` | configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
