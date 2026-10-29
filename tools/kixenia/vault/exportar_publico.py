# Exporta a parte publicável do vault para o repositório público (kixenia), já no
# layout dele: uma pasta por assunto, cada uma com um README.md de índice.
#
# Fica de fora: notas de módulos e "vizinhas" (quase só decompilação), console/NAND,
# Eternal Sonata, o emulador antigo e tudo o que vem do xam de devkit (recovery).
# Nas notas exportadas: sem cabeçalho YAML nem avisos de "nota gerada", blocos de
# código descompilado removidos, caminhos locais trocados por marcadores, callouts do
# Obsidian convertidos nos alertas do GitHub e links convertidos em links markdown
# (ou texto, se o destino ficou de fora).
#
# uso: exportar_publico.py <vault> <pasta das notas, docs/kixenia no repositório>
import getpass, os, re, shutil, sys, unicodedata

VAULT, REPO = sys.argv[1], sys.argv[2]
PASTAS = ["kinect", "avatares", "xam"]

# Marcas de saída do Ghidra: um bloco com duas ou mais delas é decompilação.
GHIDRA = re.compile(r"undefined\d|FUN_[0-9a-fA-F]{6}|func_0x|Function_8|\b[iu]Var\d|"
                    r"\bauStack_|\b[pl]Var\d|\bunaff_|\bin_r\d")
BLOCO = re.compile(r"```[^\n]*\n.*?```\n?", re.S)
SECAO_DECOMP = re.compile(r"^## (Decompila|Confirma)[^\n]*\n.*?(?=^## |\Z)", re.S | re.M)
YAML = re.compile(r"\A---\n.*?\n---\n+", re.S)
AVISO_GERADA = re.compile(r"^> \[!info\] Nota gerada[^\n]*\n(?:>[^\n]*\n)*\n?", re.M)
# Linhas com informação do xam de devkit (recovery do XDK) não são publicadas.
DEVKIT = re.compile(r"recovery 21256|\bxamd\b|xam de devkit", re.I)

CAMINHOS = [
    (re.compile(r"/run/media/[^/\s`]+/[^/\s`]+/"), "<HD>/"),
    (re.compile(r"/tmp/claude-[^\s`)]*"), "<pasta temporária>"),
    (re.compile(r"/home/[^/\s`]+/"), "~/"),
    (re.compile(re.escape(getpass.getuser())), "<usuário>"),
]
ALERTAS = {"info": "NOTE", "note": "NOTE", "abstract": "NOTE", "summary": "NOTE",
           "tip": "TIP", "hint": "TIP", "success": "TIP", "important": "IMPORTANT",
           "warning": "WARNING", "caution": "CAUTION", "danger": "CAUTION",
           "bug": "WARNING", "failure": "CAUTION", "question": "NOTE",
           "example": "NOTE", "quote": "NOTE", "todo": "IMPORTANT"}


def slug(nome):
    s = unicodedata.normalize("NFKD", nome).encode("ascii", "ignore").decode()
    s = re.sub(r"[^\w]+", "-", s).strip("-")
    return s.lower()


def destino(rel):
    """Caminho no repositório de uma nota do vault, ou None se ela fica de fora."""
    pasta, arquivo = os.path.split(rel)
    nome = os.path.splitext(arquivo)[0]
    if pasta == "Kinect/Jogos":
        return f"kinect/jogos/{slug(nome)}.md"
    if pasta == "Kinect":
        return f"kinect/{slug(nome)}.md"
    if rel == "XAM/Avatares.md":
        return "avatares/README.md"
    if rel == "XAM/Funcoes-XAM-Indice.md":
        return "xam/funcoes/README.md"
    if pasta == "XAM/Funcoes":
        m = re.match(r"(.+?) \(([0-9A-Fa-f]{8})\)$", nome)
        if m:
            return f"xam/funcoes/{m.group(1)}-{m.group(2).upper()}.md"
        if re.fullmatch(r"FUN_[0-9A-Fa-f]{8}", nome):
            return f"xam/funcoes/{nome}.md"
        return None
    if pasta == "XAM" and nome in ("XAM-Fitness", "XAM-Rede-e-Login"):
        return f"xam/{slug(nome)}.md"
    if rel == "Ferramentas/Ferramentas-Ghidra-REA.md":
        return "ghidra-e-rea.md"
    return None


def ancora(secao):
    s = secao.strip().lower()
    s = re.sub(r"[^\w\- ]", "", s)
    return s.replace(" ", "-")


def callouts(texto):
    def troca(m):
        tipo = ALERTAS.get(m.group(1).lower(), "NOTE")
        titulo = m.group(2).strip()
        return f"> [!{tipo}]" + (f"\n> **{titulo}**" if titulo else "")
    return re.sub(r"^> \[!(\w+)\][+-]?([^\n]*)", troca, texto, flags=re.M)


def limpar(texto, rel_destino, destinos):
    texto = YAML.sub("", texto)
    texto = AVISO_GERADA.sub("", texto)
    texto = SECAO_DECOMP.sub("", texto)
    removidos = 0

    def bloco(m):
        nonlocal removidos
        if len(GHIDRA.findall(m.group(0))) >= 2:
            removidos += 1
            return "*(trecho de código descompilado removido da versão pública)*\n"
        return m.group(0)

    texto = BLOCO.sub(bloco, texto)
    texto = "\n".join(l for l in texto.split("\n") if not DEVKIT.search(l))
    for padrao, troca in CAMINHOS:
        texto = padrao.sub(troca, texto)
    texto = callouts(texto)

    pasta = os.path.dirname(rel_destino)

    def link(m):
        embutido, alvo, secao, rotulo = m.groups()
        nome = alvo.strip()
        mostrado = (rotulo or nome).strip()
        if embutido or nome not in destinos:
            return mostrado
        caminho = os.path.relpath(destinos[nome], pasta).replace(" ", "%20")
        if secao:
            caminho += "#" + ancora(secao)
        return f"[{mostrado}]({caminho})"

    texto = re.sub(r"(!?)\[\[([^\]|#]+)(?:#([^\]|]+))?(?:\|([^\]]+))?\]\]", link, texto)
    return re.sub(r"\n{3,}", "\n\n", texto).strip() + "\n", removidos


def secoes_do_indice():
    """As listas do 00-Indice do vault, por título de seção."""
    texto = open(os.path.join(VAULT, "00-Indice.md"), encoding="utf-8").read()
    secoes, atual = {}, None
    for linha in texto.split("\n"):
        if linha.startswith("## "):
            atual = linha[3:].strip()
            secoes[atual] = []
        elif atual and linha.startswith(("- ", "  - ")):
            secoes[atual].append(linha)
    return secoes


INDICES = {
    "kinect/README.md": (
        "# Kinect\n\nComo o Kinect do Xbox 360 funciona por dentro (USB, firmware, "
        "câmera, áudio, inclinação, NUI no xam, Kinect ID) e o que foi preciso para "
        "rodá-lo no Xenia Edge.\n\nOs jogos testados estão em [jogos/](jogos/README.md).",
        ["Kinect no Xenia Edge", "Engenharia reversa do Kinect"]),
    "kinect/jogos/README.md": (
        "# Jogos Kinect\n\nUma nota por jogo: executável, versão do runtime NUI, o "
        "que ele pede do sistema e como roda no Xenia Edge.",
        ["Jogos Kinect"]),
    "xam/README.md": (
        "# XAM\n\nO xam é a biblioteca de sistema do Xbox 360 que os jogos chamam para "
        "Kinect, avatares, perfis e interface. Tudo aqui se refere ao xam da "
        "atualização 17559, a última de varejo.\n\n"
        "- [[Funcoes-XAM-Indice|Funções do xam ligadas ao Kinect]]: uma nota por "
        "função, com endereço, o que faz e o que chama\n"
        "- [[XAM-Fitness]]: perfil de fitness, eventos e privacidade\n"
        "- [[XAM-Rede-e-Login]]: rede e login\n"
        "- O sistema de avatares tem pasta própria: [[Avatares]]",
        []),
}


def escrever_indices(destinos):
    secoes = secoes_do_indice()
    for caminho, (abertura, titulos) in INDICES.items():
        partes = [abertura]
        for titulo in titulos:
            itens = [l for l in secoes.get(titulo, []) if "[[" in l]
            if len(titulos) > 1:
                partes.append(f"## {titulo}")
            partes.append("\n".join(itens))
        if caminho == "kinect/jogos/README.md":
            # Todas as notas de jogos exportadas que o índice do vault não lista uma a uma
            # (as geradas pelo jogos/gen_jogo.py), em ordem alfabética.
            vistos = set(re.findall(r"\[\[([^\]|#]+)", "\n".join(partes)))
            extras = sorted(n for n, d in destinos.items() if d.startswith("kinect/jogos/") and n not in vistos)
            if extras:
                partes.append("## Todos os jogos analisados\n\nUma nota por jogo do HD, gerada do executável "
                              "(importações, funções que falam com o Kinect, runtime NUI embutido) e da bateria "
                              "ao vivo: veja [[Jogos-Kinect-Indice]] e [[Jogos-Kinect-Pendencias]].\n\n"
                              + "\n".join(f"- [[{n}]]" for n in extras))
        texto, _ = limpar("\n\n".join(partes) + "\n", caminho, destinos)
        open(os.path.join(REPO, caminho), "w", encoding="utf-8").write(texto)


def main():
    lista = []
    for raiz, _, arquivos in os.walk(VAULT):
        if "/." in raiz:
            continue
        for a in arquivos:
            if a.endswith(".md"):
                rel = os.path.relpath(os.path.join(raiz, a), VAULT)
                d = destino(rel)
                if d:
                    lista.append((rel, d))
    destinos = {os.path.splitext(os.path.basename(r))[0]: d for r, d in lista}
    for p in PASTAS + ["ghidra-e-rea.md"]:
        alvo = os.path.join(REPO, p)
        if os.path.isdir(alvo):
            shutil.rmtree(alvo)
        elif os.path.exists(alvo):
            os.remove(alvo)
    total = 0
    for rel, d in sorted(lista, key=lambda x: x[1]):
        texto = open(os.path.join(VAULT, rel), encoding="utf-8").read()
        texto, removidos = limpar(texto, d, destinos)
        total += removidos
        saida = os.path.join(REPO, d)
        os.makedirs(os.path.dirname(saida), exist_ok=True)
        open(saida, "w", encoding="utf-8").write(texto)
    escrever_indices(destinos)
    print(f"{len(lista)} notas exportadas, {total} blocos descompilados removidos")


if __name__ == "__main__":
    main()
