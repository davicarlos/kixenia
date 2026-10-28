# XamUserGetXUID

**Endereço:** 0x816D8590 · **Ordinal:** 522 · **Área:** [Usuários](README.md#usuários)

## O que faz
Devolve o XUID do usuário no tipo pedido (1 offline, 2 Live); falha se o perfil não tem esse tipo.

## Chama
- Outras: FUN_816D8510

## É chamada por
- [XamFitnessGetPrivacySettings (8168F688)](XamFitnessGetPrivacySettings-8168F688.md)
- [XamFitnessGetSuggestedPrivacySettings (8168F8C0)](XamFitnessGetSuggestedPrivacySettings-8168F8C0.md)
- [XamShowNuiFriendsUI (816C3CE8)](XamShowNuiFriendsUI-816C3CE8.md)
- [XamShowNuiMessagesUI (816C3DF0)](XamShowNuiMessagesUI-816C3DF0.md)
- [XamShowNuiPartyUI (816C3F00)](XamShowNuiPartyUI-816C3F00.md)
- [XamShowNuiCommunitySessionsUI (816C4010)](XamShowNuiCommunitySessionsUI-816C4010.md)
- [XamShowNuiGamerCardUIForXUID (816C4188)](XamShowNuiGamerCardUIForXUID-816C4188.md)
- [XamShowNuiAchievementsUI (816C4718)](XamShowNuiAchievementsUI-816C4718.md)
- [XamShowNuiGamesUI (816C4948)](XamShowNuiGamesUI-816C4948.md)
- [XamShowNuiGameInviteUI (816C57B8)](XamShowNuiGameInviteUI-816C57B8.md)
- [XamShowNuiFriendRequestUI (816C5900)](XamShowNuiFriendRequestUI-816C5900.md)
- e mais 92 funções fora desta base (vizinhas): FUN_81671048, FUN_81671140, FUN_81671A08, FUN_8167F5C0, FUN_816857F0, FUN_81685EA0, XamFormatCurrency (81689D38), XamGetCurrencyFormat (8168AB38), FUN_8168B5D0, XamIsUserUsingMetricSystem (8168EFA8), FUN_81690B10, FUN_81698128, XamNetworkStorageCheckOutTitle (816A4968), FUN_816A53E0, FUN_816A57A8, FUN_816A5AF8, FUN_816A9BA0, FUN_816BE010, FUN_816BE3C0, FUN_816BE518, FUN_816C0140, XamRequestToken (816C0438), FUN_816C2890, XamShowGamerCardUI (816C40F0), XamShowGamerCardUIForXUIDp (816C4290), XamShowChangeGamerTileUI (816C4540), XamShowAvatarMiniCreatorUI (816C45F8), XamShowAchievementsUIEx (816C4678), XamShowAvatarAwardsUI (816C4A98), XamShowEditProfileUI (816C4B40) …

## Nas notas
- [XAM-Fitness](../xam-fitness.md): - sem perfil Live (`FUN_816d8590` falha) → 0;
