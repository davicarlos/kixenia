# Jogos: análise em lote

Descompila todos os jogos de um HD (Kinect ou não) e gera uma nota por jogo no vault, com o que cada um pede do sistema e o que o Xenia ainda não tem.

| Script | Para quê |
|---|---|
| `lista_jogos.py <HD> catalogo.json` | Catálogo: para cada pasta, o que o Xenia abre (`default.xex`, cabeçalho GoD ou pacote XBLA), o título e uma abreviação |
| `pipeline.py <abrev> <alvo> [log]` | Um jogo: xex (cópia ou `--dump_xex` do Xenia para pacotes) → decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`) → importações com o estado no Xenia → funções do `.pdata` que chamam Kinect/XMsg/avatar/voz → Ghidra headless (`PdataFuncs`, `Labels`, `FnGraph`, sem análise automática) → execução a partir do log da bateria |
| `rodar_todos.py [paralelo] [abrev…]` | O catálogo inteiro: xex soltos em paralelo, pacotes um por vez (abrem o Xenia por ~30 s) |
| `runtime_nui.py` | Reconhece o runtime NUI embutido: a mesma função (tamanho, importações, mensagens XMsg) em 3 ou mais jogos |
| `gen_jogo.py <abrev>\|todos` | A nota do jogo no vault; o texto escrito à mão fica em `textos/<abrev>.py` (`NOME`, `TEXTO`, `FUNCOES`, `GRUPOS`, `RELACIONADO`) |
| `gen_indice.py` | O índice de todos os jogos e a nota de pendências (importações ausentes, stubs e aproximadas, por quantos jogos pedem) |
| `descrever.py` | Descrição automática de uma função decompilada (mensagens XMsg, importações, textos) |
| `mex_decifra.py <xex> <saída>` | Decifra um XEX de varejo (AES com a chave de sessão do security info) para o `xexinfo` descomprimir |

Precisa do `xexinfo` (lê o XEX, descomprime o LZX e lista base, entrada, seções e importações em JSON) e dos scripts do Ghidra em [../ghidra/](../ghidra/README.md). Os dados ficam em `games/decompilado/<abrev>/`: `<abrev>.img` (imagem plana), `<abrev>_imports.json`, `<abrev>_kinect.json`, `<abrev>_fngraph.json`, `<abrev>_runtime_nui.json`, `<abrev>_runtime.json` e `c_full/<endereço>.c`.
