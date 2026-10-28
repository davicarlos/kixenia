# XamUserNuiGetEnrollmentIndex

**Endereço:** 0x816DADA8 · **Ordinal:** 2154 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
+0x34 se +0x30, senão −2 / 0x80070525

## Fatos do decompilado
- Códigos de erro: 0x80070057, 0x80070525
- Globais: 0x81A82FF4

## É chamada por
- [XamUserNuiGetUserIndex (816DAE00)](XamUserNuiGetUserIndex-816DAE00.md)
- [XamUserNuiUnbind (816DAE88)](XamUserNuiUnbind-816DAE88.md)
- e mais 3 funções fora desta base (vizinhas): FUN_8180E330, FUN_8180EE18, FUN_8180F6D0

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamUserNuiGetEnrollmentIndex | 2154 | 0x816DADA8 | tabela de 4 cadastros × 0x40 bytes: +0x30 ativo, +0x34 índice; sem cadastro devolve `0xFFFFFFFE` e `0x80070525` |
- [Kinect-ID](../../kinect/kinect-id.md): - `XamUserNuiGetEnrollmentIndex` < 0 também faz o Nike buscar os dados no cache.
- [Kinect-ID](../../kinect/kinect-id.md): - `XamUserNuiGetEnrollmentIndex` responde como o xam quando não há cadastro (índice −2, `0x80070525`).
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2154 | 0x816DADA8 | `XamUserNuiGetEnrollmentIndex` | +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamUserNuiGetEnrollmentIndex`:** consulta uma tabela de 4 cadastros de 0x40 bytes (+0x30 ativo, +0x34 índice). Sem cadastro, devolve −2 e `0x80070525`.
