# Scripts do Ghidra

Para rodar com o `analyzeHeadless` sobre um projeto já importado:

```
analyzeHeadless <pasta do projeto> <nome> -process <arquivo> -noanalysis -readOnly \
  -scriptPath <esta pasta> -postScript <Script.java> <argumentos...>
```

Importe os executáveis do Xbox 360 com `-processor PowerPC:BE:64:VMX128-32addr`: sem as instruções VMX128 o Ghidra corta funções. Como fazer isso e o que mais usei está em [ghidra-e-rea.md](../../../docs/kixenia/ghidra-e-rea.md).

| Script | Argumentos | O que faz |
|---|---|---|
| `Closure.java` | `<saída> <início> <fim> <raiz>...` | Descompila tudo o que as raízes alcançam dentro de um intervalo e grava `graph.json` com tamanhos e chamadas |
| `FnGraph.java` | `<lista de endereços> <saída>` | Descompila cada endereço da lista e grava quem chama e quem é chamado |
| `DecompAt.java` | `<saída> <endereço>...` | Cria funções nos endereços exatos e grava o C |
| `DecompIn.java` | `<saída> <endereço>...` | Grava o C das funções que contêm os endereços |
| `MakeDecomp.java` | `<endereço>` | Cria a função no endereço e imprime o C |
| `Dis.java` | `<endereço>...` | Mostra 4 instruções a partir de cada endereço, com os símbolos referenciados |
| `Dis2.java` | `<endereço> <n>` | Mostra n instruções a partir do endereço, e a função que o contém |
| `XrefsTo.java` | `<endereço>...` | Lista as referências para cada endereço |
| `FindScalar.java` | `<valor>...` | Procura instruções de comparação/carga com esses valores imediatos |
| `FindWord.java` | `<valor>...` | Procura palavras de 32 bits na memória |
| `Dump.java` | `<endereço> <tamanho>` | Imprime bytes em hexadecimal |
| `Floats.java` | `<endereço> <n>` | Imprime n palavras como inteiro e float |
| `ReadF.java` | `<endereço>...` | Imprime uma palavra como inteiro e float |
| `ReadS.java` | `<endereço>...` | Imprime a string C no endereço |
| `ReadWords.java` | `<lista> <saída>` | Lê a palavra de cada endereço da lista para um arquivo |
| `DumpImage.java` | `<saída>` | Grava a memória inicializada a partir de 0x82000000 (executável de jogo) |
| `DumpImageAt.java` | `<saída> <base>` | O mesmo, a partir de outra base (o xam fica em 0x815F0000) |
| `FixNoReturn.java` | | Desmarca como "noreturn" os auxiliares de salvar/restaurar registradores, que a análise completa marca errado |
| `Holes.java` | | Lista os buracos dentro das funções da `.pdata` que não têm instrução |
