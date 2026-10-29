# Roda o pipeline.py em todos os jogos do catálogo, N por vez, aproveitando o log da
# bateria ao vivo de cada um quando existe (listas "nome|caminho" das baterias).
# uso: rodar_todos.py [paralelo=3] [abrev ...]
import glob, json, os, re, subprocess, sys, time
from concurrent.futures import ThreadPoolExecutor

D = os.path.dirname(os.path.abspath(__file__))
H = os.path.expanduser("~")
PAR = int(sys.argv[1]) if len(sys.argv) > 1 else 3
SO = sys.argv[2:]
CAT = json.load(open(f"{D}/catalogo.json"))
BAT = os.environ.get("BATERIA", f"{H}/Kixenia/kinect-tools/bateria")
# Outras pastas com logs e listas da bateria (separadas por ":").
EXTRA = [d for d in os.environ.get("BATERIA_EXTRA", "").split(":") if d]

# caminho do alvo -> logs possíveis (o mais novo vence).
logs = {}
for lst in glob.glob(f"{BAT}/lista*.txt") + [l for d in EXTRA for l in glob.glob(f"{d}/lista*.txt")]:
    for l in open(lst):
        if "|" not in l:
            continue
        nome, caminho = l.rstrip("\n").split("|", 1)
        s = re.sub(r"[^A-Za-z0-9]", "_", nome)
        for d in glob.glob(f"{BAT}/saida*") + EXTRA:
            p = f"{d}/{s}.log"
            if os.path.exists(p) and os.path.getsize(p) > 100000:
                logs.setdefault(caminho, []).append(p)


def log_de(alvo):
    ls = logs.get(alvo, [])
    return max(ls, key=os.path.getmtime) if ls else ""


def um(j):
    a = j["abrev"]
    if SO and a not in SO:
        return
    if os.path.exists(f"{D}/../decompilado/{a}/{a}_runtime.json"):
        print(f"[{a}] já feito", flush=True); return
    t = time.time()
    r = subprocess.run([sys.executable, "-I", f"{D}/pipeline.py", a, j["alvo"], log_de(j["alvo"])],
                       capture_output=True, text=True)
    open(f"{D}/logs/{a}.log", "w").write(r.stdout + r.stderr)
    ultima = (r.stdout.strip().splitlines() or ["?"])[-1]
    print(f"[{a}] rc={r.returncode} {time.time() - t:.0f}s {ultima}", flush=True)


os.makedirs(f"{D}/logs", exist_ok=True)
# Os pacotes precisam do Xenia (janela, GPU): um de cada vez; os xex soltos vão em paralelo.
soltos = [j for j in CAT if j["tipo"] == "xex"]
pacotes = [j for j in CAT if j["tipo"] != "xex"]
with ThreadPoolExecutor(PAR) as ex:
    list(ex.map(um, soltos))
for j in pacotes:
    um(j)
print("CONCLUIDO", flush=True)
