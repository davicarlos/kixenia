# Nike+ Kinect Training (título 4D53090F)
Motor tipo CryEngine; paks ZIP em `Engine/` e `Game/`; `d:\` = pasta do jogo; UI em Scaleform (vetores tesselados, glifos como listas de triângulos).

## Estado
Roda, abre todos os paks e desenha a tela de aviso (NOTICE) por software. Parada nela: falta ver se espera tempo ou entrada.

## Funções-chave (endereços no XEX descomprimido)
| Endereço | O que é |
|---|---|
| `0x825BE970` | relator de erro fatal (printf) → `0x82AC0560` → KeBugCheck(0) |
| `0x82AC0560` | encerramento/fatal |
| `0x82330578`, `0x82331754`, `0x823316C0` | alocador de blocos (mspace) e reservas de VM |
| `0x825909C8` | criação de mspace (dlmalloc); base nula derruba o jogo |
| `0x83639778`, `0x836394E8` | rasterizador de linhas do Scaleform (células 24.8) |
| `0x83639DB8` | `line_to(float,float)` com escala 256 |
| `0x82ADAAF0` | conversão de dígitos de float (CRT) |
| `0x8232EC1C` | wrapper de Sleep |

## Bugs do emulador achados com ele
1. Heap sem liberação → “Memory allocation for 135168 bytes failed”.
2. `divw` zero-estendia; Rc/CTR de 64 bits (Semantica-Xenon-32-bits).
3. Base de índices de 16 bits desalinhada (glifo “E” quebrado).
4. SLIST, TLS por thread, `NtQueryDirectoryFile` com curinga e flag de reinício na pilha.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Sports](kinect-sports.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-ID](../kinect-id.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md). Decompilação completa em `~/Kixenia/nike-decomp/`; PE para o REA em `~/Kixenia/kinect-sdk-re/games/nike_4D53090F.exe`.

## Endereços do motor de identidade no Nike (2026-10-25)
As funções-chave do BiometricSetup foram procuradas no Nike pelo Ghidra MCP com assinaturas de 64 bytes, com os saltos e endereços absolutos trocados por coringa. **As 17 foram achadas, cada uma num endereço único.** As que já eram conhecidas batem: o worker com o limite de 200 em 0x82F5E040, a gravação em 0x82F6AD68, o callback de inclinação em 0x82F53ED0 e o AdjustTilt em 0x82F53A40. O código é o mesmo runtime NUI, só que relocado.

| Etapa | BiometricSetup | Nike |
|---|---|---|
| laço de comandos do motor | 0x921A8938 | 0x82F5E990 |
| espera o quadro de identidade | 0x92196FE8 | 0x82F49798 |
| handler por quadro do motor de identidade | 0x921A8570 | 0x82F5E5C8 |
| flags de qualidade do rosto | 0x921ACA38 | 0x82F62A28 |
| extração do rosto | 0x921A5358 | 0x82F5B320 |
| projeta a cabeça na imagem colorida | 0x921AA3B0 | 0x82F61948 |
| detector de rosto | 0x921B1E18 | 0x82F67E28 |
| olho (flag 0x1000) | 0x921A9470 | 0x82F5FF78 |
| olho (flag 0x800) | 0x921A9448 | 0x82F5FF50 |
| vetor do rosto | 0x921A9280 | 0x82F5FD88 |
| guarda a amostra na fila de 500 | 0x921AB438 | 0x82F62730 |
| fila circular de amostras | 0x921AA298 | 0x82F61778 |
| reduz a fila para 200 | 0x921AB3D0 | 0x82F626C8 |
| worker: grava o cadastro com 200 amostras | 0x921A7E78 | 0x82F5E040 |
| grava o cadastro (cache e perfil) | 0x92194890 | 0x82F6AD68 |
| callback de inclinação (0x2B005..0x2B00B) | 0x921A0538 | 0x82F53ED0 |
| AdjustTilt: guarda as distâncias | 0x921A00A8 | 0x82F53A40 |

Para o teste ao vivo, estes são os pontos a observar no Nike: o retorno de **0x82F62A28** (flags de qualidade), o de **0x82F5B320** (rosto extraído) e as chamadas de **0x82F62730** (amostra guardada). Os dados ficam em `~/Kixenia/kinect-sdk-re/modulos/dados/mapa_identidade_nike.json`.

## Verificação com o decompilado completo (2026-10-25)

O jogo inteiro foi decompilado pelo Ghidra MCP: **69.052 funções**, todas exceto os thunks, em `~/Kixenia/kinect-sdk-re/modulos/decomp/Nike/` (335 MB). As chamadas foram cruzadas com o estado atual do Xenia pelo `modulos/xenia_estado.py`, que lê as macros `DECLARE_*_EXPORT` do worktree. O `modulos/achar.py` procura um padrão no código decompilado.

**O que estava errado e foi corrigido:**
- **Perfil de fitness ausente** (5125708b2). O Nike resolve 9 funções `XamFitness*` por ordinal, e sem elas a biblioteca de calorias usa 175 cm, 75 kg e 28 anos para todo jogador. Detalhes em [XAM-Fitness](../../xam/xam-fitness.md).
- **`XamNuiCameraRememberFloor`** (855a12dcc): o plano do chão vem por valor nos registros, não por ponteiro.

**Conferido e correto:**
- **`XamNuiGetDeviceStatus`:** o jogo lê os bits 0x1 (sensor), 0x2 (microfones), 0x4 e 0x40 (erros) e 0x8 (superaquecimento, consultado a cada 60 s). No xam o bit 8 vem de `FUN_817FCF90`, com temperatura acima de 89. O Xenia responde certo para um sensor saudável.
- **`XamXStudioRequest(0x1005)`:** só com `0x80070057` o jogo lê a inclinação por `Function_82F53450`. No varejo, sem XStudio, o xam devolve `0x80004005`, igual ao Xenia.
- **`MicDeviceRequest` / `RmcDeviceRequest`:** servem aos fones (dispositivos 0x60000000 e 0x50000000), não ao Kinect, e só entram com um fone conectado.
- **Chamadas dinâmicas:** festa da Live (resolvidas), `XamNuiGetTrueColorInfo` (feita) e o ordinal 0x369 (modo de vídeo 3D).
- **`ResolvePath(engine\…) failed`** (cerca de 1.200 por sessão): o CryENGINE testa caminhos relativos a `ObDosDevices` (handle 0xFFFFFFFD) antes de procurar nos `.pak`. O console também responde `C000003A`. É só ruído.

**Em aberto:**
- `XamUserGetDeviceContext` com classe 1: o código de voz do jogo trata o tipo **0x70000000** como especial (`Function_82D49F18`, `Function_82D4A878`), e o Xenia devolve o índice do usuário. No xam (0x816D8A58) é uma tabela por usuário × classe em 0x81A82FF4, e ainda não se sabe quem a preenche.
- Teste ao vivo do perfil do corpo e de um treino (o evento aparece no log como `Fitness: workout`).

## Chamadas ao sistema (análise de 2026-10-25)

> [!NOTE]
> **Seção gerada por `kinect-sdk-re/games/decompilado/scripts/gen_games.py` + `write_notes.py`, deste título até o fim da nota. Edições aqui são sobrescritas; escreva acima desta seção.**

Levantamento de **tudo o que o jogo chama do sistema**: 391 importações (163 do `xam.xex`, 228 do `xboxkrnl.exe`), 3695 chamadas diretas no código, 126 vistas em execução nas gravações (log nível 3; o log omite as funções de alta frequência). Estado no Xenia Edge (branch kinect-linux): 246 implementadas, 27 aproximadas, 57 stubs, 61 ausentes.

**Como foi feito:** o XEX carregado pelo XEXLoaderWV foi exportado para um PE (`~/Kixenia/kinect-sdk-re/games/`), lido pelo REA; as importações e os thunks vêm do log do Xenia e da tabela de importações (palavra `0x00LLOOOO`: biblioteca e ordinal); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem do projeto Ghidra com o fluxo corrigido. As funções do Kinect foram decompiladas pelo **GhidraMCP 7.0.0** (servidor headless sobre o mesmo projeto corrigido); ✅ na coluna Ghidra MCP = decompilada e com o início confirmado.

### Kinect: o runtime NUI do jogo

O jogo traz o **runtime NUI da Microsoft** embutido (rastreador de esqueleto na GPU, motor de identidade, motor de inclinação, microfones). As funções abaixo são as que falam com o sistema; a maioria é idêntica no outro jogo (mesmo tamanho e mesmas chamadas, ver [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md)).

#### Motor de inclinação do jogo

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82F530F8 (KS 0x829FF008) | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 0x2B006 0x2B00C | Motor de inclinação: parar o motor (`ElevationStopMovement`) e informar o status ao sistema (0x2B006/0x2B00C). | ✅ |
| 0x82F533A8 (KS 0x829FF318) | 132 | `XamNuiCameraElevationSetAngle` |  | Motor de inclinação: mover para um ângulo (`ElevationSetAngle`). | ✅ |
| 0x82F53450 (KS 0x829FF3C0) | 76 | `XamNuiCameraTiltGetStatus` |  | Motor de inclinação: ler o status do motor e do acelerômetro (`TiltGetStatus`, 'XtSs'). | ✅ |
| 0x82F537D8 (KS 0x829FF748) | 144 | `XamNuiCameraElevationSetAngle` |  | Motor de inclinação: mover para um ângulo (segunda variante). | ✅ |
| 0x82F53868 (KS 0x829FF7D8) | 324 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 0x2B006 0x2B00C | Motor de inclinação: guardar o plano do chão (`RememberFloor`, cerca de 25 vezes por segundo) e informar o status. | ✅ |
| 0x82F53A40 (KS 0x829FF9B0) | 532 | `XamNuiCameraElevationGetAngle` |  | Motor de inclinação: ler o ângulo atual (`ElevationGetAngle`) e decidir o próximo passo. | ✅ |
| 0x82F53ED0 (KS 0x829FFE40) | 780 | `XamNuiCameraTiltReportStatus` | 0x2B006 0x2B007 0x2B008 0x2B009 0x2B00A 0x2B00C | **Callback do motor de inclinação** registrado no sistema: recebe do xam AutoTilt (0x2B005), StopMovement (0x2B009), ReverseAutoTilt (0x2B00A) e AdjustTilt (0x2B00B) e responde com o status (0x2B006/0x2B00C). | ✅ |
| 0x82F54290 (KS 0x82A00200) | 148 | `XamNuiCameraTiltSetCallback` |  | Registra o callback do motor de inclinação no xam (`XamNuiCameraTiltSetCallback`, 0x2B007). | ✅ |

#### Câmera (PsCam)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82F476B8 (KS 0x82A01458) | 96 | `PsCamDeviceRequest` |  | O único ponto que chama `PsCamDeviceRequest`: todos os pedidos à câmera (estado, leitura de quadros, ajustes) passam por aqui. | ✅ |

#### Motor de identidade (Kinect ID)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82F59850 (KS 0x82A0E018) | 540 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` |  | Motor de identidade: abre uma sessão (`IdentityGetSessionId`) e acha o perfil de um cadastro (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82F59A70 (KS 0x82A0E238) | 432 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: usa o ID de sessão nos pedidos. | ✅ |
| 0x82F5BA00 (KS 0x82A101C8) | 356 | `XamUserNuiGetUserIndex` |  | Motor de identidade: acha o perfil ligado a um cadastro (`XamUserNuiGetUserIndex`). | ✅ |
| 0x82F5CA88 (KS 0x82A10FB0) | 388 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: usa o ID de sessão. | ✅ |
| 0x82F5CC10 (KS 0x82A11138) | 3200 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: a função principal (3,2 KB), chamada por 5 lugares; trata o resultado das identificações. | ✅ |
| 0x82F65D68 (KS 0x82A1B170) | 996 | `XamNuiIdentityGetSessionId` |  | Motor de identidade: usa o ID de sessão. | ✅ |

#### Dados biométricos (cadastro do rosto)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82F6A138 (KS 0x82A1F540) | 392 | `XamUserNuiGetEnrollmentIndex` |  | Kinect ID: carrega os rostos cadastrados do cache (lê o índice do cadastro, `XamUserNuiGetEnrollmentIndex`). | ✅ |
| 0x82F6A380 (KS 0x82A1F788) | 336 | `XamUserNuiGetUserIndex` |  | Kinect ID: acha o perfil de um cadastro. | ✅ |
| 0x82F6A600 (KS 0x82A1FA08) | 248 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` |  | Kinect ID: cache de arquivos do xam (`XamCacheOpen/Close/Reset`). | ✅ |
| 0x82F6A6F8 (KS 0x82A1FB00) | 488 | `XamReadBiometricData` |  | Kinect ID: lê os dados biométricos do perfil (`XamReadBiometricData`) e repassa ao cache (0x2A007). | ✅ |
| 0x82F6AD68 (KS 0x82A20170) | 604 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` |  | **Kinect ID: grava o cadastro do rosto**: lê a cópia do perfil, soma 1 à versão, monta os dados, grava no cache (0x2A007), liga a biometria (`XamUserNuiEnableBiometric`) e grava no perfil (`XamWriteBiometricData`). Disparada quando há 200 amostras do rosto (0x82F5E040). | ✅ |

#### Mensagens NUI ao xam (0x2C0xx e 0x2A0xx)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x838C26F0 (KS 0x82A841A8) | 60 | `XMsgSystemProcessCall` | 0x2C001 | Mensagem 0x2C001 ao xam (0x10 bytes). | ✅ |
| 0x838C2730 (KS 0x82A841E8) | 72 | `XMsgSystemProcessCall` | 0x2C003 | Mensagem 0x2C003 ao xam (0x18 bytes). | ✅ |
| 0x838C2778 (KS 0x82A84230) | 72 | `XMsgSystemProcessCall` | 0x2C004 | Mensagem 0x2C004 ao xam (0x18 bytes). | ✅ |
| 0x838C27C0 (KS 0x82A84278) | 64 | `XMsgSystemProcessCall` | 0x2C005 | Mensagem 0x2C005 ao xam (0xC bytes). | ✅ |
| 0x838C2800 (KS 0x82A842B8) | 56 | `XMsgInProcessCall` | 0x2C009 | **Registra o callback de identidade** no xam (0x2C009). | ✅ |
| 0x838C2838 (KS 0x82A842F0) | 76 | `XMsgInProcessCall` | 0x2C00C | Mensagem 0x2C00C: dados de identidade (op 1 converte os dados do Kinect ID; op 2 lê parâmetros por nome como `NuiV1QualDist`). 9 chamadores. | ✅ |
| 0x838C2888 (KS 0x82A84340) | 68 | `XMsgInProcessCall` | 0x2C00D | Mensagem 0x2C00D: resolução de identidade (qual cadastro foi reconhecido). | ✅ |
| 0x838C28D0 (KS 0x82A84388) | 80 | `XMsgStartIORequest` | 0x2A009 | Mensagem 0x2A009 (biometria desligada). | ✅ |
| 0x838C2920 (KS 0x82A843D8) | 112 | `XMsgStartIORequest` | 0x2A008 | Mensagem 0x2A008: lê o cache biométrico do perfil (versão 0 = o xam ainda não tem). | ✅ |
| 0x838C2990 (KS 0x82A84448) | 100 | `XMsgStartIORequest` | 0x2A007 | Mensagem 0x2A007: grava o cache biométrico do perfil. | ✅ |
| 0x838C29F8 (KS 0x82A844B0) | 80 | `XMsgStartIORequest` | 0x2A006 | Mensagem 0x2A006. | ✅ |
| 0x838C2A48 (KS 0x82A84500) | 112 | `XMsgStartIORequest` | 0x2A005 | Mensagem 0x2A005. | ✅ |
| 0x838C2AB8 (KS 0x82A84570) | 100 | `XMsgStartIORequest` | 0x2A004 | Mensagem 0x2A004. | ✅ |

#### Atalhos das telas Nui e mensagens 0x2B0xx

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82DA6070 | 8 | `XamShowNuiGuideUI` |  | Atalho para `XamShowNuiGuideUI` (o guia do Kinect). | ✅ |
| 0x82DA6078 | 12 | `XamShowNuiSigninUI` |  | Atalho para `XamShowNuiSigninUI`. | ✅ |
| 0x82DA6088 | 8 | `XamShowNuiGamerCardUIForXUID` |  | Atalho para `XamShowNuiGamerCardUIForXUID`. | ✅ |
| 0x82DA6090 | 8 | `XamShowNuiAchievementsUI` |  | Atalho para `XamShowNuiAchievementsUI`. | ✅ |
| 0x82DA6098 | 4 | `XamShowNuiDeviceSelectorUI` |  | Atalho para `XamShowNuiDeviceSelectorUI`. | ✅ |
| 0x82DA60E8 (KS 0x82A77580) | 56 | `XMsgSystemProcessCall` | 0x2B001 | Mensagem 0x2B001 ao xam (início do runtime NUI). | ✅ |
| 0x82DA6120 | 24 | `XMsgInProcessCall` | 0x2B002 | **Publica o quadro de esqueleto** ao xam (0x2B002); é por aqui que o sistema vê o gesto do guia e o aceno. | ✅ |
| 0x82DA6138 | 24 | `XMsgInProcessCall` |  | Mensagem 0x2B003 ao xam (inicialização do título: versões e flags). | ✅ |
| 0x82DA6150 | 24 | `XMsgInProcessCall` | 0x2B004 | Mensagem 0x2B004 ao xam (fim do título). | ✅ |
| 0x82DA6168 | 40 | `XamNuiGetDeviceStatus` |  | Lê o status do sensor (`XamNuiGetDeviceStatus`); 8 chamadores. | ✅ |
| 0x82DA6520 (KS 0x82A74DE0) | 40 | `XMsgCancelIORequest` |  |  | ✅ |
| 0x82DA6DF0 | 264 | `XMsgCompleteIORequest`, `XMsgInProcessCall` |  |  | ✅ |

#### Sensor, microfones e número de série

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82E038E0 | 108 | `XamNuiGetDeviceSerialNumber`, `XamVoiceGetMicArrayStatus` |  | Lê o número de série e o estado da matriz de microfones. | ✅ |
| 0x82E039A8 | 124 | `XamNuiGetDeviceStatus` |  | Lê o status do sensor. | ✅ |
| 0x82F34B18 | 224 | `XamNuiGetDeviceSerialNumber` |  | Lê o número de série do sensor. | ✅ |
| 0x82F34D00 | 1400 | `XAudioCaptureRenderDriverFrame`, `XamVoiceGetMicArrayAudioEx`, `XamVoiceGetMicArrayUnderrunStatus` |  | **Worker dos microfones do Kinect**: lê o áudio da matriz (`XamVoiceGetMicArrayAudioEx`), a referência do que o jogo tocou (`XAudioCaptureRenderDriverFrame`, para cancelar o eco) e as faltas de dados; alimenta o reconhecedor de voz. | ✅ |
| 0x82F36688 | 1516 | `XamVoiceGetMicArrayAudioEx` |  | Lê o áudio da matriz de microfones. | ✅ |

#### Chat de voz

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82D49F18 (KS 0x82C34980) | 200 | `XamVoiceIsActiveProcess` |  | Chat de voz: abaixa o som do jogo durante o bate-papo (`XamVoiceIsActiveProcess`). | ✅ |
| 0x82D4B1B0 (KS 0x82C35C18) | 928 | `XamVoiceHeadsetPresent`, `XamVoiceIsActiveProcess` |  | Chat de voz: fone presente e voz ativa. | ✅ |
| 0x82D4B7F0 (KS 0x82C36258) | 316 | `XMsgCancelIORequest`, `XamVoiceClose` |  | Chat de voz: fecha o objeto de voz. | ✅ |
| 0x82D4BB80 (KS 0x82C365E8) | 600 | `XamVoiceSubmitPacket` |  | Chat de voz: envia pacotes de voz (`XamVoiceSubmitPacket`). | ✅ |
| 0x82D4BEE0 (KS 0x82C36948) | 240 | `XMsgCancelIORequest`, `XamVoiceClose` |  | Chat de voz: fecha o objeto de voz. | ✅ |
| 0x82D4BFD0 (KS 0x82C36A38) | 192 | `XamVoiceClose`, `XamVoiceCreate` |  | Chat de voz: cria e fecha o objeto de voz. | ✅ |
| 0x82D4EF48 (KS 0x82C398C0) | 688 | `XamVoiceIsActiveProcess` |  | Chat de voz: estado da voz. | ✅ |
| 0x82D51370 (KS 0x82C3BCE8) | 208 | `XamVoiceSubmitPacket` |  | Chat de voz: envia pacotes. | ✅ |

#### Serviços da Live (app 0xFB)

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82A50930 (KS 0x82D29E30) | 248 | `XMsgStartIORequest` | 0xB001E | envia 0xB001E | ✅ |
| 0x82A779B0 (KS 0x82D28F38) | 432 | `XMsgStartIORequest` | 0xB0010 | envia 0xB0010 | ✅ |
| 0x82A77B60 (KS 0x82D290E8) | 188 | `XMsgStartIORequest` | 0xB0018 | envia 0xB0018 | ✅ |
| 0x82A77C20 (KS 0x82D291A8) | 176 | `XMsgStartIORequest` | 0xB0011 | envia 0xB0011 | ✅ |
| 0x82A77CD0 (KS 0x82D29258) | 172 | `XMsgStartIORequest` | 0xB0012 | envia 0xB0012 | ✅ |
| 0x82A77D80 (KS 0x82D29308) | 172 | `XMsgStartIORequest` | 0xB0012 | envia 0xB0012 | ✅ |
| 0x82A77E30 (KS 0x82D293B8) | 168 | `XMsgStartIORequest` | 0xB0013 | envia 0xB0013 | ✅ |
| 0x82A77ED8 (KS 0x82D29460) | 168 | `XMsgStartIORequest` | 0xB0013 | envia 0xB0013 | ✅ |
| 0x82A77F80 (KS 0x82D29508) | 220 | `XMsgStartIORequest` | 0xB001A | envia 0xB001A | ✅ |
| 0x82A78060 (KS 0x82D295E8) | 156 | `XMsgStartIORequest` | 0xB0014 | envia 0xB0014 | ✅ |
| 0x82A78100 (KS 0x82D29688) | 176 | `XMsgStartIORequest` | 0xB0015 | envia 0xB0015 | ✅ |
| 0x82A781B0 (KS 0x82D29820) | 232 | `XMsgStartIORequest` | 0xB001C | envia 0xB001C | ✅ |
| 0x82A78298 (KS 0x82D29908) | 220 | `XMsgStartIORequest` | 0xB001B | envia 0xB001B | ✅ |
| 0x82A78378 (KS 0x82D299E8) | 192 | `XMsgStartIORequest` | 0xB0025 | envia 0xB0025 | ✅ |

#### Outras mensagens XMsg

| Função do jogo | Tamanho | Chama | Mensagens | O que faz | Ghidra MCP |
|---|---|---|---|---|---|
| 0x82640698 (KS 0x82D283E0) | 352 | `XMsgInProcessCall` |  |  | ✅ |
| 0x826407F8 (KS 0x82D28540) | 304 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82640928 (KS 0x82D28670) | 752 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82640C18 | 572 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82640E58 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` |  |  | ✅ |
| 0x8264D7E8 | 68 | `XMsgInProcessCall` |  |  | ✅ |
| 0x8264D990 (KS 0x82D2A130) | 252 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82A46BC0 (KS 0x82D29DE8) | 72 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82A46C08 | 88 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82ADE5F0 | 124 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82ADE828 | 100 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82ADE890 (KS 0x82D27BE0) | 168 | `XMsgStartIORequestEx` |  |  | ✅ |
| 0x82ADE938 (KS 0x82D27C88) | 104 | `XMsgInProcessCall` |  |  | ✅ |
| 0x82D9D348 | 232 | `XMsgStartIORequest` |  |  | ✅ |
| 0x82D9D498 | 4 | `XamShowSigninUI` |  |  | ✅ |
| 0x82DA4780 (KS 0x82A74960) | 120 | `XMsgStartIORequest` | 0xB0008 | envia 0xB0008 | ✅ |
| 0x82DA47F8 (KS 0x82A74A50) | 140 | `XMsgStartIORequest` | 0xB0006 | envia 0xB0006 | ✅ |
| 0x82DA4890 (KS 0x82A74AE8) | 184 | `XMsgStartIORequest` | 0xB0007 | envia 0xB0007 | ✅ |
| 0x82DA49F8 (KS 0x82A74C50) | 216 | `XMsgStartIORequest` | 0xB0021 | envia 0xB0021 | ✅ |
| 0x82F554F8 | 16 | `XamShowNuiTroubleshooterUI` |  | Abre o Troubleshooter do Kinect (`XamShowNuiTroubleshooterUI`). | ✅ |
| 0x82F907D0 | 228 | `XMsgInProcessCall` |  |  | ✅ |
| 0x8367D4B8 | 204 | `XMsgStartIORequest` |  |  | ✅ |

**Importações do Kinect e do runtime NUI** (chamadas no código / vistas em execução):

| Importação | Biblioteca, ordinal | Chamadas | Em execução | O que é | Xenia |
|---|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 17288 | pedidos à câmera do Kinect (estado, quadros, ajustes) | 🟡 |
| `XAudioCaptureRenderDriverFrame` | xboxkrnl 0x322 | 1 | 0 | o último quadro de áudio que o jogo tocou (referência para cancelar o eco) | 🟡 |
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | cancela uma mensagem assíncrona | ✅ |
| `XMsgCompleteIORequest` | xam 0x1F5 | 1 | 0 | completa uma mensagem assíncrona | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 17 | 12559 | mensagem no processo a um app do xam | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 30 | 39 | mensagem assíncrona a um app (0xFB Live, 0xFE xam) | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 6 | idem, com mais opções | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | mensagem síncrona a um app do sistema | ✅ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | fecha um arquivo do cache | ⚪ stub |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 3 | abre um arquivo no cache do xam | ⚪ stub |
| `XamCacheReset` | xam 0x2B7 | 1 | 3 | limpa o cache | ⚪ stub |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | lê o status ('XtSs', 0x50 bytes), atan2 do acelerômetro × 57,3 + deslocamento; também o estado (&3) | 🟡 |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 6 | FUN_8180C700(0xDEADBEEF, ângulo − 0x81AAC9D0): Detroit 0x33148/0x3314C ou Mca | 🟡 |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | Detroit {0x3304} ou Mca {0x24}, depois 0xFE 0x2B009 ao motor do título | 🟡 |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 6466 | guarda o plano do chão (4 floats em 0x81AAC9E0) e lê o status do motor (0x48 bytes) | ⚪ stub |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 13822 | status 'XtSs' de 0x50 bytes a partir do bloco de 0x48 do motor (Detroit 0xF0410, 0x45 bytes; Mca código 0x23) | 🟡 |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | o título informa 0x2B006 (0x10) ou 0x2B00C (0xE0) → callback de 0x81A85C20 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 3 | 0xFE 0x2B007: registra o motor de inclinação do título (0x81A85C40) | ✅ |
| `XamNuiGetDeviceSerialNumber` | xam 0x8DD | 2 | 0 | Detroit 0x770D4 (0x20 bytes) ou Mca 0x2B, até 3 tentativas | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 2 | 18315 | FUN_817FC750 (estado, NTSTATUS do sensor e flags); sensor desligado: 0xC000009D | ⚪ stub |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 9 | 8 bytes aleatórios, criados na primeira chamada | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 3 | XMsg 0xFB 0xB0076 (Live); até 0x32000 bytes | ✅ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | NuiHudCheck + conquistas (0x21008) | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | NuiHudCheck + seletor de dispositivo (modo Kinect) | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | NuiHudCheck + 0x21032 (Live) | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | NuiHudCheck + XamAppRequestLoadEx(4) + 0x21030 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | NuiHudCheck + login (FUN_816C1868) | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | sem sensor: caixa de mensagem (textos 0x11F/0x4F); com sensor: engaja e abre o Tuner (0x21028), modo 2, 3 ou 5 | ⚪ stub |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | tela comum | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | configuração de perfil 0x1004004E; desligar apaga os dados e envia 0x2A009 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | +0x34 se +0x30, senão −2 / 0x80070525 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 24 | o usuário com aquele cadastro | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | voz (kernel XVoiced*) | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | voz (kernel XVoiced*) | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` | xam 0x490 | 3 | 0 | kernel XVoiced* | 🟡 |
| `XamVoiceGetMicArrayStatus` | xam 0x318 | 1 | 0 | 2 sem matriz; senão o sinal de 0x81AACBF8 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` | xam 0x48E | 1 | 0 | McaDeviceRequest código 0x20 → falta de dados (bits 0/1) e contagem | 🟡 |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | kernel XVoiced* | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | kernel XVoiced* | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | kernel XVoiced* | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | XMsg 0xFB (Live); tamanho 0 apaga | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (70)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAcquireReadWriteLockExclusive` ⭐ | xboxkrnl 0x7 | 1 | 0 | ✅ |
| `ExAcquireReadWriteLockShared` ⭐ | xboxkrnl 0x8 | 6 | 0 | ✅ |
| `ExCreateThread` | xboxkrnl 0xD | 4 | 159 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ❌ |
| `ExInitializeReadWriteLock` ⭐ | xboxkrnl 0x11 | 1 | 0 | ✅ |
| `ExReleaseReadWriteLock` ⭐ | xboxkrnl 0x16 | 7 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 27 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ❌ |
| `InterlockedPopEntrySList` ⭐ | xboxkrnl 0x2C | 4 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ❌ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCancelTimer` ⭐ | xboxkrnl 0x54 | 1 | 3 | ✅ |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ❌ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ❌ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ❌ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 60 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 3 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 6 | ✅ |
| `KeInitializeMutant` | xboxkrnl 0x72 | 1 | 3 | ✅ |
| `KeInitializeSemaphore` ⭐ | xboxkrnl 0x74 | 1 | 3 | ✅ |
| `KeInitializeTimerEx` ⭐ | xboxkrnl 0x75 | 1 | 3 | ✅ |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 3 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 2 | 0 | ✅ |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | ✅ |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 48 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 1818 | ✅ |
| `KeReleaseMutant` | xboxkrnl 0x87 | 35 | 14060 | ✅ |
| `KeReleaseSemaphore` ⭐ | xboxkrnl 0x88 | 1 | 54 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 25 | 15609 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 6085 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 3 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 6085 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 141 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 6 | 84 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 101 | 0 | ✅ |
| `KeSetTimer` ⭐ | xboxkrnl 0xA6 | 3 | 21 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ❌ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 6 | 15 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 3 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 23 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 23 | 3116665 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | ✅ |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 6 | 0 | ❌ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 28 | 0 | ❌ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 45 | 0 | ❌ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 47 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 6 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 19968 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 111 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` ⭐ | xboxkrnl 0xFB | 1 | 0 | ❌ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ❌ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 12 | 0 | ❌ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 915 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 163 | 19315 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 7 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 1028 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 6102 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 1764 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 5 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (62)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` ⭐ | xboxkrnl 0x21 | 1 | 3 | ⚪ stub |
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
| `NtClose` | xboxkrnl 0xCF | 36 | 1784 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 6 | 288 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 7 | 1721 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 411 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 69 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 3 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 11 | 21824 | ✅ |
| `NtOpenSymbolicLinkObject` ⭐ | xboxkrnl 0xE0 | 1 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 903 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 12 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 5 | 609 | ✅ |
| `NtQuerySymbolicLinkObject` ⭐ | xboxkrnl 0xEC | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 2 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 6 | 30 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 4 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 2 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 16 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 14 | 0 | ✅ |
| `NtWriteFileGather` | xboxkrnl 0x100 | 1 | 0 | ❌ |
| `ObCreateObject` | xboxkrnl 0x102 | 1 | 12 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 51 | 20475 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 12 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 4 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 16 | 20475 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ❌ |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ❌ |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ⚪ stub |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 3 | ⚪ stub |
| `XamCacheReset` | xam 0x2B7 | 1 | 3 | ⚪ stub |
| `XamContentClose` | xam 0x25A | 9 | 9 | ✅ |
| `XamContentCreateDeviceEnumerator` ⭐ | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 3 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentCreateInternal` ⭐ | xam 0x271 | 3 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentDeleteInternal` ⭐ | xam 0x270 | 1 | 0 | ✅ |
| `XamContentFlush` ⭐ | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetCreator` ⭐ | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 57 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentOpenFile` | xam 0x269 | 10 | 24 | 🟡 |
| `XamContentSetThumbnail` ⭐ | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Rede</b> (48)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 27 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 3 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` ⭐ | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 3 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 6145 | ✅ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 3 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` ⭐ | xam 0x40 | 1 | 3 | ✅ |
| `NetDll_XnpLogonGetStatus` ⭐ | xam 0x70 | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 3 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 3 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 3 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 3 | ✅ |
| `XNetLogonGetTitleID` | xam 0x136 | 2 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 2 | 6 | ⚪ stub |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 2 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 2 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 2 | 0 | ❌ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (32)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` ⭐ | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 3 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 6 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowAchievementsUI` ⭐ | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` ⭐ | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` ⭐ | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` ⭐ | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceDownloadItemsUI` ⭐ | xam 0x2E7 | 1 | 0 | 🟡 |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` ⭐ | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` ⭐ | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` ⭐ | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` ⭐ | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetAgeGroup` ⭐ | xam 0x4A5 | 1 | 0 | ✅ |
| `XamUserGetCachedUserFlags` ⭐ | xam 0x222 | 1 | 0 | ✅ |
| `XamUserGetDeviceContext` | xam 0x208 | 2 | 0 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 15 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 73758 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 5 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 4 | 3 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 6 | 0 | ✅ |
| `XamUserWriteProfileSettings` ⭐ | xam 0x21A | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (29)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 3 | 3 | ✅ |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 1 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 6 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 6 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 3 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 8598 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ❌ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ❌ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ❌ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ❌ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 3 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 6 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 3 | ✅ |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 3 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 3 | ✅ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 4 | 12 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 8601 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 3 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 3 | ⚪ stub |
| `VdSetDisplayModeOverride` | xboxkrnl 0x1D4 | 2 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 3 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 12 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ❌ |
| `XGetAVPack` | xam 0x3CB | 1 | 3 | ⚪ stub |
| `XGetVideoCapabilities` ⭐ | xam 0x49E | 1 | 0 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 5 | 21 | 🟡 |
| `XamReadTileToTexture` ⭐ | xam 0x2F5 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Sistema e console</b> (28)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 15 | 13818 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ❌ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 15 | 9 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 2 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 0 | ⚪ stub |
| `XamGetActiveDashAppInfo` | xam 0x7E1 | 6 | 6 | ✅ |
| `XamGetCurrentTitleId` | xam 0x1CF | 16 | 3 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 2 | 0 | ✅ |
| `XamGetOverlappedResult` ⭐ | xam 0x1FB | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 2 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 8 | 6119 | ✅ |
| `XamIsUIActive` | xam 0x2EC | 2 | 0 | ✅ |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 0 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 17 | 35517 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ❌ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ❌ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 8 | 3 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ❌ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 8 | 51 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 12 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 13 | 54 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 3 | ✅ |
| `XexLoadImageHeaders` | xboxkrnl 0x19B | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 27 | 3 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 14 | 0 | ⚪ stub |
| `EtxProducerRegister` | xboxkrnl 0x334 | 3 | 6 | ⚪ stub |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 3 | 0 | ⚪ stub |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 11 | 0 | ✅ |
| `RtlCopyString` ⭐ | xboxkrnl 0x121 | 1 | 0 | ✅ |
| `RtlDowncaseUnicodeChar` ⭐ | xboxkrnl 0x124 | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 3 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 7 | 3 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 38 | 23563 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | ❌ |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 27 | 0 | ❌ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 111 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | ❌ |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 4 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 21 | 9 | ✅ |
| `_snwprintf` ⭐ | xboxkrnl 0x13D | 1 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` ⭐ | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 3 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 3 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 11 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 1184 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 18 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 247 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 5 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` ⭐ | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 25941 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 20685 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 8 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 9 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioEnableDucker` | xboxkrnl 0x34D | 1 | 0 | ⚪ stub |
| `XAudioGetDuckerAttackTime` | xboxkrnl 0x353 | 1 | 0 | ❌ |
| `XAudioGetDuckerHoldTime` | xboxkrnl 0x357 | 1 | 0 | ❌ |
| `XAudioGetDuckerLevel` | xboxkrnl 0x350 | 2 | 0 | ❌ |
| `XAudioGetDuckerReleaseTime` | xboxkrnl 0x355 | 1 | 0 | ❌ |
| `XAudioGetDuckerThreshold` | xboxkrnl 0x351 | 4 | 0 | ❌ |
| `XAudioGetRenderDriverTic` ⭐ | xboxkrnl 0x34C | 1 | 0 | 🟡 |
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 3 | 9 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 6 | ✅ |
| `XAudioRegisterRenderDriverMECClient` ⭐ | xboxkrnl 0x320 | 1 | 0 | 🟡 |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverMECClient` ⭐ | xboxkrnl 0x321 | 1 | 0 | 🟡 |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 17288 | 🟡 |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 1 | 0 | 🟡 |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 6 | 🟡 |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | 🟡 |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 6466 | ⚪ stub |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 1 | 13822 | 🟡 |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 3 | ✅ |
| `XamNuiGetDeviceSerialNumber` ⭐ | xam 0x8DD | 2 | 0 | ✅ |
| `XamNuiGetDeviceStatus` ⭐ | xam 0x885 | 2 | 18315 | ⚪ stub |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 3 | 24 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `MicDeviceRequest` ⭐ | xboxkrnl 0x2B1 | 3 | 0 | ⚪ stub |
| `RmcDeviceRequest` ⭐ | xboxkrnl 0x2E1 | 4 | 0 | ❌ |
| `XAudioCaptureRenderDriverFrame` ⭐ | xboxkrnl 0x322 | 1 | 0 | 🟡 |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceGetMicArrayAudioEx` ⭐ | xam 0x490 | 3 | 0 | 🟡 |
| `XamVoiceGetMicArrayStatus` ⭐ | xam 0x318 | 1 | 0 | ✅ |
| `XamVoiceGetMicArrayUnderrunStatus` ⭐ | xam 0x48E | 1 | 0 | 🟡 |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceIsActiveProcess` | xam 0x497 | 3 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 19 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 6 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 33 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 2 | 0 | ✅ |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` ⭐ | xam 0x46E | 1 | 0 | ✅ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | 🟡 |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 3 | 0 | ✅ |
| `XMsgCompleteIORequest` ⭐ | xam 0x1F5 | 1 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 17 | 12559 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 30 | 39 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 6 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 0 | ✅ |

</details>

<details><summary><b>Câmera Xbox Live Vision</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XUsbcamCreate` ⭐ | xboxkrnl 0x222 | 1 | 0 | ⚪ stub |
| `XUsbcamDestroy` ⭐ | xboxkrnl 0x223 | 1 | 0 | ❌ |
| `XUsbcamGetState` ⭐ | xboxkrnl 0x21D | 1 | 0 | ⚪ stub |
| `XUsbcamReadFrame` ⭐ | xboxkrnl 0x21E | 1 | 0 | ❌ |
| `XUsbcamSetCaptureMode` ⭐ | xboxkrnl 0x21A | 1 | 0 | ❌ |
| `XUsbcamSetConfig` ⭐ | xboxkrnl 0x21C | 3 | 0 | ❌ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 1 | 3 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 24556 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 1 | 6139 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 9 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 3 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 3 | 33 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 4 | 2145 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 3 | 27 | ✅ |

</details>

⭐ = só este jogo importa (o outro jogo Kinect não).

**Ausentes no Xenia (61):** `ExEventObjectType`, `ExLoadedCommandLine`, `ExThreadObjectType`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `KeAcquireSpinLockAtRaisedIrql`, `KeCertMonitorData`, `KeDebugMonitorData`, `KeDelayExecutionThread`, `KeTimeStampBundle`, `KeWaitForMultipleObjects`, `KeWaitForSingleObject`, `KfAcquireSpinLock`, `NetDll_XNetServerToInAddr`, `NetDll_XnpLogonGetStatus`, `NtSignalAndWaitForSingleObjectEx`, `NtWaitForMultipleObjectsEx`, `NtWaitForSingleObjectEx`, `NtWriteFileGather`, `ObIsTitleObject`, `RmcDeviceRequest`, `RtlCaptureContext`, `RtlMultiByteToUnicodeN`, `RtlNtStatusToDosError`, `RtlUnicodeToMultiByteN`, `RtlUnwind`, `StfsControlDevice`, `StfsCreateDevice`, `VdGlobalDevice`, `VdGlobalXamDevice`, `VdGpuClockInMHz`, `VdHSIOCalibrationLock`, `VdSwap`, `XAudioGetDuckerAttackTime`, `XAudioGetDuckerHoldTime`, `XAudioGetDuckerLevel`, `XAudioGetDuckerReleaseTime`, `XAudioGetDuckerThreshold`, `XUsbcamDestroy`, `XUsbcamReadFrame`, `XUsbcamSetCaptureMode`, `XUsbcamSetConfig`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XboxHardwareInfo`, `XboxKrnlVersion`, `XeKeysConsoleSignatureVerification`, `XexExecutableModuleHandle`, `__C_specific_handler`.
