# Ferramentas de avatar

Descrição dos formatos em [avatares/](../../../docs/kixenia/avatares/README.md).

| Arquivo | O que faz |
|---|---|
| `toc.py` | Lê o `AvatarAssetPack.toc` (tabela de 1282 entradas) e salva a lista em JSON |
| `strb.py` | Valida os arquivos STRB de todas as entradas e conta os tipos de bloco |
| `blocos.py` | Lista os blocos de uma entrada, com o deslocamento de cada um no arquivo |
| `lzx/avlzx.c` | Descomprime um bloco (LZX em quadros de até 32 KB, contexto reiniciado a cada quadro) |
| `malha.py` | Decodifica um bloco de modelo descomprimido: malhas, vértices quantizados, índices, texturas e costuras; exporta `.obj` |
| `render_obj.py` | Renderiza um `.obj` em três vistas num `.png` (só precisa do Pillow) |

## Passo a passo

1. Pegue o `AvatarAssetPack` da pasta de um jogo Kinect que o traga e extraia o `AvatarAssetPack.toc` de dentro dele (é um pacote PIRS; o `xenia-vfs-dump` do Xenia extrai).
2. Liste as entradas:
   ```
   python3 toc.py AvatarAssetPack.toc --json toc.json
   python3 strb.py AvatarAssetPack.toc toc.json
   ```
3. Compile o descompressor. Ele usa o `lzxd.c` do libmspack (LGPL-2.1), que o Xenia traz em `third_party/mspack`:
   ```
   M=<checkout do Xenia>/third_party/mspack
   gcc -O2 -w -I$M -o avlzx lzx/avlzx.c lzx/log.c $M/lzxd.c
   ```
4. Veja os blocos de uma entrada (por exemplo a 44, um par de sapatos), descomprima o bloco de modelo (tipo 3) com o deslocamento e o tamanho mostrados e gere a imagem:
   ```
   python3 blocos.py AvatarAssetPack.toc toc.json 44
   ./avlzx AvatarAssetPack.toc <deslocamento> <tamanho> modelo.bin
   python3 malha.py modelo.bin modelo.obj
   python3 render_obj.py modelo.obj modelo.png
   ```
