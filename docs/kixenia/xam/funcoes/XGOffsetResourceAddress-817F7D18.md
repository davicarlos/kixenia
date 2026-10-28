# XGOffsetResourceAddress

**Endereço:** 0x817F7D18 · **Ordinal:** 2257 · **Área:** [XGraphics](README.md#xgraphics)

## O que faz
Soma um deslocamento aos endereços de um recurso D3D: textura (base e, com mais de um nível, mips), buffers (+0x18) e tipo 6 (+0x20). Usada pela foto do rosto do Kinect ID.

## Chama
- Outras: FUN_817F7CB0, FUN_81834D50

## É chamada por
- [XamNuiIdentityGetColorTexture (8180DF08)](XamNuiIdentityGetColorTexture-8180DF08.md)
- e mais 5 funções fora desta base (vizinhas): FUN_81691018, FUN_816B0D98, FUN_816C8010, FUN_816CB3D0, FUN_816CCF00

## Nas notas
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### XGSetTextureHeaderEx (0x817F7C38) e XGOffsetResourceAddress (0x817F7D18)
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **XGOffsetResourceAddress:** textura soma no endereço base e, com mais de um nível, no dos mips; buffers (tipos 1, 2, 7, 8) no dword +0x18; tipo 6 no +0x20.
