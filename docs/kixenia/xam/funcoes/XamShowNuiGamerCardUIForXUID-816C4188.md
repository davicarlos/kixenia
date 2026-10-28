# XamShowNuiGamerCardUIForXUID

**Endereço:** 0x816C4188 · **Ordinal:** 1140 · **Área:** [Telas](README.md#telas) · **Xenia:** 🟡 aproximada

## O que faz
NuiHudCheck + 0x21032 (Live)

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x21032

## Chama
- [FUN_816C16C8](FUN_816C16C8.md) — Telas: resultado do pedido de UI
- [FUN_816C1808](FUN_816C1808.md) — NuiHudCheck
- [XamAppRequestLoadEx (816CAC60)](XamAppRequestLoadEx-816CAC60.md) — Telas: carregar app do sistema
- [XamUserGetXUID (816D8590)](XamUserGetXUID-816D8590.md) — Usuário: XUID
- Outras: XMsgSystemProcessCall (816A2FF8), XamUserIsGuest (816DAD00), `memset` (0x8172D4F0)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1140 | 0x816C4188 | `XamShowNuiGamerCardUIForXUID` | NuiHudCheck + 0x21032 (Live) | 🟡 |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Imports que o Nike não tem:** `XamShowNuiGuideUI` (o guia por gestos), `XamShowNuiControllerRequiredUI`, `XamShowNuiGamerCardUIForXUID` e `XamShowNuiMarketplaceUI`.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `XamShowNuiGamerCardUIForXUID` (os dois).
