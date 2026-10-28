# Kinect Sports (título 4D5308C9)

Kinect Sports (Ultimate Collection), da Rare. Executável `default.xex` no HD de 8 TB (`Xbox 360/Kinect Sports Ultimate Collection/`), com os pacotes de modelos `database.xmplr` (rastreador), `nuiidentity.bin.be` (identidade) e outros. Imagem para análise: `~/Kixenia/kinect-sdk-re/games/kinectsports_4D5308C9.exe` (PE gerado do XEX) e o projeto Ghidra com o fluxo corrigido `~/Kixenia/kinect-sdk-re/ghidra-projects/ksproj`.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Engenharia-Reversa](../kinect-engenharia-reversa.md) (seções 12, 16 e 16f), [Kinect-ID](../kinect-id.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge
- Roda com o Kinect real e com gravações; rastreia o jogador (cerca de 55% dos quadros na gravação `kinect_rec_aovivo2`, 68% numa sessão ao vivo de frente para o sensor).
- Lê a tabela de exposição (`XamNuiGetTrueColorInfo`, os dois seletores) e o ângulo do motor cerca de 5 mil vezes por gravação.
- Só falta, das chamadas que ele faz, `IoDismountVolumeByFileHandle` (armazenamento, não Kinect).

## O que já se sabe do jogo
- **Rastreador na GPU:** o esqueleto é calculado em shaders a partir da profundidade; precisou da correção de GPU `da9870cc1` (retângulos de resolve vindos da CPU) no Xenia.
- **Menu de pausa** (`Function_82523468`): Quit, Restart Game, Resume Game, Concede, **Guide** (chama `XamShowNuiGuideUI` pelo atalho 0x82A74D50: `li r4,0xFF; b` para o thunk 0x83178F24) e Troubleshooter. O gesto de pausa abre esse menu durante as partidas.
- **START** do controle (`Function_822E7200`, botão 0x10) também chama o guia.
- **O gesto do guia não é detectado pelo jogo:** é o sistema que vê, no quadro de esqueleto publicado com 0x2B002 (corrigido no Xenia em a56454014).
- **Identidade:** a resolução 0x2C00D (`0x82A84340`, chamada por `0x82A0F2B0`) compara os rostos com até 8 cadastros e recebe o cadastro reconhecido (0 a 7), 0xFFFFFFFE (ninguém) ou 0xFFFFFFFD. Parâmetros pedidos por nome na 0x2C00C: `NuiV1QualDist`, `QualPose`, `QualClip`, `FaceSample`, `SkFilter`, `ImgFilter`, `LightFilter`, `Outliers`, `GuidePanCfg`, `TitlePanCfg`.
- **Avatares:** usa a API de avatares do sistema (`XamAvatarGetAssets`, `XamAvatarLoadAnimation`, `XamAvatarManifestGetBodyType`…), que o Nike não usa.

## Chamadas ao sistema (análise de 2026-10-25)

> [!NOTE]
> **Seção gerada por `kinect-sdk-re/games/decompilado/scripts/gen_games.py` + `write_notes.py`, deste título até o fim da nota. Edições aqui são sobrescritas; escreva acima desta seção.**

Levantamento de **tudo o que o jogo chama do sistema**: 376 importações (159 do `xam.xex`, 217 do `xboxkrnl.exe`), 4224 chamadas diretas no código, 134 vistas em execução nas gravações (log nível 3; o log omite as funções de alta frequência). Estado no Xenia Edge (branch kinect-linux): 240 implementadas, 20 aproximadas, 61 stubs, 55 ausentes.

**Como foi feito:** o XEX carregado pelo XEXLoaderWV foi exportado para um PE (`~/Kixenia/kinect-sdk-re/games/`), lido pelo REA; as importações e os thunks vêm do log do Xenia e da tabela de importações (palavra `0x00LLOOOO`: biblioteca e ordinal); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem do projeto Ghidra com o fluxo corrigido. As funções do Kinect foram decompiladas pelo **GhidraMCP 7.0.0** (servidor headless sobre o mesmo projeto corrigido); ✅ na coluna Ghidra MCP = decompilada e com o início confirmado.

### Kinect: o runtime NUI do jogo

O jogo traz o **runtime NUI da Microsoft** embutido (rastreador de esqueleto na GPU, motor de identidade, motor de inclinação, microfones). As funções abaixo são as que falam com o sistema; a maioria é idêntica no outro jogo (mesmo tamanho e mesmas chamadas, ver [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md)).

#### Motor de inclinação do jogo

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x829FF008 (Nike 0x82F530F8) | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 0x2B006 0x2B00C | Motor de inclinação: parar o motor (`ElevationStopMovement`) e informar o status ao sistema (0x2B006/0x2B00C). | ✅ |
| 0x829FF2B8 | 92 | `XamNuiCameraElevationGetAngle` |  |  | ✅ |
| 0x829FF318 (Nike 0x82F533A8) | 132 | `XamNuiCameraElevationSetAngle` |  | Motor de inclinação: mover para um ângulo (`ElevationSetAngle`). | ✅ |
| 0x829FF3C0 (Nike 0x82F53450) | 76 | `XamNuiCameraTiltGetStatus` |  | Motor de inclinação: ler o status do motor e do acelerômetro (`TiltGetStatus`, 'XtSs'). | ✅ |
| 0x829FF748 (Nike 0x82F537D8) | 144 | `XamNuiCameraElevationSetAngle` |  | Motor de inclinação: mover para um ângulo (segunda variante). | ✅ |
| 0x829FF7D8 (Nike 0x82F53868) | 324 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 0x2B006 0x2B00C | Motor de inclinação: guardar o plano do chão (`RememberFloor`, cerca de 25 vezes por segundo) e informar o status. | ✅ |
| 0x829FF9B0 (Nike 0x82F53A40) | 532 | `XamNuiCameraElevationGetAngle` |  | Motor de inclinação: ler o ângulo atual (`ElevationGetAngle`) e decidir o próximo passo. | ✅ |
| 0x829FFE40 (Nike 0x82F53ED0) | 780 | `XamNuiCameraTiltReportStatus` | 0x2B006 0x2B007 0x2B008 0x2B009 0x2B00A 0x2B00C | **Callback do motor de inclinação** registrado no sistema: recebe do xam AutoTilt (0x2B005), StopMovement (0x2B009), ReverseAutoTilt (0x2B00A) e AdjustTilt (0x2B00B) e responde com o status (0x2B006/0x2B00C). | ✅ |
| 0x82A00200 (Nike 0x82F54290) | 148 | `XamNuiCameraTiltSetCallback` |  | Registra o callback do motor de inclinação no xam (`XamNuiCameraTiltSetCallback`, 0x2B007). | ✅ |

#### Câmera (PsCam)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82A01458 (Nike 0x82F476B8) | 96 | `PsCamDeviceRequest` |  | O único ponto que chama `PsCamDeviceRequest`: todos os pedidos à câmera (estado, leitura de quadros, ajustes) passam por aqui. | ✅ |

#### Motor de identidade (Kinect ID)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82A0E018 (Nike 0x82F59850) | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` |  | Motor de identidade: abre uma sessão (`IdentityGetSessionId`) e acha o perfil de um cadastro (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82A0E238 (Nike 0x82F59A70) | 432 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: usa o ID de sessão nos pedidos. | ✅ |
| 0x82A101C8 (Nike 0x82F5BA00) | 356 | `XamUserNuiGetUserIndex` |  | Motor de identidade: acha o perfil ligado a um cadastro (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82A10FB0 (Nike 0x82F5CA88) | 388 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: usa o ID de sessão. | ✅ |
| 0x82A11138 (Nike 0x82F5CC10) | 3200 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: a função principal (3,2 KB), chamada por 5 lugares; trata o resultado das identificações. | ✅ |
| 0x82A1B170 (Nike 0x82F65D68) | 996 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: usa o ID de sessão. | ✅ |

#### Dados biométricos (cadastro do rosto)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82A1F540 (Nike 0x82F6A138) | 392 | `XamUserNuiGetEnrollmentIndex` |  | Kinect ID: carrega os rostos cadastrados do cache (lê o índice do cadastro, `XamUserNuiGetEnrollmentIndex`). | ✅ |
| 0x82A1F788 (Nike 0x82F6A380) | 336 | `XamUserNuiGetUserIndex` |  | Kinect ID: acha o perfil de um cadastro. | ✅ |
| 0x82A1FA08 (Nike 0x82F6A600) | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` |  | Kinect ID: cache de arquivos do xam (`XamCacheOpen/Close/Reset`). | ✅ |
| 0x82A1FB00 (Nike 0x82F6A6F8) | 488 | `XamReadBiometricData` |  | Kinect ID: lê os dados biométricos do perfil (`XamReadBiometricData`) e repassa ao cache (0x2A007). | ✅ |
| 0x82A20170 (Nike 0x82F6AD68) | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` |  | **Kinect ID: grava o cadastro do rosto**: lê a cópia do perfil, soma 1 à versão, monta os dados, grava no cache (0x2A007), liga a biometria (`XamUserNuiEnableBiometric`) e grava no perfil (`XamWriteBiometricData`). Disparada quando há 200 amostras do rosto (0x82F5E040). | ✅ |

#### Mensagens NUI ao xam (0x2C0xx e 0x2A0xx)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82A841A8 (Nike 0x838C26F0) | 60 | `XMsgSystemProcessCall` | 0x2C001 | Mensagem 0x2C001 ao xam (0x10 bytes). | ✅ |
| 0x82A841E8 (Nike 0x838C2730) | 72 | `XMsgSystemProcessCall` | 0x2C003 | Mensagem 0x2C003 ao xam (0x18 bytes). | ✅ |
| 0x82A84230 (Nike 0x838C2778) | 72 | `XMsgSystemProcessCall` | 0x2C004 | Mensagem 0x2C004 ao xam (0x18 bytes). | ✅ |
| 0x82A84278 (Nike 0x838C27C0) | 64 | `XMsgSystemProcessCall` | 0x2C005 | Mensagem 0x2C005 ao xam (0xC bytes). | ✅ |
| 0x82A842B8 (Nike 0x838C2800) | 56 | `XMsgInProcessCall` | 0x2C009 | **Registra o callback de identidade** no xam (0x2C009). | ✅ |
| 0x82A842F0 (Nike 0x838C2838) | 76 | `XMsgInProcessCall` | 0x2C00C | Mensagem 0x2C00C: dados de identidade (op 1 converte os dados do Kinect ID; op 2 lê parâmetros por nome como `NuiV1QualDist`). 9 chamadores. | ✅ |
| 0x82A84340 (Nike 0x838C2888) | 68 | `XMsgInProcessCall` | 0x2C00D | Mensagem 0x2C00D: resolução de identidade (qual cadastro foi reconhecido). | ✅ |
| 0x82A84388 (Nike 0x838C28D0) | 80 | `XMsgStartIORequest` | 0x2A009 | Mensagem 0x2A009 (biometria desligada). | ✅ |
| 0x82A843D8 (Nike 0x838C2920) | 112 | `XMsgStartIORequest` | 0x2A008 | Mensagem 0x2A008: lê o cache biométrico do perfil (versão 0 = o xam ainda não tem). | ✅ |
| 0x82A84448 (Nike 0x838C2990) | 100 | `XMsgStartIORequest` | 0x2A007 | Mensagem 0x2A007: grava o cache biométrico do perfil. | ✅ |
| 0x82A844B0 (Nike 0x838C29F8) | 80 | `XMsgStartIORequest` | 0x2A006 | Mensagem 0x2A006. | ✅ |
| 0x82A84500 (Nike 0x838C2A48) | 112 | `XMsgStartIORequest` | 0x2A005 | Mensagem 0x2A005. | ✅ |
| 0x82A84570 (Nike 0x838C2AB8) | 100 | `XMsgStartIORequest` | 0x2A004 | Mensagem 0x2A004. | ✅ |

#### Mensagens 0x2B0xx e Live (app 0xFB)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82A74960 (Nike 0x82DA4780) | 120 | `XMsgStartIORequest` | 0xB0008 | envia 0xB0008 | ✅ |
| 0x82A749D8 | 120 | `XMsgStartIORequest` | 0xB0071 | envia 0xB0071 | ✅ |
| 0x82A74A50 (Nike 0x82DA47F8) | 140 | `XMsgStartIORequest` | 0xB0006 | envia 0xB0006 | ✅ |
| 0x82A74AE8 (Nike 0x82DA4890) | 184 | `XMsgStartIORequest` | 0xB0007 | envia 0xB0007 | ✅ |
| 0x82A74C50 (Nike 0x82DA49F8) | 216 | `XMsgStartIORequest` | 0xB0021 | envia 0xB0021 | ✅ |
| 0x82A74DE0 (Nike 0x82DA6520) | 40 | `XMsgCancelIORequest` |  |  | ✅ |
| 0x82A77580 (Nike 0x82DA60E8) | 56 | `XMsgSystemProcessCall` | 0x2B001 | Mensagem 0x2B001 ao xam (início do runtime NUI). | ✅ |

#### Chat de voz

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82C34980 (Nike 0x82D49F18) | 200 | `XamVoiceIsActiveProcess` |  | Chat de voz: abaixa o som do jogo durante o bate-papo (`XamVoiceIsActiveProcess`). | ✅ |
| 0x82C35C18 (Nike 0x82D4B1B0) | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` |  | Chat de voz: fone presente e voz ativa. | ✅ |
| 0x82C36258 (Nike 0x82D4B7F0) | 316 | `XMsgCancelIORequest`, `XamVoiceClose` |  | Chat de voz: fecha o objeto de voz. | ✅ |
| 0x82C365E8 (Nike 0x82D4BB80) | 600 | `XamVoiceSubmitPacket` |  | Chat de voz: envia pacotes de voz (`XamVoiceSubmitPacket`). | ✅ |
| 0x82C36948 (Nike 0x82D4BEE0) | 240 | `XMsgCancelIORequest`, `XamVoiceClose` |  | Chat de voz: fecha o objeto de voz. | ✅ |
| 0x82C36A38 (Nike 0x82D4BFD0) | 192 | `XamVoiceClose`, `XamVoiceCreate` |  | Chat de voz: cria e fecha o objeto de voz. | ✅ |
| 0x82C398C0 (Nike 0x82D4EF48) | 688 | `XamVoiceIsActiveProcess` |  | Chat de voz: estado da voz. | ✅ |
| 0x82C3BCE8 (Nike 0x82D51370) | 208 | `XamVoiceSubmitPacket` |  | Chat de voz: envia pacotes. | ✅ |

#### Serviços da Live (app 0xFB)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82D27BE0 (Nike 0x82ADE890) | 168 | `XMsgStartIORequestEx` |  |  | ✅ |
| 0x82D27C88 (Nike 0x82ADE938) | 104 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82D283E0 (Nike 0x82640698) | 352 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82D28540 (Nike 0x826407F8) | 304 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82D28670 (Nike 0x82640928) | 752 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82D28960 | 732 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82D28C40 | 648 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82D28F38 (Nike 0x82A779B0) | 432 | `XMsgStartIORequest` | 0xB0010 | envia 0xB0010 | ✅ |
| 0x82D290E8 (Nike 0x82A77B60) | 188 | `XMsgStartIORequest` | 0xB0018 | envia 0xB0018 | ✅ |
| 0x82D291A8 (Nike 0x82A77C20) | 176 | `XMsgStartIORequest` | 0xB0011 | envia 0xB0011 | ✅ |
| 0x82D29258 (Nike 0x82A77CD0) | 172 | `XMsgStartIORequest` | 0xB0012 | envia 0xB0012 | ✅ |
| 0x82D29308 (Nike 0x82A77D80) | 172 | `XMsgStartIORequest` | 0xB0012 | envia 0xB0012 | ✅ |
| 0x82D293B8 (Nike 0x82A77E30) | 168 | `XMsgStartIORequest` | 0xB0013 | envia 0xB0013 | ✅ |
| 0x82D29460 (Nike 0x82A77ED8) | 168 | `XMsgStartIORequest` | 0xB0013 | envia 0xB0013 | ✅ |
| 0x82D29508 (Nike 0x82A77F80) | 220 | `XMsgStartIORequest` | 0xB001A | envia 0xB001A | ✅ |
| 0x82D295E8 (Nike 0x82A78060) | 156 | `XMsgStartIORequest` | 0xB0014 | envia 0xB0014 | ✅ |
| 0x82D29688 (Nike 0x82A78100) | 176 | `XMsgStartIORequest` | 0xB0015 | envia 0xB0015 | ✅ |
| 0x82D29738 | 228 | `XMsgStartIORequest` | 0xB0016 | envia 0xB0016 | ✅ |
| 0x82D29820 (Nike 0x82A781B0) | 232 | `XMsgStartIORequest` | 0xB001C | envia 0xB001C | ✅ |
| 0x82D29908 (Nike 0x82A78298) | 220 | `XMsgStartIORequest` | 0xB001B | envia 0xB001B | ✅ |
| 0x82D299E8 (Nike 0x82A78378) | 192 | `XMsgStartIORequest` | 0xB0025 | envia 0xB0025 | ✅ |
| 0x82D29D50 | 152 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82D29DE8 (Nike 0x82A46BC0) | 72 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82D29E30 (Nike 0x82A50930) | 248 | `XMsgStartIORequest` | 0xB001E | envia 0xB001E | ✅ |
| 0x82D2A130 (Nike 0x8264D990) | 252 | `XMsgInProcessCall` |  |  | ✅ |

**Importações do Kinect e do runtime NUI** (chamadas no código / vistas em execução):

| Importação | Biblioteca, ordinal | Chamadas | Em execução | O que é | Xenia |
|---|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 14698 | pedidos à câmera do Kinect (estado, quadros, ajustes) | 🟡 |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | cancela uma mensagem assíncrona | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 12 | 6545 | mensagem no processo a um app do xam | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 29 | 18 | mensagem assíncrona a um app (0xFB Live, 0xFE xam) | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 6 | idem, com mais opções | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 3 | mensagem síncrona a um app do sistema | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | fecha um arquivo do cache | ⚪ stub |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 3 | abre um arquivo no cache do xam | ⚪ stub |
| `XamCacheReset` | xam 0x2B7 | 1 | 3 | limpa o cache | ⚪ stub |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 16753 | lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | 🟡 |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 6 | FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | 🟡 |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | 🟡 |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 5477 | guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes) | ⚪ stub |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 13279 | status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | 🟡 |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 3 | 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 15 | 8 bytes aleatórios, criados na primeira chamada | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 3 | XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | NuiHudCheck + caixa de mensagem 0x72000 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | NuiHudCheck + seletor de dispositivo (modo Kinect) | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | NuiHudCheck + 0x21032 (Live) | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | NuiHudCheck + marketplace | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | NuiHudCheck + login (FUN_816C1868) | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 2 | sem sensor: caixa de mensagem (textos 0x11F/0x4F); com sensor: engaja e abre o Tuner (0x21028), modo 2, 3 ou 5 | ⚪ stub |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | tela comum | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 3 | +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 24 | o usuário com aquele cadastro | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | voz (kernel XVoiced*) | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | voz (kernel XVoiced*) | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | kernel XVoiced* | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 89558 | kernel XVoiced* | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | kernel XVoiced* | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | XMsg 0xFB (Live); tamanho 0 apaga | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (61)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 4 | 81 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ❌ |
| `ExTerminateThread` | xboxkrnl 0x19 | 3 | 30 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ❌ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ❌ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ❌ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ❌ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 4 | 0 | ❌ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 1086 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 58 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 6 | ✅ |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 3 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | ✅ |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | ✅ |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 192 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 18 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 20737 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 22 | 37451 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 6404 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 3 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 6404 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 78 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 411 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 92 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ❌ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 10 | 156 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 10 | 135 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 793 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 31 | 38442 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | ✅ |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 5 | 0 | ❌ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 26 | 0 | ❌ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 43 | 0 | ❌ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 45 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 6 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 5 | 0 | ✅ |
| `NtQueueApcThread` ⭐ | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 849 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 2 | 111 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 17 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ❌ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 20 | 0 | ❌ |
| `NtYieldExecution` ⭐ | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 885 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 99 | 1525 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 8 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1040 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 6 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (57)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` ⭐ | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` ⭐ | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 4459 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` ⭐ | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` ⭐ | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASend` ⭐ | xam 0x17 | 1 | 0 | ❌ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` ⭐ | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` ⭐ | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` ⭐ | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 329 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 3 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` ⭐ | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 3 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` ⭐ | xam 0xA | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` ⭐ | xam 0x8 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 3 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` ⭐ | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 3 | ✅ |
| `XNetLogonGetTitleID` | xam 0x136 | 1 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 3 | ⚪ stub |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (56)</summary>

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
| `NtClose` | xboxkrnl 0xCF | 39 | 6793 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 10 | 6239 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 5 | 444 | ✅ |
| `NtCreateIoCompletion` ⭐ | xboxkrnl 0xD3 | 1 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 429 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 66 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 3 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 3 | 12 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 6 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 7 | 15 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 507 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 4 | 483 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 8 | 2148 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtRemoveIoCompletion` ⭐ | xboxkrnl 0xF4 | 1 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 8 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 9 | 0 | ✅ |
| `NtSetIoCompletion` ⭐ | xboxkrnl 0xF8 | 1 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 3 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 12 | 69 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 9 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 40 | 39010 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 9 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 2 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 12 | 39013 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ❌ |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ❌ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ⚪ stub |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 3 | ⚪ stub |
| `XamCacheReset` | xam 0x2B7 | 1 | 3 | ⚪ stub |
| `XamContentClose` | xam 0x25A | 5 | 6 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 8 | 12 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 5 | 3 | ✅ |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 6 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 9 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 3 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 10232 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ❌ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ❌ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ❌ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ❌ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 3 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 6 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 3 | ✅ |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 3 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 3 | ✅ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 6 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 12 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 10235 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 3 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 3 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 3 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 12 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ❌ |
| `XGetAVPack` | xam 0x3CB | 1 | 3 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 5 | 21 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 17 | 30 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ❌ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 12 | 9 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 3 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 2 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 6 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 12 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 10 | 3 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 2 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 7 | 6416 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 1 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 6 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 17 | 34259 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ❌ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ❌ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 7 | 12 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ❌ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 6 | 18 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 3 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 16 | 33 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 3 | ✅ |
| `XexLoadImageHeaders` | xboxkrnl 0x19B | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 7 | 0 | ⚪ stub |
| `EtxProducerRegister` | xboxkrnl 0x334 | 1 | 3 | ⚪ stub |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 1 | 0 | ⚪ stub |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 251 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 11 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 3 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 5 | 6 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 29 | 834 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | ❌ |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 20 | 0 | ❌ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | ❌ |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 12 | 6 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (23)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 3 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 6 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` ⭐ | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowGoldUpgradeUI` ⭐ | xam 0x6A5 | 1 | 0 | ⚪ stub |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 3 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 168058 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 3 | ✅ |
| `XamUserGetMembershipTierFromXUID` ⭐ | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` ⭐ | xam 0x218 | 1 | 0 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 33516 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 4 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 1 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 3 | 15 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (17)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 1 | 0 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 3 | ✅ |
| `XeCryptSha256Final` ⭐ | xboxkrnl 0x312 | 1 | 0 | ✅ |
| `XeCryptSha256Init` ⭐ | xboxkrnl 0x310 | 1 | 0 | ✅ |
| `XeCryptSha256Update` ⭐ | xboxkrnl 0x311 | 1 | 0 | ✅ |
| `XeCryptSha384Final` ⭐ | xboxkrnl 0x318 | 1 | 0 | ❌ |
| `XeCryptSha384Init` ⭐ | xboxkrnl 0x314 | 1 | 0 | ❌ |
| `XeCryptSha384Update` ⭐ | xboxkrnl 0x315 | 1 | 0 | ❌ |
| `XeCryptSha512Final` ⭐ | xboxkrnl 0x324 | 1 | 0 | ✅ |
| `XeCryptSha512Init` ⭐ | xboxkrnl 0x31F | 1 | 0 | ✅ |
| `XeCryptSha512Update` ⭐ | xboxkrnl 0x323 | 1 | 0 | ✅ |
| `XeCryptShaFinal` ⭐ | xboxkrnl 0x191 | 1 | 0 | ✅ |
| `XeCryptShaInit` ⭐ | xboxkrnl 0x18F | 1 | 0 | ✅ |
| `XeCryptShaUpdate` ⭐ | xboxkrnl 0x190 | 1 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 3 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 5 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 3 | 3933 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 870 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 180 | ✅ |
| `MmLockAndMapSegmentArray` ⭐ | xboxkrnl 0xC0 | 1 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 6 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmUnlockAndUnmapSegmentArray` ⭐ | xboxkrnl 0xC9 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 1568 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 1267 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 8 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 8 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 42015 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 2 | 9 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 6 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 3 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 14698 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 16753 | 🟡 |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 6 | 🟡 |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | 🟡 |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 5477 | ⚪ stub |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 13279 | 🟡 |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 3 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 3 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 24 | ✅ |

</details>

<details><summary><b>Avatares</b> (10)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGetAssets` ⭐ | xam 0x5E1 | 1 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` ⭐ | xam 0x5E0 | 1 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` ⭐ | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` ⭐ | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` ⭐ | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` ⭐ | xam 0x5DC | 1 | 3 | ⚪ stub |
| `XamAvatarLoadAnimation` ⭐ | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` ⭐ | xam 0x5F1 | 3 | 6 | ⚪ stub |
| `XamAvatarSetCustomAsset` ⭐ | xam 0x5E3 | 1 | 9 | ⚪ stub |
| `XamAvatarShutdown` ⭐ | xam 0x5DD | 1 | 3 | ⚪ stub |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 8 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 2 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 1 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 24 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | ✅ |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiControllerRequiredUI` ⭐ | xam 0x47B | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` ⭐ | xam 0x476 | 1 | 0 | 🟡 |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 2 | ⚪ stub |

</details>

<details><summary><b>Voz e microfones</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 89558 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 12 | 6545 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 29 | 18 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 6 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 3 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 15 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 3 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 8375 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 2511 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 6 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 2082 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 4 | 6 | ✅ |
| `LDIResetDecompression` ⭐ | xboxkrnl 0x2E2 | 1 | 9 | ✅ |

</details>

⭐ = só este jogo importa (o outro jogo Kinect não).

**Ausentes no Xenia (55):** `ExEventObjectType`, `ExLoadedCommandLine`, `ExThreadObjectType`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `KeAcquireSpinLockAtRaisedIrql`, `KeCertMonitorData`, `KeDebugMonitorData`, `KeDelayExecutionThread`, `KeTimeStampBundle`, `KeWaitForMultipleObjects`, `KeWaitForSingleObject`, `KfAcquireSpinLock`, `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetServerToInAddr`, `NtWaitForMultipleObjectsEx`, `NtWaitForSingleObjectEx`, `NtWriteFileGather`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlMultiByteToUnicodeN`, `RtlNtStatusToDosError`, `RtlUnicodeToMultiByteN`, `RtlUnwind`, `StfsControlDevice`, `StfsCreateDevice`, `VdGlobalDevice`, `VdGlobalXamDevice`, `VdGpuClockInMHz`, `VdHSIOCalibrationLock`, `VdSwap`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XamShowGamerCardUIForXUID`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XboxHardwareInfo`, `XboxKrnlVersion`, `XeCryptSha384Final`, `XeCryptSha384Init`, `XeCryptSha384Update`, `XeKeysConsoleSignatureVerification`, `XexExecutableModuleHandle`, `__C_specific_handler`.
