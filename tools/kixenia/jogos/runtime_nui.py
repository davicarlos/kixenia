# Reconhece o runtime NUI embutido nos jogos: funções com a mesma "impressão" (tamanho,
# importações chamadas, mensagens XMsg) em jogos diferentes são a biblioteca da Microsoft
# que todo título Kinect leva (XamNui*, motor de inclinação, identidade). Para cada jogo
# grava <abrev>_runtime_nui.json: quais das suas funções do Kinect são do runtime (e com
# quantos jogos coincidem) e quais são código próprio.
# uso: runtime_nui.py
import collections, json, os, re, sys

D = os.path.dirname(os.path.abspath(__file__))
DEC = os.path.join(D, "..", "decompilado")
CAT = json.load(open(f"{D}/catalogo.json"))


def impressao(abrev):
    A = f"{DEC}/{abrev}"
    if not os.path.exists(f"{A}/{abrev}_fngraph.json"):
        return {}
    kin = json.load(open(f"{A}/{abrev}_kinect.json"))
    chama = collections.defaultdict(set)
    for n, v in kin.items():
        for c in v["callers"]:
            chama[c].add(n)
    pdata = json.load(open(f"{A}/{abrev}_pdata.json"))
    fp = {}
    for a in chama:
        c = f"{A}/c_full/{a}.c"
        code = open(c).read() if os.path.exists(c) else ""
        msgs = tuple(sorted(set(re.findall(r"\(\s*0x[fF][a-eA-E]\s*,\s*(0x[0-9a-fA-F]{5})", code))))
        imps = tuple(sorted(chama[a]))
        fp[a] = (pdata.get(a, 0), imps, msgs)
    return fp


todos = {j["abrev"]: impressao(j["abrev"]) for j in CAT}
todos = {k: v for k, v in todos.items() if v}
contagem = collections.Counter()
for abrev, fp in todos.items():
    for a, k in fp.items():
        contagem[k] += 1
for abrev, fp in todos.items():
    saida = {}
    n_rt = 0
    for a, k in fp.items():
        n = contagem[k]
        runtime = n >= 3 and k[0] >= 32
        n_rt += runtime
        saida[a] = {"jogos": n, "runtime": runtime, "tamanho": k[0], "imps": list(k[1]), "msgs": list(k[2])}
    json.dump(saida, open(f"{DEC}/{abrev}/{abrev}_runtime_nui.json", "w"), indent=0)
    print(f"{abrev:14} {len(fp):4} funções do Kinect, {n_rt:4} do runtime compartilhado")
