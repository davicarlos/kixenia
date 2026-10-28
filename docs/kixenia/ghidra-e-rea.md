# Ghidra + REA
- Ghidra 12.1.4 headless, JDK 21, linguagem `PowerPC:BE:64:VMX128-32addr` (VMX128 como pcodeops `vmx128_*`). Scripts em `tools/` do projeto.
- Receita: `analyzeHeadless <dir> p -process nike.pe -noanalysis -scriptPath <scripts> -postScript DecompLista.java lista.txt saida.c`.
- Se der erro de versão de classe, apagar `~/.config/ghidra/*/osgi/compiled-bundles`.
- Leitura de XEX: `~/Kixenia/kinect-sdk-re/modulos/bin/xexinfo <xex> [imagem.bin]` (JSON com cabeçalho, importações, exports, recursos e seções; grava a imagem descomprimida). O leitor veio do antigo `x360emu`, que foi para a Lixeira em 2026-10-25. Ver 00-Modulos.
- Dump de instruções: pelo Ghidra MCP (`disassemble_bytes`, `read_memory`) ou pela imagem do `xexinfo`.

## Ghidra MCP (2026-10-25)
- **GhidraMCP 7.0.0** (bethington/ghidra-mcp, Apache-2.0) em `~/Documents/Codex/ghidra-mcp/`: servidor headless (`start-headless.sh`, porta 8089) e ponte MCP registrada no Claude Code como `ghidra`.
- Abre os projetos com o fluxo corrigido (`~/Kixenia/kinect-sdk-re/ghidra-projects/`: `xamproj`, `ksproj`, `nikeproj`) sem refazer a análise; decompila uma função em cerca de 1 s.
- REST: `POST /open_project`, `POST /open_program {auto_analyze:false}`, `GET /get_functions?function=0x…`, `GET /get_xrefs_to`.
- Usado para as 168 funções Kinect dos jogos, as 12 exports-atalho do xam e as 992 vizinhas. O REA ficou para confirmação independente de funções pontuais (a análise padrão dele não terminava nos jogos).
- Regra: chaves só via arquivo/variável de ambiente; nunca em notas ou commits.
