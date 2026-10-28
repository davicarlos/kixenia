# Fruit Ninja Kinect (título 58410B79)

Fruit Ninja Kinect, da Halfbrick Studios (Microsoft Studios, Xbox Live Arcade, agosto de 2011): o jogador corta frutas com as mãos. Contêiner STFS original `CF275FBB1DBB23BF9578F206752E88EF59F96B9558` (140 MB) em `<HD>/Xbox 360/Fruit Ninja Kinect/Content/0000000000000000/58410B79/000D0000/`; extraído com `xenia-vfs-dump` para `~/Kixenia/kinect-sdk-re/games/extraidos/fruitninja/`. Executável `default.xex` (3,5 MB, PE original `FNK.exe`, `arcadeinfo.xml` versão 1.0.619.0), menus em XML (`Menus\*.fnk`), vídeos WMV de atração, `NuiIdentity.bin.be` (identidade) e o pacote `avatarawards`. **Não traz** `Database.xmplr` nem modelos de voz: o rastreador é o do sistema. Runtime NUI **2.0.20500.0** (no log, `NUI: title runtime 5/5014`: 0x5014 = 20500; flags `00000C09`), entre o do [Kinect-Adventures](kinect-adventures.md) e do [Kinect-Joy-Ride](kinect-joy-ride.md) (2.0.11775) e o do [Kinect-Star-Wars](kinect-star-wars.md) e do [Kinect-Rush](kinect-rush.md) (2.0.21076).

**Com qual se parece:** com o [Kinect-Rush](kinect-rush.md) na parte do Kinect (o mesmo motor de inclinação, com o 'XtSs' completo, e câmera e status passando antes pelo Kinect Studio), mas sem microfones, sem voz, sem avatares e sem fitness: só câmera, esqueleto, inclinação e Kinect ID. É o jogo mais simples dos analisados.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Rush](kinect-rush.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-ID](../kinect-id.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo e o `XamCache*`). Não houve teste com o Kinect real nem captura de tela: o estado visual foi deduzido dos arquivos abertos.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e rodar | ✅ | 150 s (6 222 quadros) sem travar; nenhum `Host-side crash`. |
| Runtime NUI | ✅ | `NUI: title runtime 5/5014 with flags 00000C09, HUD on`. Busca por ordinal só `NetDll_XNetStartupEx` (0x50) e `NetDll_WSAStartupEx` (0x24) do xam: nada do Kinect. |
| Câmera | ✅ | `PsCamDeviceRequest` com cor e profundidade desde o primeiro quadro (≈ 9 000 pedidos); o Kinect Studio (`XamXStudioRequest`) é consultado antes, como no Rush. |
| Ajuste do sensor | ✅ (sem motor) | Pede −25° e 0° ao iniciar (sem chão guardado) e, com o jogador visto, 27° (o limite) e logo 7° (quadros 1712 e 1729). Não fica em loop. |
| Tela de título | ✅ | `menu_title.fnk` no quadro 546 (≈ 13 s), vídeos `classicloop`/`zenloop`/`attract.wmv`. O menu espera um usuário calibrado (`userCalibrated`) e o gesto de cortar "começar". |
| Rastreamento | 🟡 | 23 de 60 amostras (38%) com o jogador rastreado, do quadro de esqueleto 540 ao 2160; depois a gravação fica sem jogador. |
| Login pelo Kinect | ❌ | No quadro 3247 (≈ 78 s) o jogo chama `XamShowNuiSigninUI(0x6A, 0xFF, 0)` (`enrollUser`): o Xenia abre o **diálogo comum de login (ImGui)**, não o login por gesto/rosto do console. Em seguida o motor de identidade pede o registro (`XamUserNuiGetEnrollmentIndex`, XMsg 0x2A004…0x2A008). |
| Menu principal | ❌ | No quadro 3601 abre `popupKinectQuality.fnk` (condição `enrollmentFailed` da tela de título): o registro no Kinect ID falhou e o jogo voltou ao "começar". `menu_main.fnk` nunca foi aberto. |
| Kinect ID | 🟡 | Abre `NuiIdentity.bin.be` e os modelos `NuiIdentityPCA`/`NuiIdentityNN`; `XamCacheOpenFile`/`XamCacheReset` passam (implementação nova do worktree); `XamReadBiometricData` sem dados. |
| Microfones, voz | — | Nenhuma importação de microfone (`XamVoice*`, `XAudioCapture*`, `MECClient`). |
| Avatares, fitness | — | Nenhuma `XamAvatar*` nem `XamFitness*`. O pacote `avatarawards` (camisa, calça, roupa de kung fu) é prêmio de avatar, sem chamada do Kinect. |
| Telas Nui do sistema | 🟡 | 9 `XamShowNui*UI` importadas; só `XamShowNuiSigninUI` foi chamada. `XamShowNuiAchievementsUI`, `…GamerCardUIForXUID` e `…MarketplaceUI` são **ausentes**; `…DirtyDiscErrorUI` é stub. |

### Fluxo da tela de título (`Menus\menu_title.fnk`)
`whileNotCalibrated` (espera `userCalibrated`) → botão `begin` (com `userExists`: `enrollUser`) → `waitForEnrollment` (`enrollmentComplete`) → `checkEnrollment`: se `enrollmentFailed`, mostra `popupKinectQuality.fnk` e volta ao `begin`; senão `waitForSignIn` (`signInComplete`) → `progressButton` → `menu_main.fnk` (`signedIn`, `setPrimaryUser`). Ou seja, **para jogar é preciso que o login pelo Kinect termine com um usuário entrando**: é o ponto que decide se o jogo funciona no Xenia.

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 fn_replay 150 "$HOME/Kixenia/kinect-sdk-re/games/extraidos/fruitninja/default.xex"
```
(ou o arquivo do contêiner STFS no lugar do `default.xex`).

## Comparação com os outros jogos

| | Fruit Ninja Kinect | [Kinect-Rush](kinect-rush.md) | [Kinect-Adventures](kinect-adventures.md) | [Kinect-Joy-Ride](kinect-joy-ride.md) |
|---|---|---|---|---|
| Runtime NUI | 2.0.20500.0 | 2.0.21076.0 | 2.0.11775.7 | 2.0.11775.6 |
| Importações (xam) | 273 (106) | 353 (149) | 364 (167) | 332 (145) |
| Microfones | Não | Sim, no executável | Não | Sim (NUISP) |
| Campos do 'XtSs' | +0x20..+0x4C (mesmo motor do Rush) | +0x20..+0x4C | +0x20..+0x4C | +0x24..+0x4C, sem +0x38/+0x44 |
| Kinect Studio | Câmera (0x1003) e status (0x1005) | Igual | Só a câmera | — |
| Kinect ID | Sim, e **obrigatório** para sair da tela de título | Sim | Sim | Sim |
| Avatares | Não | Não | 12 funções (stubs) | 8 funções |
| `XamShowNuiSigninUI` | Importa e **chama** (login obrigatório na tela de título) | Importa | Importa | Importa |

## Motor de inclinação do jogo

É o motor do runtime NUI do [Kinect-Rush](kinect-rush.md) e do [Kinect-Star-Wars](kinect-star-wars.md), aqui entre 0x821D0000 e 0x821D2E00. 'XtSs' (0x50 bytes) em **0x82815950**; leitor do status no ponteiro 0x827D4094 → 0x821D0D38 (Kinect Studio 0x1005, depois `XamNuiCameraTiltGetStatus`); callback do sistema em 0x821D17B8.

| Campo | Endereço | Uso |
|---|---|---|
| +0x20..+0x28 gravidade | 0x82815970 | normalizada (0x821D0FA0); ângulo atual = `atan2(+0x28, +0x24)` (0x821D2458) |
| +0x2C flags | 0x8281597C | bits 0–1: motor parado/movendo; 0x04 e 0x08: erro; 0x10: força a busca (0x821D10C0, 0x821D1150, 0x821D1328, 0x821D2458) |
| +0x30..+0x3C chão guardado | 0x82815980 | +0x3C (distância, float) = 0 começa a busca de −25° (0x821D10C0); lido em 9 funções |
| +0x40 | 0x82815990 | bit 0: pedido recusado (0x821D0C88, 0x821D1328) |
| +0x44..+0x4C gravidade do chão | 0x82815994 | `atan2(+0x4C, +0x48)` roda o plano do chão (0x821D0A78); normalizada em 0x821D1CA8 |

## Teste com o Kinect real: pendente

O que verificar ao vivo (uma vez, na sessão principal):
1. **Login pelo Kinect**: cortar "começar" na tela de título chama `XamShowNuiSigninUI`. Ver o que o Xenia mostra (diálogo ImGui) e se, escolhendo o perfil, o jogo segue para `menu_main.fnk` ou cai no `popupKinectQuality` (`enrollmentFailed`). Este é o bloqueio provável; a gravação não conseguiu passar dele.
2. Se falhar: registrar no log o que o jogo espera depois do `XamShowNuiSigninUI` (notificação de login `XN_SYS_SIGNINCHANGED`, `XamUserNuiGetUserIndex` do esqueleto, `XamUserNuiGetEnrollmentIndex` ≠ −1) e comparar com o [Kinect-ID](../kinect-id.md).
3. Rastreamento das mãos durante a partida (cortes), com 1 e 2 jogadores.
4. Motor: −25°, depois o ajuste para o jogador; sem loop.
5. Telas ausentes: conquistas pelo Kinect (`XamShowNuiAchievementsUI`) e cartão do jogador.

## Pendências
- `XamShowNuiSigninUI` está marcada como aproximada (kSketchy): só repassa ao login comum. O jogo exige o login por Kinect para começar.
- `XamShowNuiAchievementsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`: ausentes; não foram chamadas.
- `XamXlfs*UploadQueue*` (envio de dados do Kinect ID) e `EtxProducer*`: ausentes; não afetaram a gravação.

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/fruitninja.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/fnvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática).
- Log do Xenia: `~/Kixenia/kinect-tools/logs/fn_replay.log`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/fn/c_full/<endereço>.c` (82 funções: as que chamam o Kinect e todo o motor de inclinação; 1 com `halt_baddata`).
- Scripts: `fn_imports.py`, `fn_funcs.py`, `fn_merge.py`, `fn_runtime.py`, `fn_acessos_base.py`, `gen_fruitninja.py` (`games/decompilado/scripts/`), `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x82140200, 15 565 funções; `.text` 0x82160000–0x827B360C.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/fn/`.**

**273 importações** (106 do `xam.xex`, 167 do `xboxkrnl.exe`), 4109 chamadas diretas no código, 112 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 199 implementadas, 29 aproximadas, 30 stubs, 14 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x821AD868 | 28 | `XamShowNuiDirtyDiscErrorUI` | 0 | Mostra a tela Nui de disco sujo (`XamShowNuiDirtyDiscErrorUI`, stub no Xenia). | ⚠️ |
| 0x821AD890 | 40 | `XMsgCancelIORequest` | 4 |  | ✅ |
| 0x821AD9C0 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x821ADA38 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x821ADAB0 | 140 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x821ADB50 | 216 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82207D30 | 432 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82207EE0 | 176 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82207F90 | 172 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82208040 | 168 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x822080E8 | 156 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82208188 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82208238 | 192 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82208328 | 352 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82208488 | 732 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82208768 | 656 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82208C00 | 244 | `XMsgInProcessCall` | 3 |  | ✅ |
| 0x823D73E0 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x823D7420 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x823D7468 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x823D74B0 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x823D74F0 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x823D7528 | 76 | `XMsgInProcessCall` | 10 |  | ✅ |
| 0x823D7578 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x823D75C0 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x823D7610 | 112 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x823D7680 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x823D76E8 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x823D7738 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x823D77A8 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x823D98E0 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` | 0 |  | ✅ |
| 0x824034D8 | 152 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82403570 | 152 | `XMsgInProcessCall` | 1 |  | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x821B65D8 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 | Para cada esqueleto, pergunta ao sistema qual usuário está associado (`XamUserNuiGetUserIndex`) e acompanha a sessão de identidade (`XamNuiIdentityGetSessionId`). | ✅ |
| 0x821B67C8 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x821B8738 | 356 | `XamUserNuiGetUserIndex` | 0 |  | ✅ |
| 0x821B9400 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x821B9588 | 3200 | `XamNuiIdentityGetSessionId` | 5 | Máquina de estados do motor de identidade (Kinect ID) do runtime NUI: confere o app ativo do painel (`XamGetActiveDashAppInfo`) e o identificador da sessão de identidade (`XamNuiIdentityGetSessionId`). | ✅ |
| 0x821C92E0 | 996 | `XamNuiIdentityGetSessionId` | 1 | Envia dados do motor de identidade pela fila de upload do sistema (`XamXlfsMountUploadQueueInstance`/`…Unmount…`, **ausentes no Xenia**); não foi chamada na gravação. | ✅ |
| 0x821CE550 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário: `XamUserNuiGetEnrollmentIndex`; sem registro, pede o perfil pelo XMsg (0x2A008). | ✅ |
| 0x821CE798 | 336 | `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x821CEA18 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: abre até 256 arquivos com `XamCacheOpenFile` (cache 0x18), `XamCacheReset` e fecha (`XamCacheCloseFile`). Igual ao Rush. | ✅ |
| 0x821CEB10 | 488 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`, 0x32000 bytes). | ✅ |
| 0x821CF128 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê, mescla e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | ✅ |

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x821BB8D8 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x821D0958 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: para o motor (`ElevationStopMovement`) nos erros e informa o status ao sistema (`XamNuiCameraTiltReportStatus`). | ✅ |
| 0x821D0C00 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para o ângulo-alvo (`ElevationSetAngle`). | ✅ |
| 0x821D0C88 | 144 | `XamNuiCameraElevationStopMovement` | 1 | Para o motor se o 'XtSs' +0x40 bit 0 indicar pedido recusado. | ✅ |
| 0x821D0D38 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'**: tenta `XamXStudioRequest(0x1005)` e, se o Studio não responder (0x80070057), chama `XamNuiCameraTiltGetStatus`. Fica no ponteiro 0x827D4094. | ✅ |
| 0x821D10C0 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | ✅ |
| 0x821D1150 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão e o guarda no sistema (`XamNuiCameraRememberFloor`); informa o status (`TiltReportStatus`). | ✅ |
| 0x821D1328 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura, flags): lê o ângulo (`ElevationGetAngle`); +0x40 bit 0 = ocupado (devolve 5); flag 0x20 só confere; flag 0x08 recalcula o alvo e reinclina; sem chão guardado, começa a busca (0x821D10C0). | ✅ |
| 0x821D17B8 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam: AutoTilt, StopMovement e AdjustTilt do sistema; responde com `XamNuiCameraTiltReportStatus`. | ✅ |
| 0x821D1B78 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x821D17B8)`) e, se o Kinect Studio responder ao pedido 1, põe 0x821D0D38 no ponteiro do leitor de status. | ✅ |
| 0x821D2458 | 2408 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: monta o 'XtSs' (0x82815950), ângulo atual `atan2(+0x28, +0x24)`; busca em degraus, corrige o chão pela gravidade do chão (+0x44..+0x4C, 0x82815994) e mira o jogador (alvo limitado a ±27°); erro do motor (+0x2C bit 0x08) encerra. | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x824048E0 | 224 | `XAudioGetVoiceCategoryVolume` | 0 | Volume da categoria de voz (`XAudioGetVoiceCategoryVolume`, stub): ajuste do som do jogo, não é microfone. | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 9268 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 11 | 3706 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 19 | 10 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 4 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 5 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 1 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 2 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 1 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (57)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 3 | 27 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExSemaphoreObjectType` ⭐ | xboxkrnl 0x17 | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 12 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 28 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 57 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 56 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 3 | 571 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 32 | 12466 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 14 | 37378 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3659 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3659 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 25 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 120 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 64 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 2 | 5 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 2 | 4 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 6 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 8 | 24 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 19 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 34 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 36 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 23 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 12 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1351 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 334 | 719 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 1 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1476 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 4 | 559 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 4 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (36)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 1 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 22 | 1665 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 9 | 965 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 3 | 1619 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 3 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 5 | 239 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 210 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 1 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 1021 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 34 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 8 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 4 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 1 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 31 | 9855 | ✅ |
| `ObOpenObjectByName` ⭐ | xboxkrnl 0x10D | 1 | 3 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 1 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 11 | 9857 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 3 | 2 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 4 | 4 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 6224 | ⚪ stub |
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
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 3 | 3 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 6225 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 1 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 2 | 7 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Rede</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XNetLogonGetTitleID` | xam 0x136 | 2 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Sistema e console</b> (25)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 17 | 4585 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 4 | 1 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 3 | 0 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 5 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 8 | 2 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetOverlappedResult` | xam 0x1FB | 1 | 0 | 🟡 |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 4 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 3 | 3 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 17 | 22612 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 5 | 4 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 1 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 10 | 3 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 6 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 1 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 1 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 114 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 18 | 977 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 16 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 2 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (19)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 7 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 2 | ✅ |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 2 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 2 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 99648 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 1 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 6 | 1 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 2 | 2 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Memória</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 2 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 494 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 126 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 71 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 2 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 54 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 11 | 411 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 9 | 302 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 8 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 6 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 9268 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 4 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 2 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 1 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 1 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 8 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 3 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 1 | 1 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 5 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 2 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 1 | 0 | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | 🟡 |
| `XMsgInProcessCall` | xam 0x1F4 | 11 | 3706 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 19 | 10 | ✅ |
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

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 1 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 2 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 1 | 1 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |

</details>

⭐ = só este jogo importa (nenhum de Nike, Kinect Sports, Adventures, Rush, Star Wars e Joy Ride).

**Ausentes no Xenia (14):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `NetDll_XNetServerToInAddr`, `RtlCaptureContext`, `RtlUnwind`, `XamShowNuiAchievementsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.
