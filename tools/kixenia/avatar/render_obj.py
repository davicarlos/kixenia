# Renderiza um .obj com sombreamento plano e z-buffer simples (só Pillow), em três vistas.
# uso: render_obj.py <entrada.obj> <saída.png>
import math, sys
from PIL import Image

vs, fs = [], []
for l in open(sys.argv[1]):
    p = l.split()
    if not p:
        continue
    if p[0] == "v":
        vs.append(tuple(map(float, p[1:4])))
    elif p[0] == "f":
        fs.append(tuple(int(x.split("/")[0]) - 1 for x in p[1:4]))

lo = [min(v[k] for v in vs) for k in range(3)]
hi = [max(v[k] for v in vs) for k in range(3)]
c = [(a + b) / 2 for a, b in zip(lo, hi)]
r = max(b - a for a, b in zip(lo, hi)) / 2 or 1
W = 360


def vista(rot_y, rot_x):
    img = Image.new("RGB", (W, W), (30, 30, 36))
    px = img.load()
    zb = [[-1e9] * W for _ in range(W)]
    cy, sy, cx, sx = math.cos(rot_y), math.sin(rot_y), math.cos(rot_x), math.sin(rot_x)

    def t(v):
        x, y, z = (v[0] - c[0]) / r, (v[1] - c[1]) / r, (v[2] - c[2]) / r
        x, z = x * cy + z * sy, -x * sy + z * cy
        y, z = y * cx - z * sx, y * sx + z * cx
        return x, y, z

    tv = [t(v) for v in vs]
    luz = (0.4, 0.6, 0.7)
    for a, b, d in fs:
        A, B, C = tv[a], tv[b], tv[d]
        u = [B[i] - A[i] for i in range(3)]
        w = [C[i] - A[i] for i in range(3)]
        n = (u[1] * w[2] - u[2] * w[1], u[2] * w[0] - u[0] * w[2], u[0] * w[1] - u[1] * w[0])
        ln = math.sqrt(sum(x * x for x in n)) or 1
        k = abs(sum(n[i] * luz[i] for i in range(3)) / ln)
        cor = (int(60 + 180 * k), int(60 + 160 * k), int(70 + 120 * k))
        P = [((p[0] * 0.45 + 0.5) * W, (0.5 - p[1] * 0.45) * W, p[2]) for p in (A, B, C)]
        x0, x1 = max(int(min(p[0] for p in P)), 0), min(int(max(p[0] for p in P)) + 1, W - 1)
        y0, y1 = max(int(min(p[1] for p in P)), 0), min(int(max(p[1] for p in P)) + 1, W - 1)
        (ax, ay, az), (bx, by, bz), (qx, qy, qz) = P
        den = (by - qy) * (ax - qx) + (qx - bx) * (ay - qy)
        if abs(den) < 1e-12:
            continue
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                l1 = ((by - qy) * (x - qx) + (qx - bx) * (y - qy)) / den
                l2 = ((qy - ay) * (x - qx) + (ax - qx) * (y - qy)) / den
                l3 = 1 - l1 - l2
                if l1 < 0 or l2 < 0 or l3 < 0:
                    continue
                z = l1 * az + l2 * bz + l3 * qz
                if z > zb[y][x]:
                    zb[y][x] = z
                    px[x, y] = cor
    return img


out = Image.new("RGB", (W * 3, W))
for i, (ry, rx) in enumerate([(0.6, 0.45), (math.pi / 2, 0.0), (0.0, math.pi / 2)]):
    out.paste(vista(ry, rx), (i * W, 0))
out.save(sys.argv[2])
print(len(vs), "vértices", len(fs), "triângulos")
