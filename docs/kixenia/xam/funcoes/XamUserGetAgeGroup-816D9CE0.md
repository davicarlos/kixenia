# XamUserGetAgeGroup

**Endereço:** 0x816D9CE0 · **Ordinal:** 1189 · **Área:** [Usuários](README.md#usuários)

## O que faz
Consulta uma característica do perfil usada por `XamFitnessGetSuggestedPrivacySettings` para escolher 3 ou 4.

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x2300E

## Chama
- Outras: XMsgStartIORequest (816A2F80), RtlGetLastError (81721AC0)

## É chamada por
- [XamFitnessGetSuggestedPrivacySettings (8168F8C0)](XamFitnessGetSuggestedPrivacySettings-8168F8C0.md)

## Nas notas
- [XAM-Fitness](../xam-fitness.md): - se a região do perfil (configuração 6) não for a de comparação em 0x815F4400 (5 bytes), ou se a leitura falhar, consulta `FUN_816d9ce0` e responde 3 ou 4.
