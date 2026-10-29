# Descrição automática de uma função decompilada de jogo: as mensagens XMsg que ela
# manda (pelo mapa do xam), as importações que chama (com a descrição do mapa do Kinect)
# e as cadeias de texto que usa. Serve de "O que faz" quando não há texto escrito à mão.
import os, re

MSGS = {
    0x2B001: "0x2B001 (NUI)", 0x2B002: "quadro de esqueleto do título ao HUD (0x2B002)",
    0x2B003: "início do título no NUI (0x2B003: versões, flags)", 0x2B004: "fim do título no NUI (0x2B004)",
    0x2B005: "AutoTilt do motor do título (0x2B005)", 0x2B006: "status do motor do título (0x2B006)",
    0x2B007: "registra o motor de inclinação do título (0x2B007)", 0x2B009: "parar o motor (0x2B009)",
    0x2B00A: "ReverseAutoTilt (0x2B00A)", 0x2B00B: "AdjustTilt (0x2B00B)", 0x2B00C: "status do motor (0x2B00C)",
    0x2B00D: "registra quem ouve o status da inclinação (0x2B00D)", 0x2B00E: "observador de quadros (0x2B00E)",
    0x2C006: "identificar o jogador (0x2C006)", 0x2C007: "identificar com biometria (0x2C007)",
    0x2C008: "qualidade da identificação (0x2C008)", 0x2C009: "registra o callback de identidade (0x2C009)",
    0x2C00A: "pergunta um valor de identidade (0x2C00A)", 0x2C00C: "dados de identidade (0x2C00C)",
    0x2C00D: "resolve o cadastro (0x2C00D)", 0x2C00E: "cancela a identificação (0x2C00E)",
    0x2C010: "cadastra o rosto para login (0x2C010)", 0x2C011: "foto do rosto (0x2C011)",
    0x2C012: "apaga o cadastro (0x2C012)", 0x2C013: "dados do cadastro (0x2C013)",
    0x2A007: "grava o cache biométrico (0x2A007)", 0x2A008: "lê o cache biométrico (0x2A008)",
    0x2A009: "biometria desligada (0x2A009)",
    0x21008: "tela de conquistas/jogos (0x21008)", 0x2100E: "tela de desligar a matriz de microfones (0x2100E)",
    0x21025: "tela de vídeo (0x21025)", 0x21028: "Troubleshooter do Kinect (0x21028)", 0x21030: "guia pelo Kinect (0x21030)",
    0x21032: "cartão de jogador (0x21032)", 0x21033: "amigos (0x21033)", 0x21034: "festa (0x21034)",
    0x21035: "convite (0x21035)", 0x21036: "pedido de amizade (0x21036)", 0x21038: "mensagens (0x21038)",
    0x2103A: "perfil corporal do fitness (0x2103A)", 0x2103B: "aviso de tempo do fitness (0x2103B)",
    0x2103C: "aviso de privacidade do fitness (0x2103C)",
}

_XD = None


def descricoes_xam():
    global _XD
    if _XD is None:
        src = open(os.path.expanduser("~/Kixenia/kinect-sdk-re/xam-nui/fnbase/mapgen.py")).read()
        ns = {}
        exec(src[src.index("D={"):src.index("ST={")], ns)
        _XD = ns["D"]
    return _XD


def descrever(codigo, importacoes):
    """Frase curta para a coluna 'O que faz' da nota, a partir do C decompilado."""
    partes = []
    msgs = []
    APPS = {0xFE: "xam", 0xFB: "Live", 0xFA: "", 0xFC: "", 0xFD: ""}
    for m in re.finditer(r"\(\s*0x(f[a-e]|F[A-E])\s*,\s*0x([0-9a-fA-F]{5})", codigo):
        v = int(m[2], 16)
        t = MSGS.get(v) or f"0x{v:05X} ao app 0x{int(m[1], 16):02X}" + (f" ({APPS[int(m[1], 16)]})" if APPS.get(int(m[1], 16)) else "")
        if t not in msgs:
            msgs.append(t)
    if msgs:
        partes.append("XMsg: " + "; ".join(msgs[:3]))
    xd = descricoes_xam()
    usadas = []
    for n in importacoes:
        d = xd.get(n)
        if d and n not in usadas:
            usadas.append(n)
    if usadas:
        partes.append("; ".join(f"`{n}`: {xd[n]}" for n in usadas[:2]))
    textos = re.findall(r'"([^"\\]{4,60})"', codigo)
    textos = [t for t in textos if not re.fullmatch(r"[\w./\\:%-]*%[\w]", t)]
    if textos:
        partes.append("textos: " + ", ".join(f'"{t}"' for t in dict.fromkeys(textos[:2])))
    s = ". ".join(partes)
    return s[:400] + ("…" if len(s) > 400 else "")
