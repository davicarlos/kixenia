# Kinect Rush: A Disney-Pixar Adventure (título 4D5309B6)

Kinect Rush: A Disney-Pixar Adventure, da Asobo Studio (Microsoft Studios, 2012): o jogador corre pelos mundos de Os Incríveis, Up, Carros, Ratatouille e Toy Story. Executável `Default.xex` (19 MB) em `~/Downloads/Xbox360_extraido/Kinect Rush/`, com `Database.xmplr` (modelos do rastreador), `HeadPosition.xmplr`, `NuiHeadOrientation.bin.be` (orientação da cabeça), `NuiIdentity.bin.be` (identidade), `AvatarAssetPack`/`AvatarAwards` e os modelos de voz `nuisp1031` … `nuisp3084` (um por idioma). Motor próprio da Asobo (arquivos `.D36`, mundos em `WORLD\`, vídeos WMV). Runtime NUI **2.0.21076.0** (`NUI`, `NUIHNDL`, `NUIAUD`; `NUISP` 2.0.21076.2): o mesmo do [Kinect-Star-Wars](kinect-star-wars.md), entre o do [Kinect-Adventures](kinect-adventures.md) (2.0.11775.7) e o do [Nike-Kinect-Training](nike-kinect-training.md) e do [Kinect-Sports](kinect-sports.md) (2.0.21173.0).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Sports](kinect-sports.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo). Não houve teste com o Kinect real nem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir, logos | ✅ | Vídeos `LOGO_PIXAR` e `LOGO_ASOBO`, bancos de som, fontes. |
| Runtime NUI | ✅ | `NUI: title runtime 5/5254 with flags 0000440B, HUD on`. `XamFitnessInitialize`, `XamFitnessGetCurrentBodyProfileRecord`, `XamNuiGetTrueColorInfo` e `XamVoiceSetMicArrayIdleUsers` são buscadas por ordinal em tempo de execução (não estão na tabela de importações). |
| Ajuste do sensor | ✅ (sem motor) | Pede −25° no quadro 84 (sem chão guardado) e 1° no quadro 103 (alvo calculado); no quadro 147 o chão já está guardado (`RememberFloor`, distância 852 mm) e a busca não recomeça. Ao contrário do [Kinect-Adventures](kinect-adventures.md), não fica em loop. |
| Menu | ✅ | Carrega o mundo do menu (`WORLD\MENUPLAY\INTROMENU`, `PG_INCREDIBLES`, `PG_UP`) no quadro 731; no quadro 3607 toca o vídeo de atração (`ATTRACT.WMV`), sinal de que ficou parado na tela de título. |
| Rastreamento | 🟡 | 16 de 48 amostras (33%) com o jogador rastreado; o primeiro esqueleto aparece no quadro 699, a 2 m do sensor. |
| Voz (comandos) | 🟡 | O reconhecedor de voz está **no próprio executável** (sem módulo à parte, ao contrário do Star Wars): thread `NuiSpeech_Z`, abre `nuisp1033` (en-US; tenta `UPDATE:` e cai para `GAME:`) e os arquivos `L1033.*`/`ai031033.am`. Pede `XamVoiceGetMicArrayAudioEx` uma vez, logo ao abrir; `XAudioRegisterRenderDriverMECClient` não foi chamada em 150 s. A gravação não tem áudio. |
| Kinect ID (rostos) | 🟡 | Abre `NuiIdentity.bin.be` e os modelos `NuiIdentityPCA`/`NuiIdentityNN`; `XamCacheOpenFile` e `XamCacheReset` caem em "undefined extern call" (ausentes): o cache dos rostos falha. |
| Avatares | — | Nenhuma função `XamAvatar*` importada, apesar de o disco trazer `AvatarAssetPack` e `AvatarAwards` (o Adventures importa 12). |
| Telas Nui do sistema | ⚪ | 8 `XamShowNui*UI` importadas, nenhuma chamada diretamente; `XamShowNuiCommunitySessionsUI`, `…ControllerRequiredUI`, `…FriendsUI` e `…GamerCardUIForXUID` são ausentes. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 rush_replay 150 "$HOME/Downloads/Xbox360_extraido/Kinect Rush/Default.xex"
```

## Comparação com os outros jogos

| | Kinect Rush | [Kinect-Star-Wars](kinect-star-wars.md) | [Kinect-Adventures](kinect-adventures.md) | [Nike-Kinect-Training](nike-kinect-training.md) / [Kinect-Sports](kinect-sports.md) |
|---|---|---|---|---|
| Runtime NUI | 2.0.21076.0 | 2.0.21076.0 | 2.0.11775.7 | 2.0.21173.0 |
| Microfones | **Sim**, no executável (`XamVoiceGetMicArrayAudioEx`, `…Status`, `…UnderrunStatus`, eco pelo `XAudioRegisterRenderDriverMECClient`/`XAudioCaptureRenderDriverFrame`) | Sim, pelo módulo `NUISpeechDLL_360.xex` | Não | Nike: sim; Kinect Sports: não |
| Campos novos do 'XtSs' | **Sim**: +0x20..+0x28, +0x2C, +0x30..+0x3C (chão guardado), +0x40, +0x44..+0x4C (gravidade do chão) | Sim (mesmo motor) | Sim (mesmo motor) | Importam as mesmas funções de inclinação e `RememberFloor` |
| Kinect Studio | Status e câmera passam antes pelo `XamXStudioRequest` (0x1003, 0x1005) | Igual | Só a câmera (0x1003) | — |
| Kinect ID | Sim (`XamCache*` ausentes) | Sim (`XamCache*` ausentes) | Sim (`XamCache*` ausentes) | Sim |
| Avatares | Não | Não | 12 funções (stubs) | Kinect Sports: sim; Nike: não |

## Motor de inclinação do jogo (como funciona)

É o motor do runtime NUI do [Kinect-Adventures](kinect-adventures.md) e do [Kinect-Star-Wars](kinect-star-wars.md), aqui entre 0x82963600 e 0x82965A00. Estado global em 0x83235DD0–0x83235FE0: estado em `0x83235E10`, contador de espera em `0x83235E14`, degraus restantes em `0x83235E18`, ângulo-alvo em `0x83235E24`, **'XtSs' em `0x83235F10`**. O leitor do status fica num ponteiro (`0x831BF81C` → 0x829639C0) que tenta primeiro o Kinect Studio (`XamXStudioRequest(0x1005)`) e só depois `XamNuiCameraTiltGetStatus`.

1. **Início** (0x82963D48): sem chão guardado (+0x3C = 0) ou com a flag 0x10, inclina para −25°.
2. **Busca**: a cada 60 quadros sem chão, se o ângulo atual (`atan2(+0x28, +0x24)`) ainda é negativo, sobe um degrau `23 − 10·n` (−17°, −7°, 3°).
3. **Chão** (0x82963DD8): o plano do quadro de esqueleto vale se o produto com a gravidade passar do limiar (0x82963CD8); vai ao sistema por `XamNuiCameraRememberFloor`.
4. **Alvo** (0x82963A10): sem chão, pela altura pedida; com chão, `atan2` da altura sobre a distância ao chão (+0x3C), limitado a ±27°. Reinclina se `|alvo − atual| > 2,0°`.
5. **Jogador**: com o chão corrigido pela inclinação, mira o jogador e termina.

O 'XtSs' (0x50 bytes) é lido a cada quadro. Campos usados e onde:

| Campo | Endereço | Uso |
|---|---|---|
| +0x00 tamanho (0x50), +0x04 'XtSs' | 0x83235F10 | preenchidos antes de cada leitura (0x82964568, 0x82964EF8) |
| +0x20..+0x28 gravidade (3 inteiros) | 0x83235F30 | normalizada (0x82963C28); ângulo atual = `atan2(+0x28, +0x24)` (0x82964EF8) |
| +0x2C flags | 0x83235F3C | bits 0–1: motor parado/movendo; 0x04 e 0x08: erro (encerra o pedido); 0x10: força a busca |
| +0x30..+0x3C chão guardado (plano, distância em mm) | 0x83235F40 | **+0x3C ≠ 0 pula a busca de −25°** (0x82963D48, 0x82963FB0); a distância × 0,001 vai no status ao sistema; o plano é rodado pela inclinação (0x82963788, 0x82964748) |
| +0x40 | 0x83235F50 | bit 0: pedido recusado (ocupado) (0x82963FB0) |
| +0x44..+0x4C gravidade do chão | 0x83235F54 | gravidade de quando o chão foi guardado: `atan2(+0x4C, +0x48)` roda o plano (0x82963788); com a gravidade atual dá o eixo e o ângulo da correção (`acos` do produto, produto vetorial; 0x82964748) |

Ou seja: com o 'XtSs' completo do worktree kinect-sistema, o jogo guarda o chão uma vez e não refaz a busca; foi o que aconteceu na gravação (só dois pedidos de inclinação).

## Pendências
- Testar com o Kinect real (motor, aviso de sensor movido, comandos de voz, gesto para sair da tela de título).
- Comandos de voz: confirmar que `XamVoiceGetMicArrayAudioEx` devolve áudio e o que o `XAudioRegisterRenderDriverMECClient` precisa (cancelamento de eco).
- `XamCacheOpenFile`, `XamCacheReset`, `XamCacheCloseFile` (cache do Kinect ID), as mesmas do Adventures e do Star Wars.
- `XamShowNuiCommunitySessionsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`: ausentes; nenhuma chamada vista.
- `EtxProducerRegister`/`EtxProducerUnregister`, `XamXlfsInitializeUploadQueue`, `XAudioGetDucker*`: chamadas ou importadas, não afetam o Kinect.
- `XamNuiGetTrueColorInfo` (por ordinal, 2 chamadas) está marcada como aproximada (kSketchy) no Xenia.

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/rush.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/rushvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática); cópias `rushpar0`…`rushpar3` para a decompilação paralela.
- Log do Xenia: `~/Kixenia/kinect-tools/logs/rush_replay.log`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/rush/c_full/<endereço>.c` (168 funções, nenhuma com `halt_baddata`), com os thunks trocados pelos nomes das importações.
- Scripts: `rush_imports.py`, `rush_funcs.py`, `rush_merge.py`, `gen_rush.py` (`games/decompilado/scripts/`), `FnGraph.java`, `ReadF.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x82337800, 46 198 funções; `.text` 0x823A0000–0x831987EC.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/rush/`.**

**353 importações** (149 do `xam.xex`, 204 do `xboxkrnl.exe`), 3338 chamadas diretas no código, 127 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 246 implementadas, 30 aproximadas, 43 stubs, 33 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Estado do sensor e mensagens do runtime NUI

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x829473F0 | 40 | `XamNuiGetDeviceStatus` | 5 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só o campo de estado. | ✅ |
| 0x82947420 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82947498 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82947510 | 140 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x829475A8 | 184 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82947708 | 216 | `XMsgStartIORequest` | 2 |  | ✅ |

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82950A60 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82963668 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: traduz o código de saída, para o motor (`ElevationStopMovement`) nos erros 4 e 0x19 e informa o status ao sistema (0x2B00C). | ✅ |
| 0x82963918 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`). | ✅ |
| 0x829639C0 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'**: tenta `XamXStudioRequest(0x1005)` (status gravado pelo Kinect Studio) e, se o Studio não responder (0x80070057), chama `XamNuiCameraTiltGetStatus`. Fica num ponteiro (0x831BF81C) que o motor chama a cada quadro. | ✅ |
| 0x82963D48 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | ✅ |
| 0x82963DD8 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão do quadro de esqueleto contra a gravidade (0x82963CD8) e o guarda no sistema (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis; com o motor parado e erro, encerra o pedido (0x2B006/0x2B00C). | ✅ |
| 0x82963FB0 | 676 | `XamNuiCameraElevationGetAngle` | 1 | **Pedido de ajuste automático** (distância, altura, flags). +0x40 bit 0 do 'XtSs' = ocupado (devolve 5). Flag 0x20: só confere. Flag 0x08: recalcula o alvo (0x82963A10) e inclina se `\|alvo − ângulo\| > 2,0°` (double em 0x82001928): o "sensor foi movido". Sem flag: começa a busca (0x82963D48). | ✅ |
| 0x82964258 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (0x2B005…0x2B00B): AutoTilt, StopMovement, ReverseAutoTilt e AdjustTilt do sistema; responde com `XamNuiCameraTiltReportStatus`. | ✅ |
| 0x82964618 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x82964258)`) e põe 0x829639C0 no ponteiro do leitor de status. | ✅ |
| 0x82964EF8 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: lê o 'XtSs', calcula o ângulo atual `atan2(+0x28, +0x24)`; na busca sobe degraus `23 − 10·n` (−17°, −7°, 3°) a cada 60 quadros sem chão; com o chão, corrige-o pela gravidade do chão (+0x44..+0x4C) e mira o jogador; erro do motor (+0x2C bit 0x08) encerra com 0x1F. | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x8295B5D8 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x8295B7F8 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x8295D788 | 356 | `XamUserNuiGetUserIndex` | 0 | Para cada um dos 8 esqueletos, pergunta ao sistema qual usuário está associado (`XamUserNuiGetUserIndex`) e acorda o motor de identidade quando muda. | ✅ |
| 0x8295E570 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x8295E6F8 | 3200 | `XamNuiIdentityGetSessionId` | 5 | Sessão do Kinect ID: acompanha os jogadores e o identificador da sessão de identidade (`XamNuiIdentityGetSessionId`). | ✅ |
| 0x8296CEA0 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82971270 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário: `XamUserNuiGetEnrollmentIndex`; sem registro, lê o perfil pelo XMsg (0x32000 bytes). | ✅ |
| 0x829714B8 | 336 | `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82971738 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: abre até 256 arquivos com `XamCacheOpenFile` (cache 0x18), `XamCacheReset` e fecha. **As três são ausentes no Xenia.** | ✅ |
| 0x82971830 | 488 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`). | ✅ |
| 0x82971E48 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê (`XamReadBiometricData`), mescla (0x82971BE0) e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D36BE8 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82D36C28 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82D36C70 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82D36CB8 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82D36CF8 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82D36D30 | 76 | `XMsgInProcessCall` | 9 |  | ✅ |
| 0x82D36D80 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82D36DC8 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D36E18 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x82D36E88 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D36EF0 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D36F40 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D36FB0 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D3AA80 | 40 | `XMsgCancelIORequest` | 9 |  | ✅ |
| 0x82D3B930 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82E715C0 | 432 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82E71770 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71820 | 172 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82E718D0 | 172 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71980 | 168 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71A28 | 156 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71AC8 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71B78 | 232 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82E71C60 | 220 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82E71D40 | 164 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71DE8 | 192 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82E71F78 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82E72120 | 252 | `XMsgInProcessCall` | 3 |  | ✅ |
| 0x82E72328 | 352 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82E72488 | 304 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82E725B8 | 752 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82E72910 | 72 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82E72958 | 88 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82E82F80 | 168 | `XMsgStartIORequestEx` | 2 |  | ✅ |
| 0x82E83028 | 104 | `XMsgInProcessCall` | 2 |  | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82E75F90 | 200 | `XamVoiceIsActiveProcess` | 3 |  | ✅ |
| 0x82E77228 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 |  | ✅ |
| 0x82E77868 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x82E77BF8 | 600 | `XamVoiceSubmitPacket` | 1 |  | ✅ |
| 0x82E77F58 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x82E78048 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 |  | ✅ |
| 0x82E7AED0 | 688 | `XamVoiceIsActiveProcess` | 3 |  | ✅ |
| 0x82E7D2F8 | 208 | `XamVoiceSubmitPacket` | 2 |  | ✅ |

#### Microfones e reconhecimento de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82EBA480 | 224 | `XamNuiGetDeviceSerialNumber` | 1 | Inicia o registro de eventos do runtime: número de série do sensor (`XamNuiGetDeviceSerialNumber`), procura a export 0x8E1 do xam e `EtxProducerRegister` (ausente). | ✅ |
| 0x82EBA668 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | **Laço de áudio dos microfones**: lê os quadros de `XamVoiceGetMicArrayAudioEx`, confere `…UnderrunStatus` e manda o áudio de referência ao cancelador de eco (`XAudioCaptureRenderDriverFrame`). | ✅ |
| 0x82EBB528 | 1252 | `XAudioUnregisterRenderDriverMECClient` | 4 | Desliga o áudio dos microfones e o cancelador de eco (`XAudioUnregisterRenderDriverMECClient`). | ✅ |
| 0x82EBBA88 | 1504 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | **Abre o áudio dos microfones** para o reconhecedor de voz: aloca os buffers (0x6000, 0x1B000, 0x4800…), pede `XamVoiceGetMicArrayAudioEx` e registra o cliente de cancelamento de eco (`XAudioRegisterRenderDriverMECClient`). | ✅ |
| 0x82ED3608 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | Se o conjunto de microfones existe (`XamVoiceGetMicArrayStatus` ≠ 2), lê o número de série do sensor. | ✅ |
| 0x82ED36D0 | 124 | `XamNuiGetDeviceStatus` | 1 | A cada 60 s, lê `XamNuiGetDeviceStatus` e guarda um bit (sensor presente/pronto). | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D474C0 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8931 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 1 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 14 | 2967 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 23 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 9 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ❌ |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | ❌ |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | ❌ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 4120 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 5 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiCommunitySessionsUI` | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 1 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 1 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 1 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 59647 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (62)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 52 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 18 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 1 | 1 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 39 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 6 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 60 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 1 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 6 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 43 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 7 | 14 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 13157 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 23 | 14813 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 2918 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 2918 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 47 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 102488 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 98 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 10 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 1 | 1 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 1 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 3 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 6 | 42 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 6 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 32 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 43 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 45 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 1 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 73 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 9 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 8 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 863 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 163 | 927 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 6 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1009 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 1 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 7 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 6 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (56)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `IoCheckShareAccess` | xboxkrnl 0x34 | 2 | 0 | ❌ |
| `IoCompleteRequest` | xboxkrnl 0x35 | 11 | 0 | ❌ |
| `IoCreateDevice` | xboxkrnl 0x37 | 1 | 0 | ⚪ stub |
| `IoDeleteDevice` | xboxkrnl 0x39 | 1 | 0 | ⚪ stub |
| `IoInvalidDeviceRequest` | xboxkrnl 0x41 | 0 | 0 | ❌ |
| `IoRemoveShareAccess` | xboxkrnl 0x45 | 2 | 0 | ❌ |
| `IoSetShareAccess` | xboxkrnl 0x47 | 3 | 0 | ❌ |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 20 | 272 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 5 | 485 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 176 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 2 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 1 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 1 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 1 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 0 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 1 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 3 | 82 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 2 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 2 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 11 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 1 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 6 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 4 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 37 | 126252 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 4 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 11 | 126253 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ❌ |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | ❌ |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | ❌ |
| `XamContentClose` | xam 0x25A | 9 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 1 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 19 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 10 | 6 | 🟡 |

</details>

<details><summary><b>Rede</b> (44)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 1 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 4117 | ✅ |
| `NetDll_XNetGetXnAddrPlatform` ⭐ | xam 0x52 | 1 | 0 | ❌ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Vídeo e GPU</b> (29)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 4 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4202 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 1 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 1 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4203 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoCapabilities` | xam 0x49E | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 11 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 1 | 1 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 14 | 3474 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 13 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 1 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 4 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 17 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 0 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 2 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 2937 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 2 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 18 | 17724 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 3 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 8 | 22 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 8 | 27 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexLoadImageHeaders` | xboxkrnl 0x19B | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 13 | 3 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 15 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 3 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 4 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 36 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 8 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 4 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 24 | 91 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 18 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 22 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 14 | 3 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (23)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 9 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 5 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 5 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 112319 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 1 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 1 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 32949 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 3 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 5 | 7 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Áudio</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 28079 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 3 | 3 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (14)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 3975 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 485 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 2165 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 5 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 1676 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 1033 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 6 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 7 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8931 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 4120 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 1 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 1 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 1 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 59647 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiCommunitySessionsUI` ⭐ | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 29 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 0 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 6 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 12 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 1 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 14 | 2967 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 23 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 9 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 5 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 1 | 🟡 |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 1 | 1 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 5 | 0 | ✅ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 11 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 715 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 3 | 9 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike, nem o Kinect Sports, nem o Kinect Adventures).

**Ausentes no Xenia (33):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_XNetGetXnAddrPlatform`, `NetDll_XNetServerToInAddr`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`, `XamShowFriendsUI`, `XamShowGamerCardUIForXUID`, `XamShowNuiCommunitySessionsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.
