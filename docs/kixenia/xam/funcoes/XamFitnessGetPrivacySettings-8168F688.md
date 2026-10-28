# XamFitnessGetPrivacySettings

**Endereço:** 0x8168F688 · **Ordinal:** 1275 · **Área:** [Fitness](README.md#fitness) · **Xenia:** ❌ ausente

## O que faz
configuração de perfil 0x10040000 + privilégio 6 → valor 0 a 6

## Fatos do decompilado
- Códigos de erro: 0x80004005, 0x8007000E, 0x80070057

## Chama
- [XamUserGetXUID (816D8590)](XamUserGetXUID-816D8590.md) — Usuário: XUID
- [XamUserGetSigninState (816DD118)](XamUserGetSigninState-816DD118.md) — Usuário: estado de login
- Outras: XamUserReadProfileSettings (816B1F28), XamUserReadUserPreference (816B1F68), FUN_816E6B10, FUN_816E6B18

## É chamada por
- e mais 1 funções fora desta base (vizinhas): FUN_81934A20

## Nas notas
- [XAM-Fitness](../xam-fitness.md): | 1275 | XamFitnessGetPrivacySettings | 0x8168F688 | — | ver abaixo |
- [XAM-Fitness](../xam-fitness.md): - **XamFitnessGetPrivacySettings(usuário, &valor):**
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1275 | 0x8168F688 | `XamFitnessGetPrivacySettings` | configuração de perfil 0x10040000 + privilégio 6 → valor 0 a 6 | ❌ |
