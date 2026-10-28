# Kinect Star Wars (título 4D53090E)

Kinect Star Wars, da Terminal Reality (LucasArts/Microsoft, 2012). Executável `default.xex` (18 MB) em `~/Downloads/Xbox360_extraido/Kinect Star Wars/`, com `Database.xmplr` (modelos do rastreador), `NuiIdentity.bin.be` (identidade), `AvatarAssetPack`, os modelos de voz `nuisp1031` … `nuisp3084` (um por idioma) e dois módulos: **`NUISpeechDLL_360.xex`** (reconhecimento de voz) e `MSTagDLL_360.xex` (Microsoft Tag). Motor próprio (Infernal Engine, pacotes `XB2PKG*.POD`), áudio Wwise. Runtime NUI **2.0.21076.0** (`NUI`, `NUIHNDL`; o módulo de voz traz `NUISP` 2.0.21076.2): entre o do Adventures (2.0.11775.7) e o do Nike e do Kinect Sports (2.0.21173.0).

Relacionado: [Kinect-Adventures](kinect-adventures.md), [Kinect-Sports](kinect-sports.md), [Nike-Kinect-Training](nike-kinect-training.md), [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md).

## Estado no Xenia Edge

> [!TIP]
> **Roda com o Kinect real (2026-10-26, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo): o jogo ajusta o sensor, rastreia o jogador, chega à fase de Kashyyyk e salva o jogo. Logs `sw_live.log` (23 mil quadros) e `sw_live2.log`; a análise abaixo também usa a gravação `kinect_rec_aovivo2` (`sw_replay.log`, 150 s).**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e carregar | ✅ | Roda os 150 s sem travar; chega ao quadro 3642. No quadro 866 o guia do agendador acusa 2000 ticks sem quadro novo enquanto o jogo carrega o módulo de voz, e depois segue. |
| Runtime NUI | ✅ | `NUI: title runtime 5/5254 with flags 00004C0B`. `XamFitnessInitialize` e `XamNuiGetTrueColorInfo` são buscadas por ordinal em tempo de execução (não estão na tabela de importações). |
| Motor (Kinect real) | ✅ | O jogo pede −25° e, depois de uns 40 quadros, 1°; o sensor vai e volta. Com um chão já guardado não faz os degraus −17°/−7°/3°. A inclinação manual pelo Kinect Tuner (7°, 14°, 23°, 1°, −2°) funciona com o jogo aberto, e o jogo não refaz o ajuste depois dela. Chão guardado a 884 mm; gravidade real do acelerômetro (−0,016; 1,000; 0,013). |
| Ajuste do sensor (gravação) | ✅ (sem motor) | Pede −25° no quadro 76 (sem chão guardado), 0° no quadro 134; o rastreador acha o chão no quadro 138 e o jogo o guarda (`RememberFloor`) por volta do quadro 474. Depois disso o 'XtSs' devolve o chão (+0x30..+0x3C) e a gravidade do chão (+0x44..+0x4C). |
| Rastreamento | 🟡 | Ao vivo: 23 de 46 amostras com o jogador rastreado (50%) na segunda sessão; na primeira, rastreado do quadro ~1300 ao ~5700 e quase nada depois, já na fase de Kashyyyk. Gravação: 18 de 54 amostras com o jogador rastreado (33%) (Adventures 27%, Kinect Sports 55% na mesma gravação). |
| Câmera (PsCam) | ✅ | 8887 chamadas a `PsCamDeviceRequest`. |
| Kinect ID (rostos) | 🟡 | `XamCacheOpenFile`/`XamCacheReset`/`XamCacheCloseFile` ausentes: a limpeza do cache de rostos (0x82B1E318) cai em "undefined extern call" no quadro 76. |
| Voz (microfones) | ✅ | O módulo `NUISpeechDLL_360.xex` carrega e abre o modelo `nuisp1033` (en-US; tenta `UPDATE:` e cai para `GAME:`). Com o Kinect real lê os microfones a cada quadro (`XamVoiceGetMicArrayAudioEx`, 10 417 chamadas na primeira sessão), e o nível aparece no Kinect Tuner quando se fala. Com a gravação não lê (não há áudio gravado). |
| Voz (comandos) | ❔ | O log não mostra o que o reconhecedor entende; falta conferir na tela se algum comando é aceito. Na primeira sessão ao vivo a leitura dos microfones parou no quadro 4542 (de 23 mil); na segunda foi até o fim. |
| Câmera USB (XUsbcam) | — | Importada, mas não chamada; quatro funções ausentes no Xenia. |
| Avatares | — | O jogo não importa a API de avatares (`XamAvatar*`), apesar de trazer o `AvatarAssetPack`. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./live_test.sh sw_live "$HOME/Downloads/Xbox360_extraido/Kinect Star Wars/default.xex"
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 sw_replay 150 "$HOME/Downloads/Xbox360_extraido/Kinect Star Wars/default.xex"
```

## O que o jogo pede do sistema

| | Kinect Star Wars | Kinect Adventures | Kinect Sports / Nike |
|---|---|---|---|
| Runtime NUI | 2.0.21076.0 | 2.0.11775.7 | 2.0.21173.0 |
| Microfones | **Sim**, pelo módulo `NUISpeechDLL_360.xex` (`XamVoiceGetMicArrayAudioEx`, `…Status`, `…UnderrunStatus`, eco pelo `XAudioRegisterRenderDriverMECClient`/`XAudioCaptureRenderDriverFrame`); o executável só chama `XamVoiceSetMicArrayIdleUsers` (por ordinal) | Não | Nike: sim, no próprio executável (`XamVoiceGetMicArray*`, `MicDeviceRequest`); Kinect Sports: não |
| Campos novos do 'XtSs' | **Sim**: +0x20 (gravidade), +0x24/+0x28 (ângulo), +0x2C (flags), +0x30..+0x3C (chão guardado), +0x40 (flag ocupado), +0x44..+0x4C (gravidade do chão) | Sim (mesmo motor) | Importam as mesmas funções de inclinação e `RememberFloor` |
| Kinect Studio | Ponteiros trocáveis para o PsCam e o status de inclinação (`XamXStudioRequest` 1, 0x1003, 0x1005) | Testa `XamXStudioRequest(0x1003)` a cada chamada | Importam `XamXStudioRequest` |
| XUsbcam (Live Vision) | Importada, não usada | Não | Nike: importada |
| Avatares | Não | 12 funções (stubs) | Kinect Sports: sim; Nike: não |

## Motor de inclinação do jogo (como funciona)

É o mesmo motor do runtime NUI do [Kinect-Adventures](kinect-adventures.md), com os estados renumerados e duas diferenças: o status 'XtSs' e o PsCam passam por ponteiros que o Kinect Studio pode trocar, e o motor fica entre 0x82B01400 e 0x82B03A00. Estado global em 0x83AF5A60–0x83AF5C80: estado em `0x83AF5AB0`, contador de espera em `0x83AF5AB4`, degraus restantes em `0x83AF5AB8`, ângulo-alvo em `0x83AF5AC4`, altura do jogador em `0x83AF5C18` (XConfig 7/9; padrão 1800 mm), **'XtSs' em `0x83AF5BB0`**.

O 'XtSs' (0x50 bytes) é lido a cada quadro. Campos usados e onde:

| Campo | Endereço | Uso |
|---|---|---|
| +0x20..+0x28 gravidade (3 inteiros) | 0x83AF5BD0 | normalizada (0x82B01A78); ângulo atual = `atan2(+0x28, +0x24)` |
| +0x2C flags | 0x83AF5BDC | bits 0–1: motor parado/movendo; 0x04 e 0x08: erro (encerra o pedido); 0x10: força a busca |
| +0x30..+0x3C chão guardado (plano, distância em mm) | 0x83AF5BE0 | **+0x3C ≠ 0 pula a busca de −25°**; o plano é passado ao rastreador (0x82B3BB20) e corrigido pela inclinação (0x82B02780) |
| +0x40 | 0x83AF5BF0 | bit 0: pedido recusado (ocupado) |
| +0x44..+0x4C gravidade do chão | 0x83AF5BF4 | ângulo da gravidade quando o chão foi guardado; com a gravidade atual dá a rotação que corrige o chão (0x82B02780, 0x82B02F30, 0x82B015D8) |

Ou seja: com o 'XtSs' completo do worktree kinect-sistema, depois do primeiro `RememberFloor` o jogo não refaz a busca, e o rastreador recebe o chão do sistema já corrigido pela inclinação. Sem esses campos (o Xenia antigo devolvia zero), cada pedido de ajuste recomeçaria em −25°.

## Pendências
- Comandos de voz: conferir na tela se o jogo reconhece algum; por que a leitura dos microfones parou no quadro 4542 na primeira sessão; o que o `XAudioRegisterRenderDriverMECClient` precisa (cancelamento de eco).
- Aviso de sensor movido: não apareceu nos testes; mover o sensor com a mão para conferir.
- `XamCacheOpenFile`, `XamCacheReset`, `XamCacheCloseFile` (cache do Kinect ID), as mesmas do Adventures.
- `XUsbcamDestroy`, `XUsbcamSetConfig`, `XUsbcamSetCaptureMode`, `XUsbcamReadFrame`: ausentes, mas não chamadas.
- `XamFitnessInitialize` (por ordinal): conferir o que o jogo faz com o resultado.
- `EtxProducerRegister`, `XamXlfsInitializeUploadQueue`, `XAudioGetDucker*`: chamadas ou importadas, não afetam o Kinect.
- O pico de 2000 ticks sem quadro ao carregar o módulo de voz (quadro 866).
- Decompilar o `NUISpeechDLL_360.xex` (o módulo não foi carregado no Ghidra).

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/starwars.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/swvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática); cópias `swpar0`…`swpar5` para a decompilação em paralelo.
- Log da execução: `~/Kixenia/kinect-tools/logs/sw_replay.log`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/sw/c_full/<endereço>.c`, com os thunks trocados pelos nomes das importações.
- Scripts (`games/decompilado/scripts/`): `sw_imports.py`, `sw_funcs.py`, `sw_runtime.py`, `sw_decomp.py`, `sw_xref.py` (chamadas diretas e ponteiros montados com `lis`/`addi`/`lwz`), `gen_starwars.py`; Ghidra: `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).

> [!TIP]
> **Lições da decompilação**
> - A tabela `.pdata` (0x8234DE00, 33 303 funções) não tem todas as funções: 0x82B01A78 (normaliza a gravidade do 'XtSs') fica entre duas entradas e foi decompilada à parte.
> - O status de inclinação e o PsCam não são chamados direto: o jogo os chama por ponteiros (0x83018768 e 0x830187CC), por isso as funções 0x82B01810 e 0x82B04410 aparecem com 0 chamadores; `sw_xref.py` acha quem carrega o ponteiro.
> - Sem análise, o Ghidra não dá chamadores nem chamados; o grafo de chamadas vem das instruções `bl` e da `.pdata`.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/sw/`.**

**320 importações** (132 do `xam.xex`, 188 do `xboxkrnl.exe`), 2327 chamadas diretas no código, 121 vistas em execução (gravação `kinect_rec_aovivo2`, 150 s, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 223 implementadas, 30 aproximadas, 40 stubs, 26 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82B04410 | 96 | `PsCamDeviceRequest` | 0 | Ponte do Kinect Studio para a câmera: tenta `XamXStudioRequest(0x1003)` e só se falhar chama `PsCamDeviceRequest`. Só é instalada no ponteiro 0x830187CC (por 0x82B06CB8) quando o Kinect Studio responde; sem ele o ponteiro aponta direto para o thunk de `PsCamDeviceRequest`, chamado por 12 funções (0x82B04F70–0x82B08EF8). | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82B014B8 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Responde ao sistema o fim de um pedido de inclinação: traduz o código interno (0x15, 0x19, 0x1F, 0x3E3…) para o status do xam, para o motor nos casos 0x19 e 4 e chama `XamNuiCameraTiltReportStatus` (0x2B006 ou 0x2B00C) com a distância do chão guardado (+0x3C × 0,001, em metros). | ✅ |
| 0x82B01768 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo entre −27° e +27° (`ElevationSetAngle`); fora disso devolve E_INVALIDARG. | ✅ |
| 0x82B01810 | 76 | `XamNuiCameraTiltGetStatus` | 0 | Mesma ponte para o status de inclinação: `XamXStudioRequest(0x1005)` e, se a resposta for 0x80070057, `XamNuiCameraTiltGetStatus`. Instalada no ponteiro 0x83018768 por 0x82B02650; sem o Kinect Studio o ponteiro vai direto ao thunk. | ✅ |
| 0x82B01B98 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: estado de busca, espera de 60 quadros e inclina para **−25°**, se o 'XtSs' não traz chão guardado (+0x3C = 0) ou se o bit 0x10 das flags (+0x2C) pede. | ✅ |
| 0x82B01C28 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: pega o plano do chão do quadro de esqueleto; se a normal estiver a menos de 10° da gravidade (0x82B01B28, cos 10° = 0,985), guarda o chão no sistema (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis. Com o motor parado por erro, informa o status. | ✅ |
| 0x82B01E00 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura e flags). Flag 0x20: só confere. Flag 0x08: recalcula o alvo e inclina se `\|alvo − atual\| > 2,0°` (0x8200D740). Sem flag: **se o 'XtSs' não tem chão guardado (+0x3C = 0), começa a busca de −25° (0x82B01B98); se tem, pula a busca** e vai direto para a mira no jogador. | ✅ |
| 0x82B02290 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam: atende AutoTilt (0x2B006), StopMovement, ReverseAutoTilt e AdjustTilt (0x2B009…) do sistema e responde com o status. | ✅ |
| 0x82B02650 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback`) e, se o Kinect Studio estiver presente (`XamXStudioRequest(1)`), troca o ponteiro do status de inclinação pela ponte 0x82B01810. | ✅ |
| 0x82B02F30 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto (chamada por 0x82B0A1A0). Lê o 'XtSs' pelo ponteiro 0x83018768 e o ângulo do acelerômetro (`atan2(+0x28, +0x24)`). Estado 1 (busca): sem chão guardado, espera 60 quadros e sobe um degrau enquanto o ângulo for negativo (−17°, −7°, 3°); com chão guardado, espera 14 quadros e calcula o alvo. Estado 2: corrige o chão guardado pela gravidade do chão (+0x44) e mira a cabeça do jogador (alvo limitado a ±27°). Estados 3 e 4: espera o motor parar e mais 15 quadros. Estado 5: fim. | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82B0D840 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82B0DA60 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82B0F9F0 | 356 | `XamUserNuiGetUserIndex` | 0 |  | ✅ |
| 0x82B10A78 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82B10C00 | 3200 | `XamNuiIdentityGetSessionId` | 5 |  | ✅ |
| 0x82B19A80 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82B1DE50 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Índice de cadastro do jogador no Kinect ID (`XamUserGetIndexFromXUID` → `XamUserNuiGetEnrollmentIndex`). | ✅ |
| 0x82B1E098 | 336 | `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82B1E318 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Limpa o cache de rostos do Kinect ID: abre (`XamCacheOpenFile`, tipo 0x18) os arquivos dos jogadores conhecidos, apaga o resto (`XamCacheReset(0x18)`) e fecha tudo. As três funções faltam no Xenia: "undefined extern call" no quadro 76. | ✅ |
| 0x82B1E410 | 488 | `XamReadBiometricData` | 1 |  | ✅ |
| 0x82B1EA28 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Grava o perfil biométrico: lê (`XamReadBiometricData`, buffer de 200 KB); se não existe, liga a biometria do usuário (`XamUserNuiEnableBiometric`) e grava (`XamWriteBiometricData`). | ✅ |

#### Câmera USB (XUsbcam, Xbox Live Vision)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82DCCB68 | 292 | `XUsbcamCreate` | 2 | Abre a câmera USB pela API `XUsbcam` (Xbox Live Vision), com um buffer de 300 KB. Chamada pelo módulo de imagem 0x82AFA9E8 (modos de 160×120 a 1280×960). Não chamada na gravação. | ✅ |
| 0x82DCCC90 | 160 | `XUsbcamDestroy` | 2 |  | ✅ |
| 0x82DCCD30 | 92 | `XUsbcamGetState` | 2 |  | ✅ |
| 0x82DCCD90 | 332 | `XUsbcamSetConfig` | 2 |  | ✅ |
| 0x82DCD058 | 284 | `XUsbcamSetCaptureMode` | 0 |  | ✅ |
| 0x82DCD178 | 240 | `XUsbcamReadFrame` | 0 |  | ✅ |
| 0x82DCDB98 | 432 | `XUsbcamSetConfig` | 0 |  | ✅ |
| 0x82DCDD48 | 428 | `XUsbcamSetConfig` | 1 |  | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x829A6F68 | 56 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x829A6FE8 | 40 | `XamNuiGetDeviceStatus` | 1 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só o campo de estado. | ✅ |
| 0x829A7010 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829A7088 | 140 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829A71C0 | 216 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829A7F68 | 40 | `XMsgCancelIORequest` | 3 |  | ✅ |
| 0x829AE240 | 352 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x829AE3A0 | 304 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x829AE4D0 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829AE718 | 432 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829AE8C8 | 176 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x829AE978 | 172 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829AEA28 | 168 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x829AEAD0 | 156 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829AEB70 | 176 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x829AEC20 | 192 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x829AEDB0 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x829AEF58 | 252 | `XMsgInProcessCall` | 3 |  | ✅ |
| 0x82FAEF30 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82FAEF70 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82FAEFB8 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82FAF000 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82FAF040 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82FAF078 | 76 | `XMsgInProcessCall` | 9 |  | ✅ |
| 0x82FAF0C8 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82FAF110 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82FAF160 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x82FAF1D0 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82FAF238 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82FAF288 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82FAF2F8 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82FB0448 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82FB84B8 | 168 | `XMsgStartIORequestEx` | 2 |  | ✅ |
| 0x82FB8560 | 104 | `XMsgInProcessCall` | 2 |  | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82DC13C8 | 200 | `XamVoiceIsActiveProcess` | 3 |  | ✅ |
| 0x82DC2598 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 |  | ✅ |
| 0x82DC2BD8 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x82DC2F68 | 600 | `XamVoiceSubmitPacket` | 1 |  | ✅ |
| 0x82DC32C8 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x82DC33B8 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 |  | ✅ |
| 0x82DC3D08 | 688 | `XamVoiceIsActiveProcess` | 3 |  | ✅ |
| 0x82DC79C8 | 208 | `XamVoiceSubmitPacket` | 2 |  | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D73238 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8887 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 13 | 3318 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 17 | 16 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 2 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |
| `XUsbcamCreate` | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |
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
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 4 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 6 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 1 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 59774 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (61)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 3 | 25 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 9 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 392 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 58 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 1 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 15 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 4 | 125 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 16189 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 15 | 11258 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3266 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3266 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 18 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 39 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 80 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 52 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 2 | 49 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 10 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 11 | 128 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 13 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 39 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 42 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 4392 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 17 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 12 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 521 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 65 | 440 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 4 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 624 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 2 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 118 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 7 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (43)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 23 | 3906 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 9 | 2441 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 1750 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 639 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 2 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 1 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 1 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 5 | 1422 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 64 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 2 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 446 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 5776 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 9 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 8 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 1 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 4 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 39 | 10844 | ✅ |
| `ObLookupThreadByThreadId` ⭐ | xboxkrnl 0x10B | 1 | 3 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 2 | 6 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 13 | 10842 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ❌ |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | ❌ |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | ❌ |
| `XamContentClose` | xam 0x25A | 5 | 2 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 2 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 19 | ✅ |
| `XamContentGetDeviceName` ⭐ | xam 0x25F | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 8 | 6 | 🟡 |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (34)</summary>

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
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 2780 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Vídeo e GPU</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 3643 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 1 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 2 | 1 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 3644 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoCapabilities` | xam 0x49E | 6 | 3 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 6 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 15 | 861 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 591 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 5 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 11 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 0 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 2 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 3274 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 4 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 587 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 18 | 22175 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 6 | 10 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 11 | 10 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 7 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 1 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 1 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 111682 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 1 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 22237 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 4 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 3 | 5 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 13 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 9 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 1 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 30 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 2 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 16 | 1549 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 16 | 0 | ❔ sem marca |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 13 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 2 | 3 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 3 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 4 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 5 | 638 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 427 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 197 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 3 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 1 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 11 | 158 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 9 | 135 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 6 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 7 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 27921 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8887 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 4 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 1 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 11 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 1 | 2 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 8 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 13 | 3318 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 17 | 16 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 2 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 59774 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Câmera Xbox Live Vision</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XUsbcamCreate` | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` | xboxkrnl 0x21C | 3 | 0 | ❌ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 2 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 6 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 13 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 736 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 2 | 10 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike, nem o Kinect Sports, nem o Kinect Adventures).

### Importações do módulo de voz (`NUISpeechDLL_360.xex`)

130 importações (23 do `xam.xex`). Abaixo as do xam, as de áudio e as que não estão implementadas no Xenia (as chamadas no código não foram contadas: o módulo não foi carregado no Ghidra).

| Importação | Biblioteca, ordinal | Em execução | Xenia |
|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 0 | ✅ |
| `XamContentClose` | xam 0x25A | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 0 | ✅ |
| `XamContentOpenFile` | xam 0x269 | 6 | 🟡 |
| `XamEnumerate` | xam 0x250 | 2 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 1 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 8 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 4 | ✅ |
| `XamUserGetCachedUserFlags` | xam 0x222 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 5 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 0 | ❌ |
| `EtxProducerLog` | xboxkrnl 0x332 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 0 | ❌ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 0 | ⚪ stub |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 392 | ⚪ stub |
| `RtlCaptureContext` | xboxkrnl 0x119 | 0 | ❌ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 0 | ❔ sem marca |
| `RtlRaiseException` | xboxkrnl 0x136 | 13 | ⚪ stub |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 0 | ❌ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 0 | ✅ |
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 0 | ✅ |
| `XeCryptRandom` | xboxkrnl 0x18A | 1 | ⚪ stub |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | ❌ |

**Ausentes no Xenia (26):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `NetDll_XNetServerToInAddr`, `NetDll_XnpLogonGetStatus`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XUsbcamDestroy`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`, `XamShowNuiControllerRequiredUI`, `XamShowNuiGamerCardUIForXUID`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.
