# Kinect Sports: Season Two (título 4D5309D6)

Kinect Sports: Season Two, da BigPark e Rare (Microsoft Studios, 2011): futebol americano, beisebol, golfe, tênis, dardos e esqui. O log do Xenia mostra o nome do título como **"KS: Season Two"** e as conquistas no executável citam "Kinect Sports: Season Two"; não confundir com o [Kinect-Sports](kinect-sports.md) (4D5308C9, a primeira temporada). Contêiner GOD em `<HD>/Xbox 360/Kinect Sports/Content/0000000000000000/4D5309D6/00007000/9D08D29C349AB0DFDD6B4CE3CCB939084D`, extraído (5,6 GB) com `xenia-vfs-dump` para `~/Kixenia/kinect-sdk-re/games/extraidos/ks2/`: `default.xex` (21 MB, compilado no ramo `Sports2 (Dev_41_RC1_060911)`), os esportes em módulos à parte (`Modules\BaseballDLLR.xex`, `DartsDLLR`, `GolfDLLR`, `GridironDLLR`, `TennisDLLR`, `WinterSportsDLLR`), os pacotes `AM\…`, `Database.xmplr` e `HeadPosition.xmplr` (rastreador), `NuiIdentity.bin.be` (identidade), `AvatarAssetPack`/`AvatarAwards` e os modelos de voz `nuisp1031` … `nuisp3084`.

Runtime NUI **2.0.20764.0** (`NUI: title runtime 5/511C`), entre o do [Kinect-Adventures](kinect-adventures.md) (2.0.11775.7) e o do [Kinect-Rush](kinect-rush.md) e do [Kinect-Star-Wars](kinect-star-wars.md) (2.0.21076.0); o mesmo número do Rapala e do Motion Explosion. Apesar do número, o código do Kinect é praticamente o do **Rush**: das 48 funções do Kinect fora das mensagens, as do motor de inclinação, da identidade, da câmera e dos microfones são iguais às do Rush (mesmas chamadas e tamanho; coluna *No Rush*). A parte de avatares é a do [Kinect-Joy-Ride](kinect-joy-ride.md) e do [Kinect-Sports](kinect-sports.md), que o Rush não tem.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Sports](kinect-sports.md), [Kinect-Rush](kinect-rush.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Adventures](kinect-adventures.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema` com o 'XtSs' completo e o XamCache). Não houve teste com o Kinect real nem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e rodar | ✅ com a configuração do jogo | **Sem ela, trava no quadro ~100** (`CRASH DUMP`, não `Host-side crash`: a análise de 2026-10-26 não viu). O `XamAvatarInitialize` do Xenia falha a menos que `allow_avatar_initialization` esteja ligada; o jogo então chama `XamAvatarShutdown`, mas depois percorre o esqueleto do avatar mesmo assim: 0x82676F20 liga as articulações do Kinect aos ossos (tabela fixa 0x820283B0 → 0x82028290), 0x83124878/0x831247D0 percorrem a sub-árvore (nós de 0x34 bytes, filho em +0x2A, irmão em +0x2C) a partir do vetor que 0x8244EF08 tira do objeto de avatar vazio, e a recursão estoura a pilha de 256 KB da thread principal (PC 0x8244EF18). **Correção:** `config/4D5309D6.config.toml` com `[Kernel] allow_avatar_initialization = true` (em `kinect-tools/storage` e `storage-replay`); assim o jogo passa do carregamento e fica na tela de título (vídeo `AM\Movies\XMV\36F50F0B`, quadro 5 400+). A compressão XCTD dos pacotes `AM` não era a causa: o jogo a detecta pela assinatura e descomprime com `LDIDecompress`. |
| Módulo `GestureLibraryDLLR.xex` | ⚪ | O jogo tenta `XexLoadImage(GAME:\Modules\GestureLibraryDLLR.xex)`, que **não existe no disco** (o `DllLoadManifest.txt` lista `GestureLibraryDll`); a thread que carrega desiste e o jogo segue. Deve ser igual no console. |
| Runtime NUI | ✅ | `NUI: title runtime 5/511C with flags 00004C1B`. É o único jogo visto até agora com o bit **0x10** nas flags (Rush e Rapala: 0x440B; Star Wars 0x4C0B). `XamFitnessInitialize`, `XamNuiGetTrueColorInfo` e `XamVoiceSetMicArrayIdleUsers` são buscadas por ordinal (fora da tabela de importações) e chamadas. |
| Ajuste do sensor | ✅ (sem motor) | Pede −25° no quadro 0 (sem chão guardado) e 0° no quadro 57; o chão fica guardado a partir do quadro 461 (876 mm) e a busca não recomeça. |
| Rastreamento | ❌ na gravação | **Nenhuma amostra com o jogador TRACKED**: 31 de 69 amostras (45%) com o jogador só em POSITION_ONLY (estado 1), de 1,8 a 2,3 m do sensor. Com a mesma gravação o Rush e o Joy Ride chegam a TRACKED em 33% das amostras (16 de 48 e 16 de 50) e o Star Wars também (18 de 54). O pedido 6 do Kinect Studio, que o motor de identidade faz para cada jogador rastreado, também nunca acontece (29 no Rush). Ver *Por que o jogador não passa de POSITION_ONLY* abaixo. |
| Kinect ID | ✅ | Abre `NuiIdentity.bin.be`; `XamCacheOpenFile` e `XamCacheReset` (a correção de 2026-10-26) e `XamReadBiometricData` rodam sem erro. |
| Voz | ❔ | Reconhecedor NUISPEECH dentro do executável (`GAME:\nuisp%d`), calibração `MicArrayCalibrate:\MicArrayCalibrate.data` e `…LowLatency.data`. Na gravação (sem áudio) nenhuma função de microfone foi chamada; só `XamVoiceSetMicArrayIdleUsers` com 0, 1, 3 e 7. |
| Contador de calorias | 🟡 | `XamFitnessInitialize` é chamada ao iniciar; o jogo tem o contador de calorias (`CalorieCounter`, `calorie_counter_enabled`). As outras `XamFitness*` não foram chamadas em 150 s. |
| Avatares | ⚪ | 11 funções `XamAvatar*` (todas stubs no Xenia); `XamAvatarInitialize` falha no Xenia por padrão e o jogo chama logo `XamAvatarShutdown` (é isso que leva ao crash, ver *Abrir e rodar*); com a configuração do jogo, ele segue e pede `XamAvatarGetManifestLocalUser`. Também lê os avatares de perfis desconectados (`XamAvatarGetMetadataSignedOutProfile`, que nem o Adventures nem o Joy Ride importam). |
| Telas Nui do sistema | 🟡 | 5 das 10 `XamShowNui*UI` são ausentes (`AchievementsUI`, `ControllerRequiredUI`, `FriendsUI`, `GamerCardUIForXUID`, `MarketplaceUI`); nenhuma foi chamada. |
| Outras ausentes chamadas | ⚪ | `IoDismountVolumeByFileHandle` (duas vezes, ao montar o cache), `EtxProducerRegister`, `XamXlfsInitializeUploadQueue`: não afetam o Kinect. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 ks2_replay 150 "$HOME/Kixenia/kinect-sdk-re/games/extraidos/ks2/default.xex"
```
O cabeçalho do contêiner GOD (`…/4D5309D6/00007000/9D08D29C349AB0DFDD6B4CE3CCB939084D`) também serve de alvo.

## Por que o jogador não passa de POSITION_ONLY

Quem escolhe os esqueletos que ganham rastreamento completo é o runtime NUI do próprio jogo (o rastreador roda na GPU do jogo); o Xenia só entrega as imagens da câmera. Na gravação, o runtime vê o jogador (estado 1, POSITION_ONLY), mas nunca o promove a TRACKED (estado 2), enquanto o Rush, com o mesmo código de runtime e a mesma gravação, promove. As diferenças conhecidas:

1. **Flags da inicialização**: `NuiInitialize` (0x82A66720) recebe 0x4C1B; o Rush passa 0x440B. Os bits 0x10 e 0x800 a mais não são testados nessa função (ela rejeita 0x33FF8104; usa 0x1/0x8 = profundidade/esqueleto, 0x200, 0x400, 0x1000, 0x2000, 0x4000 e o bit 31 = sem HUD), então são guardados para outro ponto do runtime. 0x800 também aparece no Star Wars, que rastreia; o **0x10 só existe aqui** e é o principal suspeito de um modo em que o jogo escolhe os esqueletos.
2. **O jogo escolhe quem joga**: o Season Two tem tela de calibração do jogador (`FrontendState_CalibrationScreen`, `SceneControllerPlayerCalibration`) e sua própria camada de esqueleto (`NuiSkeletonSports@nui@olympus`). Se o jogo só pede rastreamento completo para quem entra na área de jogo, um jogador fora do centro (a gravação o vê de −1,1 a −0,2 m para o lado) fica em POSITION_ONLY até se posicionar.

Não deu para separar as duas hipóteses sem o Kinect real; o teste ao vivo abaixo resolve.

## Teste com o Kinect real: pendente

- **Rastreamento**: em pé no centro, a 2–2,5 m, conferir com `--kinect_log_hands=true` (e `kinect-tools/resumo_maos.py`) se o jogador chega a TRACKED; se ficar sempre em POSITION_ONLY, investigar o bit 0x10 das flags (quem lê a cópia das flags no runtime) e a escolha de esqueletos.
- **Menus**: o cursor de mão e a tela de calibração do jogador aparecem? O jogo sai da tela de título com o gesto?
- **Motor de inclinação**: −25° → busca → chão guardado, sem repetir (como no Rush).
- **Voz**: comandos de voz nos menus ("Xbox, …") com os microfones do Kinect: `XamVoiceGetMicArrayAudioEx` e `XAudioRegisterRenderDriverMECClient` aparecem no log?
- **Um esporte inteiro** (ex.: dardos e tênis): o módulo do esporte (`Modules\<esporte>DLLR.xex`) carrega? Os módulos têm importações próprias, ainda não listadas.
- **Calorias**: o contador chama `XamFitnessGetCurrentBodyProfileRecord`/`XamFitnessAddFitnessEvent`?
- **Kinect ID**: o jogo reconhece o rosto de um perfil cadastrado?

## Pendências
- Ler no runtime onde a cópia das flags (bit 0x10) é testada; comparar com o Rush.
- Importações dos 6 módulos de esporte (`Modules\*.xex`).
- As 5 `XamShowNui*UI` ausentes (só pelos botões do Guia Kinect).
- Avatares (stubs) e `XamNuiGetTrueColorInfo` (kSketchy).

## Arquivos da análise
- Extração: `~/Kixenia/kinect-sdk-re/games/extraidos/ks2/` (log da extração em `extraidos/ks2.dump.log`).
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/ks2_4D5309D6.img` (Ghidra XEXLoaderWV, `DumpImage.java`); cópia do XEX em `games/ks2_4D5309D6.xex`.
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/ks2vmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática) e a cópia `ks2par1` da decompilação paralela.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/ks2/c_full/<endereço>.c` (190 funções: as que chamam o Kinect e quem as chama, nenhuma com `halt_baddata`); `ks2/c/extra/` tem o `NuiInitialize` (0x82A66720) e o envio da 0x2B003.
- Log da execução: `~/Kixenia/kinect-tools/logs/ks2_replay.log`.
- Scripts: `ks2_imports.py`, `ks2_funcs.py`, `ks2_merge.py`, `ks2_runtime.py`, `ks2_compare.py`, `gen_ks2.py` (`games/decompilado/scripts/`), `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata`: 0x82340E00, 52 302 funções; `.text` 0x823B0000–0x833E5190.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/ks2/`.**

**403 importações** (170 do `xam.xex`, 233 do `xboxkrnl.exe`), 6333 chamadas diretas no código, 128 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 280 implementadas, 30 aproximadas, 55 stubs, 37 ausentes, 1 sem marca.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 em 2 processos (✅ = decompilada inteira). *No Rush*: `=` mesma função do [Kinect-Rush](kinect-rush.md) (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho, `—` não existe lá (`ks2_compare.py`).

### Funções do jogo que falam com o Kinect

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82A5FD80 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | = 0x82950A60 | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82A5CE40 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: traduz o código de saída, para o motor (`ElevationStopMovement`) nos erros 4 e 0x19 e informa o status ao sistema (0x2B00C). | = 0x82963668 | ✅ |
| 0x82A5D0F0 | 92 | `XamNuiCameraElevationGetAngle` | 2 | `NuiCameraElevationGetAngle` público do runtime: confere os ponteiros e lê o ângulo (`XamNuiCameraElevationGetAngle`); chamado pelo jogo em 0x825A8D08 e 0x830E3810. O [Kinect-Sports](kinect-sports.md) tem a mesma (0x829FF2B8). | ≈ 0x82963FB0 (676) | ✅ |
| 0x82A5D150 | 132 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`). | = 0x82963918 | ✅ |
| 0x82A5D1F8 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'** (0x83507AF0): tenta `XamXStudioRequest(0x1005)` e, se o Kinect Studio não responder, `XamNuiCameraTiltGetStatus`. | = 0x829639C0 | ✅ |
| 0x82A5D580 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | = 0x82963D48 | ✅ |
| 0x82A5D610 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão do quadro de esqueleto contra a gravidade (0x82963CD8) e o guarda no sistema (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis; com o motor parado e erro, encerra o pedido (0x2B006/0x2B00C). | = 0x82963DD8 | ✅ |
| 0x82A5D7E8 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura, flags). +0x40 bit 0 do 'XtSs' = ocupado (devolve 5). Flag 0x20: só confere. Flag 0x08: recalcula o alvo (0x82963A10) e inclina se `\|alvo − ângulo\| > 2,0°` (double em 0x82001928): o "sensor foi movido". Sem flag: começa a busca (0x82963D48). | = 0x82963FB0 | ✅ |
| 0x82A5DC78 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (0x2B005…0x2B00B): AutoTilt, StopMovement, ReverseAutoTilt e AdjustTilt do sistema; responde com `XamNuiCameraTiltReportStatus`. | = 0x82964258 | ✅ |
| 0x82A5E038 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x82964258)`) e põe 0x829639C0 no ponteiro do leitor de status. | = 0x82964618 | ✅ |
| 0x82A5E880 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: lê o 'XtSs', calcula o ângulo atual `atan2(+0x28, +0x24)`; na busca sobe degraus `23 − 10·n` (−17°, −7°, 3°) a cada 60 quadros sem chão; com o chão, corrige-o pela gravidade do chão (+0x44..+0x4C) e mira o jogador; erro do motor (+0x2C bit 0x08) encerra com 0x1F. | = 0x82964EF8 | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82A6E988 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | ≈ 0x8295B5D8 (540) | ✅ |
| 0x82A6EB78 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8295B7F8 | ✅ |
| 0x82A70AF0 | 356 | `XamUserNuiGetUserIndex` | 0 | Para cada um dos 8 esqueletos, pergunta ao sistema qual usuário está associado (`XamUserNuiGetUserIndex`) e acorda o motor de identidade quando muda. | = 0x8295D788 | ✅ |
| 0x82A718D8 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8295E570 | ✅ |
| 0x82A71A60 | 3200 | `XamNuiIdentityGetSessionId` | 5 | Sessão do Kinect ID: acompanha os jogadores e o identificador da sessão de identidade (`XamNuiIdentityGetSessionId`). | = 0x8295E6F8 | ✅ |
| 0x82A7B1A8 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8296CEA0 | ✅ |
| 0x82A7F550 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário: `XamUserNuiGetEnrollmentIndex`; sem registro, lê o perfil pelo XMsg (0x32000 bytes). | = 0x82971270 | ✅ |
| 0x82A7F798 | 336 | `XamUserNuiGetUserIndex` | 1 |  | = 0x829714B8 | ✅ |
| 0x82A7FA18 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: `XamCacheOpenFile` (cache 0x18), `XamCacheReset` e fecha. Com o Xenia de agora: um `XamCacheOpenFile` e um `XamCacheReset` na gravação, sem erro. | = 0x82971738 | ✅ |
| 0x82A7FB10 | 488 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`). | = 0x82971830 | ✅ |
| 0x82A80128 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê (`XamReadBiometricData`), mescla (0x82971BE0) e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | = 0x82971E48 | ✅ |

#### Microfones e reconhecimento de voz

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82AAD9A0 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | Se o conjunto de microfones existe (`XamVoiceGetMicArrayStatus` ≠ 2), lê o número de série do sensor. | = 0x82ED3608 | ✅ |
| 0x82AADA68 | 124 | `XamNuiGetDeviceStatus` | 1 | A cada 60 s, lê `XamNuiGetDeviceStatus` e guarda um bit (sensor presente/pronto). | = 0x82ED36D0 | ✅ |
| 0x831FD7A8 | 224 | `XamNuiGetDeviceSerialNumber` | 1 | Inicia o registro de eventos do runtime: número de série do sensor (`XamNuiGetDeviceSerialNumber`), procura a export 0x8E1 do xam e `EtxProducerRegister` (ausente). | = 0x82EBA480 | ✅ |
| 0x831FD990 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | **Laço de áudio dos microfones** (o mesmo do Rush): lê os quadros de `XamVoiceGetMicArrayAudioEx`, confere `…UnderrunStatus` e manda o áudio de referência ao cancelador de eco (`XAudioCaptureRenderDriverFrame`). | = 0x82EBA668 | ✅ |
| 0x831FE838 | 1248 | `XAudioUnregisterRenderDriverMECClient` | 3 |  | ≈ 0x82EBB528 (1252) | ✅ |
| 0x831FED90 | 1512 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | **Abre o áudio dos microfones** para o reconhecedor de voz (NUISPEECH, no próprio executável): aloca os buffers, abre dois fluxos `XamVoiceGetMicArrayAudioEx` e registra o cliente de cancelamento de eco (`XAudioRegisterRenderDriverMECClient`). | ≈ 0x82EBBA88 (1504) | ✅ |

#### Avatares

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82C6D6C0 | 248 | `XamAvatarGetAssets` | 1 |  | — | ✅ |
| 0x82C6D860 | 124 | `XamAvatarShutdown` | 1 |  | — | ✅ |
| 0x82C6D8E0 | 372 | `XamAvatarInitialize` | 1 | Inicia a API de avatares (`XamAvatarInitialize`); na gravação é chamada e logo encerrada (`XamAvatarShutdown`). | — | ✅ |
| 0x82C6DA58 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | — | ✅ |
| 0x82C6DAF0 | 168 | `XamAvatarGetMetadataRandom` | 4 |  | — | ✅ |
| 0x82C6DB98 | 80 | `XamAvatarGetMetadataSignedOutProfileCount` | 1 | Quantos avatares de perfis desconectados existem (`XamAvatarGetMetadataSignedOutProfileCount`): avatares para quem joga sem perfil. | — | ✅ |
| 0x82C6DBE8 | 144 | `XamAvatarGetMetadataSignedOutProfile` | 1 | Lê o avatar de um perfil desconectado (`XamAvatarGetMetadataSignedOutProfile`). | — | ✅ |
| 0x82C6DD60 | 448 | `XamAvatarManifestGetBodyType` | 2 |  | — | ✅ |
| 0x82C6F760 | 532 | `XamAvatarManifestGetBodyType` | 1 |  | — | ✅ |
| 0x82C77A38 | 220 | `XamAvatarLoadAnimation` | 1 |  | — | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82C045E0 | 120 | `XMsgStartIORequest` | 1 |  | = 0x82947420 | ✅ |
| 0x82C04658 | 120 | `XMsgStartIORequest` | 1 |  | = 0x82947420 | ✅ |
| 0x82C046D0 | 140 | `XMsgStartIORequest` | 1 |  | = 0x82947510 | ✅ |
| 0x82C04768 | 184 | `XMsgStartIORequest` | 1 |  | = 0x829475A8 | ✅ |
| 0x82C048D0 | 216 | `XMsgStartIORequest` | 2 |  | = 0x82947708 | ✅ |
| 0x82C12AB8 | 56 | `XMsgSystemProcessCall` | 2 |  | ≈ 0x82D36BE8 (60) | ✅ |
| 0x82C12B38 | 40 | `XamNuiGetDeviceStatus` | 2 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só o campo de estado. | = 0x829473F0 | ✅ |
| 0x82C13B98 | 60 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36BE8 | ✅ |
| 0x82C13BD8 | 72 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36C28 | ✅ |
| 0x82C13C20 | 72 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36C28 | ✅ |
| 0x82C13C68 | 64 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36CB8 | ✅ |
| 0x82C13CA8 | 56 | `XMsgInProcessCall` | 2 |  | = 0x82D36CF8 | ✅ |
| 0x82C13CE0 | 76 | `XMsgInProcessCall` | 9 |  | = 0x82D36D30 | ✅ |
| 0x82C13D30 | 68 | `XMsgInProcessCall` | 1 |  | = 0x82D36D80 | ✅ |
| 0x82C13D78 | 80 | `XMsgStartIORequest` | 1 |  | = 0x82D36DC8 | ✅ |
| 0x82C13DC8 | 112 | `XMsgStartIORequest` | 3 |  | = 0x82D36E18 | ✅ |
| 0x82C13E38 | 100 | `XMsgStartIORequest` | 2 |  | = 0x82D36E88 | ✅ |
| 0x82C13EA0 | 80 | `XMsgStartIORequest` | 2 |  | = 0x82D36DC8 | ✅ |
| 0x82C13EF0 | 112 | `XMsgStartIORequest` | 1 |  | = 0x82D36E18 | ✅ |
| 0x82C13F60 | 100 | `XMsgStartIORequest` | 1 |  | = 0x82D36E88 | ✅ |
| 0x82D9E830 | 168 | `XMsgStartIORequestEx` | 2 |  | = 0x82E82F80 | ✅ |
| 0x82D9E8D8 | 104 | `XMsgInProcessCall` | 2 |  | = 0x82E83028 | ✅ |
| 0x82D9F038 | 352 | `XMsgInProcessCall` | 1 |  | = 0x82E72328 | ✅ |
| 0x82D9F198 | 304 | `XMsgInProcessCall` | 1 |  | = 0x82E72488 | ✅ |
| 0x82D9F2C8 | 752 | `XMsgStartIORequest` | 1 |  | = 0x82E725B8 | ✅ |
| 0x82D9F5B8 | 732 | `XMsgStartIORequest` | 1 |  | ≈ 0x82947420 (120) | ✅ |
| 0x82D9F898 | 648 | `XMsgStartIORequest` | 1 |  | ≈ 0x82947420 (120) | ✅ |
| 0x82D9FB20 | 656 | `XMsgStartIORequest` | 0 |  | ≈ 0x82947420 (120) | ✅ |
| 0x82D9FE20 | 152 | `XMsgInProcessCall` | 2 |  | ≈ 0x82D36CF8 (56) | ✅ |
| 0x82D9FEB8 | 152 | `XMsgInProcessCall` | 1 |  | ≈ 0x82D36CF8 (56) | ✅ |
| 0x82D9FF50 | 72 | `XMsgInProcessCall` | 1 |  | = 0x82E72910 | ✅ |
| 0x82D9FF98 | 432 | `XMsgStartIORequest` | 2 |  | = 0x82E715C0 | ✅ |
| 0x82DA0148 | 188 | `XMsgStartIORequest` | 1 |  | ≈ 0x82947420 (120) | ✅ |
| 0x82DA0208 | 176 | `XMsgStartIORequest` | 1 |  | = 0x82E71770 | ✅ |
| 0x82DA02B8 | 172 | `XMsgStartIORequest` | 1 |  | = 0x82E71820 | ✅ |
| 0x82DA0368 | 172 | `XMsgStartIORequest` | 1 |  | = 0x82E71820 | ✅ |
| 0x82DA0418 | 168 | `XMsgStartIORequest` | 1 |  | = 0x82E71980 | ✅ |
| 0x82DA04C0 | 168 | `XMsgStartIORequest` | 1 |  | = 0x82E71980 | ✅ |
| 0x82DA0568 | 220 | `XMsgStartIORequest` | 1 |  | = 0x82E71C60 | ✅ |
| 0x82DA0648 | 156 | `XMsgStartIORequest` | 1 |  | = 0x82E71A28 | ✅ |
| 0x82DA06E8 | 176 | `XMsgStartIORequest` | 1 |  | = 0x82E71770 | ✅ |
| 0x82DA0798 | 228 | `XMsgStartIORequest` | 1 |  | ≈ 0x82947420 (120) | ✅ |
| 0x82DA0880 | 232 | `XMsgStartIORequest` | 1 |  | = 0x82E71B78 | ✅ |
| 0x82DA0968 | 220 | `XMsgStartIORequest` | 1 |  | = 0x82E71C60 | ✅ |
| 0x82DA0A48 | 192 | `XMsgStartIORequest` | 1 |  | = 0x82E71DE8 | ✅ |
| 0x82DA0B08 | 248 | `XMsgStartIORequest` | 2 |  | ≈ 0x82947420 (120) | ✅ |
| 0x82DA0DF8 | 252 | `XMsgInProcessCall` | 6 |  | = 0x82E72120 | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82CAB3E0 | 200 | `XamVoiceIsActiveProcess` | 3 |  | = 0x82E75F90 | ✅ |
| 0x82CAC678 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 |  | = 0x82E77228 | ✅ |
| 0x82CACCB8 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | = 0x82E77868 | ✅ |
| 0x82CAD048 | 600 | `XamVoiceSubmitPacket` | 1 |  | = 0x82E77BF8 | ✅ |
| 0x82CAD3A8 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | = 0x82E77F58 | ✅ |
| 0x82CAD498 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 |  | = 0x82E78048 | ✅ |
| 0x82CB0320 | 688 | `XamVoiceIsActiveProcess` | 3 |  | = 0x82E7AED0 | ✅ |
| 0x82CB2748 | 208 | `XamVoiceSubmitPacket` | 2 |  | = 0x82E7D2F8 | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x82C0E0C8 | 40 | `XMsgCancelIORequest` | 8 |  | = 0x82D3AA80 | ✅ |
| 0x82C2B120 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | = 0x82D474C0 | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8929 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 13 | 4188 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 30 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 2 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfileCount` | xam 0x5EE | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 3 | 0 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |
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
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 59402 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (67)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 5 | 33 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 7 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 3 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 1 | 0 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 2 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 60 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 0 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 0 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 62 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 7 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 12502 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 22 | 8568 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 4165 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 4165 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 29 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 139 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 89 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 9 | 7 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 9 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 1395 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 31 | 411 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 6 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 32 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 43 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 45 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 2 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 205 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 32 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 3 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 20 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1487 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 203 | 496 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 10 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1698 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 1 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 17 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (61)</summary>

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
| `NtClose` | xboxkrnl 0xCF | 49 | 1788 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 10 | 1576 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 7 | 146 | ✅ |
| `NtCreateIoCompletion` | xboxkrnl 0xD3 | 1 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 125 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 38 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 1 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 3 | 4 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 1 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 11 | 5 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 108 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 5 | 180 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 8 | 1047 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 4 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtRemoveIoCompletion` | xboxkrnl 0xF4 | 1 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 15 | 0 | ✅ |
| `NtSetIoCompletion` | xboxkrnl 0xF8 | 1 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 1 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 13 | 23 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 39 | 244 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 3 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 2 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 12 | 245 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 8 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 9 | 4 | 🟡 |

</details>

<details><summary><b>Rede</b> (57)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 1 | ✅ |
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
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 2 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 2 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 1 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 2 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` | xam 0xA | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 1 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 1 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 149 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 1 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 3 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Sistema e console</b> (29)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 14 | 596 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 15 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 2 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 2 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 16 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 6 | 7 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 2 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 2 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 17 | 21517 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 7 | 4 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 9 | 11 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 2 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 19 | 16 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 2 | 2 | ✅ |
| `XexLoadImageHeaders` | xboxkrnl 0x19B | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 5 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 2 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 3 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4383 | ⚪ stub |
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
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4384 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 4 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 5 | 6 | 🟡 |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (25)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 12 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 13 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 3 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 3 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 11 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 6 | 2 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 38 | 207 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 21 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 1 | ⚪ stub |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 21 | 2 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (21)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowGoldUpgradeUI` | xam 0x6A5 | 1 | 0 | ⚪ stub |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 1 | 🟡 |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 111418 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 16 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 5 | 5 | ✅ |

</details>

<details><summary><b>Memória</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 7 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 3 | 557 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 19 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 2 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 6 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 226 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 161 | ✅ |
| `NtProtectVirtualMemory` ⭐ | xboxkrnl 0xE1 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 8 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 9 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 2 | 0 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 1 | ✅ |
| `XeCryptSha256Final` | xboxkrnl 0x312 | 1 | 0 | ✅ |
| `XeCryptSha256Init` | xboxkrnl 0x310 | 1 | 0 | ✅ |
| `XeCryptSha256Update` | xboxkrnl 0x311 | 1 | 0 | ✅ |
| `XeCryptSha384Final` | xboxkrnl 0x318 | 1 | 0 | ❌ |
| `XeCryptSha384Init` | xboxkrnl 0x314 | 1 | 0 | ❌ |
| `XeCryptSha384Update` | xboxkrnl 0x315 | 1 | 0 | ❌ |
| `XeCryptSha512Final` | xboxkrnl 0x324 | 1 | 0 | ✅ |
| `XeCryptSha512Init` | xboxkrnl 0x31F | 1 | 0 | ✅ |
| `XeCryptSha512Update` | xboxkrnl 0x323 | 1 | 0 | ✅ |
| `XeCryptShaFinal` | xboxkrnl 0x191 | 1 | 0 | ✅ |
| `XeCryptShaInit` | xboxkrnl 0x18F | 1 | 0 | ✅ |
| `XeCryptShaUpdate` | xboxkrnl 0x190 | 1 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 1 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 27854 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 2 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 1 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8929 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |

</details>

<details><summary><b>Avatares</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfileCount` | xam 0x5EE | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 3 | 0 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |

</details>

<details><summary><b>Voz e microfones</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 59402 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 11 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 9 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 13 | 4188 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 30 | 5 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 2 | ✅ |
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

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 3 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 4 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 2 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 694 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 4 | 2 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 3 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike, nem o Kinect Sports, nem o Adventures, o Rush ou o Joy Ride).

**Ausentes no Xenia (37):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetServerToInAddr`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XamShowNuiAchievementsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeCryptSha384Final`, `XeCryptSha384Init`, `XeCryptSha384Update`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.
