# XamVoiceSetMicArrayIdleUsers

**Endereço:** 0x816FB9C0 · **Ordinal:** 1164 · **Área:** [Voz e microfones](README.md#voz-e-microfones) · **Xenia:** ✅ implementada

## O que faz
usuários ociosos da matriz

## Fatos do decompilado
- Kernel: KeInsertQueueDpc (xboxkrnl 0x7B)
- Globais: 0x81A85C84

## Chama
- Outras: `KeInsertQueueDpc (xboxkrnl 0x7B)`

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1164 | 0x816FB9C0 | `XamVoiceSetMicArrayIdleUsers` | usuários ociosos da matriz | ✅ |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - `XamVoiceSetMicArrayIdleUsers` (35019b0a4).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamVoiceSetMicArrayIdleUsers` (0x48C, busca dinâmica) | Kinect Sports | implementada (35019b0a4). Guarda a máscara de usuários ociosos e devolve 0 (xam 0x816FB9C0) |
