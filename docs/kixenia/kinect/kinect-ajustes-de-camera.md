# Ajustes de câmera do Kinect (PsCamDeviceRequest 35 e 36)

O runtime NUI do jogo guarda as configurações da câmera de cor e as envia ao driver pelo `PsCamDeviceRequest`:
- **código 35** (Set) aplica as configurações;
- **código 36** (Get) lê os valores atuais.

O jogo chama `PsCamDeviceRequest` por um wrapper (`82F476B8`), que tenta antes o `XamXStudioRequest(0x1003)`, ou seja, o Kinect Studio.

Relacionado: [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md), [Kinect-ID](kinect-id.md), [Kinect-Xenia-Edge](kinect-xenia-edge.md).

## Pedido

| Offset | Campo |
|---|---|
| +0x00 | código (0x23 ou 0x24) |
| +0x14 | rotina de conclusão |
| +0x18 | geração das configurações (o driver preenche; os quadros de cor a carregam) |
| +0x1C | bloco de configurações, 0x5C bytes |

## Dois conjuntos

O Nike guarda uma tabela de **2 conjuntos** de 0xC0 bytes em `83AF5F78`. Cada conjunto tem duas cópias do bloco: a **ativa** (+0x00) e a **preparada** (+0x5C).
- `Function_82F51B60` copia a preparada para a ativa.
- `Function_82F51200` envia a ativa num código 35.
- `Function_82F51BF8(0, n)` escolhe o conjunto ativo.

| Conjunto | Word 0 | Quando |
|---|---|---|
| 0 | 0x40000000 | durante o jogo |
| 1 | 0x80000000 | enquanto o **Kinect ID captura rostos**. O worker de identidade (`82F5E990`), no evento 7, passa para o conjunto 1 com a propriedade 4 = 101 e, no evento 8, sincroniza os dados biométricos |

> Corrigido em 2026-10-24: antes eu achava que o conjunto 1 era a câmera infravermelha.

## Bloco (valores em ponto fixo 16.16)

| Offset | Campo | Valores | Padrão (`82F50D98`) |
|---|---|---|---|
| +0x00 | conjunto (0x40000000 / 0x80000000); bit 0 = tempo de exposição definido; bits 1 a 3 = ganhos R/G/B definidos | | |
| +0x04 | **modo da cor** (propriedade 1): 1 normal, 2 alta qualidade, quando a inicialização NUI tem o flag 0x40. O Nike guarda junto o período de quadro: 0x21 (33 ms) ou 0x42 (66 ms, 15 fps) | 1 a 3 | 1, ou 2 em alta qualidade |
| +0x08, +0x0C, +0x10 | região de exposição: ligada, centro x (±320), y (±240) | | desligada |
| +0x14 | compensação de exposição em EV (propriedade 5) | ±1,75 | 0 |
| +0x18 | modo de exposição (propriedade 0): 1 automática do sensor, 0 manual, 2 automática feita pelo **próprio jogo** (`82F6FD58`, a cada quadro) | | 1 |
| +0x1C | quadros que a exposição automática pode usar (propriedade 2) | 0 a 3 | 0, ou 2 em alta qualidade |
| +0x20 a +0x2C | janela de medição x, y, largura, altura (largura e altura de 0,15 a 1) | | 0,1 / 0,1 / 0,8 / 0,8 |
| +0x30 a +0x38 | ganhos R, G, B = 2^x, com x de −6 a 6 (propriedades 6 a 8) | | 1,0 |
| +0x3C | tempo de exposição manual em ms (propriedade 0) | 0 a 2000 | 0 |
| +0x40 | medido pela câmera (propriedade 3, só leitura). O Nike lê depois de 2 s de exposição automática e usa 60 se vier 0 (`82F6FBD8`) | | |
| +0x44 | taxa de atualização da tela (de `XGetVideoMode`) | | 60 |
| +0x48 | propriedade 4, significado desconhecido; o Nike usa 0 durante o jogo e 101 na captura de rostos | 0 a 5, 100, 101 → guardado 0 a 7 | 0 |

O Nike também restaura todo um conjunto salvo em `83B07400` (`82F6BE58`): região, janela, propriedades 0, 1, 2, 4 e 5, e, conforme o modo, o tempo e os ganhos ou a propriedade 9 (float, tratada pelo próprio jogo).

## Como levar ao sensor (Kinect 1473)

O **SDK do Windows** ajusta a câmera por controles **UVC**: `KinectCamera_UVC_SetAutoExposureMode`, `SetExposureTime(x*100)`, `SetWhiteBalanceTemp`, `SetPowerFrequencyCompMode` e `SetAutoBacklightCompMode`. Isso só existe no Kinect for Windows; o modelo 1473 do Xbox não tem UVC.

No 1473, o caminho são os registradores do sensor de cor, pelo **comando 0x95**, como faz o libfreenect (`src/flags.c`):
- **leitura:** `{1, reg & 0x7FFF, 0}` → valor na resposta [2];
- **escrita:** `{1, reg | 0x8000, valor}`;
- **0x0106:** bit 14 = exposição automática, bit 1 = balanço de branco automático, bit 4 = cor crua;
- **0x0009:** largura do obturador; exposição em µs = valor × 54,21 no Bayer, ou × 63,25 no YUV.

| Campo do Xbox | Sensor |
|---|---|
| +0x18 = 1 | bit 14 do 0x0106 ligado |
| +0x18 = 0 com o bit 0 dos flags | bit 14 desligado; 0x0009 = ms × 1000 / 54,21 |
| ganhos definidos (bits 1 a 3) | bit 1 do 0x0106 desligado. Os registradores dos ganhos não são conhecidos |

Nada disso foi testado no sensor; vem do código do libfreenect.

## O que o Nike envia

Em 90 segundos de menu, o Nike faz cerca de 7 pedidos:
- **conjunto 0:** sempre os padrões, com exposição e balanço de branco automáticos;
- **conjunto 1:** exposição manual e propriedade 4 = 101, sem tempo nem ganhos definidos.

## No Xenia

O commit 49c29f1c0, com nomes corrigidos no eb3843a7c, implementa:
- o código 35 guarda cada conjunto;
- o código 36 devolve o conjunto enviado por último, com ganhos neutros quando o jogo não definiu nenhum.

Antes, o código 36 não preenchia nada. O Nike copiava lixo da pilha (como `FFFFFFFF 83AF5900 82F51B50`) para os ganhos e reenviava esse lixo.

Os ajustes não chegam ao sensor, porque nenhum jogo conhecido pede ajuste manual na câmera de cor.
