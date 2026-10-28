# XamAppRequestLoadEx

**Endereço:** 0x816CAC60 · **Ordinal:** 665 · **Área:** [Telas](README.md#telas)

## O que faz
Pede ao sistema o app de UI necessário (XamAppRequestLoadEx com o modo dado); 0 = não deu, e a tela devolve 5.

## Fatos do decompilado
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66)
- Globais: 0x81AA1820, 0x81AA20E0, 0x81AA2188, 0x81AA2198

## Chama
- [KeGetCurrentProcessType (81A721C4)](KeGetCurrentProcessType-81A721C4.md) — KeGetCurrentProcessType (xboxkrnl 0x66)

## É chamada por
- [FUN_816C1868](FUN_816C1868.md) — Telas: login
- [XamShowSigninUIEx (816C1A18)](XamShowSigninUIEx-816C1A18.md)
- [XamShowNuiGuideUI (816C3590)](XamShowNuiGuideUI-816C3590.md)
- [XamVoiceDisableMicArray (816C3750)](XamVoiceDisableMicArray-816C3750.md)
- [XamShowNuiVideoRichPresenceUI (816C3B78)](XamShowNuiVideoRichPresenceUI-816C3B78.md)
- [XamShowNuiFriendsUI (816C3CE8)](XamShowNuiFriendsUI-816C3CE8.md)
- [XamShowNuiMessagesUI (816C3DF0)](XamShowNuiMessagesUI-816C3DF0.md)
- [XamShowNuiPartyUI (816C3F00)](XamShowNuiPartyUI-816C3F00.md)
- [XamShowNuiGamerCardUIForXUID (816C4188)](XamShowNuiGamerCardUIForXUID-816C4188.md)
- [XamShowNuiAchievementsUI (816C4718)](XamShowNuiAchievementsUI-816C4718.md)
- [XamShowNuiGamesUI (816C4948)](XamShowNuiGamesUI-816C4948.md)
- [XamShowNuiGameInviteUI (816C57B8)](XamShowNuiGameInviteUI-816C57B8.md)
- [XamShowNuiFriendRequestUI (816C5900)](XamShowNuiFriendRequestUI-816C5900.md)
- [FUN_816C5A80](FUN_816C5A80.md) — Fitness: abrir tela
- [XamShowCsvTransitionUI (816C5D98)](XamShowCsvTransitionUI-816C5D98.md)
- [XamShowNuiTroubleshooterUI (816C5F68)](XamShowNuiTroubleshooterUI-816C5F68.md)
- e mais 27 funções fora desta base (vizinhas): FUN_816C1718, XamShowKeyboardUI (816C1F38), XamShowVoiceMailUI (816C2218), XamShowLiveUpsellUIEx (816C27F0), FUN_816C29F0, FUN_816C2A68, FUN_816C2AF0, XamShowVoiceSettingsUI (816C2B60), XamShowCustomMessageComposeUI (816C2D88), XamShowPartyInviteUI (816C3220), XamShowCreateProfileUIEx (816C34D8), XamShowGraduateUserUI (816C36D0), XamShowQuickLaunchUI (816C3A10), XamShowWordRegisterUI (816C3A88), XamShowVideoRichPresenceUI (816C3AE0), XamShowGamerCardUI (816C40F0), XamShowChangeGamerTileUI (816C4540), XamShowAvatarMiniCreatorUI (816C45F8), XamShowAchievementsUIEx (816C4678), XamShowAvatarAwardsUI (816C4A98), XamShowEditProfileUI (816C4B40), FUN_816C4BE0, FUN_816C51E8, XamShowMessageBoxUI (816C5508), FUN_816C6470, XamAppRequestLoad (816CAD70), FUN_816F0908

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 854 | 0x816C3590 | `XamShowNuiGuideUI` | NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030 | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **No xam:** `XamShowNuiGuideUI(tracking_id, p2)` (0x816C3590) chama `FUN_816c1808`, que é o NuiHudCheck: `XamNuiHudIsEnabled`, senão 5; depois `SetEngagedTrackingID`, senão 0x65B. Em seguida chama `XamAppRequestLoadEx(4)`, que devolve 5 se falhar, e `XMsgSystemProcessCall(0xFE, 0x21030, {p1, p2, 0}, 0xC)`. O resultado passa por `FUN_816c16c8` (0, ou 0xE / 0x65B), e a função volta sem esperar o g
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Aplicativos do sistema:** nunca foram decompilados, inclusive o comportamento de `XamShowNuiGuideUI` (o xam chama `XamAppRequestLoadEx(4)`).
