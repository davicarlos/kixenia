# Rapala for Kinect (título 415608C6)

Rapala for Kinect (Activision; logos da Activision e da nowpro na abertura): jogo de pesca controlado com o corpo. Contêiner GOD em `<HD>/Xbox 360/Rapala for Kinect [PAL][NTSCU][Kinect]/Content/0000000000000000/415608C6/`, extraído com `xenia-vfs-dump` para `~/Kixenia/kinect-sdk-re/games/extraidos/rapala/`: `default.xex` (6,3 MB), `d01.dat` (697 MB, um pacote com todos os dados do jogo), vídeos em `data\movie\` (logos Activision e nowpro), `Database.xmplr` (modelos do rastreador), `NuiIdentity.bin.be` (identidade) e `AvatarAssetPack`. Compilado com o XDK de junho de 2011 (`XDK-MAIN-JUN11`). Runtime NUI **2.0.20764.0** (no log: `NUI: title runtime 5/511C with flags 0000440B, HUD on`; 0x511C = 20764), um pouco anterior ao do [Kinect-Star-Wars](kinect-star-wars.md) e do [Kinect-Rush](kinect-rush.md) (2.0.21076.0, `5/5254`).

**Com qual se parece:** com o [Kinect-Star-Wars](kinect-star-wars.md) e o [Kinect-Rush](kinect-rush.md). Das 32 funções do Kinect fora das mensagens XMsg, 22 são idênticas às do Star Wars e às do Rush (mesmas importações e mesmo tamanho), entre elas **o motor de inclinação inteiro** (0x824A6D68–0x824A8560), a câmera, o estado do sensor e o motor de identidade; com o [Kinect-Joy-Ride](kinect-joy-ride.md) só 14 e com o [Kinect-Adventures](kinect-adventures.md) 9. As funções de avatar são as do Joy Ride (o Star Wars e o Rush não usam avatares).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Rush](kinect-rush.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md), [XAM-Fitness](../../xam/xam-fitness.md).

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema`). Não houve teste com o Kinect real nem captura de tela: o estado visual do jogo não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir, logos | ✅ | Abre `d01.dat` e toca os vídeos `logo_activision_16x9.wmv` (quadro 3) e `logo_nowpro_16x9.wmv` (quadro 198). 2929 quadros em 150 s, sem travar nem derrubar o emulador; nenhum `Host-side crash`. |
| Menu | ❔ | Todos os dados vêm de dentro do `d01.dat`, então o log não mostra que tela foi carregada. O jogo continua desenhando e processando o Kinect até o fim; no quadro 928 pergunta qual cadastro de rosto é do jogador (`XamUserNuiGetEnrollmentIndex`), o que sugere a tela de entrada. Conferir na tela. |
| Runtime NUI | ✅ | `NUI: title runtime 5/511C`. Busca por ordinal (fora da tabela de importações) `XamFitnessInitialize` (quadro 35) e `XamNuiGetTrueColorInfo` (quadro 37), ambas presentes no Xenia. |
| Ajuste do sensor | ✅ (sem motor) | Pede −25° no quadro 37 (sem chão guardado) e 0° no quadro 95; o chão aparece no quadro 161 e é guardado (`RememberFloor`, 874 mm) antes do quadro 352. Depois disso a busca não recomeça. |
| Rastreamento | 🟡 | 14 de 42 amostras (33%) com o jogador rastreado (quadros de esqueleto 480 e 780–1440, depois 2520); 2520 quadros de esqueleto em 150 s (cerca de 17 por segundo). |
| Câmera (PsCam) | ✅ | 8234 chamadas a `PsCamDeviceRequest` (códigos 0, 27 e 35 na abertura). |
| Kinect ID (rostos) | ✅ | Abre `NuiIdentity.bin.be` (tenta `update:` e cai para `game:`); `XamCacheOpenFile` e `XamCacheReset(0x18)` (aproximadas no worktree kinect-sistema) e `XamReadBiometricData`, sem erros. |
| Microfones e voz | — | **Não usa**: nenhuma importação `XamVoice*`/`XAudio*MEC*` e nenhuma biblioteca de fala no executável. |
| Avatares | ⚪ stubs | `XamAvatarInitialize` e logo `XamAvatarShutdown` no quadro 2 (stubs no Xenia); as outras funções de avatar não foram chamadas em 150 s. |
| Fitness | 🟡 | O runtime NUI tem um módulo de fitness (0x82499B38…0x82499F20) que pega `XamFitnessInitialize`, `…UnInitialize`, `…GetCurrentBodyProfileRecord`, `…AddFitnessEvent`, `…SystemTimeToMsgTime` e `…ConvertFloatMetToByteMet` por ordinal. Só `XamFitnessInitialize` foi chamada na gravação. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 rapala_replay 150 ~/Kixenia/kinect-sdk-re/games/extraidos/rapala/default.xex
```
O Xenia também abre o contêiner direto: passar `…/415608C6/00007000/C34DC053D312F344AF2F` no lugar do `default.xex`.

## O que o jogo pede do Kinect

### Motor de inclinação e o status 'XtSs'
O motor de inclinação é o do runtime NUI do [Kinect-Star-Wars](kinect-star-wars.md) (as 10 funções, de 0x824A6D68 a 0x824A8560, são idênticas), com o estado global em 0x825E0270–0x825E0470: estado em `0x825E02C0`, **'XtSs' em `0x825E03C0`**. Como no Star Wars, o 'XtSs' é lido por um ponteiro (0x825B3BD8) que o Kinect Studio pode trocar: primeiro `XamXStudioRequest(0x1005)` e, se falhar, `XamNuiCameraTiltGetStatus`. Campos lidos (por `rap_acessos_base.py`):

| Campo | Endereço | Quem lê | Uso |
|---|---|---|---|
| +0x24, +0x28 gravidade | 0x825E03E4, 0x825E03E8 | 0x824A8560 | ângulo atual na máquina de estados |
| +0x2C flags | 0x825E03EC | 0x824A7448, 0x824A74D8, 0x824A76B0, 0x824A8560 | bits 0–1: motor andando; 0x10: chão esquecido, recomeça a busca |
| +0x30..+0x38 plano do chão | 0x825E03F0 | cópia vetorial | passado ao rastreador (como no Star Wars) |
| +0x3C distância do chão | 0x825E03FC | 8 funções do motor | **0 = sem chão**, começa a busca de −25° |
| +0x40 | 0x825E0400 | 0x824A76B0 | bit 0: pedido recusado (ocupado) |
| +0x48, +0x4C gravidade do chão | 0x825E0408, 0x825E040C | 0x824A6E88 | ângulo do sensor quando o chão foi guardado |

Depende, portanto, dos campos que o Xenia passou a preencher na correção do 'XtSs' (chão guardado e gravidade do chão). Na gravação, depois do `RememberFloor` o log mostra o chão guardado (`remembered (0.007 0.999 -0.033 874.471)`) e a busca não se repete.

### Fitness
O jogo não importa nada de fitness, mas o runtime NUI do título busca as funções `XamFitness*` por ordinal (`XexGetProcedureAddress` no xam: 0x4CC Initialize, 0x4DE UnInitialize, 0x4C2 GetCurrentBodyProfileRecord, 0x4C6 AddFitnessEvent, 0x4DB SystemTimeToMsgTime, 0x4E2 ConvertFloatMetToByteMet). 0x82499B38 inicia (8 entradas de 0x28 bytes, uma por jogador), 0x82499C88 lê o perfil corporal do jogador e 0x82499F20 registra eventos de exercício. O Xenia tem todas (`xam_fitness.cc`); com a gravação só `XamFitnessInitialize` foi chamada. Ver [XAM-Fitness](../../xam/xam-fitness.md).

### O que não usa
- Microfones e voz: nenhuma importação de voz, nenhum reconhecedor de fala.
- Telas Nui do sistema: importa 8 `XamShowNui*UI`, mas só `XamShowNuiMessageBoxUI` e `XamShowNuiDirtyDiscErrorUI` têm chamada direta, e nenhuma foi chamada em 150 s.

### Importações ausentes ou stubs que ele chama de verdade
- `EtxProducerRegister` e `XamXlfsInitializeUploadQueue`: ausentes, cada uma caiu uma vez em "undefined extern call" ao iniciar (telemetria e fila de upload, não afetam o Kinect; os outros jogos Kinect fazem o mesmo).
- `XamAvatarInitialize`, `XamAvatarShutdown`: stubs, chamadas uma vez no começo.
- Ausentes e não chamadas: `XamShowNuiGamerCardUIForXUID`, `XamShowGamerCardUIForXUID`, as outras `XamXlfs*`, `EtxProducerLog`/`Unregister` e funções de E/S do kernel (`Io*`, `RtlUnwind`…) que o executável importa mas não usou.

## Teste com o Kinect real: pendente
- Ver se chega ao menu e entra numa pescaria (o log não mostra as telas, os dados estão no `d01.dat`).
- Os gestos de pesca (lançar e recolher): o jogo depende do esqueleto TRACKED quadro a quadro; medir o ritmo com `--kinect_log_hands=true` e `kinect-tools/resumo_maos.py`.
- Inclinação com o motor de verdade: deve ir a −25°, achar o chão e não repetir a busca (mesmo motor do Star Wars, que funcionou ao vivo).
- Kinect ID: se o jogo reconhece o jogador pelo rosto ao entrar.
- Fitness: se aparecem `XamFitnessGetCurrentBodyProfileRecord` e `XamFitnessAddFitnessEvent` no log durante uma pescaria.
- Avatares: se o jogo mostra o avatar do jogador (as funções são stubs).

## Arquivos da análise
- Imagem plana: `~/Kixenia/kinect-sdk-re/games/rapala.img` (Ghidra XEXLoaderWV, `DumpImage.java`). Seções: `.pdata` 0x82137200 (8481 funções), `.text` 0x82150000–0x8257C5C4.
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/rapvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática) e as cópias `rappar0`, `rappar1`.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/rap/c_full/<endereço>.c` (125 funções: as que chamam o Kinect, o módulo de fitness e quem as chama), com os thunks trocados pelos nomes das importações; `rap/c/extra/` tem os acessos de fitness por ordinal.
- Log da execução: `~/Kixenia/kinect-tools/logs/rapala_replay.log`.
- Scripts (`games/decompilado/scripts/`): `rap_imports.py`, `rap_runtime.py`, `rap_funcs.py`, `rap_c.py`, `rap_compara.py`, `rap_acessos_base.py`, `rap_strref.py`, `gen_rapala.py`; Ghidra: `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/rap/`.**

**281 importações** (101 do `xam.xex`, 180 do `xboxkrnl.exe`), 1656 chamadas diretas no código, 93 vistas em execução (gravação, log nível 3). Estado no Xenia Edge (worktree kinect-sistema): 189 implementadas, 42 stubs, 27 aproximadas, 22 ausentes, 1 sem marca.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia; as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 (linguagem VMX128, 2 processos). *No Star Wars*: `=` mesma função (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho.

### Funções do jogo que falam com o Kinect

| Função | Tamanho | Chama | Chamada por | O que faz | No Star Wars | Decompilada |
|---|---|---|---|---|---|---|
| 0x822A5110 | 56 | `XamShowNuiMessageBoxUI` | 0x821566D8, 0x82263F80, 0x82263E38, 0x822640D0 |  | — | ✅ |
| 0x822A5148 | 28 | `XamShowNuiDirtyDiscErrorUI` |  |  | — | ✅ |
| 0x822A51E8 | 40 | `XamNuiGetDeviceStatus` | 0x821B4598, 0x82155BF0 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve as flags; chamada a cada quadro. | = 0x829a6fe8 | ✅ |
| 0x8234B5A0 | 376 | `XAudioGetVoiceCategoryVolume` |  |  | = 0x82d73238 | ✅ |
| 0x8247ECD8 | 152 | `XamAvatarGetManifestLocalUser` | 0x822322F8 |  | — | ✅ |
| 0x8247ED70 | 168 | `XamAvatarGetMetadataRandom` | 0x82230900, 0x822322A8 |  | — | ✅ |
| 0x8247F580 | 220 | `XamAvatarLoadAnimation` | 0x8247F908 |  | — | ✅ |
| 0x8247FA80 | 124 | `XamAvatarShutdown` | 0x8247FB00, 0x82231FE0 | Encerra o sistema de avatares (`XamAvatarShutdown`). | — | ✅ |
| 0x8247FB00 | 372 | `XamAvatarInitialize` | 0x82230978 | Inicia o sistema de avatares (`XamAvatarInitialize`). | — | ✅ |
| 0x8247FCC8 | 248 | `XamAvatarGetAssets` | 0x822327E8 |  | — | ✅ |
| 0x8247FDC0 | 172 | `XamAvatarGenerateMipMaps` | 0x82232350 |  | — | ✅ |
| 0x82491F18 | 96 | `PsCamDeviceRequest` |  | **O único ponto que chama a câmera.** Tenta `XamXStudioRequest` (Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | = 0x82b04410 | ✅ |
| 0x824A0F88 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 0x824A6310 |  | ≈ 0x82b0d840 | ✅ |
| 0x824A1178 | 432 | `XamNuiIdentityGetSessionId` | 0x824A5DE0 |  | = 0x82b0da60 | ✅ |
| 0x824A30F0 | 356 | `XamUserNuiGetUserIndex` |  |  | = 0x82b0f9f0 | ✅ |
| 0x824A3ED8 | 388 | `XamNuiIdentityGetSessionId` | 0x824A6278 |  | = 0x82b10a78 | ✅ |
| 0x824A4060 | 3200 | `XamNuiIdentityGetSessionId` | 0x824A5A18, 0x824B0508, 0x824A4CE0, 0x824A5110 |  | = 0x82b10c00 | ✅ |
| 0x824A6D68 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 0x824A6E88 | Fim de um pedido de inclinação: para o motor e informa o status ao sistema. | = 0x82b014b8 | ✅ |
| 0x824A7018 | 132 | `XamNuiCameraElevationSetAngle` | 0x824A76B0, 0x824A8560 | Inclina para um ângulo (`ElevationSetAngle`). | = 0x82b01768 | ✅ |
| 0x824A70C0 | 76 | `XamNuiCameraTiltGetStatus` |  | Lê o 'XtSs' (0x50 bytes, em 0x825E03C0): tenta `XamXStudioRequest(0x1005)` e, se o Kinect Studio não responde (0x80070057), chama `XamNuiCameraTiltGetStatus`. O ponteiro para esta função é gravado por 0x824A7D18 (o Kinect Studio pode trocá-lo). | = 0x82b01810 | ✅ |
| 0x824A7448 | 144 | `XamNuiCameraElevationSetAngle` | 0x824A76B0, 0x824A7958, 0x824A8560 | Começa a busca: inclina para **−25°** se não há chão guardado (+0x3C = 0) ou se o status tem a flag 0x10. | = 0x82b01b98 | ✅ |
| 0x824A74D8 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 0x824A8560 | Guarda o chão do quadro de esqueleto (`XamNuiCameraRememberFloor`) e informa o status. | = 0x82b01c28 | ✅ |
| 0x824A76B0 | 676 | `XamNuiCameraElevationGetAngle` | 0x824A7958 | **Pedido de ajuste automático**: lê o ângulo atual, confere +0x2C (flag 0x10) e +0x40 (bit 0 = ocupado) e inclina de novo ou começa a busca. | = 0x82b01e00 | ✅ |
| 0x824A7958 | 780 | `XamNuiCameraTiltReportStatus` |  | **Callback do motor de inclinação** registrado no xam; responde com o status. | = 0x82b02290 | ✅ |
| 0x824A7D18 | 148 | `XamNuiCameraTiltSetCallback` | 0x82498BC8 | Zera o estado do motor (0x825E0270, 0x200 bytes), registra o callback 0x824A7958 e grava o ponteiro de leitura do 'XtSs'. | = 0x82b02650 | ✅ |
| 0x824A8560 | 2436 | `XamNuiCameraElevationStopMovement` | 0x824981D0 | **Máquina de estados da inclinação**, a cada quadro de esqueleto (mesma do Star Wars e do Rush); lê a gravidade +0x24/+0x28. | = 0x82b02f30 | ✅ |
| 0x824AF878 | 996 | `XamNuiIdentityGetSessionId` | 0x824A5DE0 |  | = 0x82b19a80 | ✅ |
| 0x824B3C20 | 392 | `XamUserNuiGetEnrollmentIndex` | 0x824B3FB8 | Pergunta ao sistema qual cadastro de rosto é o do jogador (`XamUserNuiGetEnrollmentIndex`). | = 0x82b1de50 | ✅ |
| 0x824B3E68 | 336 | `XamUserNuiGetUserIndex` | 0x824B3FB8 |  | = 0x82b1e098 | ✅ |
| 0x824B40E8 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 0x824A5DE0 | Limpeza do cache do Kinect ID ao iniciar o motor de identidade (`XamCacheOpenFile`, `XamCacheReset(0x18)`, `XamCacheCloseFile`). | = 0x82b1e318 | ✅ |
| 0x824B41E0 | 488 | `XamReadBiometricData` | 0x824A5DE0 | Lê os rostos cadastrados (`XamReadBiometricData`). | = 0x82b1e410 | ✅ |
| 0x824B4850 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 0x824A5490 | Cadastro de rostos: liga a biometria do perfil e grava/lê os dados (`XamUserNuiEnableBiometric`, `XamWriteBiometricData`). | = 0x82b1ea28 | ✅ |

Mais 29 funções que só mandam mensagens XMsg aos apps do sistema (0xFE e 0xFA).

### Importações do Kinect, NUI, fitness e avatares

| Importação | Ordinal | Xenia | Chamadas no código | Em execução |
|---|---|---|---|---|
| `PsCamDeviceRequest` | 781 | ✅ | 1 | 8234 |
| `XAudioGetVoiceCategoryVolume` | 504 | ⚪ stub | 1 | 0 |
| `XMsgCancelIORequest` | 504 | ✅ | 1 | 0 |
| `XMsgInProcessCall` | 500 | ✅ | 9 | 2570 |
| `XMsgStartIORequest` | 503 | ✅ | 16 | 7 |
| `XMsgStartIORequestEx` | 508 | ✅ | 1 | 1 |
| `XMsgSystemProcessCall` | 502 | ✅ | 5 | 0 |
| `XamAvatarGenerateMipMaps` | 1506 | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssets` | 1505 | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssetsResultSize` | 1504 | ⚪ stub | 1 | 0 |
| `XamAvatarGetManifestLocalUser` | 1502 | ⚪ stub | 1 | 0 |
| `XamAvatarGetMetadataRandom` | 1515 | ⚪ stub | 1 | 0 |
| `XamAvatarInitialize` | 1500 | ⚪ stub | 1 | 1 |
| `XamAvatarLoadAnimation` | 1511 | ⚪ stub | 1 | 0 |
| `XamAvatarManifestGetBodyType` | 1521 | ⚪ stub | 1 | 0 |
| `XamAvatarSetCustomAsset` | 1507 | ⚪ stub | 1 | 0 |
| `XamAvatarShutdown` | 1501 | ⚪ stub | 1 | 1 |
| `XamCacheCloseFile` | 693 | 🟡 | 1 | 0 |
| `XamCacheOpenFile` | 692 | 🟡 | 1 | 1 |
| `XamCacheReset` | 695 | 🟡 | 1 | 1 |
| `XamNuiCameraElevationGetAngle` | 2177 | ✅ | 1 | alta freq. |
| `XamNuiCameraElevationSetAngle` | 2176 | ✅ | 2 | 2 |
| `XamNuiCameraElevationStopMovement` | 2179 | ✅ | 2 | 0 |
| `XamNuiCameraRememberFloor` | 2149 | ✅ | 2 | alta freq. |
| `XamNuiCameraTiltGetStatus` | 2160 | ✅ | 1 | alta freq. |
| `XamNuiCameraTiltReportStatus` | 2197 | ✅ | 7 | 0 |
| `XamNuiCameraTiltSetCallback` | 2157 | ✅ | 1 | 1 |
| `XamNuiGetDeviceStatus` | 2181 | ✅ | 1 | 2902 |
| `XamNuiIdentityGetSessionId` | 2163 | ✅ | 6 | 3 |
| `XamReadBiometricData` | 2121 | ✅ | 2 | 1 |
| `XamShowNuiDeviceSelectorUI` | 1136 | 🟡 | 1 | 0 |
| `XamShowNuiDirtyDiscErrorUI` | 1137 | ⚪ stub | 1 | 0 |
| `XamShowNuiGamerCardUIForXUID` | 1140 | ❌ | 1 | 0 |
| `XamShowNuiGuideUI` | 854 | ✅ | 1 | 0 |
| `XamShowNuiMessageBoxUI` | 1143 | 🟡 | 1 | 0 |
| `XamShowNuiSigninUI` | 1146 | 🟡 | 1 | 0 |
| `XamShowNuiTroubleshooterUI` | 1131 | ✅ | 1 | 0 |
| `XamUserNuiEnableBiometric` | 2147 | ✅ | 1 | 0 |
| `XamUserNuiGetEnrollmentIndex` | 2154 | ✅ | 1 | 1 |
| `XamUserNuiGetUserIndex` | 2153 | ✅ | 3 | 0 |
| `XamWriteBiometricData` | 2122 | ✅ | 1 | 0 |

### Ausentes e stubs

| Importação | Biblioteca | Xenia | Chamadas no código | Em execução |
|---|---|---|---|---|
| `EtxProducerLog` | xboxkrnl | ❌ | 8 | 0 |
| `EtxProducerRegister` | xboxkrnl | ❌ | 1 | 0 |
| `EtxProducerUnregister` | xboxkrnl | ❌ | 2 | 0 |
| `IoCheckShareAccess` | xboxkrnl | ❌ | 2 | 0 |
| `IoCompleteRequest` | xboxkrnl | ❌ | 11 | 0 |
| `IoDismountVolume` | xboxkrnl | ❌ | 2 | 0 |
| `IoDismountVolumeByFileHandle` | xboxkrnl | ❌ | 1 | 0 |
| `IoInvalidDeviceRequest` | xboxkrnl | ❌ | 0 | 0 |
| `IoRemoveShareAccess` | xboxkrnl | ❌ | 2 | 0 |
| `IoSetShareAccess` | xboxkrnl | ❌ | 3 | 0 |
| `NtWriteFileGather` | xboxkrnl | ❌ | 1 | 0 |
| `ObIsTitleObject` | xboxkrnl | ❌ | 2 | 0 |
| `RtlCaptureContext` | xboxkrnl | ❌ | 1 | 0 |
| `RtlUnwind` | xboxkrnl | ❌ | 2 | 0 |
| `XamShowGamerCardUIForXUID` | xam | ❌ | 1 | 0 |
| `XamShowNuiGamerCardUIForXUID` | xam | ❌ | 1 | 0 |
| `XamXlfsInitializeUploadQueue` | xam | ❌ | 1 | 0 |
| `XamXlfsMountUploadQueueInstance` | xam | ❌ | 1 | 0 |
| `XamXlfsUninitializeUploadQueue` | xam | ❌ | 1 | 0 |
| `XamXlfsUnmountUploadQueueInstance` | xam | ❌ | 1 | 0 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl | ❌ | 1 | 0 |
| `__C_specific_handler` | xboxkrnl | ❌ | 0 | 0 |
| `RtlNtStatusToDosError` | xboxkrnl | ❔ | 18 | 0 |
| `DbgBreakPoint` | xboxkrnl | ⚪ stub | 1 | 0 |
| `HalReturnToFirmware` | xboxkrnl | ⚪ stub | 1 | 0 |
| `IoCreateDevice` | xboxkrnl | ⚪ stub | 1 | 0 |
| `IoDeleteDevice` | xboxkrnl | ⚪ stub | 1 | 0 |
| `KeBugCheckEx` | xboxkrnl | ⚪ stub | 5 | 0 |
| `KiApcNormalRoutineNop` | xboxkrnl | ⚪ stub | 1 | 2 |
| `NtDeviceIoControlFile` | xboxkrnl | ⚪ stub | 3 | 0 |
| `RtlRaiseException` | xboxkrnl | ⚪ stub | 3 | 0 |
| `StfsControlDevice` | xboxkrnl | ⚪ stub | 0 | 0 |
| `StfsCreateDevice` | xboxkrnl | ⚪ stub | 0 | 0 |
| `VdEnableDisableClockGating` | xboxkrnl | ⚪ stub | 1 | 0 |
| `VdGetCurrentDisplayGamma` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdGetCurrentDisplayInformation` | xboxkrnl | ⚪ stub | 3 | 1 |
| `VdGetSystemCommandBuffer` | xboxkrnl | ⚪ stub | 1 | 2929 |
| `VdInitializeEngines` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdQueryVideoFlags` | xboxkrnl | ⚪ stub | 1 | 2 |
| `VdQueryVideoMode` | xboxkrnl | ⚪ stub | 4 | 4 |
| `VdRetrainEDRAM` | xboxkrnl | ⚪ stub | 2 | 2930 |
| `VdRetrainEDRAMWorker` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdSetDisplayMode` | xboxkrnl | ⚪ stub | 1 | 1 |
| `VdSetDisplayModeOverride` | xboxkrnl | ⚪ stub | 2 | 0 |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl | ⚪ stub | 3 | 4 |
| `VdShutdownEngines` | xboxkrnl | ⚪ stub | 2 | 0 |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl | ⚪ stub | 1 | 0 |
| `XGetAVPack` | xam | ⚪ stub | 1 | 1 |
| `XGetGameRegion` | xam | ⚪ stub | 2 | 0 |
| `XamAvatarGenerateMipMaps` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssets` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetAssetsResultSize` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetManifestLocalUser` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarGetMetadataRandom` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarInitialize` | xam | ⚪ stub | 1 | 1 |
| `XamAvatarLoadAnimation` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarManifestGetBodyType` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarSetCustomAsset` | xam | ⚪ stub | 1 | 0 |
| `XamAvatarShutdown` | xam | ⚪ stub | 1 | 1 |
| `XamContentFlush` | xam | ⚪ stub | 1 | 0 |
| `XamContentGetLicenseMask` | xam | ⚪ stub | 1 | 0 |
| `XamShowNuiDirtyDiscErrorUI` | xam | ⚪ stub | 1 | 0 |
| `XamTaskShouldExit` | xam | ⚪ stub | 1 | 0 |
| `XeCryptRandom` | xboxkrnl | ⚪ stub | 1 | 0 |
