# Doodle Jump for Kinect (título 584113F6)

Doodle Jump for Kinect (Xbox Live Arcade, Lima Sky): o jogador pula no lugar e se inclina para guiar o boneco. Contêiner STFS original `FB16A62AB5806AFE00DF45FF0CF16086C1F1CF9858` (226 MB) em `<HD>/Xbox 360/Doodle Jump for Kinect/584113F6/000D0000/`; extraído com `xenia-vfs-dump` para `~/Kixenia/kinect-sdk-re/games/extraidos/doodlejump/`. Executável `default.xex` (14,5 MB, PE original `DoodleJump_s.exe`, `ArcadeInfo.xml` versão 1.0.62.0), com **`Database.xmplr` próprio** (modelos do rastreador; o jogo abre `stexemplar:\database.gmsodf` e `HeadPosition.mshdf`, como o [Kinect-Rush](kinect-rush.md)), `NuiIdentity.bin.be` e a pasta `DoodleJump\` com interface em Flash (Scaleform, `flash\dod_default\*.gfx`) e **scripts Lua em texto puro** (`GUI\*.lua`), áudio XACT e vídeos WMV. Motor próprio com prefixo `Sgi::`. Runtime NUI **2.0.21250.0** (no log, `NUI: title runtime 5/5302`: 0x5302 = 21250; flags `0000080B`): o **mais novo** dos analisados, depois do [Nike-Kinect-Training](nike-kinect-training.md) e do [Kinect-Sports](kinect-sports.md) (2.0.21173.0).

**Com qual se parece:** com o [Kinect-Rush](kinect-rush.md) e o [Fruit-Ninja-Kinect](fruit-ninja-kinect.md) na parte do Kinect (o mesmo motor de inclinação com o 'XtSs' completo; câmera e status passando antes pelo Kinect Studio; banco de modelos do rastreador próprio como o Rush), mas sem microfones nem voz e sem avatares. A diferença é que o **jogo pede a inclinação por conta própria**: a calibração em Lua (`calibration.lua`) usa a API de ajuste do runtime.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Fruit-Ninja-Kinect](fruit-ninja-kinect.md), [Kinect-Rush](kinect-rush.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-ID](../kinect-id.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [XAM-Fitness](../../xam/xam-fitness.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema`). Não houve teste com o Kinect real nem captura de tela; o jogo carrega todas as telas Flash no início, então o progresso entre telas foi deduzido das chamadas ao Kinect e dos scripts Lua.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e rodar | ✅ | 150 s (4 337 quadros, ≈ 29 por segundo) sem travar; nenhum `Host-side crash`. Vídeos `esrb.wmv`, `xbox_live.wmv` (q150) e `sgi_logo.wmv` (q243). |
| Runtime NUI | ✅ | `NUI: title runtime 5/5302 with flags 0000080B, HUD on`. Busca por ordinal `XamNuiGetTrueColorInfo` (0x8DF, aproximada) e `NetDll_WSAStartupEx` (0x24). Duas ausentes chamadas logo ao abrir, sem efeito visível: `EtxProducerRegister` e `XamXlfsInitializeUploadQueue` ("undefined extern call"). |
| Câmera e rastreador | ✅ | `PsCamDeviceRequest` ≈ 8 750 pedidos; abre o banco `stexemplar:\database.gmsodf` e `HeadPosition.mshdf`. |
| Ajuste do sensor | ✅ (sem motor) | −25° e 0° ao iniciar; **−23° no quadro 550**, quando aparece o primeiro esqueleto (a tela legal chama `CalibrateSensor()` → `TiltKinectSensor(0.12, 1.5, 2.5)` se a cabeça estiver fora de 5°–18°); 11° e 3° nos quadros 1074/1097, quando o jogador volta. Sem loop. |
| Rastreamento | 🟡 | 20 de 54 amostras (37%) com jogador rastreado (esqueletos 480–540 e 900–1920). |
| Tela inicial / identificação | ❔ | Pela sequência legal → logos → tela inicial (`startscreen.lua`), o jogo deve ter chegado ao "Start!"/"Step up to play"; nenhuma chamada nova depois do quadro 550 além da inclinação. `StartSignin` (login pelo Kinect, `XamShowNuiSigninUI`) não foi chamado. |
| Kinect ID | 🟡 | Abre `NuiIdentityPCA`/`NuiIdentityNN`; `XamCacheOpenFile`/`Reset` passam; `XamUserNuiGetUserIndex` para os 8 esqueletos ao abrir. |
| Microfones, voz | — | Nenhuma importação de microfone. `XamShowVoiceMailUI` (ausente) é o correio de voz do Guide. |
| Avatares | — | Nenhuma `XamAvatar*`. |
| Fitness | — | O runtime traz as mensagens de `XamFitness*`/`NuiFitness*` (como o Rush e o Star Wars), mas não importa nem buscou nenhuma função de fitness. |
| Telas Nui do sistema | 🟡 | 16 `XamShowNui*UI` importadas; nenhuma chamada na gravação. **9 ausentes**: CommunitySessions, ControllerRequired, Friends, GamerCardUIForXUID, Achievements, Marketplace, Party, GameInvite, FriendRequest. |
| Rede | ⚪ | 33 `NetDll_Xnp*`/`WSA*`/`XNet*` ausentes (placares online); não chamadas. |

### O que o jogo faz com o Kinect (scripts Lua)
- `legal.lua`: com o primeiro esqueleto rastreado, `SensorNeedsCalibration()` lê a cabeça (`GetHeadPosForTilt`); se ela está a menos de 2,5 m e o ângulo `atan2(y, 1,5 m)` sai de 5°–18°, chama `CalibrateSensor()`.
- `calibration.lua`: `TiltKinectSensor(0.12, 1.5, 2.5)` (nativo em 0x822308E0, que chama a API `NuiCameraAdjustTilt` do runtime em 0x82C04140); a tela "Calibrando" (`Dod_motor_hud`) só fecha quando `IsKinectSensorTilting()` (0x82230AD8 → `XamNuiCameraElevationGetAngle`, estado do motor) fica falso.
- `startscreen.lua`: espera um jogador (`IsTrackingPlayer`), mostra "Start!" e identifica o jogador pelo Kinect ID ("Identifying", com um contorno para o "convidado" momentâneo durante o registro); `identityscreen.lua`/`changeprofile.lua` usam `StartSignin(GetControllingFeedTrackingId(), …)` → `XamShowNuiSigninUI`.
- `pause.lua`: `ShowKinectGuide()`.

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 dj_replay 150 "$HOME/Kixenia/kinect-sdk-re/games/extraidos/doodlejump/default.xex"
```
(ou o arquivo do contêiner STFS no lugar do `default.xex`).

## Comparação com os outros jogos

| | Doodle Jump for Kinect | [Fruit-Ninja-Kinect](fruit-ninja-kinect.md) | [Kinect-Rush](kinect-rush.md) | [Kinect-Adventures](kinect-adventures.md) |
|---|---|---|---|---|
| Runtime NUI | 2.0.21250.0 | 2.0.20500.0 | 2.0.21076.0 | 2.0.11775.7 |
| Importações (xam) | 410 (218) | 273 (106) | 353 (149) | 364 (167) |
| Banco do rastreador | Próprio (`Database.xmplr`, `HeadPosition`) | Do sistema | Próprio | Próprio |
| Microfones | Não | Não | Sim | Não |
| 'XtSs' | +0x20..+0x4C (mesmo motor) | +0x20..+0x4C | +0x20..+0x4C | +0x20..+0x4C |
| Inclinação pedida pelo jogo | **Sim** (Lua → `NuiCameraAdjustTilt`) | Não | Não | Não |
| Login pelo Kinect | `StartSignin` na identificação | Obrigatório na tela de título | — | — |
| Avatares | Não | Não | Não | 12 (stubs) |

## Motor de inclinação do jogo

Igual ao do [Fruit-Ninja-Kinect](fruit-ninja-kinect.md) e do [Kinect-Rush](kinect-rush.md), aqui entre 0x82C03000 e 0x82C06000. 'XtSs' (0x50 bytes) em **0x82E73FD0**; leitor no ponteiro 0x82E2E690 → 0x82C03890; callback em 0x82C04328; 0x82C03810 só verifica se o sensor responde (≠ 0x8007048F).

| Campo | Endereço | Uso |
|---|---|---|
| +0x20..+0x28 gravidade | 0x82E73FF0 | ângulo atual = `atan2(+0x28, +0x24)` (0x82C04F48) |
| +0x2C flags | 0x82E73FFC | lido em 0x82C03C30, 0x82C03CC0, 0x82C03E98, 0x82C04F48; zerado por 0x82C03580 |
| +0x30..+0x3C chão guardado | 0x82E74000 | +0x3C = 0 começa a busca de −25° (0x82C03C30); lido em 9 funções |
| +0x40 | 0x82E74010 | bit 0: pedido recusado (0x82C036D8, 0x82C03780, 0x82C03E98) |
| +0x44..+0x4C gravidade do chão | 0x82E74014 | `atan2(+0x4C, +0x48)` roda o plano do chão (0x82C03460) |

## Teste com o Kinect real: pendente

O que verificar ao vivo (uma vez, na sessão principal):
1. **Calibração do jogo**: com o jogador na frente, a tela "Calibrando" (`Dod_motor_hud`) deve aparecer e **fechar** quando o motor parar (`XamNuiCameraElevationGetAngle` devolvendo estado do motor parado). Se ficar presa, o problema é o estado do motor (+0x2C bits 0–1) no 'XtSs'.
2. Tela inicial: o "Start!" pelo cursor de mão e a identificação do jogador ("Identifying") pelo Kinect ID; ver se o convidado/registro passa e se `XamShowNuiSigninUI` é chamado.
3. Jogo: pular e inclinar o corpo para guiar o boneco (rastreamento do corpo inteiro, pulo detectado).
4. Aviso de sensor movido (`NewSensorMovedDialog`).
5. Telas ausentes se aparecerem: conquistas/cartão/amigos pelo Kinect.

## Pendências
- 9 `XamShowNui*UI` ausentes (as mesmas do Rush e mais Achievements, Marketplace, Party, GameInvite, FriendRequest).
- `EtxProducerRegister` e `XamXlfsInitializeUploadQueue`: ausentes e chamadas ao abrir; o jogo seguiu.
- 33 funções de rede (`NetDll_Xnp*` etc.) ausentes; só importam para placares online.

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/doodlejump.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/djvmx` (**PowerPC:BE:64:VMX128-32addr**, sem análise automática).
- Log do Xenia: `~/Kixenia/kinect-tools/logs/dj_replay.log`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/dj/c_full/<endereço>.c` (132 funções: as que chamam o Kinect, o motor de inclinação inteiro e as ligações do Lua 0x822308E0/0x82230AD8; 1 com `halt_baddata`).
- Scripts: `dj_imports.py`, `dj_funcs.py`, `dj_merge.py`, `dj_runtime.py`, `dj_acessos_base.py`, `gen_doodlejump.py` (`games/decompilado/scripts/`), `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x82183400, 35 896 funções; `.text` 0x821D0000–0x82DE8AB8.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/dj/`.**

**410 importações** (218 do `xam.xex`, 192 do `xboxkrnl.exe`), 2810 chamadas diretas no código, 106 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 269 implementadas, 34 aproximadas, 34 stubs, 72 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82BEF558 | 68 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BEF5A0 | 164 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BEF648 | 84 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BEF778 | 100 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82BEF7E0 | 96 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BEF840 | 96 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BEF8A0 | 88 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BEF8F8 | 60 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BEF938 | 68 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BEFAD0 | 252 | `XMsgInProcessCall` | 16 |  | ✅ |
| 0x82BEFCA8 | 308 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BEFDE0 | 260 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF0080 | 352 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82BF01E0 | 304 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF0310 | 752 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF0600 | 568 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF0838 | 384 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF09B8 | 312 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF0AF0 | 320 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF0C30 | 320 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF0D70 | 608 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF0FD0 | 732 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF12B0 | 648 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF1538 | 656 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF17C8 | 572 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF1A08 | 476 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF1BE8 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF1E30 | 548 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2078 | 152 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82BF2110 | 152 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF21A8 | 432 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF2358 | 188 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2418 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF24C8 | 172 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF2578 | 172 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2628 | 168 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF26D0 | 168 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2778 | 220 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2858 | 156 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF28F8 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF29A8 | 228 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2A90 | 232 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2B78 | 280 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2C90 | 220 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2D70 | 84 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82BF2DC8 | 164 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82BF2EF0 | 192 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF37E8 | 204 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82BF54F0 | 108 | `XMsgInProcessCall` | 0 |  | ✅ |
| 0x82CD2B28 | 120 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82CD2BA0 | 120 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82CD2C20 | 140 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82CD2CB8 | 184 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82CD2E20 | 124 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82CD2EB0 | 216 | `XMsgStartIORequest` | 1 |  | ✅ |

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82BFA078 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82C03340 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: para o motor (`ElevationStopMovement`) nos erros e informa o status ao sistema (`XamNuiCameraTiltReportStatus`). | ✅ |
| 0x82C035F0 | 92 | `XamNuiCameraElevationGetAngle` | 1 | API `NuiCameraElevationGetAngle` do runtime: ângulo e estado do motor (`XamNuiCameraElevationGetAngle`). Usada pelo `IsKinectSensorTilting` do Lua (0x82230AD8). | ✅ |
| 0x82C03650 | 132 | `XamNuiCameraElevationSetAngle` | 3 | API `NuiCameraElevationSetAngle` do runtime: aceita de −27° a +27° (`XamNuiCameraElevationSetAngle`). | ✅ |
| 0x82C03780 | 144 | `XamNuiCameraElevationStopMovement` | 0 | Para o motor se o 'XtSs' +0x40 bit 0 indicar pedido recusado. | ✅ |
| 0x82C03890 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'**: `XamXStudioRequest(0x1005)` e, se o Studio não responder (0x80070057), `XamNuiCameraTiltGetStatus`. Fica no ponteiro 0x82E2E690. | ✅ |
| 0x82C03C30 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se +0x2C bit 0x10 ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | ✅ |
| 0x82C03CC0 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão e o guarda no sistema (`XamNuiCameraRememberFloor`). | ✅ |
| 0x82C03E98 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura, flags): `ElevationGetAngle`; +0x40 bit 0 = ocupado; flag 0x08 recalcula o alvo; sem chão guardado, começa a busca (0x82C03C30). | ✅ |
| 0x82C04328 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam: AutoTilt, StopMovement e AdjustTilt do sistema; responde com `XamNuiCameraTiltReportStatus`. | ✅ |
| 0x82C046E8 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x82C04328)`) e, se o Kinect Studio responder, põe 0x82C03890 no ponteiro do leitor. | ✅ |
| 0x82C04F48 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação** a cada quadro de esqueleto: 'XtSs' em 0x82E73FD0, ângulo `atan2(+0x28, +0x24)`, busca em degraus, correção do chão pela gravidade do chão (+0x44..+0x4C) e mira no jogador (±27°). | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82C08648 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | Para cada esqueleto, pergunta o usuário associado (`XamUserNuiGetUserIndex`) e acompanha a sessão de identidade (`XamNuiIdentityGetSessionId`). | ✅ |
| 0x82C08868 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82C0A7F8 | 356 | `XamUserNuiGetUserIndex` | 0 |  | ✅ |
| 0x82C0BA70 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82C0BBF8 | 3200 | `XamNuiIdentityGetSessionId` | 5 |  | ✅ |
| 0x82C1ACB0 | 996 | `XamNuiIdentityGetSessionId` | 1 | Envia dados do motor de identidade pela fila de upload (`XamXlfsMountUploadQueueInstance`/`…Unmount…`, **ausentes**); não chamada na gravação. | ✅ |
| 0x82C1F568 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário (`XamUserNuiGetEnrollmentIndex`). | ✅ |
| 0x82C1F7B0 | 336 | `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82C1FA30 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: `XamCacheOpenFile` (cache 0x18), `XamCacheReset`, `XamCacheCloseFile`. Igual ao Fruit Ninja e ao Rush. | ✅ |
| 0x82C1FB28 | 488 | `XamReadBiometricData` | 1 |  | ✅ |
| 0x82C1FE60 | 708 | `XamReadBiometricData` | 0 |  | ✅ |
| 0x82C20470 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê, mescla e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | ✅ |

#### Telas do sistema

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82CCD3D0 | 232 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82CCD4F8 | 100 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82CCD560 | 132 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82CCD620 | 68 | `XamShowVoiceMailUI` | 0 | Abre a caixa de mensagens de voz do Guide (`XamShowVoiceMailUI`, **ausente**); não é microfone do Kinect. | ✅ |
| 0x82CCD848 | 56 | `XamShowNuiMessageBoxUI` | 0 | Caixa de mensagem pelo Kinect (`XamShowNuiMessageBoxUI`, aproximada). | ✅ |
| 0x82CCD890 | 28 | `XamShowNuiDirtyDiscErrorUI` | 0 | Tela Nui de disco sujo (`XamShowNuiDirtyDiscErrorUI`, stub). | ⚠️ |

#### Estado do sensor e mensagens do runtime NUI

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82CD7720 | 56 | `XMsgSystemProcessCall` | 0 |  | ✅ |
| 0x82CD77A0 | 40 | `XamNuiGetDeviceStatus` | 0 | Lê `XamNuiGetDeviceStatus` e devolve o campo de estado (sensor presente). | ✅ |
| 0x82CD78B0 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82CD78F0 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82CD7938 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82CD7980 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x82CD79C0 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82CD79F8 | 76 | `XMsgInProcessCall` | 9 |  | ✅ |
| 0x82CD7A48 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82CD7A90 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82CD7AE0 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x82CD7B50 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82CD7BB8 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82CD7C08 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82CD7C78 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x824506B8 | 376 | `XAudioGetVoiceCategoryVolume` | 0 | Volume da categoria de voz (`XAudioGetVoiceCategoryVolume`, stub): som do jogo, não microfone. | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8752 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgInProcessCall` | xam 0x1F4 | 23 | 3312 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 48 | 4 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 5 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 3 | 1 | ✅ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiCommunitySessionsUI` | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendRequestUI` | xam 0x479 | 1 | 0 | ❌ |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGameInviteUI` | xam 0x473 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Rede</b> (95)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASend` | xam 0x17 | 1 | 0 | ❌ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetCreateKey` ⭐ | xam 0x36 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetBroadcastVersionStatus` ⭐ | xam 0x4C | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` ⭐ | xam 0x4A | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetOpt` | xam 0x4E | 1 | 0 | 🟡 |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToServer` ⭐ | xam 0x3D | 1 | 0 | ❌ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosGetListenStats` | xam 0x4D | 1 | 0 | ❌ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetOpt` ⭐ | xam 0x4F | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetTsAddrToInAddr` ⭐ | xam 0x3B | 1 | 0 | ❌ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpCapture` ⭐ | xam 0x6C | 1 | 0 | ❌ |
| `NetDll_XnpConfig` ⭐ | xam 0x68 | 1 | 0 | ❌ |
| `NetDll_XnpConfigUPnP` ⭐ | xam 0x67 | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptRecv` ⭐ | xam 0x6F | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptSetCallbacks` ⭐ | xam 0x6D | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptXmit` ⭐ | xam 0x6E | 1 | 0 | ❌ |
| `NetDll_XnpGetActiveSocketList` ⭐ | xam 0x7A | 1 | 0 | ❌ |
| `NetDll_XnpGetConfigStatus` ⭐ | xam 0x69 | 1 | 0 | ❌ |
| `NetDll_XnpGetVlanXboxName` ⭐ | xam 0x78 | 1 | 0 | ❌ |
| `NetDll_XnpLoadConfigParams` ⭐ | xam 0x65 | 1 | 0 | ❌ |
| `NetDll_XnpLoadMachineAccount` ⭐ | xam 0x6A | 1 | 0 | ❌ |
| `NetDll_XnpLogonClearQEvent` ⭐ | xam 0x74 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQFlags` ⭐ | xam 0x71 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQVals` ⭐ | xam 0x75 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetPState` ⭐ | xam 0x77 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQEvent` ⭐ | xam 0x73 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQFlags` ⭐ | xam 0x72 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQVals` ⭐ | xam 0x76 | 1 | 0 | ❌ |
| `NetDll_XnpNoteSystemTime` ⭐ | xam 0x7B | 1 | 0 | ❌ |
| `NetDll_XnpRegisterKeyForCallerType` ⭐ | xam 0x7C | 1 | 0 | ❌ |
| `NetDll_XnpSaveConfigParams` ⭐ | xam 0x66 | 1 | 0 | ❌ |
| `NetDll_XnpSaveMachineAccount` ⭐ | xam 0x6B | 1 | 0 | ❌ |
| `NetDll_XnpSetVlanXboxName` ⭐ | xam 0x79 | 1 | 0 | ❌ |
| `NetDll_XnpUnregisterKeyForCallerType` ⭐ | xam 0x7D | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` | xam 0xA | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 0 | ✅ |
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
| `XNetLogonGetMachineID` ⭐ | xam 0x135 | 2 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 7 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (63)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 26 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExSemaphoreObjectType` | xboxkrnl 0x17 | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 18 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `ExTimerObjectType` ⭐ | xboxkrnl 0x1C | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 14 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 10 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 2004 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 111 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 8 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 8 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 5 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 4 | 566 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 13146 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 30 | 15515 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3291 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3291 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 25 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 27 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` ⭐ | xboxkrnl 0x9C | 1 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 101 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 252 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 3 | 250 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 7 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 9 | 519 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 2 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 7 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 27 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 38 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 40 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 6079 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 42 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 8 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 4 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 599 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 36 | 202 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 2 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 755 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 564 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 5 | 0 | ✅ |
| `XexUnloadImageAndExitThread` ⭐ | xboxkrnl 0x1A2 | 1 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (47)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 3 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 8 | 730 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 3 | 36 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 3 | 785 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 249 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 4 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 5 | 201 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 261 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 2 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 388 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 4 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 2 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 44 | 21514 | ✅ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByName` | xboxkrnl 0x10D | 3 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 2 | 3 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 20 | 21515 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 5 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceName` | xam 0x25F | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 2 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentGetThumbnail` ⭐ | xam 0x261 | 1 | 0 | ✅ |
| `XamContentInstall` ⭐ | xam 0x26A | 1 | 0 | ❌ |
| `XamContentLaunchImage` | xam 0x263 | 1 | 0 | ❌ |
| `XamContentOpenFile` | xam 0x269 | 8 | 4 | 🟡 |
| `XamContentResolve` ⭐ | xam 0x268 | 2 | 0 | 🟡 |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (37)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 2 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowPlayersUI` | xam 0x2C8 | 1 | 0 | ❌ |
| `XamShowQuickChatUI` ⭐ | xam 0x2C2 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamShowSigninUIp` ⭐ | xam 0x2BE | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 1 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 2 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 2 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 2 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 4 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 8 | 4 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 2 | 0 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (30)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4339 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 1 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 1 | 🟡 |
| `VdQuerySystemCommandBuffer` ⭐ | xboxkrnl 0x1C8 | 1 | 0 | ❌ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4340 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBuffer` ⭐ | xboxkrnl 0x1D8 | 1 | 0 | ❌ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 3 | 7 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 3 | 0 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 22 | 4537 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 1 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 1 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 12 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetLanguage` ⭐ | xam 0x3D2 | 2 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 3 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 5 | 3291 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 0 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 20 | 19185 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 6 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 4 | 2 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 4 | 2 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 3 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 33 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 10 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 10 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 2 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 15 | 496 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 16 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 5 | 1 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 2 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `XamReadString` ⭐ | xam 0x2F6 | 1 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 3 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 6 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 9 | 0 | ✅ |
| `vsprintf` ⭐ | xboxkrnl 0x14E | 2 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (19)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptAesEcb` ⭐ | xboxkrnl 0x15A | 1 | 0 | ✅ |
| `XeCryptAesKey` ⭐ | xboxkrnl 0x159 | 1 | 0 | ✅ |
| `XeCryptBnQwNeRsaKeyGen` ⭐ | xboxkrnl 0x16B | 1 | 0 | ❌ |
| `XeCryptBnQwNeRsaPrvCrypt` ⭐ | xboxkrnl 0x16C | 1 | 0 | ❌ |
| `XeCryptBnQwNeRsaPubCrypt` ⭐ | xboxkrnl 0x16D | 1 | 0 | ✅ |
| `XeCryptBnQw_SwapDwQwLeBe` ⭐ | xboxkrnl 0x170 | 21 | 0 | ✅ |
| `XeCryptRandom` | xboxkrnl 0x18A | 3 | 0 | ⚪ stub |
| `XeCryptRc4Ecb` ⭐ | xboxkrnl 0x18C | 1 | 0 | ✅ |
| `XeCryptRc4Key` ⭐ | xboxkrnl 0x18B | 1 | 0 | ✅ |
| `XeCryptSha256Final` | xboxkrnl 0x312 | 1 | 0 | ✅ |
| `XeCryptSha256Init` | xboxkrnl 0x310 | 1 | 0 | ✅ |
| `XeCryptSha256Update` | xboxkrnl 0x311 | 1 | 0 | ✅ |
| `XeCryptShaFinal` | xboxkrnl 0x191 | 1 | 0 | ✅ |
| `XeCryptShaInit` | xboxkrnl 0x18F | 1 | 0 | ✅ |
| `XeCryptShaUpdate` | xboxkrnl 0x190 | 1 | 0 | ✅ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |
| `XeKeysGetKey` ⭐ | xboxkrnl 0x244 | 1 | 0 | 🟡 |
| `XeKeysGetKeyProperties` ⭐ | xboxkrnl 0x241 | 1 | 0 | ✅ |
| `XeKeysQwNeRsaPrvCrypt` ⭐ | xboxkrnl 0x248 | 1 | 0 | ❌ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiCommunitySessionsUI` | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendRequestUI` ⭐ | xam 0x479 | 1 | 0 | ❌ |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGameInviteUI` ⭐ | xam 0x473 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiMessageBoxUI` ⭐ | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 2368 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 6 | 1807 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 12 | 25 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 5 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 1673 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 1561 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 20 | 0 | ✅ |
| `XamAllocEx` ⭐ | xam 0x1EB | 3 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 23 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8752 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 5 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 4 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 4 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 3 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 8 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 3 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 3 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 6 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 3 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 2 | 4339 | ✅ |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 2 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgInProcessCall` | xam 0x1F4 | 23 | 3312 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 48 | 4 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 2 | 1 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamShowVoiceMailUI` ⭐ | xam 0x2C3 | 1 | 0 | ❌ |

</details>

<details><summary><b>Outras</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamBackgroundDownloadItemGetHistoryStatus` ⭐ | xam 0x9D1 | 1 | 0 | ❌ |
| `XamBackgroundDownloadItemGetStatus` ⭐ | xam 0x9CF | 1 | 0 | ❌ |

</details>

⭐ = só este jogo importa (nenhum de Nike, Kinect Sports, Adventures, Rush, Star Wars, Joy Ride e Fruit Ninja).

**Ausentes no Xenia (72):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetInAddrToServer`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetServerToInAddr`, `NetDll_XNetSetOpt`, `NetDll_XNetTsAddrToInAddr`, `NetDll_XnpCapture`, `NetDll_XnpConfig`, `NetDll_XnpConfigUPnP`, `NetDll_XnpEthernetInterceptRecv`, `NetDll_XnpEthernetInterceptSetCallbacks`, `NetDll_XnpEthernetInterceptXmit`, `NetDll_XnpGetActiveSocketList`, `NetDll_XnpGetConfigStatus`, `NetDll_XnpGetVlanXboxName`, `NetDll_XnpLoadConfigParams`, `NetDll_XnpLoadMachineAccount`, `NetDll_XnpLogonClearQEvent`, `NetDll_XnpLogonGetQFlags`, `NetDll_XnpLogonGetQVals`, `NetDll_XnpLogonGetStatus`, `NetDll_XnpLogonSetPState`, `NetDll_XnpLogonSetQEvent`, `NetDll_XnpLogonSetQFlags`, `NetDll_XnpLogonSetQVals`, `NetDll_XnpNoteSystemTime`, `NetDll_XnpRegisterKeyForCallerType`, `NetDll_XnpSaveConfigParams`, `NetDll_XnpSaveMachineAccount`, `NetDll_XnpSetVlanXboxName`, `NetDll_XnpUnregisterKeyForCallerType`, `RtlCaptureContext`, `RtlUnwind`, `VdQuerySystemCommandBuffer`, `VdSetSystemCommandBuffer`, `XamBackgroundDownloadItemGetHistoryStatus`, `XamBackgroundDownloadItemGetStatus`, `XamContentInstall`, `XamContentLaunchImage`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowNuiAchievementsUI`, `XamShowNuiCommunitySessionsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendRequestUI`, `XamShowNuiFriendsUI`, `XamShowNuiGameInviteUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamShowNuiPartyUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamShowQuickChatUI`, `XamShowVoiceMailUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeCryptBnQwNeRsaKeyGen`, `XeCryptBnQwNeRsaPrvCrypt`, `XeKeysQwNeRsaPrvCrypt`, `XexUnloadImageAndExitThread`, `__C_specific_handler`.
