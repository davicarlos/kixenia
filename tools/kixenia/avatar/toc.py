# Interpreta o AvatarAssetPack.toc (versão 2, GUID 8A762DF4-C067-4B66-B4EE-56466B1B8280),
# como o leitor do avatarcore no xam 17559 (0x8197CA98, 0x8197CC18, 0x8197E428).
# uso: toc.py <AvatarAssetPack.toc> [--json saida.json]
import collections, json, struct, sys

V2 = bytes.fromhex("8a762df4c0674b66b4ee56466b1b8280")
ENTRY = 0xF8


def ler(caminho):
    d = open(caminho, "rb").read()
    if d[:16] != V2:
        raise SystemExit("não é a versão 2 do .toc")
    base_id = d[0x10:0x18]
    a, n, b, c = struct.unpack_from(">4I", d, 0x18)
    entradas = []
    for i in range(n):
        o = 0x28 + i * ENTRY
        e = d[o:o + ENTRY]
        flags, marca = struct.unpack_from(">2I", e, 0)
        numero = struct.unpack_from(">I", e, 0x98)[0]
        ident = e[0x9C:0xAC]
        resto = e[0xAC:0xBC]
        desl = list(struct.unpack_from(">13I", e, 0xBC))
        inicio, tamanho = struct.unpack_from(">2I", e, 0xF0)
        nome = e[0x08:0x98].split(b"\0", 1)[0]
        entradas.append(dict(i=i, flags=flags, marca=marca, numero=numero, id=ident.hex(),
                             resto=resto.hex(), desl=desl, inicio=inicio, tamanho=tamanho,
                             nome=nome.decode("latin1")))
    return d, dict(base_id=base_id.hex(), a=a, n=n, b=b, c=c), entradas


if __name__ == "__main__":
    d, cab, es = ler(sys.argv[1])
    fim_tabela = 0x28 + cab["n"] * ENTRY
    print("cabeçalho", cab, "fim da tabela", hex(fim_tabela), "arquivo", hex(len(d)))
    print("flags", collections.Counter(e["flags"] for e in es).most_common(12))
    print("marca", collections.Counter(e["marca"] for e in es).most_common(5))
    print("tipo do ID (1º byte)", collections.Counter(e["id"][:2] for e in es).most_common(8))
    print("sufixo do ID", collections.Counter(e["id"][16:] for e in es).most_common(5))
    print("com nome", sum(1 for e in es if e["nome"]), [e["nome"] for e in es if e["nome"]][:8])
    print("resto não nulo", sum(1 for e in es if int(e["resto"], 16)))
    ok = all(fim_tabela <= e["inicio"] <= len(d) and e["inicio"] + e["tamanho"] <= len(d) for e in es)
    print("início+tamanho dentro do arquivo:", ok)
    s = sorted((e["inicio"], e["tamanho"]) for e in es)
    buracos = sum(1 for (a, t), (b, _) in zip(s, s[1:]) if a + t != b and a != b)
    print("blocos contíguos? buracos/sobreposições:", buracos, "primeiro", hex(s[0][0]), "último fim", hex(s[-1][0] + s[-1][1]))
    for e in es[:3] + es[44:46]:
        print(e["i"], hex(e["flags"]), hex(e["numero"]), e["id"], [hex(x) for x in e["desl"][:4]], hex(e["inicio"]), hex(e["tamanho"]))
    if "--json" in sys.argv:
        json.dump(dict(cabecalho=cab, entradas=es), open(sys.argv[sys.argv.index("--json") + 1], "w"), indent=0)
