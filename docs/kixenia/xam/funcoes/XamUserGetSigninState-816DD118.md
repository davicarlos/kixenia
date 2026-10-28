# XamUserGetSigninState

**Endereço:** 0x816DD118 · **Ordinal:** 528 · **Área:** [Usuários](README.md#usuários)

## O que faz
Estado de login do usuário: 0 nenhum, 1 local, 2 Live. O fitness exige ≠ 0.

## Chama
- Outras: FUN_816D8470, FUN_816D8510, FUN_816D8618, FUN_816D87D8

## É chamada por
- [FUN_8168EF70](FUN_8168EF70.md) — Fitness: validar usuário da tela
- [XamFitnessClearAll (8168F0E0)](XamFitnessClearAll-8168F0E0.md)
- [XamFitnessGetCurrentBodyProfileRecord (8168F168)](XamFitnessGetCurrentBodyProfileRecord-8168F168.md)
- [XamFitnessGetAllTitleSummaries (8168F2C8)](XamFitnessGetAllTitleSummaries-8168F2C8.md)
- [XamFitnessGetAllBodyProfileRecords (8168F328)](XamFitnessGetAllBodyProfileRecords-8168F328.md)
- [XamFitnessGetOverallSummary (8168F3D8)](XamFitnessGetOverallSummary-8168F3D8.md)
- [XamFitnessGetPrivacySettings (8168F688)](XamFitnessGetPrivacySettings-8168F688.md)
- [XamFitnessAddBodyProfileRecord (8168FA00)](XamFitnessAddBodyProfileRecord-8168FA00.md)
- [XamFitnessAddFitnessEvent (8168FB40)](XamFitnessAddFitnessEvent-8168FB40.md)
- [XamFitnessCreateFitnessEventEnumerator (8168FCF0)](XamFitnessCreateFitnessEventEnumerator-8168FCF0.md)
- [FUN_816C1868](FUN_816C1868.md) — Telas: login
- [XamVoiceRecordUserPrivileges (816FBB28)](XamVoiceRecordUserPrivileges-816FBB28.md)
- e mais 33 funções fora desta base (vizinhas): FUN_816719C8, FUN_8167BEA8, FUN_8167F6E0, XamIsUserUsingMetricSystem (8168EFA8), FUN_81693150, FUN_816A3D60, FUN_816A6890, FUN_816A9BA0, FUN_816AD368, FUN_816C1718, XamShowCustomMessageComposeUI (816C2D88), XamShowPartyInviteUI (816C3220), FUN_816C7418, FUN_816DDD30, FUN_816DDDA0, FUN_816DFDD0, FUN_816E1788, FUN_816E1838, FUN_816E1A98, FUN_816E3A98, FUN_816F0A00, FUN_816F2618, FUN_816F4088, FUN_816FED70, FUN_8180FCD0, FUN_81810160, XampXAuthStartup (81810248), FUN_81831F28, FUN_8185B6B8, FUN_818C86E0 …

## Nas notas
- [XAM-Fitness](../xam-fitness.md): - **Usuário:** o índice deve ser menor que 4 (senão `0x80070057`) e o perfil precisa estar conectado (`FUN_816dd118` é o estado de login: 0 nenhum, 1 local, 2 Live; com 0 a resposta é `0x80004005`).
- [XAM-Rede-e-Login](../xam-rede-e-login.md): `XamUserGetSigninState` (0x210, `0x816F4768`): lê as flags do slot do usuário (`FUN_816EFD40`): bit 1 → 2 (Live), bit 2 → verifica XUID e devolve 1 (local). Sem perfil, devolve 0; `XamUserGetSigninInfo` (0x227) devolve 0x80070525 (sem usuário). O jogo consulta os 4 slots após a rede; falta ver se exige perfil (teste com `X360_XAM_RET=210:1`).
