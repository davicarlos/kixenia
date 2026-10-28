# XAM: pilha de rede (XNet) e estado de login
Descoberto decompilando `xam.xex` (projeto Ghidra `gxam`, disassembly completo em `/tmp/xam.dis`). Base 0x815F0000.

## Por que o jogo ficava parado na tela de aviso
Nike chama `XNetGetTitleXnAddr` (ordinal 0x49, `0x8175C540` → `0x8175AC78` → `0x81766E80`) em laço até sair de PENDING (0). O status só deixa de ser 0 quando o bit 0 de `ctx+0x1238` é ligado por `FUN_81778c90` ("endereço resolvido"), alcançada pela máquina de estados `FUN_8177a3f8(ctx, estado, tentativa)` nos estados 0, 5, 0xC, 0x16, 0x1B.

A máquina anda por timers de software (`FUN_81764260`, lista em `ctx+0x50`) que o **worker de rede** executa. O worker é um timer do kernel periódico de 200 ms com DPC: `KeSetTimerEx(ctx+0x28, -200ms, 200, dpc=ctx+8)` em `FUN_81765b88` (que também chama `NicAttach` e `NicGetOpt(0x2733)`). **O emulador ignorava a DPC de `KeSetTimerEx`**, logo o worker nunca rodava e o XNet ficava em PENDING para sempre. Correção: `Obj::Timer` guarda a DPC e `expire()` a enfileira (commit do dia 2026-10-22).

Valores de retorno de `XNetGetTitleXnAddr` (tabela real): PENDING 0, NONE 1, ETHERNET 2, STATIC 4, DHCP 8, PPPOE 0x10, GATEWAY 0x20, DNS 0x40, ONLINE 0x80, TROUBLESHOOT 0x8000. NONE só sai quando o XNet não foi iniciado. Forçar 1 com `X360_XAM_RET=49:1` destrava o jogo (truque de teste).

## Driver de rede (`Nic*`, kernel)
- `NicAttach(desc*, handle_out*)`: desc = {+0 opção, +4 tipo (20/26), +8 ctx, +0xC cb_recepção `0x817653D0`, +0x10 cb_envio `0x817654B0`, +0x14 cb_link `0x81765518`}. O cb_link(ctx) chama `FUN_8176a460(ctx,0)`, que relê `NicGetLinkState` (bit 0 = cabo ligado, mascarado por `ctx+0xC0/0xC4`) e, se mudou em relação a `ctx+0x1234`, reinicia a configuração (`FUN_8177abc0`).
- `NicGetOpt(handle, 0x2733, buf, &4)`: opção de 4 bytes lida/escrita na inicialização.
- Stubs devolvendo 0 = "sem cabo", aceitável.

## Login
`XamUserGetSigninState` (0x210, `0x816F4768`): lê as flags do slot do usuário (`FUN_816EFD40`): bit 1 → 2 (Live), bit 2 → verifica XUID e devolve 1 (local). Sem perfil, devolve 0; `XamUserGetSigninInfo` (0x227) devolve 0x80070525 (sem usuário). O jogo consulta os 4 slots após a rede; falta ver se exige perfil (teste com `X360_XAM_RET=210:1`).

## XMsgInProcessCall(0xFC, 0x58006)
App 0xFC = XLiveBase; 0x58006 = GetNatType. O Xenia devolve 0x80151802 (não logado); o XAM real devolveu 0x80070057.
