# Índice dos jogos Kinect no vault (Kinect/Jogos-Kinect-Indice.md) e a nota de pendências
# (Kinect/Jogos-Kinect-Pendencias.md): o que os jogos pedem do sistema e ainda falta ou é
# stub/aproximado no Xenia, com quantos jogos pedem cada coisa e quantas vezes chamam.
# uso: gen_indice.py
import collections, json, os, re, sys

D = os.path.dirname(os.path.abspath(__file__))
DEC = os.path.join(D, "..", "decompilado")
sys.path.insert(0, os.path.expanduser("~/Kixenia/kinect-sdk-re/vault"))
from vaultlib import JOGOS, KINECT, gerado
sys.path.insert(0, D)
from gen_jogo import CAT, PROPRIAS, nome_limpo, arquivo_nota
GFM, GAV = gerado("kinect-sdk-re/games/jogos/gen_indice.py")
ST = {"implementada": "✅", "aproximada": "🟡", "stub": "⚪ stub", "sem marca": "❔", "ausente": "❌"}

jogos = []
for j in CAT:
    a = j["abrev"]
    A = f"{DEC}/{a}"
    if not os.path.exists(f"{A}/{a}_runtime.json"):
        continue
    rt = json.load(open(f"{A}/{a}_runtime.json"))
    kin = json.load(open(f"{A}/{a}_kinect.json"))
    imps = json.load(open(f"{A}/{a}_imports.json"))
    nota = PROPRIAS.get(j["titulo"]) or arquivo_nota(nome_limpo(j["pasta"]))
    jogos.append({"abrev": a, "nome": nome_limpo(j["pasta"]), "titulo": j["titulo"], "tipo": j["tipo"],
                  "nota": nota[:-3], "imps": len(imps), "kin": kin, "rt": rt,
                  "sensor": sum(bool(re.search(r"Nui|PsCam|Mca|Natal|Biometric|MicArray|XAudioCapture|MECClient", n)) for n in kin)})
jogos.sort(key=lambda j: (not j["sensor"], j["nome"].lower()))

L = ["---", "tags: [jogo, kinect, indice]", "data: 2026-10-29", GFM, "---",
     "# Jogos Kinect: índice", "", GAV, "",
     f"{len(jogos)} jogos do HD ({sum(bool(j['sensor']) for j in jogos)} com importações do sensor), cada um com a sua nota (análise do executável pelo "
     "`jogos/pipeline.py` e, quando houve, a bateria ao vivo). As colunas: importações do sistema, das quais "
     "as do Kinect/NUI/voz/avatar; quantas dessas faltam ou são stub no Xenia; e a bateria (✅ rodou, 💥 travou, — não rodou).", "",
     "Relacionado: [[Jogos-Kinect-Pendencias]], [[Jogos-Kinect-Comparacao]], [[Kinect-Xenia-Edge]].", "",
     "| Jogo | Título | Formato | Sensor | Importações | Kinect/avatar/voz | Faltam/stub | Bateria | Rastreado |", "|---|---|---|---|---|---|---|---|---|"]
for j in jogos:
    rt = j["rt"]
    bat = "—" if not rt.get("log") else ("💥" if rt.get("marcas", {}).get("CRASH DUMP") else "✅")
    ras = f"{rt['rastreadas']}/{rt['amostras']}" if rt.get("log") else ""
    falt = sum(v["xenia"] in ("ausente", "stub") for v in j["kin"].values())
    L.append(f"| [[{j['nota']}|{j['nome']}]] | {j['titulo']} | {j['tipo'].upper()} | {'✅' if j['sensor'] else '—'} | {j['imps']} | {len(j['kin'])} | "
             f"{falt} | {bat} | {ras} |")
L.append("")
open(f"{KINECT}/Jogos-Kinect-Indice.md", "w").write("\n".join(L))
print("índice:", len(jogos), "jogos")

# Pendências: por importação, quem pede e quantas vezes chamou.
pend = collections.defaultdict(lambda: {"jogos": [], "chamadas": 0, "sites": 0, "xenia": "", "lib": "", "ordinal": 0})
for j in jogos:
    for n, v in j["kin"].items():
        if v["xenia"] in ("ausente", "stub", "aproximada", "sem marca"):
            p = pend[n]
            p["jogos"].append(j["nota"]); p["chamadas"] += j["rt"].get("calls", {}).get(n, 0); p["sites"] += v["sites"]
            p["xenia"], p["lib"], p["ordinal"] = v["xenia"], v["lib"], v["ordinal"]
L = ["---", "tags: [jogo, kinect, pendencias]", "data: 2026-10-29", GFM, "---",
     "# Jogos Kinect: pendências no Xenia", "", GAV, "",
     "O que os jogos Kinect importam do sistema e o Xenia ainda não tem (❌), tem só como stub (⚪) ou "
     "aproximado (🟡), ordenado por quantos jogos pedem. 'Em execução' soma as chamadas vistas nos logs das "
     "baterias ao vivo (nível 3; as de alta frequência o Xenia não registra).", "",
     "Relacionado: [[Jogos-Kinect-Indice]], [[Kinect-Xenia-Edge]], [[Kinect-Mapa-XAM]].", ""]
for estado, titulo in (("ausente", "Ausentes"), ("stub", "Stubs"), ("aproximada", "Aproximadas"), ("sem marca", "Sem marca")):
    itens = sorted(((n, p) for n, p in pend.items() if p["xenia"] == estado),
                   key=lambda x: (-len(x[1]["jogos"]), -x[1]["chamadas"], x[0]))
    if not itens:
        continue
    L += [f"## {titulo} ({len(itens)})", "", "| Importação | Biblioteca, ordinal | Jogos | Chamadas no código | Em execução | Quem pede |", "|---|---|---|---|---|---|"]
    for n, p in itens:
        quem = ", ".join(f"[[{q}]]" for q in p["jogos"][:6]) + (f" e mais {len(p['jogos']) - 6}" if len(p["jogos"]) > 6 else "")
        L.append(f"| `{n}` | {p['lib']} 0x{p['ordinal']:X} | {len(p['jogos'])} | {p['sites']} | {p['chamadas']} | {quem} |")
    L.append("")
crash = [j for j in jogos if j["rt"].get("marcas", {}).get("CRASH DUMP")]
if crash:
    L += ["## Travaram na bateria ao vivo", ""] + [f"- [[{j['nota']}|{j['nome']}]]" for j in crash] + [""]
open(f"{KINECT}/Jogos-Kinect-Pendencias.md", "w").write("\n".join(L))
print("pendências:", len(pend), "importações")
