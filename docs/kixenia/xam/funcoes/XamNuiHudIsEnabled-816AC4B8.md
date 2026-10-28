# XamNuiHudIsEnabled

**Endereço:** 0x816AC4B8 · **Ordinal:** 2158 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** ✅ implementada

## O que faz
HUD existe, HUD+0x50 ≠ 0 e XamNuiIsDeviceReady

## Fatos do decompilado
- Globais: 0x81AA07D0

## Chama
- [XamNuiIsDeviceReady (817F9988)](XamNuiIsDeviceReady-817F9988.md)

## É chamada por
- [FUN_816C1808](FUN_816C1808.md) — NuiHudCheck
- [XamNuiHudEnableInputFilter (816C7EB8)](XamNuiHudEnableInputFilter-816C7EB8.md)
- [XamNuiIdentityAbort (8180D910)](XamNuiIdentityAbort-8180D910.md)
- [XamNuiIdentityEnrollForSignIn (8180D970)](XamNuiIdentityEnrollForSignIn-8180D970.md)
- [XamNuiIdentityIdentifyWithBiometric (8180DA50)](XamNuiIdentityIdentifyWithBiometric-8180DA50.md)
- e mais 4 funções fora desta base (vizinhas): FUN_816C6470, FUN_816E18C0, FUN_8198D088, FUN_8198D240

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): - Todas as funções de identidade recusam o pedido (`0x80004005`) se `XamNuiHudIsEnabled()` for falso.
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2158 | 0x816AC4B8 | `XamNuiHudIsEnabled` | HUD existe, HUD+0x50 ≠ 0 e XamNuiIsDeviceReady | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **No xam:** `XamShowNuiGuideUI(tracking_id, p2)` (0x816C3590) chama `FUN_816c1808`, que é o NuiHudCheck: `XamNuiHudIsEnabled`, senão 5; depois `SetEngagedTrackingID`, senão 0x65B. Em seguida chama `XamAppRequestLoadEx(4)`, que devolve 5 se falhar, e `XMsgSystemProcessCall(0xFE, 0x21030, {p1, p2, 0}, 0xC)`. O resultado passa por `FUN_816c16c8` (0, ou 0xE / 0x65B), e a função volta sem esperar o g
