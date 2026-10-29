# Kinect, engenharia reversa: o suporte do xam: NUI, voz, identidade, gestos, HUD, inclinação e as funções não implementadas

Parte de [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 4, 13, 16b, 16c, 16d, 16e, 16f).

## 4. xam.xex: NUI e voz

### Áudio NUI
- **`XamVoiceGetMicArrayAudioEx(pedido de 0x68 bytes, KEVENT)`:**
  - +0x04: 5 ponteiros de buffer;
  - +0x18: os tamanhos (0x200, depois 0x400 × 4);
  - +0x40: o tic (u16).
  - Os pedidos se completam a cada 3 tics.
- **Callback do cliente MEC:** chamado a cada tic de 5,33 ms.
- **`XamNuiGetDeviceStatus`:** o bit 1 de `status_bits` significa "matriz de microfones presente". O Nike só liga o áudio NUI com esse bit.
- **`XAudioCaptureRenderDriverFrame`:** é a referência de eco, o som que o console toca.

### Quadro de esqueleto (NUI_SKELETON_FRAME, big-endian)
- Os esqueletos começam em +0x30, com 0x1C0 bytes cada, 6 ao todo.
- Cada um tem: estado (+0), trackingId (+4), enrollment (+8), userIndex (+0xC) e posição (+0x10).
- Os IDs escolhidos para o rastreador ficam em +0xACC e +0xAD0.

### Engajamento
- **`XamNuiPlayerEngagementUpdate`:** a pontuação de cada pessoa é √(x²+z²), menos 0,3 se ela já está rastreada, com mínimo 0,1. Os dois mais próximos são escolhidos e os IDs gravados no quadro. A saída tem 7 campos: ID engajado, índice, usuário, cadastro, quantos rastreados e os 2 índices.
- **`XamNuiSkeletonGetBestSkeletonIndex`:** o primeiro ID escolhido cujo esqueleto está rastreado; senão, −1.

### Pontuação de jogadores (`XamNuiSkeletonScoreUpdate`, analisada em 2026-10-24)

O export (ordinal 2196, 0x816F9980) é um desvio para o corpo em **0x8198B628**. `XamNuiSkeletonGetBestSkeletonIndex` (0x816F9988) desvia para 0x8198B170.

**`XamNuiSkeletonScoreUpdate(quadro, pontuações)`** mantém até **2 registros** de 0x3C bytes (em +0x00 e +0x3C), um por jogador rastreado:
- Percorre os 6 esqueletos do quadro (estado 2 = rastreado) e casa cada um com um registro pelo ID de rastreamento (registro +0x30).
- Se só um jogador está rastreado e ele é o do segundo registro, copia o segundo sobre o primeiro e zera o segundo.
- Para cada jogador: se o registro tinha outro ID, ele é zerado e marcado como novo. Então chama `FUN_8198B390(esqueleto, registro)` e grava o ID (+0x30), o índice do esqueleto (+0x34) e "novo" (+0x38).
- Em +0x78 fica o número de esqueletos rastreados.

**Registro por jogador** (`FUN_8198B390`; floats, salvo indicação):

| Word | Conteúdo |
|---|---|
| 0, 1 | estado do esqueleto; o 1 vira **1 se qualquer alerta abaixo estiver ligado** |
| 2 | quantos ombros (articulações 4 e 8) **não** estão rastreados (0 a 2) |
| 3 | alerta: os dois ombros e a cabeça (articulação 3) sem rastreio |
| 4 | **posição horizontal na imagem**, de −1 a 1: `(160 − px) / 160`, com `px` a projeção da posição na profundidade 320×240 |
| 5 | alerta: \|posição horizontal\| ≥ **0,7** (perto da borda lateral) |
| 6 | **distância** (z da posição, em m) |
| 7 | alerta: z ≤ **1,0 m** (perto demais) |
| 8 | **giro do corpo em graus**: o vetor do ombro mais próximo da câmera até o centro dos ombros (articulação 2), projetado no plano horizontal, normalizado e com o produto escalar com (1, 0, 0), limitado a [−1, 1], passa por `acos` e é multiplicado por 57,2958 |
| 9 | 0 |
| 10 | 1,0 se uma das mãos (articulação 7 ou 11) estiver rastreada |
| 11 | alerta: nenhuma mão rastreada |
| 12 a 14 | ID (+0x30), índice do esqueleto (+0x34), novo (+0x38) |

Os alertas (words 3, 5, 7, 9 e 11) são o que o sistema usa para dizer ao jogador "vá para o centro", "afaste-se" e "vire-se de frente".

**Projeção** (`FUN_8198B0F0`), o equivalente ao `NuiTransformSkeletonToDepthImage` do SDK:

```
px = x · 285,63 / z + 160
py = −(y · 285,63 / z − 120)
```

Com z = 0, dá (0, 0).

**Vetor "para cima" do giro:** é a **normal à gravidade do quadro** (quadro + 0x20, `vNormalToGravity`). O corpo carrega `r24 = r23 + 0x20` e faz `lvx128` antes de chamar `FUN_8198B390`.

**Quem usa:** nem o Nike nem o Kinect Sports importam `XamNuiSkeletonScoreUpdate`, `XamNuiHudInterpretFrame` ou `XamNuiGetSystemGestureControl`, porque cada um tem o próprio runtime.

**No Xenia:** `XamNuiSkeletonScoreUpdate` foi implementada no commit 00134e151, com a estrutura `X_NUI_SKELETON_SCORE` de 0x3C bytes. Como nenhum dos jogos disponíveis a chama, não foi testada por um jogo; o Kinect Sports com a gravação seguiu sem regressão.

**Diferença em relação ao xam:** no xam, com um único jogador rastreado e nenhum novo, a ficha 1 é copiada sobre a 0 mesmo quando o jogador está na ficha 0, o que parece um artefato da decompilação. O Xenia só move quando o jogador estava na ficha 1.

### Inclinação
- **`XamNuiCameraSetFlags(valor, máscara)`:** lê o status da inclinação e manda `(atual & ~máscara) | (valor & máscara)` ao motor. No controlador tipo 2 só devolve sucesso; sem motor, `0x8007048F`.
- **`XamNuiCameraRememberFloor`:** grava o plano do chão e aciona a inclinação automática. `XamNuiCameraElevationAutoTilt` e `XamNuiCameraAdjustTilt` também existem.
- **`ExGetXConfigSetting(7, 9)` = `XCONFIG_CONSOLE_NUI`** (0x1C bytes):
  - +0: flags (0x2 = calibrado, e o jogo vai direto ao ângulo salvo);
  - +4: altura, float 1800;
  - +8: ângulo, int.

### Mensagens do app 0xFE
- 0x2B001 a 0x2B004: inicialização do NUI do jogo.
- 0x2B002: quadro de esqueleto publicado.
- 0x2A005 e 0x2A006: inicialização de identidade.
- 0x2A007 e 0x2A008: cache dos dados biométricos ([Kinect-ID](kinect-id.md)).
- 0x2C006 a 0x2C013: identificação, repassada ao callback registrado pela 0x2C009 ([Kinect-ID](kinect-id.md)).
- 0x2C00C: dados de identidade, com leitura de parâmetros por nome e conversão dos dados (seção 13).
- 0x2C00D: resolução de identidade (seção 12).

### Outros
- **`XamNuiGetDeviceSerialNumber(buf, tamanho)`:** o Nike passa 0x11; o xam copia 12 caracteres e um NUL.
- **`XamXStudioRequest`:** repassa a um gancho que só o Kinect Studio (devkit) instala. Sem ele, devolve `0x80004005`. Os jogos chamam a cada quadro, e o wrapper do PsCam no Nike (82F476B8) tenta primeiro `XamXStudioRequest(0x1003)`.
- **`XamUserNuiGetEnrollmentIndex`:** consulta uma tabela de 4 cadastros de 0x40 bytes (+0x30 ativo, +0x34 índice). Sem cadastro, devolve −2 e `0x80070525`.

## 13. XMsg 0x2C00C: dados de identidade

Antes eu achava que era "registrar classificador". Na verdade é uma chamada genérica `{op, tipo (8 bytes), entrada, tamanho da entrada, saída, &tamanho da saída}`; no Nike, `838C2838`.

| op | tipo | O que faz |
|---|---|---|
| 2 | 0x101 | **lê um parâmetro de identidade pelo nome** (`NuiV1FaceHiRes`, `NuiV1ColorThre`, `NuiV1QualDist`, `NuiIdCfgVer`… são 20 ao todo) como texto, em até 0x78 bytes. O jogo o lê com `sscanf` e usa o padrão embutido se não conseguir |
| 1 | 0x10001 | **converte os dados do Kinect ID para a versão 0x10001** antes de carregar (`82F6A138`): primeiro sem saída, para obter o tamanho, depois com saída em buf+0x32000 |

No Xenia, o op 2 devolve vazio (o jogo usa os padrões) e o op 1 copia a entrada (commit 6445d2a1f). Antes, os rostos lidos nunca eram carregados.

**Formato dos dados do Kinect ID no Nike** (gravados por `82F6AB00`, já na versão 0x10001):
- cabeçalho `{u32 0x10001, u32 nº de registros}`;
- cada registro: 0x20 bytes de identificação, contagem em +0x18 e rostos de **0x7D4 bytes** a partir de +0x1C;
- cada rosto tem um vetor de 2000 bytes mais 4 bytes (`82F693D8`), e a validação exige 500 floats finitos;
- até 0x66 rostos no total e 0x32000 bytes no máximo.

## 16b. Funções de voz do xam (decompiladas em 2026-10-24)

Os decompilados estão em `~/Kixenia/kinect-sdk-re/xam-nui/decomp-voice/`.

| Função | Endereço | O que faz |
|---|---|---|
| `XamVoiceGetDirectionalData(p1, p2)` | 0x816FB450 | `KeGetCurrentProcessType` e repasse a **`XVoicedGetDirectionalData`** (kernel 0x327). A direção do som é calculada no kernel/xvoiced, não no xam |
| `XamVoiceHeadsetPresent` | 0x816FB418 | repassa a `XVoicedHeadsetPresent` (0x1E1) |
| `XamVoiceSubmitPacket` | 0x816FB498 | repassa a `XVoicedSubmitPacket` (0x1E2) |
| `XamVoiceGetBatteryStatus` | 0x816FB3C8 | perfil → `XVoicedGetBatteryStatus` (0x281); sem perfil, 0x48F |
| `XamVoiceClose` | 0x816FC1C8 | desliga o objeto (`FUN_816FBF68(obj, 0)`) e `XVoicedClose` (0x1E3) |
| `XamVoiceCreate(?, n, &obj)` | 0x816FC098 | um objeto de 0x38 bytes por usuário (tabela em 0x81AACA28) com uma fila de `n` entradas de 0x10 bytes |
| `XamVoiceSetAudioCaptureRoutine(rotina)` | 0x816FBD58 | troca atômica (lwarx/stwcx) do ponteiro global 0x81AD1264 e devolve o anterior |
| `XamVoiceRecordUserPrivileges` | 0x816FBB28 | para cada um dos 4 usuários: 0 sem login, 10 por padrão, 30 se os privilégios 0xFC e 0xFB permitirem, 20 se não |

## 16c. Gestos e HUD do xam (decompilados em 2026-10-24)

O xam tem dois detectores de gesto, os dois alimentados por `XamNuiHudInterpretFrame` (ordinal 0x87C, 0x816ACC68): o **aceno de engajamento** ("quem está jogando") e o **gesto do guia** (braço estendido parado). Os decompilados estão no scratchpad em `gest/`.

### `XamNuiHudInterpretFrame(quadro)`
1. `FUN_816ACAD8` conta presença: 6 entradas de 5 bytes `{u8 contagem, u32 ID}` em 0x81AA07B0. Devolve 1 quando um jogador rastreado continua por **mais de 120 quadros** (4 s); no outro modo (`FUN_81695EC8`), quando há qualquer esqueleto. Nesse caso `FUN_81695EA0` grava a hora em 0x81A9E0A0 (o relógio de inatividade).
2. Se o objeto do HUD (0x81AA07D0) existe, `FUN_816AC688(hud, quadro)` faz o trabalho (abaixo).
3. Se o filtro de entrada está ligado (0x81AA2188, `XamNuiHudEnableInputFilter`), devolve **0x8301000B**: o título não deve usar esse quadro, porque o sistema está com a entrada.

### `FUN_816AC688(hud, quadro)`
- `dt` = tempo desde hud+0x128, em ms (× 1000).
- Flag global 0x81AAC990 bit 0x200 → `quadro+0xAB8 |= 4`.
- **Escolha dos 2 esqueletos rastreados** (`FUN_8198B1E8`): para cada esqueleto calcula a distância horizontal `√(x² + z²)`; o já rastreado ganha **0,3 m** de vantagem; mínimo 0,1. Os dois mais próximos vão para **quadro+0xACC / +0xAD0** (os IDs que o sistema pede ao rastreador). Também devolve o engajado (ID, índice), a lista de índices rastreados e a contagem.
- Com o HUD ativo (0x81AA218C):
  - `XamNuiSkeletonScoreUpdate(quadro, hud+0x9C)`. Sem o flag 0x200, **o estado de cada registro é gravado de volta no esqueleto do quadro**: jogador com algum alerta (borda, perto demais, de costas) chega ao título como **PositionOnly (1)**.
  - Se o engajado sumiu: `XamNuiSkeletonGetBestSkeletonIndex`, `FUN_817FD2D0(1)` e o ID engajado é zerado.
  - Para os outros jogadores, `FUN_81808B08` roda o **aceno** (abaixo). Quem completa vira o engajado; o maior progresso vai para `FUN_817FD258` (a barra de progresso da UI, 0x81AD1F5C, de 0 a 1, que fica 5 quadros em 1,0).
  - Copia o quadro (0xAE0 bytes) para hud+0x130 sob trava (+0xC1C), grava o índice engajado em +0xC18 e chama `FUN_817FD108` (o cursor do HUD: se o engajado mudou há menos de 2 s, avisa a UI).
- Depois, sem HUD e com filtro, `FUN_81800828(0)`. Senão roda o **gesto do guia** (`FUN_81808C60`) e passa o progresso a `FUN_81800828` (0x81AD20D0, de 0 a 1; fora disso `0x80070057`).

### Aceno de engajamento (`FUN_818092C8` → `FUN_8180AAB8`, por mão)

Objeto 0x81AD2180; um estado de 0x1C8 bytes por mão (0 = esquerda, 1 = direita), dois jogadores. Parâmetros (`FUN_8180A388`):

| Variável | Valor | Uso |
|---|---|---|
| 0x81AD221C | **4** | meias-ondas para completar; progresso = contagem / 4 |
| 0x81AD2208 | 3 | quadros seguidos na mesma direção para contar uma meia-onda |
| 0x81AD220C | 1000 ms | tempo mínimo com a mão erguida antes de começar |
| 0x81AD2214 | 1000 ms | tempo máximo entre meias-ondas (senão zera) |
| 0x81AD2220 / 0x81AD2204 | 0,7 / 1,5 | velocidade lateral mínima (m/s) com mais / menos quadros |
| 0x81AD2210 | 0,6 | tolerância de ritmo: o período de cada meia-onda pode variar 60% da média |
| 0x81AD2218 | 20 | (não visto em uso) |
| 0x81AD21FC | 0,1 | (suavização) |
| 0x81AD2200 | 1 | exige a mão erguida em todo quadro |

Fluxo por quadro, para a mão (articulação **7** ou **11**):
- O esqueleto tem de estar rastreado (estado 2) e ser o mesmo ID; senão o estado zera.
- **Mão erguida** (`FUN_8180A788`): no referencial do corpo (`FUN_81808DE0`), a mão está **acima do cotovelo** (articulação 5 ou 9) e no máximo **6 cm** mais longe da câmera que o ombro (4 ou 8). Se houver registro de pontuação, o giro do corpo (+0x20) tem de ser **menor que 45°**.
- **Direção** (`FUN_8180A988`): a velocidade lateral da mão (`Δx / (dt · 0,001)`) dá a direção, 2 ou 3; a contagem de quadros na mesma direção suaviza a velocidade (média de até 3).
- **Meia-onda** (`FUN_8180A5C0`): troca de direção com velocidade acima do limiar → conta; `FUN_8180A6F8` mede o período e zera se o ritmo variar demais.
- Com 4 meias-ondas, devolve 1: o jogador engajou. `FUN_818092C8` devolve `{engajou, progresso máximo das duas mãos, a mão terminou na direção 3}`.

### Gesto do guia (`FUN_81808C60` → `FUN_8180A0B0`)

Objeto 0x81AD2184; um estado de 0x2C0 bytes por esqueleto (6). Por quadro, `FUN_81809FD0`:
- exige o esqueleto rastreado e os **dois ombros** rastreados (articulações 4 e 8; `FUN_818098E0` monta o referencial do corpo a partir da linha dos ombros e do "para cima");
- suaviza mãos e ombros (0,1 para as mãos, 0,05 para os ombros) e mede ângulos do braço contra o tronco (`FUN_81809AF0`, ~200 instruções vetoriais);
- se a pose vale e o tempo de espera acabou, soma `dt` ao tempo acumulado (+0x1B0); senão zera.

Progresso = tempo acumulado / **3000 ms**. O maior progresso vira o valor da UI (+0x1090) e o ID do jogador (+0x1098). Ao chegar a 1,0 o gesto dispara (+0x1094 = 1), o acumulado zera, e `FUN_81809738` põe todos os outros em espera de **1500 ms**.

Parâmetros (`FUN_81809558`): ângulos em graus 205, 245, 250, 275, 290, 295, 160, 200, 360, 110, 40, 35; 32; 9; 50; 100; 15 (inteiro). Os pares de ângulos devem ser as janelas da pose (o guia do Xbox 360 é o **braço esquerdo estendido a cerca de 45° do corpo**, segurado até a barra encher), mas qual par mede o quê não foi decifrado.

### Exports relacionados
- `XamNuiHudGetEngagedEnrollmentIndex`: na cópia do quadro do HUD, procura o esqueleto com o ID engajado (+0x118) e devolve o índice de cadastro; senão −2.
- `XamNuiHudEnableInputFilter(p)`: com o HUD ligado e o objeto 0x81AA2160, `FUN_81692C48(obj, p)` (que chama `FUN_816920F0` se obj+0x38).
- `XamNuiGetSystemGestureControl` (0x877) está na tabela do Xenia mas **não é exportado pelo xam 17559**.
- `ControlpackNuiCursorSetTrackingId`: o endereço do exports.csv (0x816DE4C8) é uma verificação de data dos dados biométricos; o rótulo deve estar errado.

### No Xenia
- Nike e Kinect Sports **não importam** nenhuma dessas funções: têm o próprio aceno e o próprio guia (no Kinect Sports, o wrapper em 0x82A74D50 chama `XamShowNuiGuideUI`).
- **Implementado no commit e19c98e7e:** `XamNuiHudInterpretFrame` (escolha dos 2 esqueletos, rebaixamento para PositionOnly, aceno de 4 meias-ondas com os parâmetros do xam, gesto do guia por pose aproximada, opção `kinect_guide_gesture`, `0x8301000B` com UI do sistema aberta), `XamNuiHudGetEngagedEnrollmentIndex` e `XamNuiHudEnableInputFilter` (só guarda o valor).
- O **tempo** entre quadros vem do relógio do host, limitado a 100 ms. `FUN_8180A900` (provável checagem extra da mão, exigida por 0x81AD2200 = 1) foi tratada como sempre verdadeira. O giro do registro de pontuação é dobrado (`min(giro, 180 − giro) < 45`), porque o ângulo pode ser medido a partir de qualquer ombro.
- No mesmo commit: as telas `XamShowNui{Achievements,Marketplace,GamerCardForXUID,ControllerRequired}UI` (NuiHudCheck e depois a tela normal; Marketplace valida usuário < 4 e tipo 0, 1, 4 ou 5; MarketplaceDownloadItems devolve 0x78 como no xam) e `XamNuiGet/StoreDepthCalibration` (0x30 bytes em `kinect_depth_calibration.bin` na raiz de armazenamento; respostas com `HRESULT_FROM_NT`, ou seja, 0x10000000 em caso de sucesso).

## 16d. As 30 funções ainda não implementadas (analisadas em 2026-10-24)

Imports do xam resolvidos pelo registro do thunk (`0x01LLOOOO`): 0x81A73134 = **PsCamDeviceRequest** (0x30D), 0x81A73164 = McaDeviceRequest (0x30E), 0x81A73154 = DetroitDeviceRequest (0x30F), 0x81A721F4 = ExGetXConfigSetting, 0x81A72204 = ExSetXConfigSetting, 0x81A72214 = ExReadModifyWriteXConfigSettingUlong, 0x81A722E4 = `_snprintf`, 0x81A72244 = KeWaitForSingleObject.

### Inclinação (todas exigem `XamNuiCameraGetTiltControllerType` ≠ 0, senão `0x8007048F`)
| Função | O que faz |
|---|---|
| `XamNuiCameraElevationAutoTilt(float)` | XMsg 0xFE **0x2B005** `{float}`: pede ao sistema a inclinação automática |
| `XamNuiCameraElevationReverseAutoTilt(u32)` | 0xFE **0x2B00A** `{u32}`: desfaz a última inclinação automática |
| `XamNuiCameraAdjustTilt(float, float, u32)` | 0xFE **0x2B00B** `{u32, float, float}` |
| `XamNuiCameraElevationSetCallback(cb)` | 0xFE **0x2B00D** `{cb}`, sem checar o controlador |

O Xenia não trata nenhuma dessas mensagens. **Atualização de 2026-10-25:** pelas importações de todos os módulos do sistema (00-Modulos), quem **envia** AutoTilt, ReverseAutoTilt e AdjustTilt é só o Guide.NuiTroubleshooter (o Kinect Tuner do guia); quem **recebe** é o motor de inclinação do título ativo (jogo, BiometricSetup, dashnui, AvatarEditor, Zune), registrado por `XamNuiCameraTiltSetCallback`. **Resolvido no mesmo dia** (Troubleshooter decompilado): o Tuner chama `XamNuiCameraAdjustTilt(2.1336, 1.3716, 0x20)` no método `StartCheckingHeadClippedStatus`. Os floats estão em metros (7 e 4,5 pés): o 1º é o limite de distância do jogador e o 2º, diferente de 0, faz o motor usar a posição corrigida pela inclinação; a flag 0x20 é "só verificar" (não move o motor) e os bits 0–1 escolhem um deslocamento de mira de 0/150/300 mm. A resposta volta pela 0x2B00C (0xE0 bytes). Ou seja, o AdjustTilt do Tuner checa se a cabeça está cortada no topo da imagem. Detalhes em Guide.NuiTroubleshooter.xex.

### Usuário
- **`XamUserNuiBind(nui, usuário)`**: `nui` < 8 e usuário < 4, senão `0x80070057`. Na tabela de usuários (0x81A82FF4, 0x40 bytes cada) grava +0x34 = `nui` e +0x30 = 1 (ligado), e chama o aviso de mudança. Liga um jogador do Kinect a um perfil.
- **`XamUserNuiUnbind(usuário)`**: lê o índice de cadastro do usuário e, com `FUN_816D8F50(cadastro, 5, …)`, apaga a entrada de tipo 5 que aponta para ele e avisa.

### Kinect ID (pelo callback que o título registra na 0x2C009)
`FUN_8180E750(obj, msg, buf)` chama o callback guardado em obj+0x24. Para 0x2C006, 0x2C007 e 0x2C008 consulta antes a 0x2C013 e para se ela der `0x83010005`. Sem callback: `0x8000FFFF`.

| Função | Mensagem | Dados |
|---|---|---|
| `XamNuiIdentityGetEnrollmentInfo(cadastro, &a, &b)` | 0x2C013 | entra `{0x10, cadastro}`, sai dois u32 |
| `XamNuiIdentityUnenroll(cadastro)` | 0x2C012 | `{8, cadastro}` |
| `XamNuiIdentityGetColorTexture(textura)` | 0x2C011 | sai largura, altura, formato e endereço; monta um cabeçalho de textura D3D (`XGSetTextureHeaderEx`, 1 nível) e aponta para a foto do rosto |
| `XamNuiIdentityIdentifyWithBiometric(?, overlapped, a, b)` | XMsg 0xFE **0x2C007** `{?, a, b, 0}` | exige HUD |
| `XamNuiIdentityGetQualityFlags(?, overlapped)` | XMsg 0xFE **0x2C008** | exige HUD |
| `XamNuiIdentityGetQualityFlagsMessage(p)` | **PsCam código 40** (0x28) | manda os dois u32 de `p` ao sensor |

### Sensor
- **`XamNuiGetLoadedDepthCalibration(p)`**: **PsCam código 44** (0x2C), devolve dois u32 em `p`.
- **`XamNuiGetFanRate(&byte)`**: pedidos acima de 2000 vão pelo **PsCam código 20** (0x14) com o número da propriedade; a **0x838** é o **ventilador** do sensor, e o byte 7 da resposta é a rotação.
- **`XamNuiGetSupportString(?, buffer)`**: o código de suporte da tela de solução de problemas, `"K:%02d:%02d:%02d:%02d:%02d.%02d.%04d.%02d:%12s"`: modo, sub-estado, estado do dispositivo (0x81AD1274), categoria e código do problema, versões, e o número de série (12 caracteres, `000000000000` sem sensor). A categoria compara as versões do firmware esperadas com as do sensor: 0 = iguais, 6/7/8 = diferenças, 1 = desconhecido, 2 a 5 = erros guardados.
- **`XamNuiSetForceDeviceOff(desligar)`**: liga ou desliga o bit **31 de `XCONFIG_USER_RETAIL_FLAGS`** (`KinectDisabled`) com ReadModifyWrite. Se já estava assim, `0xD0000184`. Depois reavalia o estado e, se desligou, manda o evento 0xF à máquina de estados.
- **`XamNuiEnableChatMic(ligar)`**: no **`XCONFIG_CONSOLE_NUI`** (categoria 7, item 9, 0x1C bytes) o **bit 3 (0x8) = microfone de chat desligado**; depois `XamVoiceMuteMicArray(!ligar, 1)`. É o mesmo campo que `XamNuiIsChatMicEnabled` lê.
- **`XamNuiNatalCameraUpdateStarting(a, b)`** / **`…Complete(hr)`**: avisos do atualizador de firmware: eventos 6 e 7 da máquina de estados e notificações 0xC e 0xD. Com `hr = 0x80070002` (arquivo não encontrado), abre o Troubleshooter.

### Telas "Nui" restantes
Todas fazem NuiHudCheck (`FUN_816C1808`) e exigem perfil da Live (sem ele: `0x4DD`/`0x525` ou 5).

| Tela | Mensagem 0xFE |
|---|---|
| Friends | 0x21033 |
| Party (só com festa ativa) | 0x21034 |
| GameInvite (exige `p4` ≠ 0) | 0x21035 |
| FriendRequest | 0x21036 |
| Messages | 0x21038 |
| Games | 0x21008 (a mesma das conquistas) |
| VideoRichPresence | IO 0x21025 |
| CommunitySessions, JoinSessionInProgress | só a checagem (nada depois no 17559) |

### Implementado no commit 4251108a5
- Bind/Unbind e a tabela usuário ↔ cadastro, que também responde `XamUserNuiGetEnrollmentIndex`, `GetUserIndex(cadastro)` e `GetUserIndexForBind` (o primeiro perfil conectado sem cadastro).
- O callback da 0x2C009 agora é guardado (Nike: 0x82F5BB68; Kinect Sports: 0x82A10330). `GetEnrollmentInfo` e `Unenroll` o chamam no thread do título, com o pedido em memória do sistema.
- `EnableChatMic` / `IsChatMicEnabled` pela opção `kinect_chat_microphone` (bit 0x8 do `XCONFIG_CONSOLE_NUI`); `SetForceDeviceOff` pela opção `kinect_disabled` (bit `KinectDisabled`), e com ela ligada `NuiInitialized` responde falso. Avisa `kXNotificationSystemNUIHardwareStatusChanged`.
- `GetSupportString`: modo, estado, categoria 0, versão 1.2.810.0 (do `fwversions.txt`) e o número de série do sensor. Os valores de estado do console (0x81AD1274) são desconhecidos e saem 0.
- Inclinação: as 4 checam o motor (`0x8007048F` sem ele); `ReverseAutoTilt` volta para `kinect_elevation`; `AutoTilt` e `AdjustTilt` só respondem (a automática do Xenia é a `kinect_auto_tilt`).
- As telas da Live respondem como o xam sem Live: `0x4DD` com perfil conectado, `0x525` sem; Party, Messages e VideoRichPresence, 5. Games abre as conquistas.
- **Ficaram de fora:** `GetColorTexture` (o Xenia não tem `XGSetTextureHeaderEx`), `IdentifyWithBiometric` e `GetQualityFlags` (app do sistema), e as três que dependem do sensor (PsCam 20, 40 e 44).

### O que dá para implementar no Xenia
- **Sem incógnitas:** Bind/Unbind, EnableChatMic (+ corrigir `IsChatMicEnabled`), SetForceDeviceOff, GetSupportString, as telas (NuiHudCheck + a tela normal do Xenia), Natal update (sem efeito).
- **Com o callback da 0x2C009 guardado:** GetEnrollmentInfo, Unenroll, GetColorTexture.
- **Depende do app do sistema:** as 4 de inclinação, IdentifyWithBiometric, GetQualityFlags (podem chamar a inclinação automática que já existe).
- **Depende do sensor (PsCam 20, 40, 44):** FanRate, QualityFlagsMessage, GetLoadedDepthCalibration.

## 16e. Texturas, inclinação pelo título e exposição (2026-10-24, commit 6500fff7d)

### XGSetTextureHeaderEx (0x817F7C38) e XGOffsetResourceAddress (0x817F7D18)
O xam traz uma cópia da XGraphics. Cadeia: `XGSetTextureHeaderEx` → `FUN_817F79C0` (campos do D3DResource e endereços) → `FUN_81838CB0` (fetch constant) → `FUN_81837BB8` (tamanhos) → `FUN_81837A98` (alinhamento de um nível).
- **Cabeçalho (13 dwords):** Common = 3 (textura) | 0x200000 (uso & 4) | 0x400000 (uso & 0x200); RefCount 1; BaseFlush e MipFlush 0xFFFF0000; +0x1C a +0x30 é o fetch constant do Xenos.
- **Fetch:** dw0 = tipo 2, sinais dos componentes (bits 9 a 16 do D3DFORMAT), pitch / 32 nos bits 22 a 30 e tiled no bit 31; dw1 = formato, endian e endereço base; dw2 = (largura − 1) | (altura − 1) << 13; dw3 = swizzle e formato numérico (bits 17 a 29 do D3DFORMAT) | exp bias << 13; dw4 = último nível << 6; dw5 = dimensão 1 (2D) << 9 | mips empacotados << 11 | endereço dos mips.
- **Bits por texel** por formato: tabela em 0x8160C121 (de 2 em 2 bytes). **Blocos** (`FUN_817F6FD0`): 4×4 nos DXT (0x12 a 0x14, 0x31, 0x33 a 0x35, 0x3A a 0x3D), 4×1 em 0x27/0x2C, 2×1 em 0x0B/0x0C/0x28.
- **Alinhamento:** pitch em múltiplos de 32 blocos (linear: 256 bytes ÷ bytes por bloco, no mínimo 32), altura em 32 blocos, nível em 4 KB. Linear de 1 nível sem empacotar: altura só no bloco, sem 4 KB. Mips até o nível de 16 texels (cauda empacotada); com MipOffset = −1 ficam logo depois da base, alinhada a 4 KB.
- Testado: 640×480 ARGB = 1.228.800 bytes, pitch 640; DXT1 256×256 = base 32 KB + mips 32 KB (blocos de 8 KB).
- **XGOffsetResourceAddress:** textura soma no endereço base e, com mais de um nível, no dos mips; buffers (tipos 1, 2, 7, 8) no dword +0x18; tipo 6 no +0x20.

### Inclinação: o motor de inclinação é do jogo
O despachante das mensagens 0x2B0xx do app 0xFE é **`FUN_816F9CB8`** (`switch (msg − 0x2B001)`):

| Mensagem | O que faz |
|---|---|
| 0x2B001 | `FUN_816AC460` |
| 0x2B002 | quadro de esqueleto (versão 5): chama `XamNuiHudInterpretFrame` |
| 0x2B003 | inicialização do título |
| 0x2B004 | fim do título |
| **0x2B005, 0x2B009, 0x2B00A, 0x2B00B** | repassadas ao **callback do jogo** em 0x81A85C40 (AutoTilt, StopMovement, ReverseAutoTilt, AdjustTilt) |
| **0x2B006, 0x2B00C** | repassadas ao callback em 0x81A85C20 (status 0x10 ou 0xE0 bytes, de `TiltReportStatus`) |
| **0x2B007** | grava 0x81A85C40: `XamNuiCameraTiltSetCallback` |
| **0x2B00D** | grava 0x81A85C20: `XamNuiCameraElevationSetCallback` |
| 0x2B00E | grava 0x81AAC9A8: **observador de quadros** do Troubleshooter (`FUN_90111C80` registra `FUN_90111610`, `FUN_90111518` retira com 0); o xam o chama a cada quadro do título (`FUN_816F9FC0`), e o Tuner copia o quadro de esqueleto (0xAE0 bytes) e 0x2D000 bytes de imagem (+0xAC4) para uma textura de 3 buffers: a visão ao vivo do sensor |

Ou seja: o Nike (0x82F53ED0) e o Kinect Sports registram o próprio motor de inclinação, e o sistema (Tuner, painel) pede a ele para inclinar sozinho. No Xenia, AutoTilt, ReverseAutoTilt e AdjustTilt agora chamam esse callback; `TiltReportStatus` repassa o status a quem chamou `ElevationSetCallback`. `StopMovement` continua parando o motor diretamente.

### Identidade 0x2C007 / 0x2C008
`FUN_8180EB20` chama o callback com 0x2C007 e depois espera um objeto de tarefa do xam (vtable +0x8 a +0x14), de onde lê o resultado (cadastro, confiança, 64 bits). Não é só um repasse: **não implementado**.

### XamNuiGetTrueColorInfo e XamNuiGetCameraIntrinsics
As duas são PsCam código 46: `{0x2E, seletor, buffer, tamanho, &devolvido}`; as intrínsecas usam o seletor 0x100 e nenhum jogo as chama.
- **Uso no Nike** (`0x82F78E40`): seletor 0 → 4 bytes (padrão `0x4208C083` = 34,19); seletor 1 → até 0x78 bytes, entradas de 12 bytes **{brilho da cena, quadros/s, exposição em s}**: a **tabela de autoexposição** da câmera colorida.
- Padrões do jogo: 7 entradas para 60 Hz (0x821CAB48), com exposições de 2/15 a 1/120 s; 6 para 50 Hz (0x821CABA0), de 0,13 a 0,01 s. São múltiplos do período da rede elétrica, para a luz não tremer.
- **No Xenia:** devolve esses valores, 60 Hz ou 50 Hz conforme `use_50Hz_mode`. O Kinect Sports e o Nike leem os dois seletores. Os valores reais do sensor continuam desconhecidos.

## 16f. Gesto do guia: quem detecta é o sistema (2026-10-24, commit a56454014)

- O despachante 0x2B0xx do xam (`FUN_816F9CB8`) passa **cada quadro de esqueleto que o título publica (0x2B002, versão 5)** por `XamNuiHudInterpretFrame`. É ali que o console vê o gesto do guia e o aceno de engajamento, em **todos** os jogos. O Xenia só guardava o quadro para o Kinect Tuner; agora chama o HUD.
- O **Kinect Sports** não detecta o gesto: chama `XamShowNuiGuideUI` só na opção **"Guide"** do próprio menu de pausa (`Function_82523468`: Quit, Restart Game, Resume Game, Concede, Guide, Troubleshooter) e pelo **START** do controle (`Function_822E7200`, botão 0x10). O import `XamShowNuiGuideUI` fica em 0x82A74D50.
- Detector ajustado ao xam: mão e ombros **suavizados** (0,1 e 0,05 por quadro), só o vetor ombro → mão (sem cotovelo), braço de **25° a 65°** a partir de caído (a janela 205–245° do xam parece 225° ± 20°), usa o estado do rastreador antes dos alertas, e é ignorado com o guia aberto.
- **Teste por simulação:** quadro 2000 de `~/kinect_rec_guia` com o braço esquerdo caído apagado e um braço desenhado a 45° na profundidade do ombro (`armsim`, no scratchpad). O rastreador do Kinect Sports mediu 43° (down 0,73, side 0,66) e o guia abriu após 3 s. Gravação sintética em `~/kinect_sim_guia` (540 quadros, em loop).
- **Testes ao vivo:** o usuário levantava o braço até a horizontal e voltava, passando pela faixa por menos de 1 s. Os alertas que o rebaixavam eram "perto demais" (0,42 e 0,85 m) e "borda".
- Formato das gravações: profundidade 640×480 em 11 bits empacotados do bit mais alto (422.400 bytes); cor Bayer 640×480 (307.200 bytes); `calibration.bin` de 142 bytes; a reprodução fica em loop.

## 16g. PsCam 20 e as intrínsecas (2026-10-28)

### As propriedades do sensor (`FUN_819838D0`)
O xam lê propriedades do Kinect por número, em três faixas, cada uma por um driver do kernel:

| Faixa | Função | Driver | Pedido |
|---|---|---|---|
| 1 a 1000 | `FUN_81983490` | `McaDeviceRequest` (matriz de microfones) | código 0x10 com o número; resposta copiada (0x20 bytes) |
| 1001 a 2000 | `FUN_819835B0` | `DetroitDeviceRequest` | pedido com 0x120000 no cabeçalho; 0x3E9 a 0x3ED têm conversões (÷ 0x319750 etc.) |
| acima de 2000 | `FUN_819837A8` | `PsCamDeviceRequest` **código 20** (0x14) | número da propriedade; resposta de um u32 |

Quando o tipo de inclinação (`FUN_816FA088`) é 1 ou 2, as propriedades 0x42/0x43/0x45 e 0x3EB/0x3EC/0x3ED trocam de driver entre Mca e Detroit: o mesmo dado vem de placas diferentes conforme o modelo do sensor.

A resposta do PsCam 20 vira uma estrutura de **5 palavras** `{1, valor, a, b, c}`, em que `a`, `b` e `c` repetem o valor ou ficam 0 conforme a propriedade (0x7DE a 0x7E5 pares → `b`; ímpares → `c`; 0x7E6 a 0x7EA e 0x834 a 0x839 → zeros; 0x83A em diante → `a` e `c`). `XamNuiGetFanRate` pede a **0x838** e devolve o byte 7 da estrutura: o **byte baixo do valor**.

### Sonda no firmware da câmera
Com uma sonda libusb (`GET_PARAM`, comando 0x02, resposta `{status u16, valor u16}`), o firmware do sensor daqui respondeu:
- 0x12 (formato de profundidade) = 3, 0x06 (fluxo de profundidade) = 2: o comando funciona.
- **0x838 = 0**, e também 0 para **todas** as propriedades de 0x7DE a 0x83F, antes e durante 4 minutos com o projetor e as duas câmeras ligados. O firmware responde 0 com status 0 até para IDs inválidos (0xFFFF), então não dá para distinguir "ventilador parado" de "propriedade desconhecida".
- `GET_CMOS_REGISTER` (0x0E) e `READ_AHB` (0x10) com 0x838 dão erro (status 7 e 0xC).

Ou seja: **se** o driver PsCam do console repassa o número da propriedade ao `GET_PARAM`, o ventilador estava parado; se ele traduz o número para outro comando, não se sabe qual. Só o kernel real (na cadeia de bootloaders da NAND, ainda não decifrada) responderia.

**No Xenia:** `UsbCamera::ReadParameter(id)` manda o `GET_PARAM` à câmera ao vivo (`false` em gravações), e `XamNuiGetFanRate` devolve o byte baixo da 0x838: hoje, 0 como o sensor. Continua 🟡.

### As intrínsecas (PsCam 46, seletor 0x100)
No Dash.FieldCalibration.lex (`FUN_9BD22C38`), os 4 floats entram numa matriz 3×3 de doubles: `[0][0]` = 1º, `[1][1]` = 2º, `[0][2]` = 3º, `[1][2]` = 4º, ou seja **fx, fy, cx, cy**, e a matriz serve ao cálculo da calibração da **profundidade** (A e B, seção acima). Logo são as intrínsecas da **câmera de profundidade**, não da de cor.

O sensor guarda no bloco de parâmetros fixos (comando 0x04, 334 bytes, plano zero no byte 94) a distância de referência (120 mm) e o tamanho do pixel nessa distância (0,1042 mm): a focal em pixels a 640×480 é `120 / (2 × 0,1042)` = **575,8** (nominal do SDK do Kinect: 571,26; a da câmera de cor, 531,15, era o que o Xenia devolvia antes). Nenhum dos floats da tabela de exposição (49,1; 7,5; 0,1333) nem o 34,19 do seletor 0 aparecem nesse bloco: a tabela do `XamNuiGetTrueColorInfo` continua a dos jogos.

**No Xenia:** `UsbCamera::DepthFocalLength()` calcula a focal da calibração (também da `calibration.bin` das gravações), e `XamNuiGetCameraIntrinsics` devolve `{focal, focal, 320, 240}`. Passa a ✅.
