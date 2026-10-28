# Motion Explosion! (título 4D4A07E6)

Motion Explosion!, da Majesco (2011; título 4D4A, prefixo da Majesco): coleção de minijogos de movimento num "instituto de pesquisa" (salas `RI_*`, `MindInMotion`, `DailyTest`, minijogos como `TetrisMinigame`), jogados com o avatar do jogador. Projeto interno **LeftBrainRightBrain** (`c:\Projects\LBRB_Branches\…\default.pdb`), motor próprio com dados em `PlatformData\Xbox360` (`.ODB`, `.set`/`.subset`, `.dpk`, efeitos `NeoFX`, som FMOD `.fsb`/`.fev`). Contêiner GOD em `<HD>/Xbox 360/Motion Explosion [PAL][NTSCU][Kinect]/` (7,1 GB no HD, mas só ~340 MB de arquivos), extraído com `xenia-vfs-dump` em `~/Kixenia/kinect-sdk-re/games/extraidos/motionexplosion/`: `default.xex` (7 MB), `PlatformData` (308 MB), `AvatarAssetPack`, `Database.xmplr` e `HeadPosition.xmplr` (rastreador), `NuiIdentity.bin.be` (identidade), os modelos de voz `nuisp1031` … `nuisp3084` (10 idiomas), `nxeart` e `$SystemUpdate` (12 MB).

Runtime NUI **2.0.20764** (`NUI: title runtime 5/511C with flags 0000000B`; XDK de junho de 2011): o mesmo do [Rapala-for-Kinect](rapala-for-kinect.md). **Parece mais com o Rapala**: das 51 funções que chamam o Kinect (fora as mensagens XMsg), 32 são iguais às do Rapala (mesmas chamadas e mesmo tamanho), 30 às do [Kinect-Rush](kinect-rush.md) e do [Kinect-Star-Wars](kinect-star-wars.md), 19 às do [Kinect-Joy-Ride](kinect-joy-ride.md) e 14 às do [Kinect-Adventures](kinect-adventures.md). As que sobram são a biblioteca de avatares e o chat de voz, que o Rapala não usa.

Relacionado: [Rapala-for-Kinect](rapala-for-kinect.md), [Kinect-Rush](kinect-rush.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Sesame-Street-TV](kinect-sesame-street-tv.md), [Kinect-Adventures](kinect-adventures.md), [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Link incremental: por que a análise precisou de dois passos a mais

O executável foi ligado com **link incremental**: a seção `.xidata` (0x82270000, 1,4 MB) não tem dados de importação, e sim **68 526 trampolins** de 16 bytes (`lis r11, alto; addi r11, r11, baixo; mtctr r11; bctr`). Todo `bl` do código vai a um trampolim, inclusive as chamadas às importações (o trampolim leva ao thunk em 0x8238BAF0…0x8238DC9C). Por isso a primeira passada achou **0 chamadas** às importações e o log mostra o motor de identidade em 0x822CB5A0 (é o trampolim; a função fica em 0x82842EF0).

1. `mex_decifra.py`: decifra o XEX (AES com a chave de sessão do security info, aberta pela chave retail do Xenia) para o `xexinfo` (`modulos/bin`) descomprimir o LZX. A imagem do XEXLoaderWV (aviso "out of input bytes") diferia em só 397 bytes da correta; guardada como `motionexplosion_4D4A07E6.ghidra-lzx-ruim.img`.
2. `mex_stubs.py`: resolve os trampolins e grava `motionexplosion_4D4A07E6.desviado.img` com 155 741 `bl`/`b` trocados por chamadas diretas ao destino; `mex_iat.py` acrescenta as 160 chamadas que leem a tabela de importações (`lis`/`lwz` + `bctrl`). Com isso: 2286 chamadas diretas a importações, 51 funções do Kinect.

## Estado no Xenia Edge

> [!NOTE]
> **Só testado com gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema`). Sem captura de tela: o que aparece na tela não foi conferido.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir | ✅ | "Motion Explosion!". 150 s sem travar e **sem `Host-side crash`**. "undefined extern call" só para `XamXlfsInitializeUploadQueue` e `EtxProducerRegister` (nada do Kinect). |
| Runtime NUI | ✅ | `5/511C`, flags 0xB, no quadro 19. `XamNuiGetTrueColorInfo` buscada por ordinal (0x8DF) no quadro 20. |
| Avatares | 🟡 | `XamAvatarInitialize` no quadro 74; o stub do Xenia devolve erro (cvar `allow_avatar_initialization` = false) e o jogo chama `XamAvatarShutdown` na hora. Segue sem avatares; não se sabe o que mostra no lugar. |
| Menu / tela de título | ✅? | Carrega `Sets\Menus\MainMenu_RI.set` (quadro 74), os `.ODB` (82–90), os subconjuntos do menu (132–150) e as fontes do menu (577); depois não abre mais nada. A tela de título (0x82405460, estado 0) espera **`START`** no controle ou o gesto **`WaveAnyHand`** (acenar) pelo Kinect. |
| Tela de solução de problemas | ⚠️ | No quadro 808 o jogo abriu `XamShowNuiTroubleshooterUI(0xFF)`: na tela de título, o botão **`Y`** de um controle abre essa tela (0x828303A0). Foi entrada de controle (do host ou do Xenia), não falha do Kinect; o jogo continuou. |
| Rastreamento | ✅ | 17 de 32 amostras (53%) com o jogador rastreado, a partir do quadro 928 (Sesame Street TV 33%, Joy Ride 36%). |
| Inclinação | ✅ (sem motor) | Pede −25° no quadro 21 (sem chão guardado) e 0° no 80; não recomeça a busca. |
| Ritmo | 🟡 | Só 2580 quadros em 150 s (~17 por segundo; o Sesame Street TV chegou a 4714). |
| Kinect ID (rostos) | ✅ | `XamCacheOpenFile`/`XamCacheReset(0x18)` sem erro e `XamReadBiometricData` do perfil; `XamUserNuiGetEnrollmentIndex` no quadro 932, quando o jogador aparece. |
| Voz e microfones | — | Não importa as funções dos microfones do Kinect (`XamVoiceGetMicArray*`) nem o cancelamento de eco (MEC). Ver abaixo. |
| Fitness | — | Nada: nem importação, nem busca por ordinal, nem mensagens `XamFitness`. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 mex-replay-n3 150 ~/Kixenia/kinect-sdk-re/games/extraidos/motionexplosion/default.xex
```
Também abre direto do contêiner: `".../Motion Explosion [PAL][NTSCU][Kinect]/Content/0000000000000000/4D4A07E6/00007000/28F5002514C25EBED3F1"`.

## O que o jogo pede do Kinect

| Recurso | Usa? | Como |
|---|---|---|
| Câmera (PsCam) | Sim | Um ponto só (0x828305E8), depois do Kinect Studio (`XamXStudioRequest(0x1003)`); 7706 pedidos em 150 s. |
| Motor de inclinação | Sim | O do Rapala/Star Wars/Rush (−25°, degraus até achar o chão; ±27°), callback 0x828486D8, máquina de estados 0x82849600. |
| Status 'XtSs' | Sim | Em **0x834BD660** (0x50 bytes). Campos lidos (`mex_acessos_base.py`): +0x24/+0x28 (gravidade: ângulo atual), +0x2C (flags do motor; 0x10 recomeça a busca), +0x3C (**chão guardado**, float, em 9 funções), +0x40 (bit 0: ocupado), +0x48/+0x4C (gravidade do chão). O mesmo conjunto do Rapala, do Rush e do Sesame Street TV. |
| Esqueleto | Sim | Rastreamento completo, `Database.xmplr` e `HeadPosition.xmplr`; gesto de acenar (`WaveAnyHand`) na tela de título. |
| Identidade (Kinect ID) | Sim | Motor de identidade em 0x82842EF0 (trampolim 0x822CB5A0); lê e grava os rostos do perfil. |
| Avatares | **Sim, é o personagem** | 13 `XamAvatar*` importadas (todas stub no Xenia), `CXBoxAvatarModule`, `CXBoxAvatarCalibrateModule`, telas `ChooseAvatarS1`…`S5`, `AvatarAssetPack`. |
| Microfones e voz | Não pelo Kinect | O motor tem gatilhos de voz (`CVoiceTriggerModule`, `DataRoot:Grammars\…`) e o disco traz `nuisp*`, mas não há a pasta `Grammars` nem chamadas aos microfones do Kinect, e nada de `nuisp` foi aberto em 150 s. O único caminho de microfone é o do chat de voz (XHV): `MicDeviceRequest` (**stub** no Xenia) e `RmcDeviceRequest` (**ausente**), em 0x832E4B90, 0x832E50A8, 0x832E55F0 e 0x832E6050, não chamadas na gravação. |
| Fitness | Não | |

## Teste com o Kinect real: pendente

O que verificar ao vivo (worktree kinect-sistema, `live_test.sh`, na sessão principal):
1. **Tela de título**: se acenar (`WaveAnyHand`) passa da tela de título sem controle. Não apertar `Y` no controle (abre a tela de solução de problemas do Kinect).
2. **Avatares**: como o jogo fica sem avatares (o stub falha); se travar ou ficar sem personagem, testar com `--allow_avatar_initialization=true` (o stub passa a devolver sucesso, mas as outras `XamAvatar*` continuam stub).
3. **Minijogos**: se o rastreamento acompanha os movimentos e a pontuação reage; medir o ritmo (na gravação, ~17 quadros/s).
4. **Motor**: se o sensor inclina (−25° e volta) e guarda o chão uma vez só.
5. **Kinect ID**: se reconhece o jogador cadastrado (`XamUserNuiGetEnrollmentIndex`).
6. Voz: confirmar que nada depende de microfone (nenhuma chamada a `MicDeviceRequest`/`RmcDeviceRequest`).

## Pendências
- `RmcDeviceRequest` ausente e `MicDeviceRequest` stub (chat de voz por headset; não afeta o Kinect enquanto não forem chamadas).
- `XamShowNui*` ausentes importadas: `AchievementsUI`, `CommunitySessionsUI`, `ControllerRequiredUI`, `FriendRequestUI`, `FriendsUI`, `GameInviteUI`, `GamerCardUIForXUID`, `MarketplaceUI`, `PartyUI`; nenhuma chamada em 150 s.
- Ausentes de rede e sistema (`NetDll_Xnp*`, `XamXlfs*`, `Etx*`, `XAudioGetDucker*`): bibliotecas ligadas, sem efeito no Kinect.

## Arquivos da análise
- Imagens: `~/Kixenia/kinect-sdk-re/games/motionexplosion_4D4A07E6.img` (PE decifrado e descomprimido pelo `xexinfo`), `…desviado.img` (trampolins resolvidos), `…ghidra-lzx-ruim.img` (a do XEXLoaderWV); XEX em `games/motionexplosion_4D4A07E6.xex`.
- Projetos Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/mexvmx` (XEXLoaderWV) e `mexbin` (a imagem desviada como binário puro na base 0x82000000), linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática.
- Seções: `.xidata` 0x82270000 (trampolins), `.pdata` 0x82214A00 (36 656 funções), `.text` 0x823C8000–0x8340EE20.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/mex/c_full/<endereço>.c` (55 funções: as 51 do Kinect e as 4 do microfone do chat de voz), `mex/c/extra/0x82405460.c` (tela de título).
- Log da execução: `~/Kixenia/kinect-tools/logs/mex-replay-n3.log`.
- Scripts (`games/decompilado/scripts/`): `mex_decifra.py`, `mex_stubs.py`, `mex_iat.py`, `mex_imports.py`, `mex_funcs.py`, `mex_runtime.py`, `mex_c.py`, `mex_compara.py`, `mex_acessos_base.py`, `gen_motionexplosion.py`; `DumpImage.java` e `FnGraph.java` em `~/Kixenia/kinect-tools/ghidra/`.

## Chamadas ao sistema (análise de 2026-10-26)

**447 importações** (233 do `xam.xex`, 214 do `xboxkrnl.exe`), 2446 chamadas diretas no código, 101 vistas em execução. Estado no Xenia Edge (worktree kinect-sistema): 280 implementadas, 31 aproximadas, 58 stubs, 77 ausentes, 1 sem marca.

**Como foi feito:** igual ao [Kinect-Rush](kinect-rush.md), com dois passos a mais (ver *Link incremental* acima): imagem decifrada e descomprimida (`mex_decifra.py` + `xexinfo`) com os trampolins do `.xidata` resolvidos (`mex_stubs.py`) e as chamadas pela tabela de importações (`mex_iat.py`); importações e thunks da listagem do log do Xenia; chamadas = `bl`/`b` para cada thunk; a função que contém cada chamada vem da `.pdata`; decompilação pelo Ghidra 12.1.4 (VMX128). Colunas *Rapala* e *Rush*: `=` mesma função (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho.

### Funções do jogo que falam com o Kinect

| Função | Tamanho | Chama | Chamada por | O que faz | Rapala | Rush |
|---|---|---|---|---|---|---|
| 0x827A4460 | 56 | `XamShowNuiMessageBoxUI` | 0 |  | = 0x822a5110 | — |
| 0x827A44B8 | 28 | `XamShowNuiDirtyDiscErrorUI` | 0 |  | = 0x822a5148 | — |
| 0x827B1650 | 248 | `XamAvatarGetAssets` | 1 |  | = 0x8247fcc8 | — |
| 0x827B1888 | 172 | `XamAvatarGenerateMipMaps` | 0 |  | = 0x8247fdc0 | — |
| 0x827B1998 | 124 | `XamAvatarShutdown` | 1 |  | = 0x8247fa80 | — |
| 0x827B1A30 | 372 | `XamAvatarInitialize` | 1 | Biblioteca de avatares do XDK: `XamAvatarInitialize` (sistema de coordenadas 4, processador 5, 3 MB de memória). No Xenia o stub devolve erro e o jogo chama `XamAvatarShutdown` em seguida. | = 0x8247fb00 | — |
| 0x827B1BF0 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | = 0x8247ecd8 | — |
| 0x827B1CA8 | 176 | `XamAvatarGetManifestsByXuid` | 0 |  | — | — |
| 0x827B1D80 | 168 | `XamAvatarGetMetadataRandom` | 1 |  | = 0x8247ed70 | — |
| 0x827B1E50 | 80 | `XamAvatarGetMetadataSignedOutProfileCount` | 0 |  | — | — |
| 0x827B1EB0 | 144 | `XamAvatarGetMetadataSignedOutProfile` | 0 |  | — | — |
| 0x827B28A8 | 448 | `XamAvatarManifestGetBodyType` | 0 |  | — | — |
| 0x827B4660 | 220 | `XamAvatarLoadAnimation` | 1 |  | = 0x8247f580 | — |
| 0x827B7058 | 532 | `XamAvatarManifestGetBodyType` | 2 |  | — | — |
| 0x827C2688 | 1888 | `XamAvatarGetAssets`, `XamAvatarManifestGetBodyType` | 1 |  | — | — |
| 0x827C2F68 | 744 | `XamAvatarGenerateMipMaps` | 1 |  | ≈ 0x8247fdc0 (172) | — |
| 0x828305E8 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta `XamXStudioRequest(0x1003)` (Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | = 0x82491f18 | = 0x82950a60 |
| 0x82840530 | 496 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | = 0x824a0f88 | ≈ 0x8295b5d8 (540) |
| 0x82840788 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x824a1178 | = 0x8295b7f8 |
| 0x82842D40 | 356 | `XamUserNuiGetUserIndex` | 1 |  | = 0x824a30f0 | = 0x8295d788 |
| 0x82844370 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x824a3ed8 | = 0x8295e570 |
| 0x82844548 | 3200 | `XamNuiIdentityGetSessionId` | 5 |  | = 0x824a4060 | = 0x8295e6f8 |
| 0x828473B0 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 |  | = 0x824a6d68 | = 0x82963668 |
| 0x828476F0 | 92 | `XamNuiCameraElevationGetAngle` | 0 | Função pública `NuiCameraElevationGetAngle` do runtime (como no Sesame Street TV). | ≈ 0x824a76b0 (676) | ≈ 0x82963fb0 (676) |
| 0x82847760 | 132 | `XamNuiCameraElevationSetAngle` | 3 |  | = 0x824a7018 | = 0x82963918 |
| 0x828478C8 | 144 | `XamNuiCameraElevationStopMovement` | 0 | Função pública `NuiCameraElevationStopMovement`: recusa com o bit 0 de +0x40 (ocupado) ou com um pedido em andamento. | ≈ 0x824a8560 (2436) | ≈ 0x82964ef8 (2436) |
| 0x82847A18 | 76 | `XamNuiCameraTiltGetStatus` | 0 | Lê o status 'XtSs' (0x834BD660): tenta `XamXStudioRequest(0x1005)` e só se falhar chama `XamNuiCameraTiltGetStatus`. | = 0x824a70c0 | = 0x829639c0 |
| 0x82847E78 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: inclina para **−25°** sem chão guardado (+0x3C = 0) ou com a flag 0x10. | = 0x824a7448 | = 0x82963d48 |
| 0x82847F28 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | Guarda o chão do quadro de esqueleto (`XamNuiCameraRememberFloor`) e informa o status. | = 0x824a74d8 | = 0x82963dd8 |
| 0x82848160 | 676 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático**: lê o ângulo, confere +0x2C (flag 0x10) e +0x40 (bit 0 = ocupado) e inclina de novo ou começa a busca. | = 0x824a76b0 | = 0x82963fb0 |
| 0x828486D8 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam; responde com o status. | = 0x824a7958 | = 0x82964258 |
| 0x82848B58 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Zera o estado do motor, registra o callback 0x828486D8 (`XamNuiCameraTiltSetCallback`) e grava o ponteiro do leitor do 'XtSs'. | = 0x824a7d18 | = 0x82964618 |
| 0x82849600 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto (a do Rapala, do Star Wars e do Rush); preenche o cabeçalho do 'XtSs'. | = 0x824a8560 | = 0x82964ef8 |
| 0x82855180 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x824af878 | = 0x8296cea0 |
| 0x8285A8E0 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Pergunta ao sistema qual cadastro de rosto é o do jogador (`XamUserNuiGetEnrollmentIndex`). | = 0x824b3c20 | = 0x82971270 |
| 0x8285ABA0 | 336 | `XamUserNuiGetUserIndex` | 1 |  | = 0x824b3e68 | = 0x829714b8 |
| 0x8285AEA8 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Limpeza do cache do Kinect ID ao iniciar o motor de identidade: `XamCacheOpenFile`, `XamCacheReset(0x18)`, `XamCacheCloseFile`. | = 0x824b40e8 | = 0x82971738 |
| 0x8285AFD8 | 488 | `XamReadBiometricData` | 1 |  | = 0x824b41e0 | = 0x82971830 |
| 0x8285B3B8 | 708 | `XamReadBiometricData` | 0 |  | ≈ 0x824b41e0 (488) | ≈ 0x82971830 (488) |
| 0x8285BB00 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Grava os rostos (`XamWriteBiometricData`) e liga a biometria do perfil (`XamUserNuiEnableBiometric`). | = 0x824b4850 | = 0x82971e48 |
| 0x83127DA0 | 40 | `XamNuiGetDeviceStatus` | 0 | Devolve as flags do estado do sensor (`XamNuiGetDeviceStatus`, +0xC). | = 0x822a51e8 | = 0x829473f0 |
| 0x831293E8 | 68 | `XamShowVoiceMailUI` | 0 |  | — | — |
| 0x831312D0 | 376 | `XAudioGetVoiceCategoryVolume` | 0 |  | = 0x8234b5a0 | = 0x82d474c0 |
| 0x832CCEF0 | 200 | `XamVoiceIsActiveProcess` | 3 | Chat de voz (XHV): confere se o processo de voz está ativo. | — | = 0x82e75f90 |
| 0x832CEA80 | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` | 1 |  | — | = 0x82e77228 |
| 0x832CF208 | 316 | `XamVoiceClose` | 2 |  | — | = 0x82e77868 |
| 0x832CF658 | 600 | `XamVoiceSubmitPacket` | 1 |  | — | = 0x82e77bf8 |
| 0x832CFA68 | 240 | `XamVoiceClose` | 2 |  | — | = 0x82e77f58 |
| 0x832CFB88 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 |  | — | = 0x82e78048 |
| 0x832D30F0 | 688 | `XamVoiceIsActiveProcess` | 3 |  | — | = 0x82e7aed0 |
| 0x832D8AD0 | 208 | `XamVoiceSubmitPacket` | 2 |  | — | = 0x82e7d2f8 |

### Importações do Kinect, NUI, voz e mensagens

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 7706 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 5 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 25 | 2051 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 53 | 6 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 3 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestsByXuid` | xam 0x5DF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfileCount` | xam 0x5EE | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 4 | 0 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | — (alta freq.) | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | — (alta freq.) | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | — (alta freq.) | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 3 | ✅ |
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
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 1 | ✅ |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 1 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Rede</b> (95)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` ⭐ | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` ⭐ | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` ⭐ | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` ⭐ | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` ⭐ | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASend` ⭐ | xam 0x17 | 1 | 0 | ❌ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` ⭐ | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` ⭐ | xam 0x21 | 1 | 0 | ✅ |
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
| `NetDll_getpeername` ⭐ | xam 0xA | 1 | 0 | ✅ |
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
| `XNetLogonGetMachineID` ⭐ | xam 0x135 | 3 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 7 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (67)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 11 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 4 | 0 | ✅ |
| `ExSemaphoreObjectType` ⭐ | xboxkrnl 0x17 | 1 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 1 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 9 | 0 | ✅ |
| `ExTimerObjectType` ⭐ | xboxkrnl 0x1C | 1 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 14 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 10 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 11 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 38 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 36 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 105 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 1 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 8 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 6 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 4 | 86 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 7903 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 23 | 12313 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 1933 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 1933 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 11 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 4 | 7 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` ⭐ | xboxkrnl 0x9C | 1 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 92 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 1 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 4 | 6 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 3 | 4 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 12 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 17 | 16 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 2 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 7 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 25 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 42 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 45 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 2483 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 9 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` ⭐ | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 6 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 325 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 33 | 77 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 1 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 404 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 83 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 5 | 0 | ✅ |
| `XexUnloadImageAndExitThread` ⭐ | xboxkrnl 0x1A2 | 1 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (54)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscGetCacheElementCount` ⭐ | xboxkrnl 0x20 | 1 | 0 | ⚪ stub |
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 13 | 550 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 3 | 8 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 3 | 98 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 2 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 5 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 8 | 405 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 149 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 63 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 95 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 4 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 10 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 4 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 2 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 47 | 12581 | ✅ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByName` ⭐ | xboxkrnl 0x10D | 3 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 2 | 3 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 17 | 12581 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 4 | 2 | ✅ |
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
| `XamContentOpenFile` | xam 0x269 | 7 | 4 | 🟡 |
| `XamContentResolve` ⭐ | xam 0x268 | 2 | 0 | 🟡 |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (38)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 14 | 0 | ✅ |
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
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 4 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 2 | 0 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 1 | ✅ |
| `XamUserGetMembershipTierFromXUID` ⭐ | xam 0x217 | 2 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` ⭐ | xam 0x218 | 2 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 108 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 9 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 0 | ✅ |
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
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 2595 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 45 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 42 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 1 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 1 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 1 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 1 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 1 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 1 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 1 | 🟡 |
| `VdQuerySystemCommandBuffer` ⭐ | xboxkrnl 0x1C8 | 1 | 0 | ❌ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 2 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 4 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 2596 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBuffer` ⭐ | xboxkrnl 0x1D8 | 1 | 0 | ❌ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 5 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 3 | 0 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 22 | 678 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 1 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 1 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 3 | 0 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 3 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 10 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 3 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 7 | 1935 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 20 | 12723 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 1 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 3 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 4 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 1 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 5 | 3 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 5 | 9 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 2 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 8 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 33 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 10 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 2 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 21 | 566 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 23 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 4 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 2 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 2 | 0 | ❌ |
| `XamReadString` ⭐ | xam 0x2F6 | 1 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 3 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 6 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 9 | 0 | ✅ |
| `vsprintf` ⭐ | xboxkrnl 0x14E | 2 | 0 | ✅ |

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
| `XamShowNuiMessageBoxUI` | xam 0x477 | 1 | 0 | 🟡 |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 1 | ✅ |

</details>

<details><summary><b>Memória</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 4 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 1437 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 6 | 85 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 18 | 198 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 5 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 14 | 1277 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 11 | 1092 | ✅ |
| `NtProtectVirtualMemory` ⭐ | xboxkrnl 0xE1 | 2 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 21 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 24 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptAesEcb` ⭐ | xboxkrnl 0x15A | 1 | 0 | ✅ |
| `XeCryptAesKey` ⭐ | xboxkrnl 0x159 | 1 | 0 | ✅ |
| `XeCryptBnQwNeRsaKeyGen` ⭐ | xboxkrnl 0x16B | 1 | 0 | ❌ |
| `XeCryptBnQwNeRsaPrvCrypt` ⭐ | xboxkrnl 0x16C | 1 | 0 | ❌ |
| `XeCryptBnQwNeRsaPubCrypt` ⭐ | xboxkrnl 0x16D | 1 | 0 | ✅ |
| `XeCryptBnQw_SwapDwQwLeBe` ⭐ | xboxkrnl 0x170 | 21 | 0 | ✅ |
| `XeCryptRandom` | xboxkrnl 0x18A | 10 | 0 | ⚪ stub |
| `XeCryptRc4Ecb` ⭐ | xboxkrnl 0x18C | 1 | 0 | ✅ |
| `XeCryptRc4Key` ⭐ | xboxkrnl 0x18B | 1 | 0 | ✅ |
| `XeCryptShaFinal` ⭐ | xboxkrnl 0x191 | 1 | 0 | ✅ |
| `XeCryptShaInit` ⭐ | xboxkrnl 0x18F | 1 | 0 | ✅ |
| `XeCryptShaUpdate` ⭐ | xboxkrnl 0x190 | 1 | 0 | ✅ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |
| `XeKeysGetKey` ⭐ | xboxkrnl 0x244 | 1 | 0 | 🟡 |
| `XeKeysGetKeyProperties` ⭐ | xboxkrnl 0x241 | 1 | 0 | ✅ |
| `XeKeysQwNeRsaPrvCrypt` ⭐ | xboxkrnl 0x248 | 1 | 0 | ❌ |

</details>

<details><summary><b>Avatares</b> (13)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestsByXuid` ⭐ | xam 0x5DF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfileCount` | xam 0x5EE | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 4 | 0 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |

</details>

<details><summary><b>Áudio</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 0 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 7706 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 3 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 1 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MicDeviceRequest` ⭐ | xboxkrnl 0x2B1 | 4 | 0 | ⚪ stub |
| `RmcDeviceRequest` ⭐ | xboxkrnl 0x2E1 | 5 | 0 | ❌ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamShowVoiceMailUI` ⭐ | xam 0x2C3 | 1 | 0 | ❌ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 3 | 0 | ✅ |
| `XNotifyPositionUI` | xam 0x28C | 1 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 3 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 3 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 2 | ✅ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 5 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 25 | 2051 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 53 | 6 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 3 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 553 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 2 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 2 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 2 | 1 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 3 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 3 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 3 | 1 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

⭐ = nenhum dos jogos comparados (Adventures, Joy Ride, Rush, Star Wars, Rapala) importa.

**Ausentes no Xenia (77):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetInAddrToServer`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetServerToInAddr`, `NetDll_XNetSetOpt`, `NetDll_XNetTsAddrToInAddr`, `NetDll_XnpCapture`, `NetDll_XnpConfig`, `NetDll_XnpConfigUPnP`, `NetDll_XnpEthernetInterceptRecv`, `NetDll_XnpEthernetInterceptSetCallbacks`, `NetDll_XnpEthernetInterceptXmit`, `NetDll_XnpGetActiveSocketList`, `NetDll_XnpGetConfigStatus`, `NetDll_XnpGetVlanXboxName`, `NetDll_XnpLoadConfigParams`, `NetDll_XnpLoadMachineAccount`, `NetDll_XnpLogonClearQEvent`, `NetDll_XnpLogonGetQFlags`, `NetDll_XnpLogonGetQVals`, `NetDll_XnpLogonGetStatus`, `NetDll_XnpLogonSetPState`, `NetDll_XnpLogonSetQEvent`, `NetDll_XnpLogonSetQFlags`, `NetDll_XnpLogonSetQVals`, `NetDll_XnpNoteSystemTime`, `NetDll_XnpRegisterKeyForCallerType`, `NetDll_XnpSaveConfigParams`, `NetDll_XnpSaveMachineAccount`, `NetDll_XnpSetVlanXboxName`, `NetDll_XnpUnregisterKeyForCallerType`, `NtWriteFileGather`, `RmcDeviceRequest`, `RtlCaptureContext`, `RtlUnwind`, `VdQuerySystemCommandBuffer`, `VdSetSystemCommandBuffer`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XamContentInstall`, `XamContentLaunchImage`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowNuiAchievementsUI`, `XamShowNuiCommunitySessionsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendRequestUI`, `XamShowNuiFriendsUI`, `XamShowNuiGameInviteUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamShowNuiPartyUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamShowQuickChatUI`, `XamShowVoiceMailUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeCryptBnQwNeRsaKeyGen`, `XeCryptBnQwNeRsaPrvCrypt`, `XeKeysQwNeRsaPrvCrypt`, `XexUnloadImageAndExitThread`, `__C_specific_handler`.
