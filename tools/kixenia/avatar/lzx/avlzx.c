/* Descomprime um bloco comprimido de asset de avatar (STRB blocos 2 e 3), como o
 * avatarcore do xam 17559 (0x81976888 / 0x819767E8): quadros com um cabeçalho de 12
 * bytes little-endian {tamanho comprimido, deslocamento na saída, tamanho
 * descomprimido}, cada um LZX com janela de 32 KB e o contexto reiniciado a cada
 * quadro (LDIResetDecompression). Usa o lzxd.c que o Xenia embute.
 * uso: avlzx <arquivo> <deslocamento> <tamanho> <saída>
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mspack.h"
#include "lzx.h"

struct memfile {
  struct mspack_system sys;
  unsigned char* buffer;
  off_t size;
  off_t offset;
};

static int mem_read(struct mspack_file* f, void* b, int n) {
  struct memfile* m = (struct memfile*)f;
  off_t left = m->size - m->offset;
  off_t t = n < left ? n : left;
  memcpy(b, m->buffer + m->offset, t);
  m->offset += t;
  return (int)t;
}
static int mem_write(struct mspack_file* f, void* b, int n) {
  struct memfile* m = (struct memfile*)f;
  off_t left = m->size - m->offset;
  off_t t = n < left ? n : left;
  memcpy(m->buffer + m->offset, b, t);
  m->offset += t;
  return (int)t;
}
static void* mem_alloc(struct mspack_system* s, size_t n) { return calloc(n, 1); }
static void mem_free(void* p) { free(p); }
static void mem_copy(void* s, void* d, size_t n) { memcpy(d, s, n); }

static uint32_t le32(const unsigned char* p) {
  return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24);
}

int main(int argc, char** argv) {
  if (argc != 5) {
    fprintf(stderr, "uso: avlzx <arquivo> <deslocamento> <tamanho> <saída>\n");
    return 2;
  }
  FILE* f = fopen(argv[1], "rb");
  if (!f) return 2;
  long off = strtol(argv[2], NULL, 0), len = strtol(argv[3], NULL, 0);
  unsigned char* in = malloc(len);
  fseek(f, off, SEEK_SET);
  if (fread(in, 1, len, f) != (size_t)len) return 2;
  fclose(f);

  struct mspack_system sys = {0};
  sys.read = mem_read;
  sys.write = mem_write;
  sys.alloc = mem_alloc;
  sys.free = mem_free;
  sys.copy = mem_copy;

  size_t out_cap = 0;
  unsigned char* out = NULL;
  long p = 0;
  int frames = 0;
  while (p + 12 <= len) {
    uint32_t csize = le32(in + p), doff = le32(in + p + 4), dsize = le32(in + p + 8);
    p += 12;
    if (!csize || csize > 0x9800 || !dsize || dsize > 0x8000 || p + csize > len) {
      fprintf(stderr, "quadro %d inválido: c=%#x o=%#x d=%#x\n", frames, csize, doff, dsize);
      return 1;
    }
    if (doff + dsize > out_cap) {
      out_cap = doff + dsize;
      out = realloc(out, out_cap);
    }
    struct memfile mi = {sys, in + p, csize, 0};
    struct memfile mo = {sys, out + doff, dsize, 0};
    struct lzxd_stream* s = lzxd_init(&sys, (struct mspack_file*)&mi,
                                      (struct mspack_file*)&mo, 15, 0, 0x8000, 0, 0);
    lzxd_set_output_length(s, dsize);
    int r1 = lzxd_decompress(s, dsize - 1), r2 = r1 ? r1 : lzxd_decompress(s, 1);
    lzxd_free(s);
    if (r1 || r2 || mo.offset != (off_t)dsize) {
      fprintf(stderr, "quadro %d falhou (erro %d/%d, saída %ld de %#x)\n", frames, r1, r2,
              (long)mo.offset, dsize);
      return 1;
    }
    p += csize;
    frames++;
  }
  FILE* o = fopen(argv[4], "wb");
  fwrite(out, 1, out_cap, o);
  fclose(o);
  printf("%d quadros, %zu bytes, sobra %ld\n", frames, out_cap, len - p);
  return 0;
}
