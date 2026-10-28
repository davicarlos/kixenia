# Kinect Rabbids Invasion (título 555308D0)

Rabbids Invasion: The Interactive TV Show, da Ubisoft (2014): episódios da série animada em que o jogador imita os Rabbids, tira fotos e grita ("Bwaah"). O log do Xenia mostra o título como **"Rabbids Invasion"**. Contêiner GOD em `<HD>/Xbox 360/Kinect Rabbids Invasion/Content/0000000000000000/555308D0/00007000/70098586BE0AE3F0CF39`, extraído (6,7 GB, 964 arquivos) com `xenia-vfs-dump` para `~/Kixenia/kinect-sdk-re/games/extraidos/rabbids/`.

**É um jogo Unity** (4.3.4f1), diferente de todos os outros analisados: o `default.xex` (7 MB) é o player do Unity para Xbox 360 (`XenonPlayer`), o jogo é C# em `Media\Managed\Assembly-CSharp.dll` (compilado para PowerPC, `.dll.xex`, com a cópia Mono `.dll.mono`) e o Kinect fica num **plugin nativo da Ubisoft**, `Media\Plugins\unityKinect.xex` (1,6 MB, `X:\RTV\Engine\unityKinect(x360)`), carregado em 0x86500000. Os outros plugins: `AkSoundEngine.xex` (som Wwise), `XAVpluginX360.xex` (vídeo, `X:\RTV\Engine\XAVpluginX360`) e `unityUbiservices.xex` (Uplay). O plugin do Kinect exporta uma API em C para o Unity: `KinectSensor_NuiInitialize`, `…_NuiSkeletonTrackingEnable`, `…_GetSkeletonNextFrame`, `…_NuiSkeletonSetTrackedSkeletons`, `…_NuiTransformSmooth`, `…_ColorStreamOpen`, `…_DepthStreamOpen`, `…_GetColorTexture`, `ExtMapColorAndDepthFrames`, `…_CreateAudio`, `…_AudioBeamFrameSource_Amplitude`, `…_NuiCameraSetAngle`, `X360Utils_ShowNuiGuideUI` e outras (36 no `unityKinect.def`).

Runtime NUI do plugin: **2.0.21256.0** (`NUI: title runtime 5/5308`), o mais novo dos jogos analisados. O código do Kinect é o do [Kinect-Rush](kinect-rush.md): das 27 funções do plugin que chamam o Kinect fora das mensagens, 26 são iguais às do Rush (mesmas chamadas e tamanho) e a outra é o `NuiCameraElevationGetAngle` público, igual ao do [Kinect-Sports-Season-Two](kinect-sports-season-two.md). O player do Unity (`default.xex`) também importa um runtime NUI completo (inclinação, identidade, microfones, avatares), mas quem abre a câmera é o plugin (as threads da câmera começam em 0x865A7578 e 0x865B68A8, dentro do `unityKinect.xex`).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Rush](kinect-rush.md), [Kinect-Sports-Season-Two](kinect-sports-season-two.md), [Kinect-Star-Wars](kinect-star-wars.md), [Kinect-Joy-Ride](kinect-joy-ride.md), [Kinect-Adventures](kinect-adventures.md), [Kinect-Sports](kinect-sports.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!CAUTION]
> ****Trava antes do menu**, por um problema no player de vídeo e não no Kinect. Testado só com a gravação (`kinect_rec_aovivo2`, 150 s, log nível 3, worktree `xenia-edge-kinect-sistema`).**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir, Unity e Mono | ✅ | Carrega o `default.xex` e as 9 bibliotecas C# (`mscorlib`, `UnityEngine`, `Assembly-CSharp`…); quadros 0–255. |
| Plugin do Kinect | ✅ | No quadro 255 carrega `unityKinect.xex`, `AkSoundEngine.xex` e `XAVpluginX360.xex`; o runtime NUI inicia (`NUI: title runtime 5/5308 with flags 0000000A`, motor de identidade em 0x865B3D20), a câmera roda (8 554 `PsCamDeviceRequest`), `XamNuiGetTrueColorInfo` (por ordinal) lê a tabela de exposição. |
| Ajuste do sensor | ✅ (sem motor) | −25° no quadro 255 e 0° no quadro 258. |
| Kinect ID | ✅ | `XamUserNuiGetUserIndex` para os 8 esqueletos, `XamCacheOpenFile`, `XamCacheReset(0x18)` e `XamReadBiometricData`, sem erro. |
| Microfones | ✅ (sem áudio) | `XamVoiceGetMicArrayAudioEx` é aberto logo na inicialização (o jogo mede o volume do grito, `KinectSensor_AudioBeamFrameSource_Amplitude`). |
| **Vídeo da Ubisoft** | ❌ **trava** | No quadro 258 o jogo toca `Media\Raw\~\UBISOFT\SplashScreen\TMP_Ubi_sphere_HD_NTSC_3sec.wmv` pelo `XAVpluginX360.xex`; a thread principal fica num laço sem fim e **nenhum quadro é apresentado depois do 258** (o vigia do Xenia avisa: "no guest frame presented in 2000 watchdog ticks"). Ver *O travamento no vídeo de abertura* abaixo. Sem `Host-side crash`. |
| Rastreamento | ❔ | Nenhum quadro de esqueleto publicado: o jogo trava antes. |
| Avatares, fitness | — | O plugin não importa avatares nem `XamFitness*` (tem os nomes `NUIFITNESS1`/`NuiFitnessJointWeights` do runtime, sem uso visto). O player do Unity importa 10 `XamAvatar*` (stubs no Xenia), não chamadas. |
| Ausentes chamadas | ⚪ | `EtxProducerRegister`/`Unregister`, `XamXlfsInitializeUploadQueue`, `XamBackgroundDownloadSetMode`, `VdRegisterHDCPNotification` (duas vezes, pelo player de vídeo), `IoDismountVolumeByFileHandle`; o player de vídeo também pede `VdSetStudioRGBMode` (xboxkrnl 0x379) por ordinal, não acha e segue. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema LOGLEVEL=3 ./replay_test.sh ~/kinect_rec_aovivo2 rabbids_replay 150 "$HOME/Kixenia/kinect-sdk-re/games/extraidos/rabbids/default.xex"
```
O cabeçalho do contêiner GOD (`…/555308D0/00007000/70098586BE0AE3F0CF39`) também serve de alvo.

## O travamento no vídeo de abertura

O diagnóstico do vigia do Xenia, no quadro 258, mostra a thread principal (`Main XThread`) **rodando** em lr=0x870A184C, sem esperar nada, e todas as outras bloqueadas; os decodificadores do vídeo (threads com entrada 0x87372D70 e 0x870C22B8 do `XAVpluginX360.xex`) esperam em `WaitForMultipleObjects` (0x873679A8) por eventos que ninguém sinaliza.

1. **0x870A16F8** (`XAVpluginX360`, a função do lr): cria o filme (0x18 bytes, duração em +0x10 = −1), começa a abri-lo (0x870A1548) e, se não pediram para não esperar, fica em `while (filme->duração == −1) { 0x870A00B8(filme); 0x870A0A60(filme); }`.
2. **0x870A00B8** pede o estado ao player (método +0x30) e, na primeira vez, dá a ordem de tocar (+0x14).
3. **0x870A0A60** só faz alguma coisa nos estados 5, 6 e 7; no estado 6 grava a duração em +0x10 (o que encerra o laço); no estado 9 (fim/erro) marca o filme como parado, **mas não mexe na duração**. Se a abertura falha, o laço nunca termina: é um defeito do jogo que só aparece quando o vídeo não abre.
4. No log, a thread que abre o vídeo (0x87372D70 → `XThread001F`) abre o `.wmv`, consulta o tamanho (`NtQueryInformationFile`, classe 0x22), aloca 15 MB, cria a thread de leitura e os decodificadores, lê a configuração de vídeo (`ExGetXConfigSetting` 3/1…3/0xC) e a hora, e então **libera tudo, fecha o arquivo e termina** (depois de chamar `XamBackgroundDownloadSetMode`, ausente). Ou seja, a abertura desiste. O `NtReadFile` é de alta frequência e não aparece no log nível 3, então não dá para dizer quanto do arquivo foi lido.

Hipóteses para a abertura falhar no Xenia: algum retorno do `NtQueryInformationFile` (classe 0x22, `FileNetworkOpenInformation`) ou das configurações de vídeo; a decodificação do WMV/XMA no próprio plugin; ou a falta do `VdSetStudioRGBMode` (o plugin trata a falta, mas pode mudar o caminho). Para seguir: rodar com `NtReadFile` no log (ou um ponto de parada em 0x870A0A60 para ver o estado devolvido pelo método +0x30) e decompilar a abertura (0x870A1548 e o objeto do player). O Kinect não tem relação com o travamento.

## Teste com o Kinect real: pendente

Só faz sentido depois que o vídeo de abertura deixar de travar (no Xenia, não no Kinect). Quando passar:
- **Rastreamento**: o jogo escolhe os esqueletos (`KinectSensor_NuiSkeletonSetTrackedSkeletons`); conferir se o jogador vira TRACKED e se o cursor/gestos dos menus funcionam.
- **Câmera colorida**: o jogo tira fotos do jogador (`AR_PhotoManager`, `KinectSensor_GetColorTexture`, `ExtMapColorAndDepthFrames`): conferir a imagem e o alinhamento cor–profundidade.
- **Microfones**: o medidor de grito ("Bwaah meter", `KinectSensor_AudioBeamFrameSource_Amplitude`) precisa do áudio do Kinect: conferir `XamVoiceGetMicArrayAudioEx` e `XAudioRegisterRenderDriverMECClient` no log.
- **Motor de inclinação**: −25° → busca → chão guardado (`KinectSensor_NuiCameraSetAngle` também deixa o jogo mover o motor).
- **Kinect ID**: reconhecimento de perfis.

## Pendências
- O travamento do `XAVpluginX360.xex` no vídeo de abertura (acima). Pode afetar outros jogos Unity da Ubisoft que usem o mesmo plugin.
- `VdSetStudioRGBMode` (xboxkrnl 0x379) e `VdRegisterHDCPNotification`: ausentes no Xenia, pedidas pelo player de vídeo.
- O runtime NUI importado pelo `default.xex` (player do Unity): parece não ser usado; conferir ao vivo.

## Arquivos da análise
- Extração: `~/Kixenia/kinect-sdk-re/games/extraidos/rabbids/` (log em `extraidos/rabbids.dump.log`).
- Imagens planas: `~/Kixenia/kinect-sdk-re/games/rab_unityKinect.img` (plugin, base 0x86500000), `rab_555308D0.img` (player do Unity), `rab_XAVpluginX360.img` (vídeo, base 0x87000000); cópias dos XEX ao lado (`rab_*.xex`).
- Projetos Ghidra (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática): `~/Kixenia/kinect-sdk-re/ghidra-projects/rabvmx` (plugin do Kinect; cópia `rabpar1`), `rabexe` (player), `rabxav` (vídeo).
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/rab/c_full/<endereço>.c` (74 funções do plugin, nenhuma com `halt_baddata`); `rab/xav/` tem o laço do vídeo (0x870A16F8) e as funções em volta.
- Log da execução: `~/Kixenia/kinect-tools/logs/rabbids_replay.log`.
- Scripts: `rab_imports.py` (importações de um módulo escolhido no log), `rab_funcs.py`, `rab_merge.py`, `rab_runtime.py`, `ks2_compare.py`, `gen_rab.py` (`games/decompilado/scripts/`), `DumpImage.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).
- Tabela `.pdata` do plugin: 0x86594E00, 1 848 funções; `.text` 0x865A0000–0x866749A8.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/rab/`.**

**195 importações do plugin `unityKinect.xex`** (49 do `xam.xex`, 146 do `xboxkrnl.exe`), 977 chamadas diretas no código, 98 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 147 implementadas, 19 aproximadas, 18 stubs, 10 ausentes, 1 sem marca.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 em 2 processos (✅ = decompilada inteira). Tudo se refere ao plugin `unityKinect.xex`. *No Rush*: `=` mesma função do [Kinect-Rush](kinect-rush.md) (mesmas chamadas e tamanho), `≈` mesmas chamadas e outro tamanho, `—` não existe lá (`ks2_compare.py`).

### Funções do jogo que falam com o Kinect

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x865A0B68 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio) e só se falhar chama `PsCamDeviceRequest`. | = 0x82950A60 | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x865A8B30 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: traduz o código de saída, para o motor (`ElevationStopMovement`) nos erros 4 e 0x19 e informa o status ao sistema (0x2B00C). | = 0x82963668 | ✅ |
| 0x865A8DE0 | 92 | `XamNuiCameraElevationGetAngle` | 1 | `NuiCameraElevationGetAngle` público do runtime: confere os ponteiros e lê o ângulo (`XamNuiCameraElevationGetAngle`). Base de `KinectSensor_NuiCameraElevationGetAngle`. | ≈ 0x82963FB0 (676) | ✅ |
| 0x865A8E40 | 132 | `XamNuiCameraElevationSetAngle` | 3 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`). | = 0x82963918 | ✅ |
| 0x865A8F90 | 76 | `XamNuiCameraTiltGetStatus` | 0 | **Leitor do status 'XtSs'**: tenta `XamXStudioRequest(0x1005)` e, se o Kinect Studio não responder, `XamNuiCameraTiltGetStatus`. | = 0x829639C0 | ✅ |
| 0x865A9470 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: se as flags do 'XtSs' pedem (+0x2C bit 0x10) ou **não há chão guardado (+0x3C = 0)**, inclina para **−25°**. | = 0x82963D48 | ✅ |
| 0x865A9500 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: confere o plano do chão do quadro de esqueleto contra a gravidade (0x82963CD8) e o guarda no sistema (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis; com o motor parado e erro, encerra o pedido (0x2B006/0x2B00C). | = 0x82963DD8 | ✅ |
| 0x865A96D8 | 676 | `XamNuiCameraElevationGetAngle` | 1 | **Pedido de ajuste automático** (distância, altura, flags). +0x40 bit 0 do 'XtSs' = ocupado (devolve 5). Flag 0x20: só confere. Flag 0x08: recalcula o alvo (0x82963A10) e inclina se `\|alvo − ângulo\| > 2,0°` (double em 0x82001928): o "sensor foi movido". Sem flag: começa a busca (0x82963D48). | = 0x82963FB0 | ✅ |
| 0x865A9980 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam (0x2B005…0x2B00B): AutoTilt, StopMovement, ReverseAutoTilt e AdjustTilt do sistema; responde com `XamNuiCameraTiltReportStatus`. | = 0x82964258 | ✅ |
| 0x865A9D40 | 148 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback (`XamNuiCameraTiltSetCallback(0x82964258)`) e põe 0x829639C0 no ponteiro do leitor de status. | = 0x82964618 | ✅ |
| 0x865AA778 | 2436 | `XamNuiCameraElevationStopMovement` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto: lê o 'XtSs', calcula o ângulo atual `atan2(+0x28, +0x24)`; na busca sobe degraus `23 − 10·n` (−17°, −7°, 3°) a cada 60 quadros sem chão; com o chão, corrige-o pela gravidade do chão (+0x44..+0x4C) e mira o jogador; erro do motor (+0x2C bit 0x08) encerra com 0x1F. | = 0x82964EF8 | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x865B1A08 | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | = 0x8295B5D8 | ✅ |
| 0x865B1C28 | 432 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8295B7F8 | ✅ |
| 0x865B3BB8 | 356 | `XamUserNuiGetUserIndex` | 0 | Para cada um dos 8 esqueletos, pergunta ao sistema qual usuário está associado (`XamUserNuiGetUserIndex`) e acorda o motor de identidade quando muda. | = 0x8295D788 | ✅ |
| 0x865B49A0 | 388 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8295E570 | ✅ |
| 0x865B4B28 | 3200 | `XamNuiIdentityGetSessionId` | 5 | Sessão do Kinect ID: acompanha os jogadores e o identificador da sessão de identidade (`XamNuiIdentityGetSessionId`). | = 0x8295E6F8 | ✅ |
| 0x865BDFC0 | 996 | `XamNuiIdentityGetSessionId` | 1 |  | = 0x8296CEA0 | ✅ |
| 0x865C2370 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 | Carrega o registro biométrico de um usuário: `XamUserNuiGetEnrollmentIndex`; sem registro, lê o perfil pelo XMsg (0x32000 bytes). | = 0x82971270 | ✅ |
| 0x865C25B8 | 336 | `XamUserNuiGetUserIndex` | 1 |  | = 0x829714B8 | ✅ |
| 0x865C2838 | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 1 | Cache dos rostos do Kinect ID: `XamCacheOpenFile` (cache 0x18), `XamCacheReset` e fecha. Na gravação: um `XamCacheOpenFile` e um `XamCacheReset(0x18)`, sem erro. | = 0x82971738 | ✅ |
| 0x865C2930 | 488 | `XamReadBiometricData` | 1 | Lê os dados biométricos guardados (`XamReadBiometricData`). | = 0x82971830 | ✅ |
| 0x865C2FA0 | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 | Atualiza os dados biométricos: lê (`XamReadBiometricData`), mescla (0x82971BE0) e grava (`XamWriteBiometricData`, `XamUserNuiEnableBiometric`). | = 0x82971E48 | ✅ |

#### Microfones e cancelamento de eco

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x865E99C8 | 224 | `XamNuiGetDeviceSerialNumber` | 1 | Inicia o registro de eventos do runtime: número de série do sensor (`XamNuiGetDeviceSerialNumber`), procura a export 0x8E1 do xam e `EtxProducerRegister` (ausente). | = 0x82EBA480 | ✅ |
| 0x865E9BB0 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` | 0 | **Laço de áudio dos microfones**: lê os quadros de `XamVoiceGetMicArrayAudioEx`, confere `…UnderrunStatus` e manda o áudio de referência ao cancelador de eco (`XAudioCaptureRenderDriverFrame`). | = 0x82EBA668 | ✅ |
| 0x865EAA70 | 1252 | `XAudioUnregisterRenderDriverMECClient` | 2 | Desliga o áudio dos microfones e o cancelador de eco (`XAudioUnregisterRenderDriverMECClient`). | = 0x82EBB528 | ✅ |
| 0x865EB078 | 1504 | `XAudioRegisterRenderDriverMECClient`, `XamVoiceGetMicArrayAudioEx` | 0 | **Abre o áudio dos microfones** para o reconhecedor de voz: aloca os buffers (0x6000, 0x1B000, 0x4800…), pede `XamVoiceGetMicArrayAudioEx` e registra o cliente de cancelamento de eco (`XAudioRegisterRenderDriverMECClient`). | = 0x82EBBA88 | ✅ |

#### Estado do sensor e mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | No Rush | Decompilada |
|---|---|---|---|---|---|---|
| 0x86634000 | 40 | `XamNuiGetDeviceStatus` | 0 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só o campo de estado. | = 0x829473F0 | ✅ |
| 0x86634E60 | 60 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36BE8 | ✅ |
| 0x86634EA0 | 72 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36C28 | ✅ |
| 0x86634EE8 | 72 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36C28 | ✅ |
| 0x86634F30 | 64 | `XMsgSystemProcessCall` | 1 |  | = 0x82D36CB8 | ✅ |
| 0x86634F70 | 56 | `XMsgInProcessCall` | 2 |  | = 0x82D36CF8 | ✅ |
| 0x86634FA8 | 76 | `XMsgInProcessCall` | 9 |  | = 0x82D36D30 | ✅ |
| 0x86634FF8 | 68 | `XMsgInProcessCall` | 1 |  | = 0x82D36D80 | ✅ |
| 0x86638468 | 80 | `XMsgStartIORequest` | 1 |  | = 0x82D36DC8 | ✅ |
| 0x866384B8 | 112 | `XMsgStartIORequest` | 3 |  | = 0x82D36E18 | ✅ |
| 0x86638528 | 100 | `XMsgStartIORequest` | 2 |  | = 0x82D36E88 | ✅ |
| 0x86638590 | 80 | `XMsgStartIORequest` | 2 |  | = 0x82D36DC8 | ✅ |
| 0x866385E0 | 112 | `XMsgStartIORequest` | 1 |  | = 0x82D36E18 | ✅ |
| 0x86638650 | 100 | `XMsgStartIORequest` | 1 |  | = 0x82D36E88 | ✅ |
| 0x86661070 | 168 | `XMsgStartIORequestEx` | 2 |  | = 0x82E82F80 | ✅ |
| 0x86661118 | 104 | `XMsgInProcessCall` | 2 |  | = 0x82E83028 | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8554 | ✅ |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 7 | 26 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 6 | 4 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |
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
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 1 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (51)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 29 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 1 | 7 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 1 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 45 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 1 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 1 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 3 | 85 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 9 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 16 | 0 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 2 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 2 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 28 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 115 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 80 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 1 | 31 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 1 | 3 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 2 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 5 | 982 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 3 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 20 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 35 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 37 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 127 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 4 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 110 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 8 | 389 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 1 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 138 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 1 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 4 | 11 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (29)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NtClose` | xboxkrnl 0xCF | 8 | 354 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 2 | 92 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 0 | 185 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 273 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 0 | 15 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 3 | 31 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 3 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 8 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 46 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 1 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 4 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 1 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 3 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 3 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 2 | 15 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 1 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 23 | 306 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 1 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 9 | 306 | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | 🟡 |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 1 | 🟡 |
| `XamCacheReset` | xam 0x2B7 | 1 | 1 | 🟡 |
| `XamContentClose` | xam 0x25A | 5 | 2 | ✅ |
| `XamContentCreateInternal` | xam 0x271 | 1 | 1 | ✅ |
| `XamContentOpenFile` | xam 0x269 | 8 | 4 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 3 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 258 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 259 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetVideoMode` | xam 0x3D1 | 1 | 9 | 🟡 |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (20)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 10 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 2 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 3 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 14 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 8 | 221 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 12 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 7 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnwind` | xboxkrnl 0x147 | 1 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 2 | 2 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (19)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 10 | 93 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 4 | ✅ |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 7 | 1 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 12 | 1 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 3 | 9 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamXStudioRequest` | xam 0x686 | 18 | 8530 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 2 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 2 | 7 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 2 | 382 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 13 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 1 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 8554 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 2 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 1 | 0 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 8 | ✅ |

</details>

<details><summary><b>Memória</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 580 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 275 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 1 | 1 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 12 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 11 | 250 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 9 | 131 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 2 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 1 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 2 | ✅ |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 1 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgInProcessCall` | xam 0x1F4 | 7 | 26 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 6 | 4 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 1 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 4 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetRenderDriverTic` | xboxkrnl 0x34C | 1 | 0 | ✅ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 3 | ✅ |
| `XAudioRegisterRenderDriverMECClient` | xboxkrnl 0x320 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` | xboxkrnl 0x321 | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 1 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 2 | 1 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 1 | 8 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike, nem os dois Kinect Sports, nem o Adventures, o Rush ou o Joy Ride).

**Ausentes no Xenia (10):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `RtlCaptureContext`, `RtlUnwind`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `__C_specific_handler`.
