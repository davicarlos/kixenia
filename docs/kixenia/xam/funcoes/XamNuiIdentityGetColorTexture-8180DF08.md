# XamNuiIdentityGetColorTexture

**Endereço:** 0x8180DF08 · **Ordinal:** 2140 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** 🟡 aproximada

## O que faz
callback 0x2C011 → cabeçalho de textura (XGSetTextureHeaderEx)

## Fatos do decompilado
- Constantes de mensagem: 0x2C011
- Globais: 0x81AD23F0, 0x81AD23F8

## Chama
- [XGSetTextureHeaderEx (817F7C38)](XGSetTextureHeaderEx-817F7C38.md) — XGraphics: cabeçalho de textura 2D
- [XGOffsetResourceAddress (817F7D18)](XGOffsetResourceAddress-817F7D18.md) — XGraphics: deslocar endereços
- [FUN_8180E750](FUN_8180E750.md) — Identidade: chamar o callback

## Nas notas
- [Kinect-ID](../../kinect/kinect-id.md): | XamNuiIdentityGetColorTexture | 2140 | 0x8180DF08 | imagem colorida do rosto (para a interface) |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2140 | 0x8180DF08 | `XamNuiIdentityGetColorTexture` | callback 0x2C011 → cabeçalho de textura (XGSetTextureHeaderEx) | 🟡 |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiIdentityGetColorTexture(textura)` | 0x2C011 | sai largura, altura, formato e endereço; monta um cabeçalho de textura D3D (`XGSetTextureHeaderEx`, 1 nível) e aponta para a foto do rosto |
