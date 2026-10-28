# Lista os blocos STRB de uma entrada do AvatarAssetPack.toc, com o deslocamento
# absoluto do corpo de cada um no arquivo (o que o avlzx recebe).
# Tipos: 3 = modelo e 2 = texturas (comprimidos), 8 = descritor, 7 = paleta, 4 = animação.
# uso: blocos.py <AvatarAssetPack.toc> <toc.json> <índice da entrada>
import json, sys

from strb import blocos, cabecalho

d = open(sys.argv[1], "rb").read()
e = json.load(open(sys.argv[2]))["entradas"][int(sys.argv[3])]
if not e["tamanho"]:
    sys.exit("entrada sem dados")
b = d[e["inicio"]:e["inicio"] + e["tamanho"]]
c = cabecalho(b)
nome = (e.get("nome") or "").strip()
print(f"entrada {e['i']} {nome!r}: {e['tamanho']:#x} bytes em {e['inicio']:#x}")
for x in blocos(b, c["dados"], len(b), c["alinhamento"], c["w_tipo"], c["w_tam"]):
    print(f"  tipo {x['tipo']}  deslocamento {e['inicio'] + x['corpo']:#x}  tamanho {x['tamanho']:#x}")
