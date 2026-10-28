# Michael Jackson The Experience (título 55530851)

Michael Jackson The Experience, da Ubisoft (versão Kinect da Ubisoft Montreal, 2011): dança e canto com as músicas do Michael Jackson. Executável `Default.xex` (7 MB, PE original `Default.exe`, compilado em 24/02/2011) dentro do contêiner GOD em `<HD>/Xbox 360/Kinect Michael Jackson The Experience/Content/0000000000000000/55530851/` (HD externo), extraído com `xenia-vfs-dump` em `~/Kixenia/kinect-sdk-re/games/extraidos/mj/` (5 GB). No disco: `Database.xmplr` (modelos do rastreador), `NuiIdentity.bin.be` (identidade), `AvatarAssetPack`, o índice `AllFats.BIN` com centenas de pacotes `*_L.LIN`, os vídeos Bink (`Bink/`, `Bink_Intro/`), os bancos de som do motor **Dare** (`Audio/xenon/Dare/Packages/`) e `Data/PostureRecording/*.dat` (as poses gravadas de cada coreografia). Não há modelos de voz (`nuisp*`).

Runtime NUI **2.0.20353** (log: `NUI: title runtime 5/4F81`; bibliotecas `NUI 2.0.20353.1`, `NUISP`, `XHV2`, `XMIC`, todas do XDK 20353): entre o do [Kinect-Adventures](kinect-adventures.md) (2.0.11775) e o do [Kinect-Star-Wars](kinect-star-wars.md)/[Kinect-Rush](kinect-rush.md) (2.0.21076). **Parece-se com o Kinect Rush e o Star Wars**: das 53 funções do jogo que chamam o Kinect, 52 têm as mesmas importações de uma função do Rush e 37 também o mesmo tamanho (Star Wars: 38; Joy Ride: 31; Adventures: 32). O motor de inclinação é o do Rush quase byte a byte (só a máquina de estados tem 28 bytes a menos).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Rush](kinect-rush.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo e o XamCache). Não houve teste com o Kinect real nem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir, logos | ✅ | Vídeo `Bink_Intro\MJ_UbiLogo_FullScreen.bik`, bancos `CommonInit`/`CommonSong`, pacotes `.LIN`. 150 s sem travar e sem `Host-side crash`; 3756 quadros (~25 por segundo). Um aviso do escalonador ("no guest frame presented in 2000 watchdog ticks") no quadro 200, durante o carregamento; depois segue normal. |
| Runtime NUI | ✅ | `NUI: title runtime 5/4F81 with flags 0000080B, HUD on`. Abre `Database.xmplr`, `NuiIdentity.bin.be` e os modelos `NuiIdentityPCA`/`NuiIdentityNN`. |
| Câmera | ✅ | `PsCamDeviceRequest` 8730 vezes (thread `CamTools`), sempre depois do `XamXStudioRequest`. |
| Ajuste do sensor | 🟡 (sem motor) | Pede **−25°** no quadro 9 (sem chão guardado) e **0°** no quadro 71. Depois disso nenhum `XamNuiCameraTiltGetStatus`, `XamXStudioRequest(0x1005)` ou `RememberFloor` em 150 s: o chão não foi guardado, mas a busca também não recomeçou (sem loop, ao contrário do Adventures). |
| Menu | 🟡 | Toca o vídeo de atração `Bink\MJ_Attract.bik` nos quadros 267 e 3183: parou na tela de título esperando o jogador (como o Rush com a gravação). |
| Rastreamento | 🟡 | 20 de 55 amostras (36%) com o jogador rastreado; o primeiro esqueleto aparece no quadro de esqueleto 540. |
| Kinect ID (rostos) | ✅ | `XamCacheOpenFile` e `XamCacheReset(0x18)` chamadas no quadro 9, agora respondidas pelo Xenia; `XamReadBiometricData` uma vez. |
| Microfones do Kinect (canto) | ❌ | Ver *Microfones* abaixo: `XamVoiceGetMicArrayStatus` devolve 2 (sem microfones) com a gravação; e mesmo com o Kinect real o caminho do jogo passa por `XamVoiceCreate`, que é **stub** no Xenia. |
| Microfones USB/Lips/headset | ⚪ | Biblioteca XMIC: `MicDeviceRequest` (stub, nenhuma chamada em 150 s); `XamInputRawState`/`XamInputControl` ausentes (nenhuma chamada). |
| Avatares, fitness | — | Nenhuma importação `XamAvatar*` nem `XamFitness*` (o disco traz `AvatarAssetPack`, mas o jogo não usa a API). |
| Telas Nui do sistema | 🟡 | 7 `XamShowNui*UI` importadas; só `XamShowNuiMessageBoxUI` tem chamada direta (0x82747450). `XamShowNuiMarketplaceUI` é ausente. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 mj_replay 150 ~/Kixenia/kinect-sdk-re/games/extraidos/mj/Default.xex
```
(ou o arquivo de cabeçalho `.../55530851/00007000/36E7BE98173ACD42B7DE` no lugar do `Default.xex`).

## Microfones: quatro fontes para cantar

O jogo canta por quatro microfones (strings `MIC_KINECT_ACTIVE`, `MIC_HEADSET_ACTIVE`, `MIC_LIPS_ACTIVE`, `MIC_USB_ACTIVE`, "Kinect Mic Now ACTIVE"…). Os microfones do Kinect vão pelo **motor de voz XHV2** ligado ao executável (não pelo `XamVoiceGetMicArrayAudioEx` do Joy Ride e do Rush, que ele nem importa):

1. A thread **`MicThread`** (início 0x82F51328, criada no quadro 67) roda para sempre: atualiza o estado do microfone (0x82F4E8F0, 0x82F512C0) e dorme 1 ms (0x8273E4D0). A cada volta chama `XamVoiceGetMicArrayStatus` e `XamUserGetDeviceContext`: **860 mil e 1,7 milhão de chamadas em 150 s** (o grosso do log). Não é travamento, é a espera normal pelo microfone.
2. `XamVoiceGetMicArrayStatus` (ordinal 0x318), `XamVoiceGetDirectionalData` (0x31A, **ausente no Xenia**) e `XamVoiceSetMicArrayIdleUsers` (0x48C) **não estão na tabela de importações**: são buscadas em execução por `XexGetProcedureAddress` (0x828A39A8, com os ponteiros em 0x8310CFA0, 0x8310CFA4, 0x8310D018). Sem a export, 0x828A3A10 devolve 2 (sem microfones) e 0x828A3A78 devolve 0x80004005.
3. A leitura do áudio local do XHV2 (0x828A6B50, entrada 0x14 da tabela do motor montada em 0x828A6DF8) só segue se `XamVoiceGetMicArrayStatus` = **1**; senão devolve 0x8007048F (não conectado). Depois procura, entre as 4 portas de voz, uma do tipo 0x70000000 (o conjunto de microfones) e pede a direção do som (`XamVoiceGetDirectionalData`).
4. As portas vêm de `XamVoiceCreate(usuário, 0xF)` (0x828A8268), chamada para os usuários 0–3 no quadro 67, junto com `XamVoiceSetMicArrayIdleUsers(0x0F…0x00)`. No Xenia `XamVoiceCreate` é **stub e devolve ACCESS_DENIED** com o ponteiro zerado: nenhuma porta existe, então o canto pelo Kinect não deve funcionar nem com o sensor real.

No Xenia, `XamVoiceGetMicArrayStatus` devolve 1 só quando o áudio do Kinect está disponível (`microphones_available()`); com a gravação devolve 2.

Os microfones USB (Lips, Rock Band, headset) vão pela biblioteca **XMIC** (`MicDeviceRequest`, stub) e pelo `XamInputRawState`/`XamInputControl` (ausentes): nenhum foi chamado em 150 s.

As buscas por ordinal do `xbdm.xex` em 0x82754F90 e 0x82756600 (ordinais 0x9B–0xA0) só rodam em kit de desenvolvimento (`KeDebugMonitorData` e `XboxHardwareInfo` bit 0x10) e não têm a ver com o Kinect.

## Motor de inclinação e o status 'XtSs'

É o motor do runtime NUI do [Kinect-Rush](kinect-rush.md) e do [Kinect-Star-Wars](kinect-star-wars.md), aqui entre 0x826CEAF0 e 0x826D0F00. **'XtSs' em `0x830FAC60`**; estado do motor em 0x830FAB38–0x830FACB4. Campos do 'XtSs' lidos (busca por ponteiro-base, `mj_acessos_base.py`; aproximada):

| Campo | Nome no Xenia | Quem lê | Uso |
|---|---|---|---|
| +0x00, +0x04 | tamanho, 'XtSs' | 0x826CFC30, 0x826D0680 (gravam) | preenchidos antes de cada leitura |
| +0x24, +0x28 | `gravity[1]`, `gravity[2]` | 0x826D0680 | ângulo atual na máquina de estados |
| +0x2C | `motor_state` | 0x826CF228, 0x826CF2B8, 0x826CF490, 0x826D0680 | bits do motor; 0x10 força a busca |
| +0x3C | `floor_distance` | 9 funções do motor | **chão guardado**: 0 = começa a busca de −25° |
| +0x40 | `unknown_40` | 0x826CF490 | bit 0: pedido de ajuste recusado (ocupado) |
| +0x48, +0x4C | `floor_gravity[1]`, `floor_gravity[2]` | 0x826CEC10 | gravidade de quando o chão foi guardado |

Não foi visto lendo +0x20, +0x30..+0x38 nem +0x44 por essa busca (o Rush lê todos). Depende, portanto, dos campos que o Xenia passou a preencher na correção do 'XtSs' (chão guardado e gravidade do chão), como o Rush e o Joy Ride.

## Teste com o Kinect real: pendente

O que verificar ao vivo (worktree kinect-sistema, `live_test.sh` na sessão principal):

- **Motor de inclinação**: depois do −25° e do 0° iniciais, se a busca guarda o chão (`XamNuiCameraRememberFloor`) e não fica em loop; se o leitor do 'XtSs' (0x826CEEA0) passa a ser chamado.
- **Rastreamento e menus**: sair da tela de título (vídeo de atração) com o gesto do jogo, navegar o menu pela mão, entrar numa música e ver se a pontuação de dança sobe (as poses de `Data/PostureRecording`).
- **Microfones do Kinect**: se `XamVoiceGetMicArrayStatus` passa a devolver 1; esperado: o canto pelo Kinect **não** funciona porque `XamVoiceCreate` é stub (ACCESS_DENIED) e `XamVoiceGetDirectionalData` é ausente. Conferir no log se aparecem `XamVoiceGetDirectionalData`/`undefined extern` e se o jogo mostra "Kinect Mic" ativo.
- **Kinect ID**: se reconhece/registra o jogador (biometria, `XamCache*`).
- **Telas Nui**: se alguma `XamShowNui*UI` é aberta (MessageBox, Sign-in, Troubleshooter).

## Pendências
- `XamVoiceCreate`/`XamVoiceSubmitPacket`/`XamVoiceClose` (stubs) e `XamVoiceGetDirectionalData` (ausente): necessários para cantar pelos microfones do Kinect.
- `MicDeviceRequest` (stub), `XamInputRawState`, `XamInputControl` (ausentes): microfones USB (Lips) e headset; fora do Kinect.
- `XamShowNuiMarketplaceUI` ausente (sem chamada direta).
- `EtxProducerRegister` (chamado, "undefined extern"), `XamXlfsInitializeUploadQueue`, `XamBackgroundDownloadSetMode`, `XAudioGetDuckerLevel`: não afetam o Kinect.

## Arquivos da análise
- Jogo extraído: `~/Kixenia/kinect-sdk-re/games/extraidos/mj/`; cópia do XEX `~/Kixenia/kinect-sdk-re/games/mj.xex`; imagem plana `~/Kixenia/kinect-sdk-re/games/mj.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/mjvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática); cópias `mjpar0`, `mjpar1` para a decompilação paralela.
- Log do Xenia: `~/Kixenia/kinect-tools/logs/mj_replay.log` (393 MB, quase todo do `MicThread`).
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/mj/c_full/<endereço>.c` (77 funções, nenhuma com `halt_baddata`), com os thunks trocados pelos nomes das importações.
- Scripts (`games/decompilado/scripts/`): `mj_imports.py`, `mj_funcs.py`, `mj_runtime.py`, `mj_merge.py`, `mj_callers.py`, `mj_acessos_base.py`, `gen_mj.py`; `FnGraph.java`, `DumpImage.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x82228C00, 47 532 funções; `.text` 0x82290000–0x83060844.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/mj/`.**

**344 importações** (127 do `xam.xex`, 217 do `xboxkrnl.exe`), 3515 chamadas diretas no código, 110 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 246 implementadas, 30 aproximadas, 40 stubs, 27 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x826D0FE8 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera**: tenta `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest` (= Rush 0x82950A60). | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x826CEAF0 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: para o motor (`ElevationStopMovement`) nos erros e informa o status ao sistema (= Rush 0x82963668). | ✅ |
| 0x826CED98 | 92 | `XamNuiCameraElevationGetAngle` | 2 | Lê o ângulo atual do motor (`ElevationGetAngle`). | ✅ |
| 0x826CEDF8 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`); foi quem pediu o 0° no quadro 71 (= Rush 0x82963918). | ✅ |
| 0x826CEEA0 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'**: tenta `XamXStudioRequest(0x1005)` (Kinect Studio) e, se o Studio responder 0x80070057, chama `XamNuiCameraTiltGetStatus` (= Rush 0x829639C0). Sem chamada direta (fica num ponteiro); não rodou nos 150 s da gravação. | ✅ |
| 0x826CF228 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°** (= Rush 0x82963D48). Foi o pedido do quadro 9. | ✅ |
| 0x826CF2B8 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão contra a gravidade e o guarda (`XamNuiCameraRememberFloor`); informa o status ao sistema (= Rush 0x82963DD8). | ✅ |
| 0x826CF490 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura, flags); +0x40 bit 0 do 'XtSs' = ocupado (= Rush 0x82963FB0). | ✅ |
| 0x826CF920 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (`XamNuiCameraTiltSetCallback(0x826CF920)`, quadro 9): pedidos de ajuste do sistema, respondidos com `XamNuiCameraTiltReportStatus` (= Rush 0x82964258). | ✅ |
| 0x826CFCE0 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback do motor e o leitor de status (= Rush 0x82964618). | ✅ |
| 0x826D0680 | 2408 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação** (2408 bytes; a do Rush tem 2436): ângulo atual `atan2(+0x28, +0x24)`, degraus de busca, chão e mira do jogador. | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x826DCCD8 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | Sessão do Kinect ID: acompanha os jogadores (`XamUserNuiGetUserIndex`) e a sessão de identidade (`XamNuiIdentityGetSessionId`). | ✅ |
| 0x826DCEC8 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x826DE6F8 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário (`XamUserNuiGetEnrollmentIndex`) (= Rush 0x82971270). | ✅ |
| 0x826DE940 | 348 | `XamUserNuiGetUserIndex` | 1 | Para cada esqueleto, pergunta qual usuário está associado (`XamUserNuiGetUserIndex`). | ✅ |
| 0x826DF1E8 | 360 | `XamUserNuiGetUserIndex` | 0 |  | ✅ |
| 0x826DFCB8 | 472 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x826E01D8 | 704 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê, mescla e grava (`XamReadBiometricData`, `XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | ✅ |
| 0x826E0498 | 3216 | `XamNuiIdentityGetSessionId` | 8 |  | ✅ |
| 0x826E1A60 | 260 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: `XamCacheOpenFile`, `XamCacheReset(0x18)`, `XamCacheCloseFile` (as três agora existem no worktree kinect-sistema). | ✅ |
| 0x826E1B68 | 600 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`). | ✅ |
| 0x826E9498 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |

#### Estado do sensor e mensagens do runtime NUI

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82747450 | 56 | `XamShowNuiMessageBoxUI` | 1 |  | ✅ |
| 0x82747488 | 120 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82747500 | 140 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82747710 | 56 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82747790 | 40 | `XamNuiGetDeviceStatus` | 1 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só o campo de estado (= Rush 0x829473F0). | ✅ |
| 0x82748348 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82748388 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x827483D0 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82748418 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82748458 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82748490 | 76 | `XMsgInProcessCall` | 10 |  | ✅ |
| 0x827484E0 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x8274B938 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x8274B988 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x8274B9F8 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x8274BA60 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x8274BAB0 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x8274BB20 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x8275BF80 | 116 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x8275C230 | 168 | `XMsgStartIORequestEx` | 2 |  | ✅ |
| 0x8275C2D8 | 104 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82B21350 | 40 | `XMsgCancelIORequest` | 0 |  | ✅ |

#### Voz: motor XHV2 (headset e microfones do Kinect)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x828A6208 | 216 | `XamVoiceIsActiveProcess` | 3 |  | ✅ |
| 0x828A7448 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 | Motor de voz XHV2: confere se há headset (`XamVoiceHeadsetPresent`) e se o processo de voz está ativo. | ✅ |
| 0x828A7A88 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x828A7E18 | 600 | `XamVoiceSubmitPacket` | 1 | Motor de voz XHV2: envia um pacote de voz (`XamVoiceSubmitPacket`). | ✅ |
| 0x828A8178 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x828A8268 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 | Motor de voz XHV2: cria a porta de voz de cada usuário (`XamVoiceCreate(usuário, 0xF)`) — **stub no Xenia, devolve ACCESS_DENIED** (as 4 chamadas do quadro 67). | ✅ |
| 0x828A9F78 | 724 | `XamVoiceIsActiveProcess` | 3 |  | ✅ |
| 0x828AC3C8 | 208 | `XamVoiceSubmitPacket` | 2 |  | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x8267CAB0 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8730 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 4 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 8 | 3369 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 8 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 4 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 58562 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (65)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 34 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 8 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedFlushSList` | xboxkrnl 0x2B | 6 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 10 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 2 | 0 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 72001 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 56 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 2 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 2 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 39 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 86 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 44 | 9845 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 12 | 14338 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3345 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3345 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 33 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 95 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` ⭐ | xboxkrnl 0x9C | 1 | 8 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 66 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 9 | 6 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 7 | 2 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 10 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 21 | 59 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 4 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 18 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 42 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 44 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 2 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 217369 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 30 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 20 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 872 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 148 | 254 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 40 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1140 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 7 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (56)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoDismountVolume` | xboxkrnl 0x3B | 2 | 0 | ❌ |
| `IoDismountVolumeByFileHandle` | xboxkrnl 0x3C | 1 | 0 | ❌ |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 44 | 307 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 10 | 542 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 8 | 107 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 6 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 1 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 3 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 1 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 11 | 2 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 219 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 4 | 175 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 3 | 375 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 8 | 444 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 16 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 1 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 13 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 2 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 35 | 13733 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObLookupThreadByThreadId` ⭐ | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 2 | 2 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 3 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 16 | 13736 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 5 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 4 | 4 | 🟡 |

</details>

<details><summary><b>Rede</b> (36)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` ⭐ | xam 0x4A | 1 | 1 | ⚪ stub |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 1 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 1 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XNetLogonGetTitleID` | xam 0x136 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Vídeo e GPU</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 2 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 3757 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 2 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 1 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 3 | 3 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 3758 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 4 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 1 | 4 | 🟡 |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 6 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 11 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 5 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 40 | 313 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 29 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 35 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 16 | 0 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 17 | 1126 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 13 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 2 | 0 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 1 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 8 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 2 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 4 | 3 | ✅ |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 15 | 18985 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 6 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 6 | 5 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 1 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 15 | 4 | ✅ |
| `XexLoadImageHeaders` | xboxkrnl 0x19B | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (20)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 1 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 2 | 1 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 2 | 1829406 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 5 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 2 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 3 | 1 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 3 | 7 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (19)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 1 | 0 | ❌ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | 4 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 3 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMADisableContext` ⭐ | xboxkrnl 0x228 | 3 | 0 | ✅ |
| `XMAEnableContext` ⭐ | xboxkrnl 0x227 | 5 | 0 | ✅ |
| `XMAGetOutputBufferReadOffset` ⭐ | xboxkrnl 0x22B | 1 | 0 | ✅ |
| `XMAGetOutputBufferWriteOffset` ⭐ | xboxkrnl 0x229 | 1 | 0 | ✅ |
| `XMAInitializeContext` ⭐ | xboxkrnl 0x225 | 1 | 0 | ✅ |
| `XMAIsInputBuffer0Valid` ⭐ | xboxkrnl 0x22F | 1 | 0 | ✅ |
| `XMAIsInputBuffer1Valid` ⭐ | xboxkrnl 0x231 | 1 | 0 | ✅ |
| `XMAIsOutputBufferValid` ⭐ | xboxkrnl 0x22D | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |
| `XMASetInputBuffer0Valid` ⭐ | xboxkrnl 0x22E | 1 | 0 | ✅ |
| `XMASetInputBuffer1Valid` ⭐ | xboxkrnl 0x230 | 1 | 0 | ✅ |
| `XMASetOutputBufferReadOffset` ⭐ | xboxkrnl 0x22A | 1 | 0 | ✅ |
| `XMASetOutputBufferValid` ⭐ | xboxkrnl 0x22C | 2 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 9 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 1948 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 54 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 2 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 3 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 54 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 2621 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 2223 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8730 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 5 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 2 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 3 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Voz e microfones</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MicDeviceRequest` | xboxkrnl 0x2B1 | 4 | 0 | ⚪ stub |
| `RmcDeviceRequest` | xboxkrnl 0x2E1 | 5 | 0 | ❌ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 4 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 58562 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiMessageBoxUI` ⭐ | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputControl` ⭐ | xam 0x3DE | 1 | 0 | ❌ |
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetCapabilitiesEx` ⭐ | xam 0x2AD | 1 | 0 | 🟡 |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputRawState` ⭐ | xam 0x195 | 2 | 0 | ❌ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 4 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 8 | 3369 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 8 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 2 | 1 | ✅ |

</details>

<details><summary><b>Outras</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamBackgroundDownloadSetMode` ⭐ | xam 0x9C4 | 1 | 0 | ❌ |

</details>

⭐ = só este jogo importa (nem o Nike, nem o Kinect Sports, nem o Kinect Adventures).

**Ausentes no Xenia (27):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_XNetServerToInAddr`, `NtWriteFileGather`, `ObIsTitleObject`, `RmcDeviceRequest`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerLevel`, `XamBackgroundDownloadSetMode`, `XamInputControl`, `XamInputRawState`, `XamShowNuiMarketplaceUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.
