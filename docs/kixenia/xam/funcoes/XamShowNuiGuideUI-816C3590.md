# XamShowNuiGuideUI

**Endereço:** 0x816C3590 · **Ordinal:** 854 · **Área:** [Telas](README.md#telas) · **Xenia:** ✅ implementada

## O que faz
NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x21030

## Chama
- [FUN_816C16C8](FUN_816C16C8.md) — Telas: resultado do pedido de UI
- [FUN_816C1808](FUN_816C1808.md) — NuiHudCheck
- [XamAppRequestLoadEx (816CAC60)](XamAppRequestLoadEx-816CAC60.md) — Telas: carregar app do sistema
- Outras: XMsgSystemProcessCall (816A2FF8)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 854 | 0x816C3590 | `XamShowNuiGuideUI` | NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030 | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamShowNuiGuideUI` e `XamShowGuideUI` abrem o menu do botão Guide, com `XN_SYS_UI` enquanto ele está aberto.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Imports que o Nike não tem:** `XamShowNuiGuideUI` (o guia por gestos), `XamShowNuiControllerRequiredUI`, `XamShowNuiGamerCardUIForXUID` e `XamShowNuiMarketplaceUI`.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **No xam:** `XamShowNuiGuideUI(tracking_id, p2)` (0x816C3590) chama `FUN_816c1808`, que é o NuiHudCheck: `XamNuiHudIsEnabled`, senão 5; depois `SetEngagedTrackingID`, senão 0x65B. Em seguida chama `XamAppRequestLoadEx(4)`, que devolve 5 se falhar, e `XMsgSystemProcessCall(0xFE, 0x21030, {p1, p2, 0}, 0xC)`. O resultado passa por `FUN_816c16c8` (0, ou 0xE / 0x65B), e a função volta sem esperar o g
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **No Kinect Sports:** o wrapper 0x82A74D50 faz `li r4,0xFF; b XamShowNuiGuideUI`. É chamado pelo gesto de pausa e pela opção do menu do jogo (`param_2 == 4`).
