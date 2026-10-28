# XamUserNuiUnbind

**Endereço:** 0x816DAE88 · **Ordinal:** 2155 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
apaga a ligação de tipo 5

## Fatos do decompilado
- Códigos de erro: 0x80070057

## Chama
- [FUN_816D8F50](FUN_816D8F50.md) — Usuário: apagar ligação
- [XamUserNuiGetEnrollmentIndex (816DADA8)](XamUserNuiGetEnrollmentIndex-816DADA8.md)

## É chamada por
- e mais 2 funções fora desta base (vizinhas): FUN_816DCAB0, FUN_8180E330

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2155 | 0x816DAE88 | `XamUserNuiUnbind` | apaga a ligação de tipo 5 | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamUserNuiUnbind(usuário)`**: lê o índice de cadastro do usuário e, com `FUN_816D8F50(cadastro, 5, …)`, apaga a entrada de tipo 5 que aponta para ele e avisa.
