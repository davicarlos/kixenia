# Adrenalin Misfits (título 4B4E0819)

Adrenalin Misfits, da Konami (2010, lançamento do Kinect): manobras de snowboard e skate em pranchas voadoras, com o corpo e avatares. Contêiner GOD em `<HD>/Xbox 360/Adrenalin Misfits [Kinect]/Content/0000000000000000/4B4E0819/`, extraído com `xenia-vfs-dump` para `~/Kixenia/kinect-sdk-re/games/extraidos/misfits/` (3,3 GB): `default.xex` (12,9 MB na memória; PDB `c:\Prg\Xenon\AdrenalinMisfits\nature\Release\nature.pdb`), pacote `DATA\DATA.LXHD`/`DATA.LXBD`, sons `SOUND\bgm.xwb`/`voice.xwb`, vídeos em `VIDEO\` (`op.wmv`, `demo01`–`03`, `m00`–`m06`, versões PAL), `database.xmplr` (modelos do rastreador), `nuiidentity.bin.be` (identidade) e `AvatarAssetPack`; avatares com shaders e animações próprias (`game:\AVATAR\`). Compilado com o XDK de julho de 2010 (`xdk-main-jul10`). Runtime NUI **2.0.11775.3** (no log: `NUI: title runtime 5/2DFF with flags 0000000B, HUD on`; 0x2DFF = 11775), a mesma versão-base do [Kinect-Adventures](kinect-adventures.md) (2.0.11775.7) e do [Kinect-Joy-Ride](kinect-joy-ride.md) (2.0.11775.6).

**Com qual se parece:** com o [Kinect-Joy-Ride](kinect-joy-ride.md) e o [Kinect-Adventures](kinect-adventures.md). Das 32 funções do Kinect fora das mensagens XMsg, 30 são idênticas às do Joy Ride (mesmas importações e mesmo tamanho) e 26 às do Adventures; com o [Kinect-Star-Wars](kinect-star-wars.md) e o [Kinect-Rush](kinect-rush.md), só 9. O motor de inclinação é **exatamente o do Adventures**: inclusive a máquina de estados (0x82772830, igual a 0x82D2CD20 do Adventures), que no Joy Ride tem uma pequena diferença.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Star-Wars](kinect-star-wars.md), [Rapala-for-Kinect](rapala-for-kinect.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema`). Não houve teste com o Kinect real nem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir, abertura | ✅ | Abre `DATA.LXHD`/`DATA.LXBD`, os bancos de som (quadro 64) e o vídeo de abertura `op.wmv` (quadro 506). 3030 quadros em 150 s, sem travar; fechou normalmente com o `kill` do script (`Cheap-skate exit!`); nenhum `Host-side crash`. |
| Passar da tela de título | ✅ (pelo log) | Com o jogador rastreado (a partir do quadro de esqueleto 1020), pergunta o cadastro de rosto do jogador (`XamUserNuiGetEnrollmentIndex`, quadros 1034–1042), abre o seletor de armazenamento pelo Kinect (`XamShowNuiDeviceSelectorUI`, quadro 1074, id de rastreamento 0x6A; o Xenia escolhe o dispositivo 1) e procura saves (`XamContentCreateEnumerator`, 0 encontrados). Ou seja, o jogador "entrou" no jogo pelo Kinect. |
| Tela de solução de problemas | 🟡 | No quadro 2829, cerca de 11 s depois de o jogador sair da gravação (último esqueleto rastreado no quadro de esqueleto 2100), o jogo chama `XamShowNuiTroubleshooterUI(0xFF, 0, 0)`; no Xenia isso abre o *Kinect Tuner* por cima do jogo, sem segurá-lo. Ver *Tela de solução de problemas* abaixo. |
| Ajuste do sensor | ✅ (sem motor) | Pede −25° logo ao abrir (sem chão guardado) e 0° no quadro 39; o rastreador acha o chão no quadro 241 e o jogo o guarda (`RememberFloor`, 868 mm) antes do quadro 361. A busca não se repete na gravação. |
| Rastreamento | 🟡 | 20 de 49 amostras (41%) com o jogador rastreado (quadros de esqueleto 600 e 1020–2100; 2 jogadores no 1320). |
| Câmera (PsCam) | ✅ | 8048 chamadas a `PsCamDeviceRequest`. |
| Kinect ID (rostos) | ✅ | `XamCacheOpenFile` ×2 e `XamCacheReset(0x18)` (aproximadas no worktree kinect-sistema) e `XamReadBiometricData`, sem erros; `XamUserNuiGetEnrollmentIndex` 4 vezes. |
| Microfones | 🟡 | Não lê áudio dos microfones do Kinect (nenhum `XamVoiceGetMicArrayAudio*`, nenhum MEC, nenhum reconhecedor de fala). Busca por ordinal `XamVoiceSetMicArrayIdleUsers` (chamada com 0, 1, 3, 7, 0xF, 0xE) e `XamVoiceGetMicArrayStatus`, que consulta **a cada quadro** (3032 vezes); na gravação o Xenia responde 2 (sem microfones). |
| Chat de voz | ⚪ stubs | `XamVoiceCreate` chamada uma vez ao abrir (stub); `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket` e `XamVoiceClose` não foram chamadas. |
| Avatares | ⚪ stubs | `XamAvatarInitialize` e logo `XamAvatarShutdown` ao abrir; as outras funções de avatar não foram chamadas em 150 s. O jogo traz shaders e animações próprios para os avatares. |
| Fitness | — | Não usa (nenhuma importação nem busca por ordinal). |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 misfits_replay 150 ~/Kixenia/kinect-sdk-re/games/extraidos/misfits/default.xex
```
O Xenia também abre o contêiner direto: passar `…/4B4E0819/00007000/C89387123E87BE559950` no lugar do `default.xex`.

## O que o jogo pede do Kinect

### Motor de inclinação e o status 'XtSs'
O motor de inclinação é o do runtime NUI do [Kinect-Adventures](kinect-adventures.md), função por função (0x827710E0–0x82772830). O 'XtSs' fica em **0x82BB0FD0** e é lido diretamente por `XamNuiCameraTiltGetStatus` (sem o desvio pelo Kinect Studio do Star Wars e do Rapala), em 0x82771FA0 e na máquina de estados. Campos lidos (por `amf_acessos_base.py`):

| Campo | Endereço | Quem lê | Uso |
|---|---|---|---|
| +0x20 gravidade | 0x82BB0FF0 | 0x82771FA0 (vetorial) | vetor da gravidade (acelerômetro) |
| +0x24, +0x28 | 0x82BB0FF4, 0x82BB0FF8 | 0x82772830, 0x82770278 | ângulo atual na máquina de estados |
| +0x2C flags | 0x82BB0FFC | 0x82771780, 0x82771810, 0x827719E8, 0x82772830 | bits 0–1: motor andando; 0x10: chão esquecido, recomeça a busca |
| +0x3C distância do chão | 0x82BB100C | 9 funções do motor | **0 = sem chão**, começa a busca de −25° |
| +0x40 | 0x82BB1010 | 0x827719E8 | bit 0: pedido recusado (ocupado) |
| +0x48, +0x4C gravidade do chão | 0x82BB1018, 0x82BB101C | 0x82771200 | ângulo do sensor quando o chão foi guardado |

Depende dos campos que o Xenia passou a preencher na correção do 'XtSs' (chão guardado e gravidade do chão). **Atenção:** é o mesmo motor que, no [Kinect-Adventures](kinect-adventures.md) com o Kinect real, repetiu a varredura (−25° → −17° → −7° → 3°) sem achar o chão enquanto o jogador não era rastreado. Na gravação o Misfits achou e guardou o chão logo no começo, mas ao vivo, com o motor de verdade, o risco é o mesmo.

### Tela de solução de problemas (`XamShowNuiTroubleshooterUI`)
O jogo a chama por um envoltório de chamada de cauda (0x8275DF48, sem entrada no `.pdata`) a partir de dois pontos:
1. 0x8247F2D0 (chamada indiretamente, sem `bl` no código): confere quatro objetos (um por jogador, tabela em 0x82961360, por um método virtual +0x18) e, se algum estiver ligado, abre a tela; depois lê `XNotifyGetNext` (notificação 9).
2. 0x824A4DC0: abre a tela e devolve se ela retornou 0.

Na gravação a tela abriu uma vez, no quadro 2829, depois de o jogador sumir; o objeto do jogador provavelmente marca "jogador perdido". No Xenia (`xam_ui.cc`), com o sensor pronto, `XamShowNuiTroubleshooterUI` abre o *Kinect Tuner* (`KinectTunerUI`) sem segurar o jogo; sem sensor, mostra uma caixa "The Kinect sensor isn't connected".

### Seletor de armazenamento pelo Kinect (`XamShowNuiDeviceSelectorUI`)
Chamado por 0x82358F70 (envoltório 0x824E38C0) com o id de rastreamento do jogador, tipo de conteúdo 1 (saves) e 0x800 bytes. No Xenia é aproximada: confere o HUD do Kinect e cai no `XamShowDeviceSelectorUI` normal, que escolhe o dispositivo 1 sem tela.

### O que não usa
- Microfones para fala ou comandos de voz: não lê o áudio do Kinect; só ajusta os usuários ociosos do microfone e consulta o estado (ver tabela).
- Fitness.
- Telas Nui do sistema além das duas acima: importa `XamShowNuiGuideUI`, `XamShowNuiSigninUI` e `XamShowNuiGamerCardUIForXUID` (ausente), não chamadas em 150 s.

### Importações ausentes ou stubs que ele chama de verdade
- `EtxProducerRegister` e `XamXlfsInitializeUploadQueue`: ausentes, cada uma caiu uma vez em "undefined extern call" ao iniciar (telemetria e fila de upload; os outros jogos Kinect fazem o mesmo).
- `XamAvatarInitialize`, `XamAvatarShutdown`, `XamVoiceCreate`: stubs, uma vez no começo.
- `XamUserGetDeviceContext`: stub, chamada 97 mil vezes (4 por quadro, uma por controle) — comum a muitos jogos, não é do Kinect.
- Ausentes e não chamadas: `XamShowNuiGamerCardUIForXUID`, `XamShowGamerCardUIForXUID`, `NetDll_XnpLogonGetStatus`, as outras `XamXlfs*`, `EtxProducerLog`/`Unregister`, `RtlUnwind`, `RtlCaptureContext`.

## Teste com o Kinect real: pendente
- **Inclinação**: é o motor do Adventures; conferir se a busca do chão termina com o motor de verdade ou se repete −25° → −17° → −7° → 3° como no Adventures.
- Entrar no jogo pelo Kinect (cadastro de rosto, seletor de armazenamento) e chegar a uma corrida; conferir as manobras (corpo inclinado, saltos).
- Tela de solução de problemas: quando o jogador sai da frente do sensor, o Kinect Tuner deve abrir e fechar, e o jogo voltar.
- Microfones: com o áudio do Kinect presente, `XamVoiceGetMicArrayStatus` passa a responder 1; ver se o jogo muda algo (chat pelo Kinect).
- Avatares: se o jogo mostra o avatar do jogador (as funções são stubs).
- Ritmo dos quadros de esqueleto com `--kinect_log_hands=true` e `kinect-tools/resumo_maos.py`.

## Arquivos da análise
- Imagem plana: `~/Kixenia/kinect-sdk-re/games/misfits.img` (Ghidra XEXLoaderWV, `DumpImage.java`). Seções: `.pdata` 0x82197000 (28020 funções), `.text` 0x821D0000–0x8291BDE4.
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/amfvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática) e as cópias `amfpar0`, `amfpar1`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/amf/c_full/<endereço>.c` (121 funções: as que chamam o Kinect, as que abrem as telas Nui e quem as chama), com os thunks trocados pelos nomes das importações; `amf/c/extra/` tem as funções da checagem da tela de solução de problemas.
- Log da execução: `~/Kixenia/kinect-tools/logs/misfits_replay.log`.
- Scripts (`games/decompilado/scripts/`): `amf_imports.py`, `amf_runtime.py`, `amf_funcs.py`, `amf_c.py`, `amf_acessos_base.py`, `rap_compara.py` (comparação, comum aos dois), `gen_misfits.py`; Ghidra: `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/amf/`.**

**255 importações** (89 do `xam.xex`, 166 do `xboxkrnl.exe`), 1339 chamadas diretas no código, 110 vistas em execução (gravação, log nível 3). Estado no Xenia Edge (worktree kinect-sistema): 177 implementadas, 40 stubs, 24 aproximadas, 13 ausentes, 1 sem marca.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia; as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 (linguagem VMX128, 2 processos). *No Joy Ride*: `=` mesma função (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho.

### Funções do jogo que falam com o Kinect

| Função | Tamanho | Chama | Chamada por | O que faz | No Joy Ride | Decompilada |
|---|---|---|---|---|---|---|
| 0x826DC5A0 | 224 | `XAudioGetVoiceCategoryVolume` |  |  | = 0x830f3e28 | ✅ |
| 0x8274CB70 | 220 | `XamAvatarLoadAnimation` | 0x8274CEF8 |  | = 0x82ce3048 | ✅ |
| 0x8274D0C0 | 248 | `XamAvatarGetAssets` | 0x824B2A10, 0x824B3CE0 |  | = 0x82cc4188 | ✅ |
| 0x8274D1B8 | 172 | `XamAvatarGenerateMipMaps` | 0x824B38B8 |  | — | ✅ |
| 0x8274D268 | 152 | `XamAvatarGetManifestLocalUser` | 0x824B2A10 |  | = 0x82cc3e28 | ✅ |
| 0x8274D300 | 168 | `XamAvatarGetMetadataRandom` | 0x824B2A10 |  | = 0x82cc3ec0 | ✅ |
| 0x8274E6D0 | 124 | `XamAvatarShutdown` | 0x8274E750, 0x824B5C50 | Encerra o sistema de avatares (`XamAvatarShutdown`). | = 0x82cc3c30 | ✅ |
| 0x8274E750 | 372 | `XamAvatarInitialize` | 0x824B5268 | Inicia o sistema de avatares (`XamAvatarInitialize`). | = 0x82cc3cb0 | ✅ |
| 0x82762018 | 96 | `PsCamDeviceRequest` |  | **O único ponto que chama a câmera.** Tenta `XamXStudioRequest` (Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | = 0x82ce47f8 | ✅ |
| 0x8276A690 | 392 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 0x8276FC10 |  | = 0x82cf2548 | ✅ |
| 0x8276A818 | 340 | `XamNuiIdentityGetSessionId` | 0x8276F830 |  | = 0x82cf26d0 | ✅ |
| 0x8276B718 | 392 | `XamUserNuiGetEnrollmentIndex` | 0x8276E348 | Pergunta ao sistema qual cadastro de rosto é o do jogador (`XamUserNuiGetEnrollmentIndex`). | = 0x82cf35d0 | ✅ |
| 0x8276BEE0 | 300 | `XamUserNuiGetUserIndex` |  |  | = 0x82cf3d98 | ✅ |
| 0x8276CCB8 | 568 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 0x8276EE68 | Cadastro de rostos: liga a biometria do perfil e grava/lê os dados. | = 0x82cf48b8 | ✅ |
| 0x8276CEF0 | 3868 | `XamNuiIdentityGetSessionId` | 0x8276EB00, 0x8276F530, 0x8276DE10, 0x8276E740 |  | = 0x82cf4af0 | ✅ |
| 0x8276E528 | 244 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 0x8276F830 | Limpeza do cache do Kinect ID ao iniciar o motor de identidade (`XamCacheOpenFile` ×2, `XamCacheReset(0x18)`, `XamCacheCloseFile`). | = 0x82cf6128 | ✅ |
| 0x8276E620 | 288 | `XamReadBiometricData` | 0x8276F830 | Lê os rostos cadastrados (`XamReadBiometricData`). | = 0x82cf6220 | ✅ |
| 0x8276F208 | 672 | `XamNuiIdentityGetSessionId` | 0x8276FC10 |  | = 0x82cf6e08 | ✅ |
| 0x827710E0 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 0x82771200 | Fim de um pedido de inclinação: para o motor e informa o status ao sistema. | = 0x82cf7958 | ✅ |
| 0x82771390 | 112 | `XamNuiCameraElevationSetAngle` | 0x827719E8, 0x82772830 | Inclina para um ângulo entre −27° e +27° (`ElevationSetAngle`). | = 0x82cf7c08 | ✅ |
| 0x82771780 | 144 | `XamNuiCameraElevationSetAngle` | 0x82771C90, 0x827719E8, 0x82772830 | Começa a busca: inclina para **−25°** se não há chão guardado (+0x3C = 0) ou se o status tem a flag 0x10. | = 0x82cf7ff8 | ✅ |
| 0x82771810 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 0x82772830 | Guarda o chão do quadro de esqueleto (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis. | = 0x82cf8088 | ✅ |
| 0x827719E8 | 680 | `XamNuiCameraElevationGetAngle` | 0x82771C90 | **Pedido de ajuste automático**: confere +0x2C e +0x40 (bit 0 = ocupado) e inclina de novo ou começa a busca. | = 0x82cf8260 | ✅ |
| 0x82771C90 | 780 | `XamNuiCameraTiltReportStatus` |  | **Callback do motor de inclinação** registrado no xam; responde com o status. | = 0x82cf8508 | ✅ |
| 0x82771FA0 | 144 | `XamNuiCameraTiltGetStatus` | 0x82772830, 0x82764B30 | Lê o 'XtSs' (`XamNuiCameraTiltGetStatus`, 0x50 bytes, em 0x82BB0FD0) e converte a gravidade em vetor. | = 0x82cf8818 | ✅ |
| 0x82772030 | 80 | `XamNuiCameraTiltSetCallback` | 0x827680B8 | Registra o callback 0x82771C90 (`XamNuiCameraTiltSetCallback`). | = 0x82cf88a8 | ✅ |
| 0x82772830 | 2456 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus` | 0x82767850 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: **a mesma do Adventures** (busca −25° → −17° → −7° → 3°, mira a cabeça do jogador). | ≈ 0x82cf90a8 | ✅ |
| 0x82778798 | 976 | `XamNuiIdentityGetSessionId` | 0x8276F830 |  | = 0x82cff580 | ✅ |
| 0x8278AFC8 | 888 | `XamVoiceHeadsetPresent` | 0x8278B970 | Chat de voz: confere se há headset (`XamVoiceHeadsetPresent`). | = 0x831386b0 | ✅ |
| 0x8278B5E0 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 0x82789660, 0x8278BDC0 |  | — | ✅ |
| 0x8278B970 | 600 | `XamVoiceSubmitPacket` | 0x8278A170 |  | = 0x83139058 | ✅ |
| 0x8278BCD0 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 0x82789F78, 0x8278BE80 |  | — | ✅ |
| 0x8278BDC0 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 0x827895C0 | Chat de voz: cria/fecha o motor de voz (`XamVoiceCreate`, `XamVoiceClose`), o mesmo do Joy Ride. | = 0x831394a8 | ✅ |
| 0x827901E8 | 208 | `XamVoiceSubmitPacket` | 0x82790988, 0x827903F8 |  | = 0x8313e318 | ✅ |

Mais 30 funções que só mandam mensagens XMsg aos apps do sistema (0xFE e 0xFA).

### Importações do Kinect, NUI, voz e avatares

| Importação | Ordinal | Xenia | Chamadas no código | Em execução |
|---|---|---|---|---|
| `PsCamDeviceRequest` | 781 | ✅ | 1 | 8048 |
| `XAudioGetVoiceCategoryVolume` | 504 | ⚪ stub | 1 | 0 |
| `XMsgCancelIORequest` | 504 | ✅ | 3 | 1 |
| `XMsgInProcessCall` | 500 | ✅ | 11 | 3199 |
| `XMsgStartIORequest` | 503 | ✅ | 17 | 12 |
| `XMsgSystemProcessCall` | 502 | ✅ | 5 | 1 |
| `XamAvatarGenerateMipMaps` | 1506 | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssets` | 1505 | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssetsResultSize` | 1504 | ⚪ stub | 1 | 0 |
| `XamAvatarGetManifestLocalUser` | 1502 | ⚪ stub | 1 | 0 |
| `XamAvatarGetMetadataRandom` | 1515 | ⚪ stub | 1 | 0 |
| `XamAvatarInitialize` | 1500 | ⚪ stub | 1 | 1 |
| `XamAvatarLoadAnimation` | 1511 | ⚪ stub | 1 | 0 |
| `XamAvatarManifestGetBodyType` | 1521 | ⚪ stub | 1 | 0 |
| `XamAvatarShutdown` | 1501 | ⚪ stub | 1 | 1 |
| `XamCacheCloseFile` | 693 | 🟡 | 1 | 0 |
| `XamCacheOpenFile` | 692 | 🟡 | 1 | 2 |
| `XamCacheReset` | 695 | 🟡 | 1 | 1 |
| `XamNuiCameraElevationGetAngle` | 2177 | ✅ | 1 | alta freq. |
| `XamNuiCameraElevationSetAngle` | 2176 | ✅ | 2 | 2 |
| `XamNuiCameraElevationStopMovement` | 2179 | ✅ | 2 | 0 |
| `XamNuiCameraRememberFloor` | 2149 | ✅ | 2 | alta freq. |
| `XamNuiCameraTiltGetStatus` | 2160 | ✅ | 2 | alta freq. |
| `XamNuiCameraTiltReportStatus` | 2197 | ✅ | 7 | 0 |
| `XamNuiCameraTiltSetCallback` | 2157 | ✅ | 1 | 1 |
| `XamNuiIdentityGetSessionId` | 2163 | ✅ | 6 | 6 |
| `XamReadBiometricData` | 2121 | ✅ | 2 | 1 |
| `XamShowNuiDeviceSelectorUI` | 1136 | 🟡 | 1 | 1 |
| `XamShowNuiGamerCardUIForXUID` | 1140 | ❌ | 1 | 0 |
| `XamShowNuiGuideUI` | 854 | ✅ | 1 | 0 |
| `XamShowNuiSigninUI` | 1146 | 🟡 | 1 | 0 |
| `XamShowNuiTroubleshooterUI` | 1131 | ✅ | 1 | 1 |
| `XamUserNuiEnableBiometric` | 2147 | ✅ | 1 | 0 |
| `XamUserNuiGetEnrollmentIndex` | 2154 | ✅ | 1 | 4 |
| `XamUserNuiGetUserIndex` | 2153 | ✅ | 2 | 0 |
| `XamVoiceClose` | 783 | ⚪ stub | 3 | 0 |
| `XamVoiceCreate` | 780 | ⚪ stub | 1 | 1 |
| `XamVoiceHeadsetPresent` | 781 | ⚪ stub | 1 | 0 |
| `XamVoiceSubmitPacket` | 782 | ⚪ stub | 2 | 0 |
| `XamWriteBiometricData` | 2122 | ✅ | 1 | 0 |

### Ausentes e stubs

| Importação | Biblioteca | Xenia | Chamadas no código | Em execução |
|---|---|---|---|---|
| `EtxProducerLog` | xboxkrnl | ❌ | 3 | 0 |
| `EtxProducerRegister` | xboxkrnl | ❌ | 1 | 0 |
| `EtxProducerUnregister` | xboxkrnl | ❌ | 1 | 0 |
| `NetDll_XnpLogonGetStatus` | xam | ❌ | 1 | 0 |
| `RtlCaptureContext` | xboxkrnl | ❌ | 2 | 0 |
| `RtlUnwind` | xboxkrnl | ❌ | 3 | 0 |
| `XamShowGamerCardUIForXUID` | xam | ❌ | 1 | 0 |
| `XamShowNuiGamerCardUIForXUID` | xam | ❌ | 1 | 0 |
| `XamXlfsInitializeUploadQueue` | xam | ❌ | 1 | 0 |
| `XamXlfsMountUploadQueueInstance` | xam | ❌ | 1 | 0 |
| `XamXlfsUninitializeUploadQueue` | xam | ❌ | 1 | 0 |
| `XamXlfsUnmountUploadQueueInstance` | xam | ❌ | 1 | 0 |
| `__C_specific_handler` | xboxkrnl | ❌ | 0 | 0 |
| `RtlNtStatusToDosError` | xboxkrnl | ❔ | 17 | 0 |
| `DbgBreakPoint` | xboxkrnl | ⚪ stub | 1 | 0 |
| `FscSetCacheElementCount` | xboxkrnl | ⚪ stub | 1 | 1 |
| `HalReturnToFirmware` | xboxkrnl | ⚪ stub | 1 | 0 |
| `KeBugCheckEx` | xboxkrnl | ⚪ stub | 5 | 0 |
| `KeEnableFpuExceptions` | xboxkrnl | ⚪ stub | 1 | 312 |
| `KiApcNormalRoutineNop` | xboxkrnl | ⚪ stub | 1 | 1 |
| `RtlRaiseException` | xboxkrnl | ⚪ stub | 3 | 0 |
| `VdEnableDisableClockGating` | xboxkrnl | ⚪ stub | 1 | 0 |
| `VdGetCurrentDisplayGamma` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdGetCurrentDisplayInformation` | xboxkrnl | ⚪ stub | 3 | 1 |
| `VdGetSystemCommandBuffer` | xboxkrnl | ⚪ stub | 1 | 3030 |
| `VdInitializeEngines` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdQueryVideoFlags` | xboxkrnl | ⚪ stub | 1 | 2 |
| `VdQueryVideoMode` | xboxkrnl | ⚪ stub | 3 | 3 |
| `VdRetrainEDRAM` | xboxkrnl | ⚪ stub | 2 | 3031 |
| `VdRetrainEDRAMWorker` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdSetDisplayMode` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl | ⚪ stub | 3 | 2 |
| `VdShutdownEngines` | xboxkrnl | ⚪ stub | 2 | 0 |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl | ⚪ stub | 1 | 0 |
| `XGetAVPack` | xam | ⚪ stub | 1 | 0 |
| `XGetGameRegion` | xam | ⚪ stub | 2 | 1 |
| `XamAvatarGenerateMipMaps` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssets` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssetsResultSize` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetManifestLocalUser` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetMetadataRandom` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarInitialize` | xam | ⚪ stub | 1 | 1 |
| `XamAvatarLoadAnimation` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarManifestGetBodyType` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarShutdown` | xam | ⚪ stub | 1 | 1 |
| `XamContentGetDeviceState` | xam | ⚪ stub | 1 | 0 |
| `XamReadTileToTexture` | xam | ⚪ stub | 2 | 0 |
| `XamUserGetDeviceContext` | xam | ⚪ stub | 1 | 97723 |
| `XamVoiceClose` | xam | ⚪ stub | 3 | 0 |
| `XamVoiceCreate` | xam | ⚪ stub | 1 | 1 |
| `XamVoiceHeadsetPresent` | xam | ⚪ stub | 1 | 0 |
| `XamVoiceSubmitPacket` | xam | ⚪ stub | 2 | 0 |
| `XeCryptRandom` | xboxkrnl | ⚪ stub | 1 | 0 |
