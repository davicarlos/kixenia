# XamNuiIdentityGetEnrollmentInfo

**Endereço:** 0x8180E048 · **Ordinal:** 2138 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ✅ implementada

## O que faz
callback 0x2C013 → dois u32

## Fatos do decompilado
- Constantes de mensagem: 0x2C013
- Códigos de erro: 0x8000FFFF
- Globais: 0x81AD23F0, 0x81AD23F8

## Chama
- [FUN_8180E750](FUN_8180E750.md) — Identidade: chamar o callback

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | 0x2C013 | `XamNuiIdentityGetEnrollmentInfo` | dados do cadastro (resposta de 0x10 bytes; devolve 2 dwords) |
- [Kinect-ID](../../kinect/kinect-id.md): | XamNuiIdentityGetEnrollmentInfo | 2138 | 0x8180E048 | 0x2C013 → dois dwords |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2138 | 0x8180E048 | `XamNuiIdentityGetEnrollmentInfo` | callback 0x2C013 → dois u32 | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiIdentityGetEnrollmentInfo(cadastro, &a, &b)` | 0x2C013 | entra `{0x10, cadastro}`, sai dois u32 |
