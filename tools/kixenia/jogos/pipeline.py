# Pipeline de um jogo: executável → imagem decifrada → importações → funções que falam
# com o Kinect → decompilação dessas funções no Ghidra (sem análise completa) → runtime
# do log do Xenia. Grava tudo em games/decompilado/<abrev>/ com os mesmos nomes de
# arquivo dos jogos já feitos (dj, fn, …), para o gen_jogo.py gerar a nota do vault.
# uso: pipeline.py <abrev> <alvo> [log do Xenia]   (alvo: default.xex, cabeçalho GoD ou pacote XBLA)
import bisect, collections, glob, json, os, re, shutil, struct, subprocess, sys, time

ABREV, ALVO = sys.argv[1:3]
LOG = sys.argv[3] if len(sys.argv) > 3 else ""
H = os.path.expanduser("~")
# Onde ficam as coisas (variáveis de ambiente para outra máquina).
RE = os.environ.get("KINECT_RE", f"{H}/Kixenia/kinect-sdk-re")
R = f"{RE}/games/decompilado/{ABREV}"
XENIA = os.environ.get("XENIA_EDGE", f"{H}/Kixenia/xenia-edge-kinect-sistema/build/bin/Linux/Release/xenia_edge")
HEADLESS = os.environ.get("GHIDRA_HEADLESS", f"{H}/Documents/Codex/ghidra-install/app/ghidra_12.1.4_PUBLIC/support/analyzeHeadless")
SCRIPTS = os.environ.get("GHIDRA_SCRIPTS", f"{H}/Kixenia/kinect-tools/ghidra")
B = 0x82000000
sys.path.insert(0, f"{RE}/modulos")
import xenia_estado
xenia_estado.SRC = os.environ.get("XENIA_SRC", f"{H}/Kixenia/xenia-edge-kinect-sistema/src/xenia/kernel")
os.makedirs(R, exist_ok=True)
KINECT = re.compile(r"Nui|PsCam|Mca|Natal|Biometric|Voice|MicArray|XAudioCapture|MECClient|Fitness|^XMsg|Avatar|XamCache|Usbcam")


def passo(msg):
    print(f"[{ABREV}] {time.strftime('%H:%M:%S')} {msg}", flush=True)


def xex_gravado():
    for v in glob.glob(f"{R}/*.xex"):
        b = os.path.basename(v).lower()
        if b not in (f"{ABREV}.xex", f"{ABREV}.dec.xex") and os.path.getsize(v) > 0x1000:
            return v
    return None


# 1. O executável.
xex = f"{R}/{ABREV}.xex"
if not os.path.exists(xex):
    if ALVO.lower().endswith(".xex"):
        shutil.copy(ALVO, xex)
    else:
        passo("extraindo o xex do pacote com o Xenia (--dump_xex)")
        for v in glob.glob(f"{R}/*.xex"):
            if os.path.basename(v).lower() != f"{ABREV}.xex" and not os.path.basename(v).endswith(".dec.xex"):
                os.remove(v)

        p = subprocess.Popen([XENIA, "--dump_xex=true", "--allow_kinect_camera=false", "--log_level=1",
                              f"--log_file={R}/{ABREV}.dump.log", ALVO], cwd=R,
                             stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        for _ in range(60):
            time.sleep(1)
            if xex_gravado():
                time.sleep(3)
                break
        p.kill(); p.wait()
        g = xex_gravado()
        if not g:
            passo("ERRO: o Xenia não gravou o xex"); sys.exit(1)
        os.rename(g, xex)

# 2. Imagem decifrada e descomprimida.
img, info = f"{R}/{ABREV}.img", f"{R}/{ABREV}.info.json"
if not os.path.exists(img):
    d = open(xex, "rb").read(0x20000)
    n = struct.unpack_from(">I", d, 0x14)[0]
    cifrado = False
    for k in range(n):
        key, val = struct.unpack_from(">II", d, 0x18 + 8 * k)
        if key == 0x3FF and struct.unpack_from(">H", d, val + 4)[0] == 1:
            cifrado = True
    src = xex
    if cifrado:
        src = f"{R}/{ABREV}.dec.xex"
        subprocess.run([sys.executable, "-I", f"{RE}/games/decompilado/scripts/mex_decifra.py", xex, src],
                       check=True, stdout=subprocess.DEVNULL)
    out = subprocess.run([f"{RE}/modulos/bin/xexinfo", src, img], capture_output=True, text=True).stdout
    open(info, "w").write(out)
    j = json.loads(out)
    if j.get("erro") or not os.path.exists(img):
        passo("ERRO xexinfo: " + str(j.get("erro"))); sys.exit(1)
j = json.load(open(info))
base = j["base"]
if base != B:
    passo(f"AVISO: base {base:08X} (não 0x82000000)")
d = open(img, "rb").read()
passo(f"imagem {len(d)} bytes, base {base:08X}, entrada {j['entry']:08X}, {len(j['imports'])} registros de importação")

# 3. Importações: nome, thunk, estado no Xenia, pontos de chamada.
est_nomes = json.load(open(f"{RE}/modulos/dados/estado.json"))["names"]
grupos = {}
for a, lib, kind, ordi in j["imports"]:
    g = grupos.setdefault((lib, ordi), {"imp": None, "thunk": None})
    g["thunk" if kind == 1 else "imp"] = a
est = xenia_estado.estado_xenia()
imps = []
for (lib, ordi), g in sorted(grupos.items(), key=lambda x: (x[0][0], x[1]["imp"] or 0)):
    nome_kind = est_nomes.get(lib, {}).get(str(ordi))
    name = nome_kind[0] if nome_kind else f"{lib.split('.')[0]}_{ordi:X}"
    kind = "variável" if nome_kind and nome_kind[1] == "kVariable" else "função"
    s, arq = est.get(name, ("ausente", ""))
    if kind == "variável" and s == "ausente":
        s = "implementada"
    imps.append({"lib": lib.split(".")[0], "kind": kind, "imp": hex(g["imp"]) if g["imp"] else "",
                 "thunk": hex(g["thunk"]) if g["thunk"] else "", "ordinal": ordi, "name": name,
                 "sites": [], "xenia": s, "arquivo": arq})
alvos = {int(i["thunk"], 16): i for i in imps if i["thunk"]}
for off in range(0, len(d) - 3, 4):
    w = struct.unpack_from(">I", d, off)[0]
    if w >> 26 == 18 and not w & 2:
        li = w & 0x03FFFFFC
        if li & 0x02000000:
            li -= 0x04000000
        t = (base + off + li) & 0xFFFFFFFF
        if t in alvos and base + off != t:
            alvos[t]["sites"].append(hex(base + off))
json.dump(imps, open(f"{R}/{ABREV}_imports.json", "w"), indent=0, ensure_ascii=False)
c = collections.Counter(i["xenia"] for i in imps)
passo(f"{len(imps)} importações {dict(collections.Counter(i['lib'] for i in imps))} {dict(c)}, "
      f"{sum(len(i['sites']) for i in imps)} chamadas diretas")

# 4. Funções do .pdata e as que chamam o Kinect.
pe = struct.unpack("<I", d[0x3C:0x40])[0]
nsec = struct.unpack("<H", d[pe + 6:pe + 8])[0]
opt = struct.unpack("<H", d[pe + 20:pe + 22])[0]
secs = {}
for i in range(nsec):
    o = pe + 24 + opt + i * 40
    nm = d[o:o + 8].split(b"\0")[0].decode("latin1")
    vs, va, rs, rp = struct.unpack("<IIII", d[o + 8:o + 24])
    secs[nm] = (base + va, vs)
funcs = []
if ".pdata" in secs:
    pa, ps = secs[".pdata"]
    for off in range(pa - base, pa - base + ps, 8):
        if off + 8 > len(d):
            break
        begin, packed = struct.unpack_from(">II", d, off)
        if begin == 0:
            continue
        funcs.append((begin, ((packed >> 8) & 0x3FFFFF) * 4))
funcs.sort()
starts = [f[0] for f in funcs]
set_starts = set(starts)


def containing(a):
    i = bisect.bisect_right(starts, a) - 1
    if i >= 0 and funcs[i][0] <= a < funcs[i][0] + funcs[i][1]:
        return funcs[i][0]
    return None


callers = collections.defaultdict(set)
ta, ts = secs.get(".text", (base, len(d)))
for off in range(ta - base, min(ta - base + ts, len(d) - 3), 4):
    w = struct.unpack_from(">I", d, off)[0]
    if w >> 26 == 18 and not w & 2 and w & 1:
        li = w & 0x03FFFFFC
        if li & 0x02000000:
            li -= 0x04000000
        t = (base + off + li) & 0xFFFFFFFF
        f = containing(base + off)
        if f is not None and t in set_starts:
            callers[t].add(f)
kin = {}
for i in imps:
    if not KINECT.search(i["name"]):
        continue
    fs = sorted({containing(int(s, 16)) for s in i["sites"]} - {None})
    kin[i["name"]] = {"lib": i["lib"], "ordinal": i["ordinal"], "sites": len(i["sites"]), "xenia": i["xenia"],
                      "callers": [hex(f) for f in fs]}
size = dict(funcs)
fnset = sorted({c for v in kin.values() for c in v["callers"]}, key=lambda a: int(a, 16))
graph = [{"addr": a, "size": size[int(a, 16)], "callers": sorted(hex(c) for c in callers.get(int(a, 16), ()))}
         for a in fnset]
json.dump(kin, open(f"{R}/{ABREV}_kinect.json", "w"), indent=1, ensure_ascii=False)
json.dump(graph, open(f"{R}/{ABREV}_graph.json", "w"), indent=1)
json.dump({hex(a): s for a, s in funcs}, open(f"{R}/{ABREV}_pdata.json", "w"))
json.dump({k: [hex(v[0]), v[1]] for k, v in secs.items()}, open(f"{R}/{ABREV}_sections.json", "w"))
open(f"{R}/{ABREV}_kinect_funcs.txt", "w").write("\n".join(fnset) + "\n")
passo(f"{len(funcs)} funções no .pdata; {len(kin)} importações do Kinect; {len(fnset)} funções que as chamam")
SENSOR = re.compile(r"Nui|PsCam|Mca|Natal|Biometric|MicArray|XAudioCapture|MECClient|Usbcam")
sem_kinect = not any(SENSOR.search(n) for n in kin)
if sem_kinect:
    passo("sem importações do sensor: jogo sem Kinect (avatar/voz/XMsg entram mesmo assim)")

# 5. Ghidra: importa a imagem, cria as funções do .pdata, nomeia os thunks e decompila
# as funções que chamam o Kinect (e quem as chama, um nível acima).
proj = f"{R}/ghidra"
prog = f"{ABREV}.img"
with open(f"{R}/{ABREV}.labels", "w") as f:
    for i in imps:
        if i["imp"]:
            f.write(f"{int(i['imp'], 16):08X} 0 {i['name']}\n")
        if i["thunk"]:
            f.write(f"{int(i['thunk'], 16):08X} 1 {i['name']}\n")
nivel2 = sorted({c for g in graph for c in g["callers"]} - set(fnset), key=lambda a: int(a, 16))
lista = fnset + nivel2
open(f"{R}/decomp_list.txt", "w").write("\n".join(lista) + "\n")
os.makedirs(f"{R}/c/p0", exist_ok=True)
if not lista and not os.path.exists(f"{R}/c/p0/graph.json"):
    open(f"{R}/c/p0/graph.json", "w").write("[]\n")
if not os.path.exists(f"{R}/c/p0/graph.json"):
    os.makedirs(proj, exist_ok=True)
    pd = secs.get(".pdata")
    importado = os.path.exists(f"{proj}/{ABREV}.gpr")
    cmd = [HEADLESS, proj, ABREV]
    if importado:
        cmd += ["-process", prog]
    else:
        cmd += ["-import", img, "-processor", "PowerPC:BE:64:VMX128-32addr", "-loader", "BinaryLoader",
                "-loader-baseAddr", f"0x{base:08X}"]
    cmd += ["-noanalysis", "-scriptPath", f"{SCRIPTS};{RE}/modulos/ghidra"]
    if pd and not importado:
        cmd += ["-postScript", "PdataFuncs.java", f"{pd[0]:08X}", str(pd[1])]
    cmd += ["-postScript", "Labels.java", f"{R}/{ABREV}.labels",
            "-postScript", "FnGraph.java", f"{R}/decomp_list.txt", f"{R}/c/p0"]
    passo(f"Ghidra: {len(lista)} funções para decompilar ({len(fnset)} do Kinect + {len(nivel2)} chamadoras)")
    with open(f"{R}/c/p0.log", "w") as lg:
        subprocess.run(["nice", "-n", "10"] + cmd, stdout=lg, stderr=subprocess.STDOUT)
    if not os.path.exists(f"{R}/c/p0/graph.json"):
        passo("ERRO: o Ghidra não gravou o grafo (veja c/p0.log)"); sys.exit(1)

# 6. Junta: c_full com os nomes das importações no lugar dos thunks, e o grafo.
th = {i["thunk"][2:]: i["name"] for i in imps if i["thunk"]}
g = []
for p in sorted(glob.glob(f"{R}/c/p*/graph.json")):
    g += json.load(open(p))
os.makedirs(f"{R}/c_full", exist_ok=True)
for f in glob.glob(f"{R}/c/p*/0x*.c"):
    s = re.sub(r"func_0x([0-9a-f]{8})", lambda m: th.get(m[1], m[0]), open(f).read())
    open(f"{R}/c_full/{os.path.basename(f)}", "w").write(s)
for x in g:
    x["callees"] = [th.get(c, c) for c in x["callees"]]
json.dump(g, open(f"{R}/{ABREV}_fngraph.json", "w"), indent=0)
ruins = [f for f in glob.glob(f"{R}/c_full/*.c") if "halt_baddata" in open(f).read()]
passo(f"{len(g)} funções no grafo, {len(glob.glob(f'{R}/c_full/*.c'))} decompiladas, {len(ruins)} com halt_baddata")

# 7. Execução: contagem de chamadas e rastreamento, do log do Xenia (bateria ao vivo).
rt = {"calls": {}, "amostras": 0, "rastreadas": 0, "log": "", "sem_kinect": sem_kinect}
if LOG and os.path.exists(LOG):
    calls, frames, tracked = collections.Counter(), 0, 0
    extra = collections.Counter()
    linhas = {}
    for l in open(LOG, errors="replace"):
        m = re.match(r"^\S> f:\S+ \S+ ([A-Za-z_]\w*)\(", l)
        if m:
            calls[m[1]] += 1
            continue
        m = re.search(r"Kinect: skeleton frame \d+: (\d+) tracked", l)
        if m:
            frames += 1
            tracked += int(m[1]) > 0
            continue
        for pat in ("NUI: title runtime", "CRASH DUMP", "Avatar:", "no room", "copied out of its image",
                    "Invalid instruction", "no contiguous range", "stream: ", "the title is reading the microphones",
                    "identity engine", "XamShowNuiSigninUI(", "Title name", "Module loaded"):
            if pat in l:
                extra[pat] += 1
                if pat in ("NUI: title runtime", "Title name") and pat not in linhas:
                    linhas[pat] = l.split(pat, 1)[1].strip()[:120]
    rt = {"calls": dict(calls.most_common()), "amostras": frames, "rastreadas": tracked,
          "log": LOG, "marcas": dict(extra), "linhas": linhas, "sem_kinect": sem_kinect}
json.dump(rt, open(f"{R}/{ABREV}_runtime.json", "w"), indent=0, ensure_ascii=False)
passo(f"pronto: {len(rt['calls'])} funções chamadas no log; {rt['rastreadas']} de {rt['amostras']} amostras rastreadas")
