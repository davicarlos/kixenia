# FUN_8177ABC0

Endereço dentro de uma função da pilha de rede do xam (citado em [XAM-Rede-e-Login](../xam-rede-e-login.md)). Fica dentro de `FUN_8177ABB0`.

## Nas notas
- [XAM-Rede-e-Login](../xam-rede-e-login.md): - `NicAttach(desc*, handle_out*)`: desc = {+0 opção, +4 tipo (20/26), +8 ctx, +0xC cb_recepção `0x817653D0`, +0x10 cb_envio `0x817654B0`, +0x14 cb_link `0x81765518`}. O cb_link(ctx) chama `FUN_8176a460(ctx,0)`, que relê `NicGetLinkState` (bit 0 = cabo ligado, mascarado por `ctx+0xC0/0xC4`) e, se mudou em relação a `ctx+0x1234`, reinicia a configuração (`FUN_8177abc0`).
