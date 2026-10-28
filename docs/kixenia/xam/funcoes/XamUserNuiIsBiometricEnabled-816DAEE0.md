# XamUserNuiIsBiometricEnabled

**Endereço:** 0x816DAEE0 · **Ordinal:** 2146 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
lê 0x1004004E

## Chama
- Outras: XamUserReadProfileSettings (816B1F28), XamAlloc (816E67A0), XamFree (816E6AB0)

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamUserNuiIsBiometricEnabled | 2146 | 0x816DAEE0 | lê essa configuração (perfil do painel, título 0xFFFE07D1) |
- [Kinect-ID](../../kinect/kinect-id.md): - `XamUserNuiEnableBiometric` grava a configuração de perfil 0x1004004E; desligar apaga os dados. `XamUserNuiIsBiometricEnabled` lê essa configuração.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2146 | 0x816DAEE0 | `XamUserNuiIsBiometricEnabled` | lê 0x1004004E | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamUserNuiEnableBiometric` grava a configuração 0x1004004E; desligar apaga os dados. `XamUserNuiIsBiometricEnabled` lê essa configuração.
