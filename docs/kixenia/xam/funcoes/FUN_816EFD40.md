# FUN_816EFD40

**Endereço:** 0x816EFD40 · **Área:** [Usuários](README.md#usuários)

## O que faz
Lê as flags do slot do usuário (bit 1 = Live, bit 2 = local); base de `XamUserGetSigninState`.

## Nas notas
- [XAM-Rede-e-Login](../xam-rede-e-login.md): `XamUserGetSigninState` (0x210, `0x816F4768`): lê as flags do slot do usuário (`FUN_816EFD40`): bit 1 → 2 (Live), bit 2 → verifica XUID e devolve 1 (local). Sem perfil, devolve 0; `XamUserGetSigninInfo` (0x227) devolve 0x80070525 (sem usuário). O jogo consulta os 4 slots após a rede; falta ver se exige perfil (teste com `X360_XAM_RET=210:1`).
