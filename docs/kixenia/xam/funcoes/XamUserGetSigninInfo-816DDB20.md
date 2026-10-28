# XamUserGetSigninInfo

**Endereço:** 0x816DDB20 · **Ordinal:** 551 · **Área:** [Usuários](README.md#usuários)

## O que faz
Confere um privilégio do perfil (usado pelo fitness e pela privacidade).

## Fatos do decompilado
- Kernel: RtlInitUnicodeString (xboxkrnl 0x12D), RtlUnicodeToMultiByteN (xboxkrnl 0x143)
- Globais: 0x81AA28E0, 0x81AA28E8, 0x81AA28F4, 0x81AA28FC, 0x81AA291C

## Chama
- Outras: FUN_816D8618, FUN_816D87D8, `RtlUnicodeToMultiByteN (xboxkrnl 0x143)`, `RtlInitUnicodeString (xboxkrnl 0x12D)`

## É chamada por
- e mais 4 funções fora desta base (vizinhas): FUN_816C0140, XamXlfsMountUploadQueueInstance (816F53E8), FUN_818E44C0, FUN_81A33AE8

## Nas notas
- [XAM-Rede-e-Login](../xam-rede-e-login.md): `XamUserGetSigninState` (0x210, `0x816F4768`): lê as flags do slot do usuário (`FUN_816EFD40`): bit 1 → 2 (Live), bit 2 → verifica XUID e devolve 1 (local). Sem perfil, devolve 0; `XamUserGetSigninInfo` (0x227) devolve 0x80070525 (sem usuário). O jogo consulta os 4 slots após a rede; falta ver se exige perfil (teste com `X360_XAM_RET=210:1`).
