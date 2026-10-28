# XamUserNuiBind

**Endereço:** 0x816DAD28 · **Ordinal:** 2152 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
tabela de usuários: +0x34 = cadastro, +0x30 = 1

## Fatos do decompilado
- Códigos de erro: 0x80070057
- Globais: 0x81A82FF4

## Chama
- Outras: FUN_816D8C78

## É chamada por
- e mais 2 funções fora desta base (vizinhas): FUN_8180EE18, FUN_8180F6D0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2152 | 0x816DAD28 | `XamUserNuiBind` | tabela de usuários: +0x34 = cadastro, +0x30 = 1 | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamUserNuiBind(nui, usuário)`**: `nui` < 8 e usuário < 4, senão `0x80070057`. Na tabela de usuários (0x81A82FF4, 0x40 bytes cada) grava +0x34 = `nui` e +0x30 = 1 (ligado), e chama o aviso de mudança. Liga um jogador do Kinect a um perfil.
