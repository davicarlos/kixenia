# Decifra um XEX2 de varejo (AES-128-CBC com a chave de sessão do security info, ela mesma cifrada
# com a chave retail que o Xenia usa em xex_module.cc) e grava uma cópia marcada como sem
# criptografia, para o xexinfo (modulos/bin) fazer a descompressão LZX. O XEXLoaderWV do Ghidra
# descomprimiu errado o Motion Explosion ("out of input bytes" e o .text deslocado).
# uso: mex_decifra.py <xex> <saída .xex>
import struct, sys
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

RETAIL = bytes([0x20, 0xB1, 0x85, 0xA5, 0x9D, 0x28, 0xFD, 0xC3, 0x40, 0x58, 0x3F, 0xBB, 0x08, 0x96, 0xBF, 0x91])
src, out = sys.argv[1:3]
d = bytearray(open(src, "rb").read())
assert d[:4] == b"XEX2"
pe_off, sec_off, n_opt = struct.unpack_from(">I", d, 8)[0], struct.unpack_from(">I", d, 0x10)[0], struct.unpack_from(">I", d, 0x14)[0]
ffi = None
for k in range(n_opt):
    key, val = struct.unpack_from(">II", d, 0x18 + 8 * k)
    if key == 0x3FF:  # file format info: tamanho, criptografia (u16), compressão (u16)
        ffi = val
assert ffi is not None and struct.unpack_from(">H", d, ffi + 4)[0] == 1, "não é XEX criptografado"
# security info: a chave da imagem fica em +0x150.
dec = Cipher(algorithms.AES(RETAIL), modes.ECB()).decryptor()
sessao = dec.update(bytes(d[sec_off + 0x150:sec_off + 0x160])) + dec.finalize()
corpo = bytes(d[pe_off:])
corpo = corpo[:len(corpo) // 16 * 16]
dec = Cipher(algorithms.AES(sessao), modes.CBC(bytes(16))).decryptor()
d[pe_off:pe_off + len(corpo)] = dec.update(corpo) + dec.finalize()
struct.pack_into(">H", d, ffi + 4, 0)
open(out, "wb").write(d)
print("decifrado:", len(corpo), "bytes a partir de", hex(pe_off))
