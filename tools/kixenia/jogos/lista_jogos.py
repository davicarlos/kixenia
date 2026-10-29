# Catálogo dos jogos do HD externo: para cada pasta, o alvo que o Xenia abre (default.xex,
# cabeçalho GoD em Content/…/00007000/<id> ou pacote XBLA em <título>/000D0000/<hash>), o
# título (do caminho ou do cabeçalho XEX, opcional 0x40006) e uma abreviação para os dados.
# uso: lista_jogos.py <pasta do HD> <saída .json>
import json, os, re, struct, sys, unicodedata

HD, OUT = sys.argv[1:3]


def titulo_xex(p):
    d = open(p, "rb").read(0x10000)
    if d[:4] != b"XEX2":
        return None
    n = struct.unpack_from(">I", d, 0x14)[0]
    for k in range(n):
        key, val = struct.unpack_from(">II", d, 0x18 + 8 * k)
        if key == 0x40006:
            return "%08X" % struct.unpack_from(">I", d, val + 12)[0]
    return None


def abrev(nome):
    t = unicodedata.normalize("NFKD", nome).encode("ascii", "ignore").decode().lower()
    t = re.sub(r"\[.*?\]|\(.*?\)|-ghost.*|for kinect|kinect", " ", t)
    palavras = [w for w in re.split(r"[^a-z0-9]+", t) if w and w not in ("the", "of", "and", "for", "a")]
    return "".join(w[:4] for w in palavras[:3]) or "jogo"


def alvo(pasta):
    """(tipo, caminho) do que lançar: 'xex', 'god' ou 'xbla'; o default.xex mais raso vence."""
    xex, god, xbla = [], [], []
    for raiz, dirs, arqs in os.walk(pasta):
        prof = raiz[len(pasta):].count(os.sep)
        if prof > 6:
            dirs[:] = []
            continue
        for a in arqs:
            p = os.path.join(raiz, a)
            if a.lower() == "default.xex":
                xex.append((prof, p))
            elif re.search(r"/00007000$", raiz) and re.fullmatch(r"[0-9A-F]{20}", a):
                god.append((prof, p))
            elif re.search(r"/000D0000$", raiz) and re.fullmatch(r"[0-9A-F]{40,42}", a):
                xbla.append((prof, p))
    for tipo, lst in (("xex", xex), ("god", god), ("xbla", xbla)):
        if lst:
            return tipo, sorted(lst)[0][1]
    return None, None


jogos = []
for nome in sorted(os.listdir(HD)):
    pasta = os.path.join(HD, nome)
    if not os.path.isdir(pasta):
        continue
    tipo, p = alvo(pasta)
    if not tipo:
        continue
    if tipo == "xex":
        tid = titulo_xex(p)
    else:
        m = re.search(r"/([0-9A-F]{8})/(?:00007000|000D0000)/", p)
        tid = m[1] if m else None
    jogos.append({"pasta": nome, "abrev": abrev(nome), "tipo": tipo, "alvo": p, "titulo": tid})

# Abreviações únicas.
vistos = {}
for j in jogos:
    a = j["abrev"]
    vistos[a] = vistos.get(a, 0) + 1
    if vistos[a] > 1:
        j["abrev"] = f"{a}{vistos[a]}"
json.dump(jogos, open(OUT, "w"), indent=1, ensure_ascii=False)
for j in jogos:
    print(f'{j["abrev"]:14} {j["tipo"]:4} {j["titulo"] or "????????"} {j["pasta"]}')
print(len(jogos), "jogos")
