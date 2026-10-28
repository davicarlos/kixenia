# Interpreta os blocos STRB ("Structured Binary") dos assets de avatar, como o leitor do
# avatarcore no xam 17559 (0x8196DBE8 abre, 0x8196DF68/0x8196E0E8 andam pelos blocos).
# uso: strb.py <AvatarAssetPack.toc> <toc.json>   (valida todos e mostra os tipos de bloco)
import collections, json, struct, sys

FORMATO = bytes.fromhex("59bde08b49094ee4b991ada87b7c0b6b")


def alinhar(v, a):
    return (v + a - 1) & ~(a - 1)


def cabecalho(b):
    if b[:4] != b"STRB":
        raise ValueError("sem STRB")
    versao, flag = b[4], b[5]
    if versao != 1:
        raise ValueError(f"versão {versao}")
    guid = b[6:0x16]
    # Larguras do tipo e do tamanho de cada bloco (+0x16/+0x17 do leitor) e o alinhamento.
    w_tipo, w_tam, alinh = b[0x16], b[0x17], b[0x1A]
    dados = alinhar(0x1D, alinh)
    return dict(versao=versao, flag=flag, guid=guid, w_tipo=w_tipo, w_tam=w_tam,
                alinhamento=alinh, dados=dados, bytes=b[0x16:0x1E].hex())


def blocos(b, inicio, fim, alinh, w_tipo=1, w_tam=4):
    """Blocos de [inicio, fim): tipo (w_tipo bytes), tamanho e um terceiro campo (w_tam
    bytes cada), big-endian; o corpo começa alinhado depois do cabeçalho."""
    o = inicio
    cab = w_tipo + 2 * w_tam
    while o < fim:
        if o + cab > fim:
            raise ValueError(f"cabeçalho de bloco cortado em {o:#x}")
        tipo = int.from_bytes(b[o:o + w_tipo], "big")
        tam = int.from_bytes(b[o + w_tipo:o + w_tipo + w_tam], "big")
        extra = int.from_bytes(b[o + w_tipo + w_tam:o + cab], "big")
        corpo = o + alinhar(cab, alinh)
        if corpo + tam > fim:
            raise ValueError(f"bloco em {o:#x} passa do fim ({tam:#x})")
        yield dict(o=o, tipo=tipo, tamanho=tam, extra=extra, corpo=corpo)
        o = corpo + alinhar(tam, alinh)


if __name__ == "__main__":
    d = open(sys.argv[1], "rb").read()
    t = json.load(open(sys.argv[2]))
    tipos, extras, falhas, guids, sequencias = (collections.Counter() for _ in range(5))
    for e in t["entradas"]:
        if not e["tamanho"]:
            continue
        b = d[e["inicio"]:e["inicio"] + e["tamanho"]]
        try:
            c = cabecalho(b)
            guids[c["guid"].hex()] += 1
            bs = list(blocos(b, c["dados"], len(b), c["alinhamento"], c["w_tipo"], c["w_tam"]))
            for x in bs:
                tipos[x["tipo"]] += 1
                extras[(x["tipo"], x["extra"])] += 1
            sequencias[tuple(x["tipo"] for x in bs)] += 1
        except ValueError as erro:
            falhas[str(erro)[:40]] += 1
    print("assets com dados:", sum(1 for e in t["entradas"] if e["tamanho"]), "falhas:", sum(falhas.values()), falhas.most_common(5))
    print("GUID do formato:", guids.most_common(3))
    print("tipos de bloco (nível 1):", sorted(tipos.items()))
    print("tipo/extra:", sorted(extras.items())[:20])
    print("sequências mais comuns:", sequencias.most_common(8))
