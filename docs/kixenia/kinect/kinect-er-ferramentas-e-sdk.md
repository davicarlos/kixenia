# Kinect, engenharia reversa: ferramentas, preparo dos binários e o SDK do Windows

Parte de [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 1, 2).

## 1. Ferramentas e preparo dos binários

- **REA** (com Ghidra 12.1.4 por baixo) para binários comuns. Ele não abre XEX nem cria funções num binário sem símbolos, então para os módulos do Xbox usei o Ghidra direto.
- **`xam.xex` (atualização 17559):**
  - base da imagem 0x815F0000;
  - o PE extraído está no layout de memória, então foi preciso corrigir os cabeçalhos de seção (deslocamento de raw = endereço virtual). Sem isso, o código saía deslocado 0x1400 bytes;
  - os exports são por ordinal (`exports.csv`);
  - a linguagem certa é `PowerPC:BE:64:VMX128-32addr`, porque sem ela as instruções VMX128 saem truncadas.
- **Scripts Ghidra** (`ghs/` no scratchpad):
  - `DecompAt.java`: cria funções num endereço exato;
  - `DecompIn.java`: decompila a função que contém um endereço;
  - `DumpImage.java`: despeja a imagem do Nike;
  - `XamFixFlow` e `XamNuiDecomp`: preparo do xam.
- **Lugares:**
  - `~/Kixenia/kinect-sdk-re/xam-nui/`: 93 funções decompiladas, `exports.csv` e `xam_17559.exe` corrigido;
  - `~/Kixenia/nike-decomp/`: projeto Ghidra `NikeTraining` e export completo em C (`export/src/nike_*.c`);
  - dump do Nike: `nikedump/DEFAULT.82000000.bin`, com base 0x82000000.
- **Achar chamadas** sem símbolos: varrer o dump procurando `bl` (opcode 18, LK=1) cujo alvo seja o thunk do import. A tabela de imports com os endereços dos thunks aparece no log do Xenia, nas linhas `F <slot> <thunk> <ordinal>`.

## 2. Windows SDK (para comparar)

### `kinectcamera_x64.sys` (driver da câmera, v1.6.0.476)
- **Inicialização:** escreve registradores numa sequência fixa, a mesma do libfreenect. Os únicos a mais são o 0x15 e o 0x2EF, do *near mode*, que só existe no Kinect for Windows.
- **Pacotes:** começam com "RB" e um marcador de início, meio ou fim de quadro (0x71/0x72/0x75 para profundidade, 0x81/0x82/0x85 para cor). Quadros incompletos são marcados com 0xbad0 e descartados.
- **Horário:** carimbado com o relógio do PC, com a cor adiantada em 3,6 ms. No Xbox, o runtime do jogo usa o próprio relógio (mftb).
- **Conclusão:** o Xenia lê a câmera do mesmo jeito. Perda de pacotes e horário foram descartados como causa de problemas de rastreamento.

### `Kinect10_x64.dll` (rastreamento de esqueleto)
Pipeline, em ordem:
1. **PipelineSource:** recebe só a profundidade 640×480, o contador de quadros, os parâmetros e os IDs escolhidos. Não usa cor, horário nem acelerômetro.
2. **CDRP:** alinha cor e profundidade, só para a API de mapeamento de coordenadas.
3. **BGR (remoção de fundo):** separa até 6 pessoas. Rastreia por completo no máximo 2; as outras ficam só com posição. Estima o tamanho de cada pessoa pela área do corpo.
4. **HD (detector de cabeça):** roda na CPU, numa profundidade reduzida a 80×60, com o banco `HeadPosition.mshdf`.
5. **Exemplar:** a floresta de decisão que rotula cada pixel com uma parte do corpo (`database.wsdf`). No Xbox essa etapa roda na **GPU**, e foi ela que a correção de resolve do Edge consertou.
6. **Centroids** e **ModelFitting:** articulações rastreadas ou inferidas, nos modos em pé e sentado.

Conclusões:
- O rastreamento não depende do acelerômetro: o chão vem da profundidade.
- A distância focal de projeção é a mesma do Tuner, 285,63 px em 320×240.
