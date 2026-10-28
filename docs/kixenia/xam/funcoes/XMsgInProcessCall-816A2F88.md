# XMsgInProcessCall

**Endereço:** 0x816A2F88 · **Ordinal:** 500 · **Área:** [Sensor e estado](README.md#sensor-e-estado)

## O que faz
Envia uma mensagem a um app do xam e espera a resposta (usado nas verificações do fitness antes do envio de verdade).

## Fatos do decompilado
- Códigos de erro: 0x80070057

## Chama
- Outras: FUN_816A2B90, FUN_816B8138, FUN_816BC968

## É chamada por
- [XamFitnessAddBodyProfileRecord (8168FA00)](XamFitnessAddBodyProfileRecord-8168FA00.md)
- [XamFitnessAddFitnessEvent (8168FB40)](XamFitnessAddFitnessEvent-8168FB40.md)
- [XamFitnessCreateFitnessEventEnumerator (8168FCF0)](XamFitnessCreateFitnessEventEnumerator-8168FCF0.md)
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- e mais 122 funções fora desta base (vizinhas): XamUserCreateAvatarAssetEnumerator (81670040), XamUserCreateAchievementEnumerator (81670190), XamUserCreateTitlesPlayedEnumerator (81670460), FUN_81671DA8, XamAvatarGetAssetsResultSize (81674A70), XamAvatarSetCustomAsset (81675038), FUN_816753F8, FUN_81675460, FUN_816754B0, FUN_81675540, FUN_816826E0, FUN_8168C2F8, FUN_8168C6D8, FUN_8168C7E0, XamGetCurrentSystemOnlineManifestRevision (8168C9D0), FUN_81698128, FUN_81699EE8, FUN_81699FD8, FUN_816A35D0, FUN_816A3D60, FUN_816A41E0, XamPartyGetUserList (816AFB78), XamPartyGetUserListInternal (816AFBB8), XamPartySetCustomData (816AFD28), XamPartyGetBandwidth (816AFD68), XamPartyGetState (816AFDE8), XamPartyGetFormation (816AFE78), FUN_816AFF68, XamPartyGetAccessLevel (816B0020), XamUserAddRecentPlayer (816B0858) …

## Nas notas
- [XAM-Fitness](../xam-fitness.md): - primeiro uma chamada síncrona de verificação `FUN_816a2f88(0xFB, msg, pacote, 0)`; depois zera o flag e faz o envio de verdade;
