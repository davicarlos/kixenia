# XGSetTextureHeaderEx

**Endereço:** 0x817F7C38 · **Ordinal:** 2262 · **Área:** [XGraphics](README.md#xgraphics)

## O que faz
Cabeçalho de uma textura 2D a partir de largura, altura, níveis, uso, formato, exp bias, flags (1 = mips não empacotados), deslocamentos e pitch; devolve o tamanho. Chama o montador do D3DResource.

## Chama
- [FUN_817F79C0](FUN_817F79C0.md) — XGraphics: cabeçalho de textura

## É chamada por
- [XamNuiIdentityGetColorTexture (8180DF08)](XamNuiIdentityGetColorTexture-8180DF08.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2140 | 0x8180DF08 | `XamNuiIdentityGetColorTexture` | callback 0x2C011 → cabeçalho de textura (XGSetTextureHeaderEx) | 🟡 |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): 5. **Kinect ID do painel:** o callback da 0x2C009 já é guardado e chamado pelas 0x2C012/0x2C013 (4251108a5). Falta repassar 0x2C006 a 0x2C011 e montar a textura do rosto (`XGSetTextureHeaderEx`). Também manter a tabela de 4 cadastros, para o login pelo rosto e respostas reais na 0x2C00D.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): | `XamNuiIdentityGetColorTexture(textura)` | 0x2C011 | sai largura, altura, formato e endereço; monta um cabeçalho de textura D3D (`XGSetTextureHeaderEx`, 1 nível) e aponta para a foto do rosto |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Ficaram de fora:** `GetColorTexture` (o Xenia não tem `XGSetTextureHeaderEx`), `IdentifyWithBiometric` e `GetQualityFlags` (app do sistema), e as três que dependem do sensor (PsCam 20, 40 e 44).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### XGSetTextureHeaderEx (0x817F7C38) e XGOffsetResourceAddress (0x817F7D18)
