# Nota de um jogo no vault a partir dos dados do pipeline.py (games/decompilado/<abrev>/):
# identificação, estado no Xenia (bateria ao vivo), funções do jogo que falam com o Kinect,
# importações do Kinect e todas as importações por área, mais o texto escrito à mão em
# jogos/textos/<abrev>.py (TEXTO, FUNCOES, GRUPOS, NOME), quando existe.
# uso: gen_jogo.py <abrev> [...]   (ou "todos": todos os jogos do catálogo com dados prontos)
import collections, importlib.util, json, os, re, sys, unicodedata

D = os.path.dirname(os.path.abspath(__file__))
DEC = os.path.join(D, "..", "decompilado")
sys.path.insert(0, os.path.expanduser("~/Kixenia/kinect-sdk-re/vault"))
sys.path.insert(0, os.path.join(DEC, "scripts"))
from vaultlib import JOGOS, gerado, slug
sys.path.insert(0, D)
from descrever import descrever
_src = open(f"{DEC}/scripts/groups.py").read()
exec(_src[_src.index("RULES="):_src.index("def cat")])
GFM, GAV = gerado("kinect-sdk-re/games/jogos/gen_jogo.py", "kinect-sdk-re/games/jogos/textos/<abrev>.py")
ST = {"implementada": "✅", "aproximada": "🟡", "stub": "⚪ stub", "sem marca": "❔", "ausente": "❌"}
CAT = json.load(open(f"{D}/catalogo.json"))
# Jogos com nota própria (gerador dedicado): o gen_jogo.py não mexe nelas.
PROPRIAS = {}
for f in os.listdir(JOGOS):
    s = open(f"{JOGOS}/{f}").read(600)
    m = re.search(r"^titulo: ([0-9A-F]{8})", s, re.M)
    if m and "gen_jogo.py" not in s:
        PROPRIAS[m[1]] = f


def nome_limpo(pasta):
    n = re.sub(r"\[.*?\]|\(.*?\)|-GHOST.*|_", " ", pasta)
    n = re.sub(r"\s+", " ", n).strip()
    return n


def arquivo_nota(nome):
    t = unicodedata.normalize("NFKD", nome).encode("ascii", "ignore").decode()
    partes = [p for p in re.split(r"[^A-Za-z0-9]+", t) if p]
    return "-".join(p[0].upper() + p[1:] for p in partes) + ".md"


def area(nome):
    for cat, rx in RULES:
        if re.search(rx, nome):
            return cat
    return "Outras"


def gerar(abrev):
    j = next((x for x in CAT if x["abrev"] == abrev), None)
    A = f"{DEC}/{abrev}"
    if not j or not os.path.exists(f"{A}/{abrev}_fngraph.json"):
        print(abrev, "sem dados"); return
    sem_kinect = not any(re.search(r"Nui|PsCam|Mca|Natal|Biometric|MicArray|XAudioCapture|MECClient", n)
                         for n in json.load(open(f"{A}/{abrev}_kinect.json")))
    if j["titulo"] in PROPRIAS:
        print(abrev, "tem nota própria:", PROPRIAS[j["titulo"]]); return
    imps = json.load(open(f"{A}/{abrev}_imports.json"))
    kin = json.load(open(f"{A}/{abrev}_kinect.json"))
    graph = {x["addr"]: x for x in json.load(open(f"{A}/{abrev}_fngraph.json"))}
    pdata = json.load(open(f"{A}/{abrev}_pdata.json"))
    secs = json.load(open(f"{A}/{abrev}_sections.json"))
    rtj = json.load(open(f"{A}/{abrev}_runtime.json"))
    rt = rtj["calls"]
    info = json.load(open(f"{A}/{abrev}.info.json"))
    tx = {"TEXTO": "", "FUNCOES": {}, "GRUPOS": [], "NOME": None, "RELACIONADO": []}
    tp = f"{D}/textos/{abrev}.py"
    if os.path.exists(tp):
        spec = importlib.util.spec_from_file_location("tx", tp)
        m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
        for k in tx:
            if hasattr(m, k):
                tx[k] = getattr(m, k)
    nome = tx["NOME"] or nome_limpo(j["pasta"])
    titulo = j["titulo"] or "????????"
    chama = collections.defaultdict(list)
    for n, v in kin.items():
        for c in v["callers"]:
            chama[c].append(n)
    fnset = sorted({c for v in kin.values() for c in v["callers"]}, key=lambda a: int(a, 16))

    def grupo(a):
        v = int(a, 16)
        for g, lo, hi in tx["GRUPOS"]:
            if lo <= v < hi:
                return g
        return "Funções do jogo que falam com o Kinect"

    L = []
    L.append(f"# {nome} (título {titulo})")
    L.append("")
    L.append(GAV)
    L.append("")
    if sem_kinect:
        L.append("> [!note] Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações "
                 "de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.")
        L.append("")
    tipo = {"xex": "`default.xex` solto", "god": "pacote GoD (cabeçalho em `Content/…/00007000/`)",
            "xbla": "pacote XBLA (`000D0000/`)"}[j["tipo"]]
    L.append(f"**No HD:** `{j['pasta']}` ({tipo}). **Executável:** {info['image_size'] // 1024} KB de imagem, "
             f"base 0x{info['base']:08X}, entrada 0x{info['entry']:08X}, {len(pdata)} funções no `.pdata`"
             + (f", `.text` {secs['.text'][0]}–0x{int(secs['.text'][0], 16) + secs['.text'][1]:08X}" if ".text" in secs else "") + ".")
    L.append("")
    if tx["TEXTO"]:
        L.append(tx["TEXTO"].strip())
        L.append("")
    rel = ["Jogos-Kinect-Comparacao", "Kinect-Xenia-Edge", "Kinect-Mapa-XAM"] + list(tx["RELACIONADO"])
    L.append("Relacionado: " + ", ".join(f"[[{r}]]" for r in dict.fromkeys(rel)) + ".")
    L.append("")
    # Estado no Xenia.
    L.append("## Estado no Xenia Edge")
    L.append("")
    marcas = rtj.get("marcas", {})
    if rtj.get("log"):
        L.append(f"> [!info] Bateria ao vivo (Kinect real, log nível 3, `{os.path.basename(rtj['log'])}`): "
                 f"{'**travou** (crash dump no log)' if marcas.get('CRASH DUMP') else 'rodou o tempo todo sem travar'}; "
                 f"{rtj['rastreadas']} de {rtj['amostras']} amostras com jogador rastreado.")
        L.append("")
        linhas = []
        lg = rtj.get("linhas", {})
        if lg.get("Title name"):
            linhas.append(f"| Nome no XEX | | {lg['Title name']} |")
        if marcas.get("NUI: title runtime"):
            linhas.append(f"| Runtime NUI | ✅ | iniciado: `{lg.get('NUI: title runtime', '')}` |")
        if marcas.get("the title is reading the microphones"):
            linhas.append("| Microfones | ✅ | o jogo lê a matriz de microfones (`XamVoiceGetMicArrayAudioEx`) |")
        if marcas.get("identity engine"):
            linhas.append("| Kinect ID | ✅ | motor de identidade iniciado |")
        if marcas.get("XamShowNuiSigninUI("):
            linhas.append(f"| Login pelo Kinect | 🟡 | `XamShowNuiSigninUI` chamado {marcas['XamShowNuiSigninUI(']}× (login automático desde 47e873396) |")
        if marcas.get("Avatar:"):
            linhas.append(f"| Avatares | 🟡 | {marcas['Avatar:']} mensagens `Avatar:` no log |")
        if marcas.get("no room"):
            linhas.append(f"| Avatares: espaço das peças | ⚠️ | {marcas['no room']} vezes \"no room for the component\" |")
        if marcas.get("copied out of its image"):
            linhas.append("| Arquivos do pacote | ✅ | abre arquivos de dentro da imagem (Kinect ID, bancos do rastreador) |")
        if marcas.get("Invalid instruction"):
            linhas.append(f"| CPU | ⚠️ | {marcas['Invalid instruction']} `Invalid instruction` (VMX128 não traduzida) |")
        if marcas.get("no contiguous range"):
            linhas.append(f"| Memória física | ⚠️ | {marcas['no contiguous range']} falhas de alocação contígua |")
        if marcas.get("stream: "):
            linhas.append(f"| Câmera USB | ℹ️ | {marcas['stream: ']} relatórios de quadros perdidos |")
        if linhas:
            L += ["| Etapa | Situação | Detalhe |", "|---|---|---|"] + linhas + [""]
    else:
        L.append("> [!info] Ainda não rodado na bateria ao vivo; só a análise estática.")
        L.append("")
    # Como usa o Kinect, por área, separando código próprio e runtime.
    rn0 = json.load(open(f"{A}/{abrev}_runtime_nui.json")) if os.path.exists(f"{A}/{abrev}_runtime_nui.json") else {}
    AREAS = [("Câmera e sensor", r"PsCam|XamNuiGet|XamNuiIs|XamNuiCamera(Get|Set|Remember)|Natal|XamNuiHudGet"),
             ("Inclinação e motor", r"Elevation|Tilt|AutoTilt|AdjustTilt"),
             ("HUD, esqueleto e gestos", r"NuiHud(?!Get)|NuiSkeleton|NuiPlayer|NuiHand"),
             ("Kinect ID e biometria", r"NuiIdentity|Biometric|UserNui"),
             ("Voz e microfones", r"Voice|MicArray|XAudioCapture|MECClient"),
             ("Telas Nui do sistema", r"ShowNui"),
             ("Câmera Xbox Live Vision", r"XUsbcam"),
             ("Fitness", r"Fitness"), ("Avatares", r"Avatar"),
             ("Mensagens aos apps do sistema (XMsg)", r"^XMsg"), ("Cache do xam", r"XamCache")]
    L.append("## Como o jogo usa o Kinect")
    L.append("")
    if sem_kinect:
        L.append("Não usa o sensor. O que importa do mesmo grupo de funções:")
        L.append("")
    usados = set()
    for nome_area, rx in AREAS:
        ns = sorted(n for n in kin if re.search(rx, n) and n not in usados)
        if not ns:
            continue
        usados |= set(ns)
        fs = {c for n in ns for c in kin[n]["callers"]}
        prop = [f for f in fs if not rn0.get(f, {}).get("runtime")]
        rtc = len(fs) - len(prop)
        exec_ = sum(rt.get(n, 0) for n in ns)
        falt = [n for n in ns if kin[n]["xenia"] in ("ausente", "stub")]
        L.append(f"- **{nome_area}:** {len(ns)} importações ({', '.join('`' + n + '`' for n in ns[:8])}"
                 + (f" e mais {len(ns) - 8}" if len(ns) > 8 else "") + f"); {len(prop)} funções próprias"
                 + (f" ({', '.join(a.upper().replace('0X', '0x') for a in sorted(prop)[:4])}" + (" …" if len(prop) > 4 else "") + ")" if prop else "")
                 + f" e {rtc} do runtime" + (f"; {exec_} chamadas vistas em execução" if exec_ else "")
                 + (f"; **faltam/stub: {', '.join('`' + n + '`' for n in falt)}**" if falt else "") + ".")
    resto = sorted(n for n in kin if n not in usados)
    if resto:
        L.append(f"- Outras: {', '.join('`' + n + '`' for n in resto)}.")
    L.append("")
    # Funções do Kinect.
    libs = collections.Counter(x["lib"] for x in imps)
    est = collections.Counter(x["xenia"] for x in imps)
    L.append("## Chamadas ao sistema")
    L.append("")
    L.append(f"**{len(imps)} importações** ({libs['xam']} do `xam.xex`, {libs['xboxkrnl']} do `xboxkrnl.exe`), "
             f"{sum(len(x['sites']) for x in imps)} chamadas diretas no código"
             + (f", {len(rt)} vistas em execução" if rt else "") + ". Estado no Xenia Edge (kixenia): "
             f"{est['implementada']} implementadas, {est['aproximada']} aproximadas, {est['stub']} stubs, "
             f"{est['ausente']} ausentes.")
    L.append("")
    L.append("**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e "
             "thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do "
             "`.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem "
             "VMX128, sem análise automática (`jogos/pipeline.py`).")
    L.append("")
    rn = json.load(open(f"{A}/{abrev}_runtime_nui.json")) if os.path.exists(f"{A}/{abrev}_runtime_nui.json") else {}
    do_runtime = [a for a in fnset if rn.get(a, {}).get("runtime")]
    proprias = [a for a in fnset if a not in set(do_runtime)]
    L.append("### Funções do jogo que falam com o Kinect")
    L.append("")
    if rn:
        L.append(f"Das {len(fnset)} funções que chamam o sistema, **{len(do_runtime)} são o runtime NUI embutido** "
                 "(a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas "
                 f"importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **{len(proprias)} são código próprio do jogo**. "
                 "As próprias vêm primeiro; o runtime fica recolhido no fim.")
        L.append("")
    por = collections.defaultdict(list)
    for a in proprias:
        por[grupo(a)].append(a)
    ordem = list(dict.fromkeys([g[0] for g in tx["GRUPOS"]] + ["Funções do jogo que falam com o Kinect"]))
    blocos = [(g, por[g], False) for g in ordem if por.get(g)]
    if do_runtime:
        blocos.append(("Runtime NUI embutido", do_runtime, True))
    for g, fs, recolhido in blocos:
        if recolhido:
            L.append(f"<details><summary><b>{g}</b> ({len(fs)} funções)</summary>")
            L.append("")
        elif len(blocos) > 1 and g != "Funções do jogo que falam com o Kinect":
            L.append(f"#### {g}")
            L.append("")
        elif len(blocos) > 1:
            L.append("#### Código próprio do jogo")
            L.append("")
        L.append("| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |")
        L.append("|---|---|---|---|---|---|")
        for a in fs:
            c = f"{A}/c_full/{a}.c"
            ok = os.path.exists(c) and "halt_baddata" not in open(c).read()
            gr = graph.get(a, {})
            faz = tx["FUNCOES"].get(a) or (descrever(open(c).read(), sorted(chama[a])) if os.path.exists(c) else "")
            L.append(f"| {a.upper().replace('0X', '0x')} | {pdata.get(a, gr.get('size', 0))} | "
                     f"{', '.join('`' + n + '`' for n in sorted(chama[a]))} | {len(gr.get('callers', []))} | "
                     f"{faz.replace('|', chr(92) + '|')} | {'✅' if ok else ('⚠️' if os.path.exists(c) else '—')} |")
        L.append("")
        if recolhido:
            L.append("</details>")
            L.append("")
    L.append("### Importações do Kinect, NUI, voz e avatares")
    L.append("")
    L.append("| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |")
    L.append("|---|---|---|---|---|")
    for n, v in sorted(kin.items()):
        L.append(f"| `{n}` | {v['lib']} 0x{v['ordinal']:X} | {v['sites']} | {rt.get(n, 0)} | {ST[v['xenia']]} |")
    L.append("")
    L.append("### Todas as importações, por área")
    L.append("")
    areas = collections.defaultdict(list)
    for x in imps:
        areas[area(x["name"])].append(x)
    for cat, items in sorted(areas.items(), key=lambda kv: -len(kv[1])):
        L.append(f"<details><summary><b>{cat}</b> ({len(items)})</summary>")
        L.append("")
        L.append("| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |")
        L.append("|---|---|---|---|---|")
        for x in sorted(items, key=lambda x: x["name"]):
            L.append(f"| `{x['name']}` | {x['lib']} 0x{x['ordinal']:X} | {len(x['sites'])} | "
                     f"{rt.get(x['name'], 0)} | {ST[x['xenia']]} |")
        L.append("")
        L.append("</details>")
        L.append("")
    aus = sorted(x["name"] for x in imps if x["xenia"] == "ausente")
    L.append(f"**Ausentes no Xenia ({len(aus)}):** " + (", ".join(f"`{m}`" for m in aus) if aus else "nenhuma") + ".")
    L.append("")
    kin_aus = sorted(n for n, v in kin.items() if v["xenia"] in ("ausente", "stub"))
    L.append("## Pendências")
    L.append("")
    if kin_aus:
        L.append("- Importações do Kinect ausentes ou stub: " + ", ".join(f"`{n}`" for n in kin_aus) + ".")
    usadas = sorted(n for n, v in kin.items() if v["xenia"] == "aproximada" and rt.get(n, 0))
    if usadas:
        L.append("- Aproximadas e chamadas em execução: " + ", ".join(f"`{n}`" for n in usadas) + ".")
    if marcas.get("CRASH DUMP"):
        L.append("- Travou na bateria ao vivo (veja [[Kinect-Xenia-Edge]]).")
    if not kin_aus and not usadas and not marcas.get("CRASH DUMP"):
        L.append("- Nenhuma: tudo o que o jogo pede do Kinect está implementado.")
    L.append("")
    L.append("## Arquivos da análise")
    L.append("")
    L.append(f"- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/{abrev}/` (`{abrev}.img`, `{abrev}_imports.json`, "
             f"`{abrev}_kinect.json`, `{abrev}_fngraph.json`, `c_full/<endereço>.c`: {len(graph)} funções).")
    L.append(f"- Projeto Ghidra: `games/decompilado/{abrev}/ghidra/{abrev}` (PowerPC:BE:64:VMX128-32addr, sem análise automática).")
    if rtj.get("log"):
        L.append(f"- Log da bateria: `{rtj['log'].replace(os.path.expanduser('~'), '~')}`.")
    L.append("")
    tags = ["jogo", "kinect" if not sem_kinect else "sem-kinect", slug(nome)]
    nota = f"---\ntags: [{', '.join(tags)}]\ntitulo: {titulo}\ndata: 2026-10-29\n{GFM}\n---\n" + "\n".join(L)
    arq = f"{JOGOS}/{arquivo_nota(nome)}"
    open(arq, "w").write(nota)
    print(arq, len(nota), "caracteres")


alvos = sys.argv[1:]
if alvos == ["todos"]:
    alvos = [j["abrev"] for j in CAT if os.path.exists(f"{DEC}/{j['abrev']}/{j['abrev']}_fngraph.json")]
for a in alvos:
    gerar(a)
