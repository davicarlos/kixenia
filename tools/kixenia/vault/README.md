# Exportação do vault

`exportar_publico.py <vault> <repositório>` gera as notas de [docs/kixenia/](../../../docs/kixenia/README.md) (pastas `kinect/`, `avatares/` e `xam/`) a partir do meu vault do Obsidian. Ele:

- escolhe as notas que podem ser publicadas e deixa de fora as que são quase só decompilação;
- remove os blocos de código descompilado que sobrarem, o cabeçalho YAML e os avisos de nota gerada;
- troca caminhos locais por marcadores;
- converte os links e os callouts do Obsidian para o formato do GitHub;
- gera os índices (`README.md`) das pastas.
