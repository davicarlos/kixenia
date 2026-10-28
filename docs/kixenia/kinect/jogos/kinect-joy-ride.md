# Kinect Joy Ride (título 4D53093B)

Kinect Joy Ride, da BigPark (Microsoft Game Studios), 2010: corrida com volante imaginário. Executável `default.xex` (8,4 MB, PE original `wheelsf.exe`, compilado em 10/09/2010) em `<HD>/Xbox 360/Kinect Joy Ride/` (HD externo; antes em `~/Downloads/Xbox360_extraido/`), com os pacotes `.bpk` (pistas e carros), `Database.xmplr` (modelos do rastreador), `NuiIdentity.bin.be` (identidade), `AvatarAssetPack` e `AvatarAwards`. Runtime NUI **2.0.11775.6** (o Adventures tem 2.0.11775.7; Nike e Kinect Sports, 2.0.21173.0) e a biblioteca de fala **NUISP 2.0.11775.4**, que o Adventures não tem.

Relacionado: [Kinect-Adventures](kinect-adventures.md), [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Sports](kinect-sports.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!TIP]
> **Funciona com o Kinect real (testes ao vivo de 2026-10-26 e 2026-10-27, worktree kinect-sistema, com as correções do 'XtSs' e do XamCache): menus pelo cursor de mão, direção e jogo. As linhas sem "ao vivo" vêm da gravação `kinect_rec_aovivo2` (150 s, log nível 3). **Análise fechada em 2026-10-27.****

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir e rodar | ✅ | 150 s sem travar nem derrubar o emulador. |
| Rastreamento | ✅ ao vivo | Ao vivo, 68 s seguidos com o jogador TRACKED (um id só) e as mãos rastreadas em 93–94% dos quadros. Em 2026-10-27, 150 de 200 amostras TRACKED (75%; as outras são de antes de o jogador se posicionar), parado a 2,15 m e 0,3 m à esquerda (log `joyride-rastreio.log`, profundidade gravada em `~/kinect_rec_joy`). Na gravação, 36% das amostras (Adventures 27%, Kinect Sports 55%). |
| Cursor de mão dos menus | ✅ ao vivo | Funciona. Os quadros de esqueleto chegam lentos (mediana de 87 ms com o jogador rastreado, até 2 s): ver *Cursor de mão dos menus* abaixo. |
| Direção (volante) | ✅ ao vivo | Confirmada pelo usuário em 2026-10-27: o carro responde ao volante imaginário. |
| Inclinação | ✅ ao vivo | Com o motor real: −25° e depois 1° ao abrir, e o motor fica parado durante todo o jogo; não repete a varredura como o Adventures. O motor de inclinação é o do Adventures. |
| Kinect Tuner | ✅ ao vivo | Aberto pelo menu do botão central do controle durante o jogo (2026-10-27): imagens, esqueleto e inclinação manual (1° a 17°, um grau por vez) funcionam. |
| Microfones e cancelamento de eco | ❔ | O jogo usa (ver abaixo), mas só liga ao ver o bit 1 (microfones) nas flags do `XamNuiGetDeviceStatus` ou na notificação 0x60019; com a gravação não há áudio e nenhuma função de microfone foi chamada. Falta testar com o Kinect real. |
| Kinect ID (rostos) | ✅ corrigido | O cache só é usado numa limpeza ao iniciar o motor de identidade; `XamCacheOpenFile`/`XamCacheReset`/`XamCacheCloseFile` agora existem no Xenia (ver *Cache do sistema* abaixo). Testado com a gravação no Joy Ride e no Nike. |
| Telas do Guia Kinect | 🟡 | 5 das 10 `XamShowNui*` ausentes: `CommunitySessionsUI`, `FriendsUI`, `GamerCardUIForXUID`, `MarketplaceUI`, `PartyUI`. |
| Avatares | ⚪ | Toda a API de avatares (`XamAvatar*`, 8 funções) é stub no Xenia. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 joy-replay-n3 150 "<HD>/Xbox 360/Kinect Joy Ride/default.xex"
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema ./live_test.sh <log> "<mesmo xex>"
```

## O que o jogo pede do sistema

### Motor de inclinação e o status 'XtSs'
O runtime NUI é o mesmo do Adventures: das 39 funções do Kinect fora das mensagens, 27 têm o mesmo tamanho e as mesmas chamadas (coluna *No Adventures*). O status 'XtSs' (`XamNuiCameraTiltGetStatus`, 0x50 bytes) fica em 0x83790D30. Campos lidos, pelos nomes do `X_NUI_TILT_STATUS` do Xenia:

| Campo | Nome no Xenia | Quem lê | Uso |
|---|---|---|---|
| +0x20 | `gravity[3]` | 0x82CF8818 | Vetor da gravidade (acelerômetro filtrado). |
| +0x24, +0x28 | `gravity[1]`, `gravity[2]` | 0x82CF90A8 | Ângulo atual na máquina de estados. |
| +0x2C | `motor_state` | 0x82CF7FF8, 0x82CF8088, 0x82CF8260, 0x82CF90A8 | Bits 0–1 (motor andando) e 0x10 (chão esquecido: recomeça a busca). |
| +0x3C | `floor_distance` | 6 funções do motor | **Chão guardado**: 0 = sem chão (começa a busca); também vai no status informado ao sistema. |
| +0x40 | `unknown_40` | 0x82CF8260 | Bit 0 ligado: o pedido de ajuste automático é abandonado. |
| +0x48, +0x4C | `floor_gravity[1]`, `floor_gravity[2]` | 0x82CF7A78 | **Gravidade do chão**: `atan2(+0x4C, +0x48)` dá o ângulo do sensor quando o chão foi guardado, ao fim de cada movimento do motor. |

Não lê +0x38 (`floor_normal_z`) nem +0x44 (`floor_gravity[0]`). Ou seja, depende dos campos que o Xenia passou a preencher na correção do 'XtSs' (chão guardado e gravidade do chão).

### Microfones e cancelamento de eco (o que o Adventures não tem)
O Joy Ride liga os microfones do Kinect com o **cancelamento de eco multicanal (MEC)** do próprio jogo, para os comandos de voz:

1. `0x82D12EE8` lê as flags do `XamNuiGetDeviceStatus` e as passa a `0x82D12DF8`, que também recebe a notificação **0x60019** (`kXNotificationSystemNUIHardwareStatusChanged`). Com o bit 1 (microfones), liga; só o bit 0 dá 0x80070015 (não pronto); nenhum, 0x8007048F (não conectado).
2. `0x82D29AA0` abre `MicArrayCalibrate:\MicArrayCalibrate.mec`, a **calibração de áudio feita no painel** (conteúdo do sistema `MicArrayCalibrate`, via `XamContentCreateInternal`), inverte os bytes e carrega os coeficientes; abre dois fluxos `XamVoiceGetMicArrayAudioEx` e registra o callback de áudio no driver de som (`XAudioRegisterRenderDriverMECClient`).
3. `0x82D28F20`, a cada quadro do driver de som, troca entre os dois buffers do microfone e captura o que sai dos alto-falantes (`XAudioCaptureRenderDriverFrame`) para subtrair o eco.
4. Também pega `XamVoiceSetMicArrayIdleUsers` **por ordinal em execução** (`GetProcAddressByOrdinal`, fora da tabela de importações) e o chama com 0, 1, 3 e 7.

No Xenia: `XamNuiGetDeviceStatus` só põe o bit 1 com o áudio do Kinect presente (não há na gravação); a notificação 0x60019 só é enviada em `XamNuiSetForceDeviceOff`. Falta conferir se o Xenia tem o arquivo `MicArrayCalibrate.mec` e o que acontece sem ele.

### Cache do sistema (Kinect ID) — corrigido em 2026-10-26
O motor de identidade usa o cache do sistema só uma vez, ao começar (thread 0x82CF7430): para cada arquivo de identidade em uso, monta uma entrada de 0x248 bytes (tipo **0x18**, chave de 64 bits em +0x10) e chama `XamCacheOpenFile` para segurá-lo; depois `XamCacheReset(0x18)` apaga o resto do cache e `XamCacheCloseFile` fecha o que abriu (0x82CF6128). Logo em seguida lê os rostos com `XamReadBiometricData`, que não passa pelo cache. Os quatro jogos Kinect (Nike, Kinect Sports, Adventures e Joy Ride) importam só essas três funções.

No xam do sistema (17559): `XamCacheOpenFile` em 0x816794A0 devolve um erro Win32 (2 se o arquivo não está no cache, 0xF se nenhuma partição aceita um novo, 0x57 com tipo inválido; tipos válidos em 0x81675BD0), `XamCacheCloseFile` em 0x816794D0, `XamCacheReset` em 0x816794E8 (devolve TRUE). Antes, no Xenia, as três caíam em "undefined extern call" e a limpeza não acontecia, sem outro efeito.

**Correção** (`xam_content.cc`, worktree kinect-sistema, não commitada): um cache sempre vazio. `XamCacheOpenFile` devolve 2 (ou 0xF ao criar), `XamCacheReset` devolve TRUE para um tipo válido, `XamCacheCloseFile` só invalida o handle. Com a gravação, o Joy Ride chama `XamCacheOpenFile` duas vezes (os dois arquivos de identidade em uso) e `XamCacheReset(0x18)`, e segue para `XamReadBiometricData`, sem erros; o Nike faz o mesmo com um arquivo.

```c
dword_result_t XamCacheOpenFile_entry(pointer_t<X_XCACHE_FILE> file) {
  if (!file || !IsCacheType(file->type)) {
    return X_ERROR_INVALID_PARAMETER;
  }
  file->handle = X_INVALID_HANDLE_VALUE;
  file->path[0] = '\0';
  constexpr X_RESULT kErrorInvalidDrive = X_RESULT_FROM_WIN32(0x0000000FL);
  return (file->flags & 1) ? kErrorInvalidDrive : X_ERROR_FILE_NOT_FOUND;
}
```

### Cursor de mão dos menus (por que a mão não é reconhecida)
Tudo acontece **dentro do jogo**, na biblioteca Kinect da BigPark (`bpnatallib`) sobre o runtime NUI do título: nenhuma chamada ao sistema decide se a mão aparece. O Xenia só entrega as imagens da câmera (`PsCamDeviceRequest`); o rastreador de esqueleto do runtime roda na GPU do jogo e publica os quadros (mensagem 0x2B002, que o Xenia só observa).

1. **Jogadores** (`DefaultTrackingPolicy`, 0x827E26F8): até 2 esqueletos para rastreamento completo, primeiro os já TRACKED, depois os POSITION_ONLY.
2. **Estado do jogador** (`SkeletonExtension`, 0x827E8AF8): para cada slot com esqueleto, copia o estado do esqueleto (+0x4D8) para +0xB8; sem esqueleto, 0.
3. **Cursor** (`FECursorLayer`, vtable 0x82033780; a cada quadro 0x826B3960): só com o estado **2 = NUI_SKELETON_TRACKED**; em qualquer outro quadro **zera o cursor e os filtros** (0x826ABA68, 0x826ABDA8). O cursor é angular (0x826ABEE0): direção ombro → mão, com o ombro = média das juntas 2, 4 e 8 e o quadril = média de 0, 12 e 16; limites ±38° na horizontal e ±27° na vertical, raio central = 0,8 × ombro–quadril, troca de mão com histerese de 0,1. Não confere o estado de cada junta.
4. **Refinamento da mão** (`HandRefinementLayer`, vtable 0x8203409C; 0x826BD040): também só com o estado 2. Para as juntas 7 e 11 (mãos), lê a imagem de profundidade numa caixa de ±12 cm em volta da junta, aceita os pixels a ±15 cm da profundidade dela (em mm) e, com mais de 3, usa a média; senão fica com a junta. Depois, uma mola semi-implícita (`SpringK` 800) presa ao braço (5 cm, ou 10 cm estendido).

**Conclusão:** a mão só existe para o jogo enquanto o esqueleto do jogador fica **TRACKED quadro a quadro**; cada quadro sem isso apaga o cursor e recomeça os filtros. Ao vivo (log `--kinect_log_hands`, resumido por `kinect-tools/resumo_maos.py`) isso se cumpriu: 68 s TRACKED seguidos, um id, mãos rastreadas em 93–94% dos quadros, até 70 cm à frente dos ombros; só 6 quadros TRACKED vieram com as juntas zeradas. O cursor funcionou. O que destoa do console é o ritmo: o runtime publica os quadros a cerca de 11 por segundo com o jogador rastreado (mediana de 87 ms, 10% acima de 126 ms, buracos de até 2 s), contra 30 no console.

## Pendências
- Ritmo dos quadros de esqueleto: cerca de 11 por segundo com o jogador rastreado (30 no console). Medir com `--kinect_log_hands=true` e `kinect-tools/resumo_maos.py`.
- Conferir no log ao vivo os microfones e o MEC (`XamVoiceGetMicArrayAudioEx`, `XAudioRegisterRenderDriverMECClient`) e o motor de inclinação.
- `MicArrayCalibrate.mec`: o Xenia monta esse conteúdo do sistema? (o jogo segue sem ele, mas sem os coeficientes do MEC).
- As 5 telas `XamShowNui*` ausentes (o jogo só as abre por botões do Guia Kinect).
- `EtxProducerRegister` e `XamXlfsInitializeUploadQueue`: chamadas, não afetam o Kinect.
- Avatares (stubs).

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/joyride.img` (Ghidra XEXLoaderWV, `DumpImage.java`). O XEX usa compressão LZX; o loader avisa "out of input bytes", mas `.text` e `.pdata` saem completos.
- Projeto Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/joyvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática) e as cópias `joypar0`–`joypar5` da decompilação paralela.
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/joy/c_full/<endereço>.c` (195 funções: as que chamam o Kinect e quem as chama), com os thunks trocados pelos nomes das importações; `joy/c/extra/` tem os chamadores do início dos microfones.
- Log da execução: `~/Kixenia/kinect-tools/logs/joy-replay-n3.log`.
- Scripts: `joy_imports.py`, `joy_runtime.py`, `joy_funcs.py`, `joy_c.py`, `gen_joyride.py` (`games/decompilado/scripts/`), `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/joy/`.**

**332 importações** (145 do `xam.xex`, 187 do `xboxkrnl.exe`), 6566 chamadas diretas no código, 110 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 243 implementadas, 27 aproximadas, 42 stubs, 19 ausentes, 1 sem marca.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata` (0x822D8200, 62723 funções); as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 em 6 processos (✅ = decompilada inteira). *No Adventures*: `=` mesma função (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho, `—` não existe lá.

### Funções do jogo que falam com o Kinect

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x82CE47F8 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta `XamXStudioRequest(0x1003)` (Kinect Studio do kit de desenvolvimento) e só se falhar chama `PsCamDeviceRequest`. | = 0x82D1F408 | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x82CF7958 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: para o motor (`ElevationStopMovement`) e informa o status ao sistema (0x2B006/0x2B00C). | = 0x82D2B3C8 | ✅ |
| 0x82CF7C08 | 112 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo entre −27° e +27° (`ElevationSetAngle`); fora disso, E_INVALIDARG. | = 0x82D2B698 | ✅ |
| 0x82CF7FF8 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: estado 2, espera de 60 quadros e inclina para **−25°**, se o status tem a flag 0x10 (chão esquecido) ou não há chão guardado (+0x3C = 0). | = 0x82D2BA88 | ✅ |
| 0x82CF8088 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: pega o plano do chão do quadro de esqueleto e o guarda no sistema (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis. | = 0x82D2BB18 | ✅ |
| 0x82CF8260 | 680 | `XamNuiCameraElevationGetAngle` | 1 | **Pedido de ajuste automático** (distância, altura e flags): confere o ângulo e, se preciso, inclina de novo ou começa a busca. Igual ao do Adventures. | = 0x82D2BCF0 | ✅ |
| 0x82CF8508 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (AutoTilt, StopMovement, ReverseAutoTilt, AdjustTilt); responde com o status. | = 0x82D2C180 | ✅ |
| 0x82CF8818 | 144 | `XamNuiCameraTiltGetStatus` | 2 | Lê o status do motor e do acelerômetro (`TiltGetStatus`, 'XtSs', 0x50 bytes, em 0x83790D30) e converte a gravidade (+0x20) em vetor. | = 0x82D2C490 | ✅ |
| 0x82CF88A8 | 80 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback 0x82CF8508 (`XamNuiCameraTiltSetCallback`). | = 0x82D2C520 | ✅ |
| 0x82CF90A8 | 2412 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: a mesma do Adventures (busca −25° → −17° → −7° → 3°, até 16 tentativas, mira a cabeça do jogador). Única diferença: ao escolher entre os dois pontos girados, compara a coordenada y em vez de dois `atan2`. | ≈ 0x82D2CD20 (2456) | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x82CF2548 | 392 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | = 0x82D2F5C8 | ✅ |
| 0x82CF26D0 | 340 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x82D2F750 | ✅ |
| 0x82CF35D0 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 |  | = 0x82D30650 | ✅ |
| 0x82CF3D98 | 300 | `XamUserNuiGetUserIndex` | 0 |  | = 0x82D30E18 | ✅ |
| 0x82CF48B8 | 568 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 |  | = 0x82D31938 | ✅ |
| 0x82CF4AF0 | 3868 | `XamNuiIdentityGetSessionId` | 7 |  | = 0x82D31B70 | ✅ |
| 0x82CF6128 | 244 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 |  | = 0x82D331A8 | ✅ |
| 0x82CF6220 | 288 | `XamReadBiometricData` | 1 |  | = 0x82D332A0 | ✅ |
| 0x82CF6E08 | 672 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x82D33E88 | ✅ |
| 0x82CFF580 | 976 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x82D3A4F0 | ✅ |

#### Microfones, cancelamento de eco e estado do sensor

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x82D13E30 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` | 2 | Na primeira consulta, se o microfone não está no estado 2, lê o número de série do sensor (`XamNuiGetDeviceSerialNumber`, 17 bytes). | — | ✅ |
| 0x82D13EF8 | 124 | `XamNuiGetDeviceStatus` | 1 | A cada 60 s, lê o estado do sensor e guarda o bit 3 das flags (superaquecimento). | ≈ 0x831AE010 (40) | ✅ |
| 0x82D28F20 | 1316 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | **Callback de áudio do MEC** (cancelamento de eco), a cada quadro do driver de som: confere o underrun do microfone, troca entre dois buffers de `XamVoiceGetMicArrayAudioEx` (pingue-pongue) e captura o que sai dos alto-falantes (`XAudioCaptureRenderDriverFrame`). | — | ✅ |
| 0x82D29828 | 632 | `XAudioUnregisterRenderDriverMECClient` | 3 | Desliga o MEC: tira o cliente do driver de som (`XAudioUnregisterRenderDriverMECClient`) e libera os buffers. | — | ✅ |
| 0x82D29AA0 | 1212 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 1 | **Liga os microfones e o MEC.** Abre `MicArrayCalibrate:\MicArrayCalibrate.mec` (a calibração de áudio do painel, conteúdo do sistema) e carrega os coeficientes; aloca os buffers; abre dois fluxos `XamVoiceGetMicArrayAudioEx`; registra o callback 0x82D28F20 (`XAudioRegisterRenderDriverMECClient`). | — | ✅ |

#### Avatares

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x82CC3C30 | 124 | `XamAvatarShutdown` | 2 |  | ≈ 0x82BFFEF8 (320) | ✅ |
| 0x82CC3CB0 | 372 | `XamAvatarInitialize` | 1 |  | ≈ 0x82BFFD50 (424) | ✅ |
| 0x82CC3E28 | 152 | `XamAvatarGetManifestLocalUser` | 1 |  | — | ✅ |
| 0x82CC3EC0 | 168 | `XamAvatarGetMetadataRandom` | 1 |  | = 0x82BF9998 | ✅ |
| 0x82CC3F78 | 444 | `XamAvatarManifestGetBodyType` | 2 |  | ≈ 0x82C074B8 (560) | ✅ |
| 0x82CC4188 | 248 | `XamAvatarGetAssets` | 1 |  | — | ✅ |
| 0x82CE3048 | 220 | `XamAvatarLoadAnimation` | 1 |  | ≈ 0x82BFE460 (284) | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x8270C4F8 | 120 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x8270C570 | 120 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x8270C5E8 | 140 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x8270C680 | 184 | `XMsgStartIORequest` | 0 |  |  | ✅ |
| 0x8270C7E8 | 216 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x8270CB08 | 56 | `XMsgSystemProcessCall` | 1 |  |  | ✅ |
| 0x8270CB88 | 40 | `XamNuiGetDeviceStatus` | 1 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só as flags (+0xC): bit 0 pronto, bit 1 microfones, bit 3 superaquecido. | = 0x831AE010 | ✅ |
| 0x82CC1868 | 68 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x82CC1950 | 28 | `XamShowNuiDirtyDiscErrorUI` | 1 |  | = 0x831AE988 | ⚠️ |
| 0x82CC2280 | 40 | `XMsgCancelIORequest` | 12 |  |  | ✅ |
| 0x82E7FB38 | 352 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x82E7FC98 | 304 | `XMsgInProcessCall` | 2 |  |  | ✅ |
| 0x82E7FDC8 | 732 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x82E800A8 | 656 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x82E80338 | 152 | `XMsgInProcessCall` | 2 |  |  | ✅ |
| 0x82E803D0 | 152 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x82E80538 | 68 | `XMsgInProcessCall` | 3 |  |  | ✅ |
| 0x82E806E0 | 244 | `XMsgInProcessCall` | 4 |  |  | ✅ |
| 0x82E824A0 | 124 | `XMsgStartIORequest` | 0 |  |  | ✅ |
| 0x82E82520 | 184 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x82E82638 | 292 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x82E82760 | 148 | `XMsgStartIORequest` | 0 |  |  | ✅ |
| 0x82E827F8 | 108 | `XMsgInProcessCall` | 0 |  |  | ✅ |
| 0x82E82868 | 100 | `XMsgInProcessCall` | 0 |  |  | ✅ |
| 0x82E828D0 | 136 | `XMsgStartIORequest` | 0 |  |  | ✅ |
| 0x82E82958 | 104 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x82E829C0 | 156 | `XMsgStartIORequest` | 0 |  |  | ✅ |
| 0x830F1408 | 60 | `XMsgSystemProcessCall` | 1 |  |  | ✅ |
| 0x830F1448 | 72 | `XMsgSystemProcessCall` | 1 |  |  | ✅ |
| 0x830F1490 | 72 | `XMsgSystemProcessCall` | 1 |  |  | ✅ |
| 0x830F14D8 | 64 | `XMsgSystemProcessCall` | 1 |  |  | ✅ |
| 0x830F1518 | 56 | `XMsgInProcessCall` | 2 |  |  | ✅ |
| 0x830F1550 | 76 | `XMsgInProcessCall` | 9 |  |  | ✅ |
| 0x830F15A0 | 68 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x830F15E8 | 80 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x830F1638 | 112 | `XMsgStartIORequest` | 2 |  |  | ✅ |
| 0x830F16A8 | 100 | `XMsgStartIORequest` | 2 |  |  | ✅ |
| 0x830F1710 | 80 | `XMsgStartIORequest` | 2 |  |  | ✅ |
| 0x830F1760 | 112 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x830F17D0 | 100 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x83134D20 | 432 | `XMsgStartIORequest` | 3 |  |  | ✅ |
| 0x83134ED0 | 188 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x83134F90 | 176 | `XMsgStartIORequest` | 5 |  |  | ✅ |
| 0x83135040 | 172 | `XMsgStartIORequest` | 4 |  |  | ✅ |
| 0x831350F0 | 172 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x831351A0 | 168 | `XMsgStartIORequest` | 2 |  |  | ✅ |
| 0x83135248 | 168 | `XMsgStartIORequest` | 3 |  |  | ✅ |
| 0x831352F0 | 220 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x831353D0 | 156 | `XMsgStartIORequest` | 3 |  |  | ✅ |
| 0x83135470 | 176 | `XMsgStartIORequest` | 4 |  |  | ✅ |
| 0x83135520 | 232 | `XMsgStartIORequest` | 2 |  |  | ✅ |
| 0x83135608 | 220 | `XMsgStartIORequest` | 2 |  |  | ✅ |
| 0x831356E8 | 164 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x83135790 | 192 | `XMsgStartIORequest` | 1 |  |  | ✅ |
| 0x83135850 | 72 | `XMsgInProcessCall` | 1 |  |  | ✅ |
| 0x83135898 | 248 | `XMsgStartIORequest` | 1 |  |  | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x831386B0 | 888 | `XamVoiceHeadsetPresent` | 1 |  | = 0x831ECD98 | ✅ |
| 0x83138CC8 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  |  | ✅ |
| 0x83139058 | 600 | `XamVoiceSubmitPacket` | 1 |  | = 0x831ED740 | ✅ |
| 0x831393B8 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  |  | ✅ |
| 0x831394A8 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 |  | = 0x831EDB90 | ✅ |
| 0x8313E318 | 208 | `XamVoiceSubmitPacket` | 2 |  | = 0x831F1F38 | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | No Adventures | Decompilada |
|---|---|---|---|---|---|---|
| 0x830F3E28 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | = 0x82D93ED8 | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8805 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 18 | 3123 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 33 | 15 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 2 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | — (alta freq.) | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | — (alta freq.) | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | — (alta freq.) | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 4 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiCommunitySessionsUI` | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiPartyUI` | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 2 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | ✅ |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (62)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 25 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 4 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` | xboxkrnl 0x54 | 4 | 0 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 200 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 58 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 1 | 🟡 |
| `KeInitializeSemaphore` | xboxkrnl 0x74 | 1 | 0 | ✅ |
| `KeInitializeTimerEx` | xboxkrnl 0x75 | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 2 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 4 | 111205 | ✅ |
| `KeReleaseSemaphore` | xboxkrnl 0x88 | 1 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 11 | 9198 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3059 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3059 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 23 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 23 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 68 | 0 | ✅ |
| `KeSetTimer` | xboxkrnl 0xA6 | 3 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 7 | 38 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 7 | 25 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 2933 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 21 | 95 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 22 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 40 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 53 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 85002 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 29 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 3 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 12 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 1074 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 154 | 211 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 9 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1257 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 5 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (44)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 1 | ⚪ stub |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtCancelIoFile` ⭐ | xboxkrnl 0x2B8 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 24 | 316 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 8 | 90 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 9 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 406 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 23 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 1 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 0 | ✅ |
| `NtOpenSymbolicLinkObject` | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 1 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 7 | ✅ |
| `NtQuerySymbolicLinkObject` | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 1 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 9 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 1 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 3 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 3 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 40 | 14365 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 3 | 2 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 15 | 14367 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | 🟡 |
| `XamContentClose` | xam 0x25A | 7 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 2 | 1 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 3 | 0 | ✅ |
| `XamContentDeleteInternal` | xam 0x270 | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 6 | 4 | 🟡 |

</details>

<details><summary><b>Rede</b> (42)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetOpt` ⭐ | xam 0x4E | 1 | 0 | 🟡 |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetReplaceKey` ⭐ | xam 0x51 | 1 | 0 | ❌ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 1 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4224 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4225 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 3 | 6 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 7 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 2 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 221 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCopyString` | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 19 | 39 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 19 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 15 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 10 | 0 | ✅ |
| `_snwprintf` | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 14 | 13 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 1 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 1 | ⚪ stub |
| `XamGetCurrentTitleId` | xam 0x1CF | 9 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 2 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 5 | 4 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 1 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 10 | 15916 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 2 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 6 | 5 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 11 | 11 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (22)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 2 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 4 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 13 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 1 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetCachedUserFlags` | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 106922 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 2 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 6 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 8 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 2 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 5 | 5 | ✅ |

</details>

<details><summary><b>Memória</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 1 | 2 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 3 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 3 | 1033 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 2 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 39 | ✅ |
| `MmLockAndMapSegmentArray` | xboxkrnl 0xC0 | 1 | 0 | 🟡 |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 2 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 8 | ✅ |
| `MmUnlockAndUnmapSegmentArray` | xboxkrnl 0xC9 | 1 | 0 | 🟡 |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 32 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 10 | 25 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 6 | 4 | ✅ |
| `XamFree` | xam 0x1EC | 7 | 4 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8805 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | — | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | — | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | — | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 2 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiCommunitySessionsUI` ⭐ | xam 0x46F | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiPartyUI` ⭐ | xam 0x478 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (9)</summary>

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
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Áudio</b> (9)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 1 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Avatares</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGetAssets` | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 2 | 0 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 18 | 3123 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 33 | 15 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 1 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 18 | — | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 2 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 3 | 3 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 9 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 4 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 4 | 1 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 34212 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 2 | 0 | ⚪ stub |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

⭐ = só este jogo importa (nem o Adventures, nem o Nike, nem o Kinect Sports). — = export de alta frequência, fora do log.

**Ausentes no Xenia (19):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `NetDll_XNetReplaceKey`, `NetDll_XNetServerToInAddr`, `RtlCaptureContext`, `RtlUnwind`, `XamShowFriendsUI`, `XamShowGamerCardUIForXUID`, `XamShowNuiCommunitySessionsUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamShowNuiPartyUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.
