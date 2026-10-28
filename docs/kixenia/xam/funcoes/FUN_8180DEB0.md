# FUN_8180DEB0

**Endereço:** 0x8180DEB0 · **Área:** [Kinect ID](README.md#kinect-id)

## O que faz
Pergunta ao callback de identidade (mensagem 0x2C00A) um valor de 4 bytes; 0 em caso de erro.

## Fatos do decompilado
- Constantes de mensagem: 0x2C00A
- Globais: 0x81AD23F0, 0x81AD23F8

## Chama
- [FUN_8180E750](FUN_8180E750.md) — Identidade: chamar o callback

## É chamada por
- [XamUserNuiEnableBiometric (816DE380)](XamUserNuiEnableBiometric-816DE380.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): **0x2C0xx (identidade)**: 0x2C006 a 0x2C008 identificar (antes consulta 0x2C013); 0x2C009 registra o callback; 0x2C00C dados de identidade; 0x2C00D resolve o cadastro; 0x2C00E cancelar; 0x2C010 cadastrar para login; 0x2C011 foto do rosto; 0x2C012 apagar cadastro; 0x2C013 dados do cadastro; **0x2C00A** pergunta um valor de 4 bytes ao callback (`FUN_8180DEB0`). O despachante das 0x2C0xx não foi loca
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - Mensagem nova: **0x2C00A** (`FUN_8180DEB0`) pergunta ao callback de identidade e devolve um valor de 4 bytes, ou 0 em caso de erro.
