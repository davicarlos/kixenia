# Avatares do Xbox 360

Como o sistema entrega os avatares aos jogos, o que o Xenia Edge faz hoje e o que muda nos jogos Kinect. Análise do `xam.xex` da atualização 17559 (projeto Ghidra `~/Kixenia/kinect-sdk-re/ghidra-projects/xamvmx`, linguagem VMX128, sem análise automática; decompilação em `~/Kixenia/kinect-sdk-re/xam-nui/avatar/`).

Relacionado: [Kinect-Sports-Season-Two](../kinect/jogos/kinect-sports-season-two.md), [Kinect-Sports](../kinect/jogos/kinect-sports.md), [Kinect-Adventures](../kinect/jogos/kinect-adventures.md), [Kinect-Joy-Ride](../kinect/jogos/kinect-joy-ride.md), [Kinect-Xenia-Edge](../kinect/kinect-xenia-edge.md), [Funcoes-XAM-Indice](../xam/funcoes/README.md), AvatarEditor.xex, Guide.AvatarMiniCreator.xex.

## Arquitetura

As exports `XamAvatar*` quase não fazem nada sozinhas: cada uma manda uma **mensagem a um app do sistema**, e são dois, ambos **embutidos no próprio xam** (tabela de apps do sistema em 0x815F91F0, 16 bytes por app: nome, ID, tratador, flags):

| App | Nome | Tratador | Papel |
|---|---|---|---|
| 0xF2 | `avatarcore.xex` | 0x81965AC0 | partes do corpo, roupas, texturas, animações, avatares aleatórios |
| 0xF3 | `avatarsyscore.xex` | 0x81981740 | o avatar (manifesto) de cada perfil |

`XamAvatarInitialize` (0x816755E8) carrega o avatarcore com `XamLoadSysApp(0xF2, …)` (0x816BD7A0) **na memória que o próprio jogo cede**: a área começa em `unk5 + 1 MB` e tem `unk6 − 1 MB` (os jogos passam 3 MB em `unk6`, 0x00300000). Depois manda 0x600001 com os 5 ponteiros de função que o jogo passou e 0x28 bytes de parâmetros. Se algo falhar, descarrega o app.

O avatarcore trata três grupos de mensagem (0x81965AC0):
- **0x600001 (iniciar)**: 0x819652F8.
- **0x600005 e 0x60000A (síncronas)**: tratadores 0x81964D00 e 0x81964CE0, chamados por 0x819653C0.
- **Demais (assíncronas)**: 0x819659D0 cria um item de trabalho de 0x48 bytes e o põe na fila (0x819658E8) de uma thread de trabalho; o tratador de cada mensagem vem de uma tabela (0x81964F70 → descritores em 0x81632D78).

## Exports e mensagens

`XMsgStartIORequest` = 0x816A2CF0 (assíncrono, com XOVERLAPPED), `XMsgProcessRequest` = 0x816A2F88 (síncrono).

| Export | Ordinal | App | Mensagem | Tratador no app | Xenia |
|---|---|---|---|---|---|
| `XamAvatarInitialize` | 1500 | 0xF2 | carrega o app + 0x600001 | 0x819652F8 | stub: falha (`~0`) a menos que `allow_avatar_initialization` |
| `XamAvatarShutdown` | 1501 | 0xF2 | 0x600002 / descarrega | — | stub |
| `XamAvatarGetManifestLocalUser` | 1502 | 0xF3 | 0x600003 | avatarsyscore | stub |
| `XamAvatarGetManifestsByXuid` | 1503 | 0xF3 | 0x600003 | avatarsyscore | stub |
| `XamAvatarGetAssetsResultSize` | 1504 | 0xF2 | 0x600005 (síncrona) | 0x81964D00 → 0x81965EC0 | stub |
| `XamAvatarGetAssets` | 1505 | 0xF2 | 0x600006 | 0x81964D30 → 0x81969848 | stub |
| `XamAvatarGenerateMipMaps` | 1506 | 0xF2 | 0x600007 | 0x81964E20 | stub |
| `XamAvatarGetAssetBinary` | 1525 | 0xF2 | 0x600008 | 0x81964D80 | — |
| `XamAvatarSetManifest` | 1508 | 0xF3 | 0x600009 | avatarsyscore | — |
| `XamAvatarSetCustomAsset` | 1507 | 0xF2 | 0x60000A (síncrona) | 0x81964CE0 | stub |
| `XamAvatarGetAssetIcon` | 1523 | 0xF2 | 0x60000B | 0x81964DD0 | — |
| `XamAvatarBeginEnumAssets` | 1512 | 0xF2 | 0x60000C | 0x81964EA8 | — |
| `XamAvatarEnumAssets` | 1514 | 0xF2 | 0x60000D | 0x81964EF0 | — |
| `XamAvatarEndEnumAssets` | 1513 | 0xF2 | 0x60000E | 0x81964F28 | — |
| `XamAvatarLoadAnimation` | 1511 | 0xF2 | 0x60000F | 0x81964E70 → 0x81968390 | stub |
| `XamAvatarGetMetadataRandom` | 1515 | 0xF2 | 0x600010 | 0x81964F30 → 0x8196B5C8 | stub |
| `XamAvatarGetMetadataSignedOutProfileCount` | 1518 | 0xF3 | 0x600013 | avatarsyscore | stub |
| `XamAvatarGetMetadataSignedOutProfile` | 1519 | 0xF3 | 0x600014 | avatarsyscore | stub |
| `XamAvatarWearNow` | 1524 | 0xF3 | 0x600018 | avatarsyscore | — |
| `XamAvatarManifestGetBodyType` | 1521 | — | lê o manifesto direto (0x8173EE60, 0x8196CA80) | — | stub |

No Xenia Edge são 24 funções de avatar, todas stubs (`src/xenia/kernel/xam/xam_avatar.cc`).

## Partes do avatar (13 categorias)

`XamAvatarGetAssetsResultSize` calcula o tamanho do resultado (0x81965EC0): 0x1F80 bytes fixos (mais 0x3147C com a flag 0x1000) e, para cada categoria pedida, o cabeçalho e os dados alinhados. A tabela fica em 0x816332B8 (16 bytes por categoria: índice, máscara, cabeçalho, dados):

| Categoria | Máscara | Cabeçalho | Dados |
|---|---|---|---|
| 0 | 0x00010000 | 0x800 | 0x53800 |
| 1 | 0x00020000 | 0x400 | 0x12C00 |
| 2 | 0x00040000 | 0x800 | 0xE000 |
| 3 | 0x00080000 | 0x800 | 0x14400 |
| 4 | 0x00100000 | 0x800 | 0xF000 |
| 5 | 0x00200000 | 0x800 | 0xD400 |
| 6 | 0x00400000 | 0x800 | 0xA400 |
| 7 | 0x00800000 | 0x800 | 0x6800 |
| 8 | 0x01000000 | 0x800 | 0x8400 |
| 9 | 0x02000000 | 0x800 | 0x5C00 |
| 10 | 0x04000000 | 0x800 | 0x5800 |
| 11 | 0x08000000 | 0x800 | 0x5C00 |
| 12 | 0x10000000 | 0x800 | 0x19000 |

Qual categoria é qual parte (corpo, cabeça, camisa…) ainda não foi mapeado; a 0, a maior, deve ser o corpo.

`XamAvatarGetAssets` manda 1000 bytes do manifesto do avatar e, a partir de +1000: uma máscara de 16 bits com as partes pedidas e três ponteiros (os buffers do resultado); o avatarcore resolve em 0x81969848.

## O manifesto (1000 bytes)

O avatar de um perfil é um **manifesto** de 1000 bytes (0x3E8): o que o avatarsyscore devolve em `GetManifestLocalUser` e o que o jogo manda em `GetAssets`. Campos vistos até agora (funções de acesso em 0x8196C588–0x8196C600):

| Deslocamento | Tamanho | Conteúdo | Onde |
|---|---|---|---|
| +0x120 | 16 | ID do **corpo** | `ManifestGetBodyType` (0x8196CA80) compara com os IDs em 0x81632E68 e 0x81632E78; 0x8196C5C0 |
| +0x140 | 16 | outro ID de parte; tem de ser não nulo | 0x8196C5D0 |
| +0x380 | 8 | identificador que o xam usa para buscar o avatar num serviço (`*piRam81a81114 + 0xC`) | 0x8196C588, 0x8196C5E0 |
| +0x387/+0x388 | 5 | marca: zerada, o `GetAssets` busca pelo identificador de +0x380 (flag 0x1000000); senão usa o manifesto (flag 0x800000) | 0x8196C598, 0x8196C5F0 |

IDs de corpo (16 bytes): masculino `00000002 00000001 C1C8F109 A19CB2E0` (`ManifestGetBodyType` devolve 1), feminino `00000002 00010002 C1C8F109 A19CB2E0` (devolve 2); outro valor, 0. O sufixo `C1C8F109 A19CB2E0` é comum a todos os assets do pacote do sistema.

## Como o `GetAssets` monta o resultado

0x81969848: lê a marca do manifesto (0x8196C598), reserva um contexto de 0x2E210 bytes (0x81967C10 → 0x819686B8), escolhe a origem (flag 0x1000000 pelo identificador de +0x380, ou 0x800000 pelo manifesto) e começa o carregamento (0x81969350: guarda as partes pedidas em +0x11A2 do contexto, as flags em +0x11A8 e o tipo de corpo em +0x11A0; exige os IDs de +0x120 e +0x140 diferentes do ID nulo em 0x81618000; prepara o resultado com 0x81966030 e começa com 0x81968D88). Depois repete 0x81969490 enquanto ele devolver "pendente" (0x8000000A), consultando o cancelamento do jogo a cada volta, e libera o contexto (0x819692C0, 0x81967D30).

## De onde vêm as partes: o pacote `AvatarAssetPack`

O avatarcore abre, ao iniciar (0x8196B728):
1. monta o pacote **`\System??\AvatarAssetPack`** (0x816EFD80, nome em 0x81632E38);
2. abre `AvatarAssetPack:\AvatarAssetPack.toc` (formato `%s:\%s` em 0x815FCAE8) com o leitor 0x8197E238 → objeto global 0x81B4B1FC, que deve ter versão 2 ou 1 (método +0x24; +0x30 inicia);
3. se a versão não bater, abre também `AvatarAssetPackLegacyV1.toc` com o leitor 0x8197D400 → objeto 0x81B4B200.

O desligamento (0x8196B5F0) libera os dois objetos e desmonta o pacote. `GetMetadataRandom` é o método +0x34 do objeto 0x81B4B1FC (0x8196B5C8).

**Os jogos trazem esse pacote.** O arquivo `AvatarAssetPack` na pasta do jogo é um pacote **PIRS assinado pela Microsoft**, idêntico no Adventures, no Joy Ride e no Nike (MD5 `c9465b61a52e65b1965702d16d6f3aca`, 13,8 MB): é o pacote de avatares do sistema, que os jogos levam para instalar no console. Dentro dele há um único arquivo, **`AvatarAssetPack.toc`** (13 661 900 bytes; extraído em `~/Kixenia/kinect-sdk-re/avatar/pack-adventures/`). Não há `AvatarAssetPackLegacyV1.toc`.

### Formato do `.toc`

Leitor no xam: objeto composto de 0x9C bytes (0x8197E190 → construtor 0x8197E020, cinco vtables a partir de 0x81634384) que junta três componentes (0x8197DB78): o leitor do arquivo (0x8197D2F0 → objeto de 0x1B0 bytes, vtables 0x81634198/0x81634170/0x81634164/0x81634188), um de 0x81981290 e um de 0x81980960. O leitor abre o arquivo (0x8197D060 → 0x8197E718) e lê o cabeçalho (0x8197CA98); a leitura em si é um `ReadAt(arquivo, deslocamento, buffer, tamanho)` (0x8197E4F8).

**Três versões**, pelo GUID dos primeiros 16 bytes (0x8197CA98, 0x8197CC18):

| GUID | Cabeçalho | Entrada *i* | Tamanho da entrada |
|---|---|---|---|
| `9AD6EBCE-6262-4EEB-8A82-A3F70B817369` (v1) | 0x20 bytes | `0x20 + i × 0xF8` | 0xF8 |
| **`8A762DF4-C067-4B66-B4EE-56466B1B8280` (v2, o pacote dos jogos)** | 0x28 bytes | `0x28 + i × 0xF8` | 0xF8 |
| `580A07D6-4BCD-40BE-BF37-254BE826FB0B` (v3) | 0x30 bytes | `0x30 + i × 0x10C` | 0x10C |

**Cabeçalho v2** (lido para +0x168 do leitor; métodos 0x8197C0F8, 0x8197C150, 0x8197C1A8 devolvem +0x180/+0x184/+0x188):

| Deslocamento | Valor no pacote | Conteúdo |
|---|---|---|
| 0x00 | GUID | versão |
| 0x10 | `01C8F109 A19CB2E0` | base dos IDs |
| 0x18 | 2 | ? |
| 0x1C | **0x502 = 1 282** | **número de entradas** (a enumeração 0x8197CE48 vai até ele) |
| 0x20 | 2 | ? |
| 0x24 | 0 | ? |

**Entrada v2 (0xF8 bytes)**; o leitor a converte para um registro de 0x10C bytes (0x8197E428) e a enumeração entrega ao chamador registros de 0x2C4 bytes:

| Deslocamento | Tamanho | Conteúdo |
|---|---|---|
| +0x00 | 4 | **máscara da parte do avatar** (ver abaixo) |
| +0x04 | 4 | `01010100`, `02010100`, `01000000`… |
| +0x08 | 0x90 | dados (zeros na maioria) |
| +0x98 | 4 | número do asset (0x193, 0x2711…) |
| +0x9C | 16 | **ID do asset**: `01000000 03C00001 C1C8F109 A19CB2E0` (roupas, 266 com tipo 0x01), `00000004 01AF0003 C1C8F109…`; 881 entradas têm o ID zerado e são identificadas pelo número de +0x98 |
| +0xAC | 16 | zeros em todas |
| +0xBC | 13 × 4 | deslocamentos dos **nomes do asset em 13 idiomas** (UTF-16 big-endian, na área 0x4DA18–0x83BCC); a conversão troca os zeros pelo segundo valor |
| +0xF0 | 4 | **início do bloco de dados** do asset |
| +0xF4 | 4 | **tamanho do bloco** |

Os blocos são contíguos de 0x83BCC até o fim do arquivo (0xD076CC). **Todos começam com `STRB`** (`53545242`), o mesmo formato que os jogos mandam em `XamAvatarSetCustomAsset` (o [Kinect-Sports](../kinect/jogos/kinect-sports.md) manda três: tamanhos 0x254, 0x2A0, 0x238). Exemplo: a entrada 44 (máscara 0x20, número 0x193, ID `01000000 03C00001…`) é "Moccasins" / "モカシン" / "Mokkasins", bloco em 0x1B9528 com 0x5B70 bytes.

**Máscara da parte** (+0x00) e quantas entradas há de cada uma no pacote (nomes deduzidos dos assets, a confirmar):

| Máscara | Entradas | Parte provável |
|---|---|---|
| 0x01000000 | 230 | sem ID; animações ou traços do corpo |
| 0x00000000 | 203 | sem ID nem dados (bloco vazio) |
| 0x00000004 | 178 | cabelo |
| 0x00000008 | 90 | camisa (parte de cima) |
| 0x00000010 | 54 | calça (parte de baixo) |
| 0x00000208 | 47 | parte de cima + 0x200 (vestido/conjunto) |
| 0x00002000 | 45 | ? (IDs `00040000…`) |
| 0x00040000, 0x00400000 | 45, 41 | traços do rosto (sem ID) |
| 0x00000020 | 36 | **sapatos** (confirmado: "Moccasins") |
| 0x00000040, 0x00000100, 0x00000400, 0x00000800 | 34, 34, 34, 16 | acessórios (chapéu, óculos, brincos, anel?) |
| 0x00800018 | 15 | roupa inteira (parte de cima + de baixo + 0x800000) |

Interpretador: `~/Kixenia/kinect-sdk-re/avatar/toc.py` (gera `toc.json` com as 1 282 entradas).

Falta: como os IDs do manifesto (`00000002 00000001 …` dos corpos) chegam às entradas sem ID (pelo número de +0x98?), e a área de +0x08 que algumas entradas usam.

## Formato STRB ("Structured Binary")

Cada asset do pacote é um STRB; o nome vem da mensagem de erro em 0x816331EC ("Location not recognized as a Structured Binary", e "Structured Binary version too high" em 0x8163321C). O xam o grava em 0x8196DA48 e o lê em 0x8196DBE8; os blocos são percorridos por 0x8196DF68 e 0x8196E0E8, e os campos lidos com a largura que o cabeçalho manda (0x8196DD88: 1, 2, 4 ou 8 bytes, trocando a ordem dos bytes com os descritores `s`, `i`, `ll` de 0x81633240–0x81633248).

**Cabeçalho** (versão 1, a do pacote):

| Deslocamento | Conteúdo |
|---|---|
| 0x00 | `STRB` |
| 0x04 | versão (0 ou 1; o leitor recusa acima de 1) |
| 0x05 | flag de ordem de bytes (0 no pacote) |
| 0x06 | 16 bytes: GUID do formato, `59BDE08B-4909-4EE4-B991-ADA87B7C0B6B` em todos os assets |
| 0x16 | largura do **tipo** de cada bloco: 1 |
| 0x17 | largura do **tamanho** (e do terceiro campo) de cada bloco: 4 |
| 0x1A | **alinhamento**: 4 |
| 0x1C | número do asset (por exemplo 0x025A, 0x02AF) |
| alinhar(0x1D) = 0x20 | primeiro bloco |

**Blocos**: tipo (1 byte), tamanho (4 bytes, big-endian), um terceiro campo (4 bytes, sempre 1 no pacote); o corpo começa no próximo múltiplo do alinhamento (0x0C) e o bloco seguinte em `corpo + alinhar(tamanho)`. Os 1 079 assets com dados do pacote são lidos assim do começo ao fim, sem sobra (`~/Kixenia/kinect-sdk-re/avatar/strb.py`).

**Tipos de bloco** e onde aparecem:

| Bloco | Em quais assets | Provável conteúdo |
|---|---|---|
| 8 (15 bytes, sempre o primeiro) | todos | descritor do asset (`02 03 01 00 00 00 04 …`, `02 01 03 …`) |
| 3, seguido de 7 nas roupas | cabelo (0x4), camisa (0x8), calça (0x10), sapatos (0x20), 0x40, 0x100, 0x400, conjuntos (0x208) | **modelo 3D**; o 7 (28 bytes, `01000000 000000FF …`) vem com as roupas, talvez cores |
| 2 | 0x2000, 0x4000, 0x8000, 0x20000, 0x40000 (traços do rosto) | **texturas** |
| 4 | 0x01000000 (230), 0x80000 (18) | provavelmente **animações** (`XamAvatarLoadAnimation`) |
| 1 | 0x400000 (41) | ? (`6C000000 0080A041 47000000 …`) |

Os blocos 2 (texturas do rosto) e 3 (modelo) são **LZX com janela de 32 KB**, em quadros: cada quadro tem um cabeçalho de 12 bytes little-endian `{tamanho comprimido, deslocamento na saída, tamanho descomprimido}` e o contexto do LZX é reiniciado a cada quadro (avatarcore 0x81976888 / 0x819767E8, que chamam `LDIResetDecompression` e `LDIDecompress` do kernel). Confirmado: os 585 blocos de modelo e os 187 de textura do pacote descomprimem sem erro com o `lzxd.c` que o Xenia embute (ferramenta `kinect-sdk-re/avatar/lzx/avlzx`, saída em `avatar/descomprimido/`).

Falta: o significado dos 15 bytes do bloco 8 e do bloco 7, e o formato das animações (bloco 4).

## Do bloco descomprimido aos buffers do jogo (0x81974A90)

O `GetAssets` (0x81966450) soma os tamanhos das partes pedidas, reserva o buffer de resultado (alinhado a 16) e o de GPU (alinhado a 4096) do jogo, abre o bloco descomprimido como um fluxo (0x81976080, 0x819766B0) e chama **0x81974A90**. Não é uma conversão pesada: é um desserializador de campos empacotados em bits, LSB primeiro (0x8197A388 lê `n` bits a partir do bit `k`; 0x81970E40 é a versão de 16 bits), que copia os dados para os dois buffers corrigindo ponteiros. O formato abaixo foi conferido: o decodificador em Python (`kinect-sdk-re/avatar/malha.py`) consome **os 585 blocos de modelo do pacote até o último byte**, e a malha dos mocassins renderizada (`render_obj.py`) sai certa.

### Ordem do fluxo

```
cabeçalho 0x30 → malhas (N) → texturas (M) → costuras
```

**Cabeçalho** (12 × 32 bits): bytes usados no resultado, bytes usados na GPU, tamanho da área de texturas na GPU, tamanho dos vértices, tamanho dos índices, N, M, deslocamento dos vértices na GPU, dos índices na GPU, das malhas no resultado, das texturas no resultado, tamanho mínimo. O contexto de 0x34 bytes do componente guarda os 7 primeiros e os ponteiros: +0x1C resultado, +0x20 GPU, +0x24/+0x28 vértices/índices, +0x2C/+0x30 malhas/texturas no resultado.

### Malha (0x819747D8, registro de 0x200 bytes no resultado)

- Cabeçalho de 0x21 bytes: 32 bits (?), 5 bits = nº de subconjuntos, triângulos, vértices, tipo de vértice, stride, ?, deslocamento dos vértices, dos índices.
- Subconjuntos (0x81971408, 0x18 bytes cada, até 20, em +4 do registro): `{tipo, id, …}`. Tipo 1 = faixa de índices `{a16, b16, c32}`; os outros = `{id, cor RGBA float}`, a cor que 0x819709E0 pinta a partir das cores do manifesto (ids 0x16–0x18 são cores calculadas depois em 0x81966450).
- **Vértices**: tipos 1 a 7, stride `0x1C + 4·tipo`. Cabeçalho de 0x1B2 bits: contagem, escala, origem xyz, 3 larguras de 6 bits, e 4 descritores `{bits, soma}` (em bits 0xD2/0xB2, 0x112/0xF2, 0x152/0x132, 0x192/0x172). Cada vértice: posição quantizada, depois 4 campos `valor + soma` (+0x0C normal empacotada, +0x10, +0x14, +0x18), depois `2·tipo` valores de 16 bits (UVs em half float).
- **Posição** numa rede hexagonal compacta (0x81970FA0): `y = qy·s·1,633 + oy`, `z = qz·s·√3 + oz (+ s·√3/3 se qy ímpar)`, `x = qx·s·2 + ox (+ s se qy⊕qz ímpar)`. Os passos {2, 2·√(2/3), √3} são calculados em 0x81A70F50 e guardados em 0x81A975C0.
- Se o modo de coordenadas (0x81A975E0, 2º argumento do `Initialize`) não for 1, inverte o sinal de z e dos 10 bits altos da normal.
- **Índices** (0x81971658/0x81971730): cabeçalho de 8 bytes `{contagem, soma16, bits16}`, cada índice com `bits` bits mais a soma, gravado em 16 bits.
- No pacote: tipos de vértice 3 (509 malhas), 2 (116) e 6 (2).

### Textura (0x81978680, registro de 0x2C bytes)

8 bytes `{deslocamento na GPU, tamanho}` e cabeçalho de 0x21 bytes: formato, largura, altura, tamanho, tamanho da fatia, nº de fatias, 1 bit "vazia", 1 bit "precisa de tiling", bytes por linha e nº de linhas. Depois vêm as linhas de blocos. O tamanho e o tiling vêm de duas funções de uma tabela em 0x81A975CC. Por padrão ela aponta para o XGraphics que o próprio xam traz (0x81977AB8 → 0x817F7BC8/0x817F7718; 0x81977E38 → 0x817F87C8 = tiling), e o jogo pode trocá-la no `Initialize` (0x81977C80). Formatos no pacote: 0x1A200054 (DXT5, 1191), 0x1A200152 (222), 0x1A200052 (DXT1, 144), 0x1A200154 (51). 272 texturas pedem tiling na carga.

### Costuras (0x81971CA8)

Blocos `{contagem, soma, bits}` (até 400 valores cada) de pares (origem, destino) de deslocamentos no vertex buffer. Para cada par, copia a posição e os 8 bytes de +0x10 de um vértice para o outro. Um bloco de contagem ímpar encerra a lista.

### Estrutura de topo do resultado (0x81967628)

No início do buffer de resultado fica um cabeçalho R de 0x14 bytes: ponteiro para um bloco de 0x1B10 bytes, ponteiro para o corpo (0x5C, só com o bit 0x1000), nº de partes, ponteiro para 13 cópias de 0x20 bytes dos registros de parte do manifesto, e ponteiro para 13 contextos de componente de 0x34 bytes. Com o corpo, o xam também reserva 0x1B10 + 0x5990 + 0x28000 bytes, o que dá os 0x3147C fixos (**corrigido** no commit f59d22cb0: o valor é atribuído, não somado aos 0x1F80).

## Do manifesto à lista de peças

**Layout do manifesto (M, 0x3E8 bytes):** `M+0x0C + i·0x10` (i<3) IDs de forma; `M+0x3C + i·0x20` (i<6) registros de textura do rosto; `M+0xFC + i·4` cores D3DCOLOR; `M+0x120` e `M+0x140` as duas partes do corpo; `M+0x160 + i·0x20` até 13 roupas `{ID16, máscara16, 3 cores}` (a contagem é o nº de IDs não nulos seguidos, 0x8196C650); `M+0x300 + k·0x20` os padrões das 4 categorias obrigatórias (máscaras 0x20, 0x10, 0x8, 0x4, tabela 0x816331A0); `M+0x380` identificador de 64 bits.

**Fila do `GetAssets`** (0x81968D88): corpo A se a máscara dele cruza a pedida (cores 0x16 = cor 0 do manifesto e 0x1B = pele); corpo B (cores 0xD–0x15 = cores 0–8); as roupas cuja máscara cruza a pedida (cores 0x16–0x18 = as 3 do registro, mais a pele); com o corpo B, também os 6 slots de textura do rosto (tipo 2) e as 3 formas (tipo 3). Cada peça vira um contexto de 0x1A0 bytes (0x81968790), com até 10 cores `{float4, id}`. A cor sai de `vupkd3d` + `madd(32896,5, v, −32896,5)` = canal/255.

**De onde vem cada ID** (0x8196A3F0, pelo nibble `(id >> 8) & 0xF`): 0 = pacote do sistema; 1–2 = conteúdo do título/marketplace; 0xF = asset do próprio jogo.

**IDs do pacote:** `{máscara32, índice16, tipo de corpo16}` + `C1C8F109A19CB2E0`. **O índice é a posição da entrada no TOC** (sem +1), e a máscara da entrada (+0) tem que bater com a palavra 0 do ID. O campo de 16 bytes em +0x9C da entrada não é o ID dela (só 401 de 1282 têm algo ali). Os corpos fixos são `00000002 0000 0001` (masculino, entrada 0) e `00000002 0001 0002` (feminino, entrada 1), em 0x81632E68/0x81632E78.

**Descritor do asset** (bloco 8 do STRB, 0x8196EE50): `{versão 2, tipo de corpo, tipo do asset (1 componente, 2 textura, 3 forma), categoria/slot, ?, versão de skin}`. A versão de skin esperada é `2 − (v < 3)`, onde `v` é o 1º argumento do `XamAvatarInitialize` (global 0x81B4B238); o 2º argumento é o modo de coordenadas (0x81A975E0; diferente de 1 inverte z).

## `XamAvatarSetCustomAsset` (handler 0x81965B98)

**Recebe 6 argumentos:** tamanho, dados STRB, nº de cores, cores, slot habilitado e **manifesto**. O Xenia declarava 5. **Altera o manifesto do jogo**: monta um ID com a categoria na palavra 0, origem 0xF no byte 6, tipo de corpo no byte 7, tamanho nos bytes 8–11 (byte 8 = 0xC0) e **o endereço do STRB** nos bytes 12–15. O `GetAssets` lê o asset direto da memória do jogo.
- **Tipo 1**: registro `{ID, máscara = categoria, 3 cores (padrão FF0000/00FF00/0000FF ou as do jogo)}`. Vira o padrão da categoria, se ela for obrigatória; remove as roupas que colidem; entra na lista; os padrões que ficaram descobertos voltam (0x8196D1B0).
- **Tipo 2**: o slot 0–5 vira a máscara 0x8000/0x2000/0x4000/0x10000/0x40000/0x20000 e vai para `M+0x3C + slot·0x20`.
- **Tipo 3**: o slot 0–2 vira 0x100000/0x80000/0x200000 e vai para `M+0x0C + slot·0x10`.

## Porte para o Xenia (2026-10-27)

Commit `1768622c0` (worktree `kinect-sistema`, local): `src/xenia/kernel/xam/xam_avatar_assets.{h,cc}`.
- abre `game:\AvatarAssetPack` (pacote PIRS numa pasta do host) e lê o `AvatarAssetPack.toc`;
- `SetCustomAsset` edita o manifesto como o xam;
- `GetAssets` monta a fila, acha cada peça, monta R/partes/contextos e decodifica malhas, texturas (com tiling via `texture_address::Tiled2D` quando pedido) e costuras nos buffers do jogo, e pinta as cores.

**Teste (Kinect Sports, replay de 90 s, avatares ligados):** as 6 peças do tipo 1 do manifesto do jogo são montadas: corpo (0x2) e cabeça (0x1) e cabelo (0x4) do pacote do sistema, e camisa (0x8), calça (0x10) e sapatos (0x20), que são os assets próprios do jogo. O `GetAssets` agora devolve sucesso, sem travar; o jogo segue e rastreia o jogador. **Ainda não foi conferido na tela.**

**Falta:** texturas do rosto e formas (tipos 2 e 3, 0x819672F8/0x81966138, com os codecs 0x8197A078/0x8197A710); o corpo 0x1000 (esqueleto e animações, 0x81966C98/0x81977310); a validação da paleta (bloco 7, 0x81976598); o remapeamento de skin 0x819713A8 (só para assets de outra versão); pacotes dentro de imagens de disco.

## O que acontece nos jogos Kinect (levantamento de 2026-10-27)

Cada jogo rodou 90 s com a gravação `kinect_rec_aovivo2`, com `allow_avatar_initialization` desligada (o padrão) e ligada:

| Jogo | Avatares desligados (padrão) | Avatares ligados |
|---|---|---|
| [Kinect-Adventures](../kinect/jogos/kinect-adventures.md) | ✅ roda; chama `Initialize`, recebe falha e `Shutdown` | ✅ roda |
| [Kinect-Joy-Ride](../kinect/jogos/kinect-joy-ride.md) | ✅ roda (idem) | ✅ roda |
| [Kinect-Sports](../kinect/jogos/kinect-sports.md) (4D5308C9) | ✅ roda | ❌ **trava**: lê ponteiro nulo em threads de trabalho depois de `GetManifestLocalUser`, `SetCustomAsset` e `ManifestGetBodyType` (✅ corrigido, ver abaixo) |
| [Kinect-Sports-Season-Two](../kinect/jogos/kinect-sports-season-two.md) | ❌ **trava**: desliga os avatares e depois percorre o esqueleto do avatar vazio (recursão infinita) | ✅ passa do carregamento |
| [Motion-Explosion](../kinect/jogos/motion-explosion.md) | ✅ roda | ✅ roda |
| [Rapala-for-Kinect](../kinect/jogos/rapala-for-kinect.md) | ✅ roda | ✅ roda |
| [Adrenalin-Misfits](../kinect/jogos/adrenalin-misfits.md) | ✅ roda | ✅ roda |
| [Carnival-Games-Monkey-See-Monkey-Do](../kinect/jogos/carnival-games-monkey-see-monkey-do.md), [Harry-Potter-for-Kinect](../kinect/jogos/harry-potter-for-kinect.md) | não chamaram `XamAvatarInitialize` em 90 s | idem |
| [Kinect-Rabbids-Invasion](../kinect/jogos/kinect-rabbids-invasion.md) | trava no vídeo de abertura (outro motivo) | idem |

**Conclusão:** nenhum valor global serve. O Season Two precisa dos avatares "aceitos" e o Kinect Sports precisa deles recusados. Enquanto não houver avatares de verdade, a saída é a configuração por jogo: `config/4D5309D6.config.toml` com `[Kernel] allow_avatar_initialization = true` (criada em `~/Kixenia/kinect-tools/storage` e `storage-replay`). O que aparece na tela no lugar do avatar ainda não foi conferido.

**Correção (commit c6e54b054):** o `XamAvatarGetAssetsResultSize` do Xenia devolvia 0 para os dois buffers, então o Kinect Sports reservava buffers nulos e lia o endereço 0. Agora ele devolve os tamanhos do xam (0x81965EC0: 0x1F80 fixos, mais 0x3147C com o bit 12, mais por parte o cabeçalho alinhado a 16 no resultado e os dados alinhados a 4096 no buffer de GPU, pela tabela 0x816332B8). Novo teste de 90 s com avatares ligados: o **Kinect Sports** chega ao quadro 2912 sem travar e rastreia o jogador. Ele monta um manifesto próprio (o do perfil falha), registra 3 assets STRB com `SetCustomAsset`, pede `GetAssets` com a máscara 0x93F e segue sem avatar quando recebe a falha. O **Season Two** também roda (não chega a pedir assets). Com isso, `allow_avatar_initialization = true` passa a servir aos dois.

## Para implementar

- O mínimo que destrava os jogos: um **manifesto padrão** por perfil (avatarsyscore, 0x600003) e respostas coerentes do avatarcore, sobretudo `GetAssets` (0x81969848) e `LoadAnimation` (0x81968390), em vez de stubs que não preenchem nada.
- **Os dados existem:** o `AvatarAssetPack` que os jogos trazem é o pacote do sistema. O Xenia pode montá-lo como `\System??\AvatarAssetPack` (de um jogo, ou copiado uma vez para a pasta de dados) e ler o `AvatarAssetPack.toc`.
- Próximos passos da engenharia reversa: o resto do formato do `.toc` (os leitores 0x8197E238 e 0x8197D400 e os métodos do objeto 0x81B4B1FC), o carregamento em 0x81968D88/0x81969490, o gerador de avatar aleatório (método +0x34 do leitor), que mostra como montar um manifesto válido, e o avatarsyscore (0x81981740).
