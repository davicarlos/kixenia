# Decodifica o bloco de modelo descomprimido (STRB bloco 3) como o avatarcore do xam 17559:
# 0x81974A90 (cabeçalho), 0x819747D8 (malha), 0x81973798/0x819742D8 (vértices quantizados),
# 0x81971658/0x81971730 (índices). Os campos são lidos de um fluxo de bits LSB primeiro.
# uso: malha.py <bloco descomprimido> [saída.obj]
import math, struct, sys

# Passos da rede hexagonal compacta (0x81A975C0, calculados em 0x81A70F50) e os
# deslocamentos das linhas/camadas ímpares (0x8163338C, 0x81633390).
PASSO = (2.0, math.sqrt(6.0) * (2.0 / 3.0), math.sqrt(3.0))
DESLOC_Z, DESLOC_X = 1.0 / 3.0, 0.5


class Bits:
    def __init__(self, b):
        self.b = bytes(b) + bytes(8)

    def __call__(self, pos, n):
        if not n:
            return 0
        i = pos >> 3
        v = int.from_bytes(self.b[i:i + ((pos & 7) + n + 7) // 8], "little")
        return (v >> (pos & 7)) & ((1 << n) - 1)


def f32(u):
    return struct.unpack("<f", struct.pack("<I", u))[0]


def s32(u):
    return u - (1 << 32) if u & 0x80000000 else u


class Fluxo:
    def __init__(self, b):
        self.b, self.p = b, 0

    def ler(self, n):
        r = self.b[self.p:self.p + n]
        if len(r) != n:
            raise ValueError(f"fim do fluxo em {self.p:#x} (pedidos {n:#x})")
        self.p += n
        return r

    def pular(self, n):
        self.ler(n)


def cabecalho(f):
    c = struct.unpack("<12I", f.ler(0x30))
    return dict(resultado=c[0], gpu=c[1], c2=c[2], c3=c[3], c4=c[4], malhas=c[5], texturas=c[6],
                gpu_a=c[7], gpu_b=c[8], res_malhas=c[9], res_texturas=c[10], minimo=c[11])


def subconjunto(f):
    b = Bits(f.ler(0x18))
    tipo, ident = b(0, 32), b(32, 32)
    if tipo == 1:
        return dict(tipo=tipo, id=ident, a=b(0x40, 16), b=b(0x50, 16), c=b(0x60, 32))
    return dict(tipo=tipo, id=ident, cor=[f32(b(0x40 + 32 * i, 32)) for i in range(4)])


def campo(b, pos, desc):
    bits, bias = desc
    return (b(pos, bits) + bias) & 0xFFFFFFFF


def vertices_q(f, n, pares):
    """0x81973798 + 0x819742D8 (tipo 7) e as variantes dos tipos 1 a 6: cabeçalho de 0x1B2
    bits e n vértices empacotados, cada um com `pares` pares de valores de 16 bits no fim."""
    inicio = f.p
    cab = f.b[f.p:f.p + 0x37]
    b = Bits(cab)
    nv = b(0, 32)
    q = 0x20
    escala = f32(b(q, 32))
    origem = [f32(b(q + 0x20 + 32 * i, 32)) for i in range(3)]
    larg = [b(q + 0x80 + 6 * i, 6) for i in range(3)]
    # {bits, soma} de cada campo (0x8197A630 e 0x8197A4D8), em bits absolutos do cabeçalho.
    d34 = (b(0xD2, 32), b(0xB2, 32))
    d40 = (b(0x112, 32), b(0xF2, 32))
    d4c = (b(0x152, 32), b(0x132, 32))
    d58 = (b(0x192, 32), b(0x172, 32))
    passo = [escala * p for p in PASSO]
    vbits = sum(larg) + d34[0] + d40[0] + d4c[0] + d58[0] + 0x20 * pares
    total = (nv * vbits + 0x1B9) >> 3
    corpo = Bits(f.b[inicio + 0x36:inicio + total + 8])
    vs = []
    p = 2
    for _ in range(min(n, nv)):
        qx, qy, qz = (corpo(p, larg[0]), corpo(p + larg[0], larg[1]),
                      corpo(p + larg[0] + larg[1], larg[2]))
        y = qy * passo[1] + origem[1]
        z = qz * passo[2] + origem[2] + (passo[2] * DESLOC_Z if qy & 1 else 0)
        x = qx * passo[0] + origem[0] + (passo[0] * DESLOC_X if (qy ^ qz) & 1 else 0)
        o = p + sum(larg)
        normal = campo(corpo, o, d34); o += d34[0]
        w10 = campo(corpo, o, d40); o += d40[0]
        w14 = campo(corpo, o, d4c); o += d4c[0]
        w18 = campo(corpo, o, d58); o += d58[0]
        m16 = [corpo(o + 16 * i, 16) for i in range(2 * pares)]
        vs.append(dict(pos=(x, y, z), normal=normal, w10=w10, w14=w14, w18=w18, m16=m16))
        p += vbits
    f.p = inicio + total
    return dict(n=nv, escala=escala, origem=origem, larguras=larg, vbits=vbits,
                descritores=(d34, d40, d4c, d58), vertices=vs)


def indices(f, n):
    """0x81971658 + 0x81971730."""
    inicio = f.p
    b = Bits(f.b[f.p:f.p + 8])
    ni, bias, bits = b(0, 32), b(0x20, 16), b(0x30, 16)
    total = (ni * bits + 0x47) >> 3
    corpo = Bits(f.b[inicio + 8:inicio + total + 8])
    ix = [(corpo(i * bits, bits) + bias) & 0xFFFF for i in range(min(n, ni))]
    f.p = inicio + total
    return dict(n=ni, bias=bias, bits=bits, indices=ix)


def malha(f):
    b = Bits(f.ler(0x21))
    m = dict(r0=b(0, 32), nsub=b(0x20, 5), triangulos=b(0x25, 32), nvert=b(0x45, 32),
             tipo=b(0x65, 32), stride=b(0x85, 32), r1f4=b(0xA5, 32), off_vb=b(0xC5, 32),
             off_ib=b(0xE5, 32))
    m["subconjuntos"] = [subconjunto(f) for _ in range(m["nsub"])]
    if not 1 <= m["tipo"] <= 7 or m["stride"] != 0x1C + 4 * m["tipo"]:
        raise ValueError(f"tipo de vértice {m['tipo']} com stride {m['stride']:#x}")
    m["vb"] = vertices_q(f, m["nvert"], m["tipo"])
    m["ib"] = indices(f, m["triangulos"] * 3)
    return m


def textura(f):
    """0x81974A90 (8 bytes de posição) + 0x81978680 (cabeçalho de 0x21 bytes e as linhas)."""
    b = Bits(f.ler(8))
    t = dict(off_gpu=b(0, 32), tamanho=b(0x20, 32))
    b = Bits(f.ler(0x21))
    t.update(formato=b(0, 32), largura=b(0x20, 32), altura=b(0x40, 32), r3=b(0x60, 32),
             r5=b(0x80, 32), fatias=b(0xA0, 32), vazia=b(0xC0, 1), tiling=b(0xC1, 1),
             bytes_linha=b(0xC2, 32), linhas=b(0xE2, 32))
    t["dados"] = f.p
    if not t["vazia"]:
        f.pular(t["fatias"] * t["linhas"] * t["bytes_linha"])
    return t


def costuras(f):
    """0x81971CA8: blocos {contagem, soma, bits} de pares (origem, destino) no vertex buffer;
    um bloco de contagem ímpar encerra a lista."""
    pares = []
    while True:
        b = Bits(f.ler(0xC))
        n, soma, bits = b(0, 32), b(0x20, 32), b(0x40, 32)
        corpo = Bits(f.b[f.p:f.p + ((n * bits + 7) >> 3) + 8]) if n < 0x10000 else None
        if n & 1:
            f.p += ((n * bits + 0x67) >> 3) - 0xC if n < 0x10000 else 0
            return pares
        vals = [(corpo(i * bits, bits) + soma) & 0xFFFFFFFF for i in range(n)]
        pares += list(zip(vals[0::2], vals[1::2]))
        f.p += ((n * bits + 0x67) >> 3) - 0xC


def modelo(b):
    f = Fluxo(b)
    c = cabecalho(f)
    c["lista"] = [malha(f) for _ in range(c["malhas"])]
    c["fim_malhas"] = f.p
    c["texturas_l"] = [textura(f) for _ in range(c["texturas"])]
    c["costuras"] = costuras(f)
    c["fim"] = f.p
    return c


if __name__ == "__main__":
    d = open(sys.argv[1], "rb").read()
    m = modelo(d)
    print({k: (hex(v) if isinstance(v, int) else v) for k, v in m.items() if k != "lista"})
    for i, x in enumerate(m["lista"]):
        vb, ib = x["vb"], x["ib"]
        print(f"malha {i}: tipo {x['tipo']} stride {x['stride']:#x} vért {x['nvert']} tri {x['triangulos']}"
              f" subconj {x['nsub']} vb@{x['off_vb']:#x} ib@{x['off_ib']:#x} | vbits {vb['vbits']} larg {vb['larguras']}"
              f" escala {vb['escala']:.5f} origem {[round(o, 3) for o in vb['origem']]} | idx bits {ib['bits']} bias {ib['bias']}"
              f" máx {max(ib['indices']) if ib['indices'] else '-'}")
        for s in x["subconjuntos"]:
            print("   ", s)
        ps = [v["pos"] for v in vb["vertices"]]
        if ps:
            print("    caixa:", [round(min(p[k] for p in ps), 3) for k in range(3)],
                  [round(max(p[k] for p in ps), 3) for k in range(3)])
            print("    v0:", vb["vertices"][0])
    if len(sys.argv) > 2:
        with open(sys.argv[2], "w") as o:
            base = 1
            for x in m["lista"]:
                for v in x["vb"]["vertices"]:
                    o.write("v %f %f %f\n" % v["pos"])
                ix = x["ib"]["indices"]
                for t in range(0, len(ix) - 2, 3):
                    o.write("f %d %d %d\n" % (ix[t] + base, ix[t + 1] + base, ix[t + 2] + base))
                base += len(x["vb"]["vertices"])
    for t in m["texturas_l"]:
        print("textura", {k: hex(v) for k, v in t.items()})
    print(f"costuras: {len(m['costuras'])} pares; consumido {m['fim']:#x} de {len(d):#x}")
