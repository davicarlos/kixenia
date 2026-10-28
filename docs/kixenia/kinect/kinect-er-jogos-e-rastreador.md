# Kinect, engenharia reversa: nike, Kinect Sports, o rastreador na GPU e a correção de GPU

Parte de [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 5, 6, 8, 12, 16).

## 5. Nike+ Kinect Training (4D53090F)

### Inclinação
- O módulo fica em 0x82F53000 a 0x82F55500.
- `XamNuiCameraTiltSetCallback` registra o tratador 0x82F53ED0, que atende as mensagens 0x2B005 (inclinação automática), 0x2B00A (reverter) e 0x2B00B (ajustar).
- **Máquina de estados:** inclina a −25°, chama `RememberFloor` e calcula o ângulo em `Function_82F534A0`.
  - Sem calibração, o chão global (0x83B0449C) continua 0 e o resultado é 0°.
  - Com o flag "calibrado" no `XCONFIG_CONSOLE_NUI`, o Nike vai direto ao ângulo salvo.

### Voz
- O worker 0x82F353F0 roda o próprio MEC (`FUN_82f38af8`) sobre os 4 microfones crus mais a referência de eco.
- A saída de 16 bits fica em `*0x83ACEF10`.
- O callback 0x82E07D50 só repassa ao reconhecedor quando o objeto de fala (`*0x83ACDFB4`) tem +0x34 == 3.
- O conversor 0x82F3A9C8 lê int32 big-endian e corta em 0x79999998, o que confirma o formato.

### Câmera
- Configurações em [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md).
- No `PsCamDeviceRequest` de leitura (código 5), a descrição +72 é o contador de quadros. O jogo aceita profundidade de 0x96000 bytes (desempacotada) ou 0x67200 bytes (empacotada).

### Kinect ID
- O próprio jogo cadastra os rostos e os guarda como dados biométricos ([Kinect-ID](kinect-id.md)).

### Menu
- Responde ao braço direito estendido para o lado, não acima da cabeça.

## 6. Correção da GPU que liberou o rastreamento

- **Sintoma:** com a câmera funcionando, o esqueleto saía sempre "0 rastreados".
- **Causa:**
  - Os draws de cópia sem vértices em vf0 recebem os retângulos do interpretador de shader da CPU.
  - O interpretador precisa chamar `ProvideResolveOutputForGpuWrite` antes de ler texels.
  - Essa chamada pode fechar a submissão que o `IssueCopy` abriu, e as cópias iam para um command buffer fechado.
  - Com isso, a saída do classificador (0x1E1E5000) ficava vazia e os pesos zerados.
- **Correção:** `BeginSubmission(true)` depois do *provide* (commit da9870cc1).
- **Dica de medição:** um dump pela CPU da saída de um resolve só vale depois que o resolve é submetido.

## 8. Rastreador na GPU (Kinect Sports e Nike)

- **Entrada:** a GPU recebe o anel de profundidade como uma textura 1D larga `k_16_16_16_16` no endereço físico 0x1E430000, recarregada a cada quadro.
- **Saídas que a CPU lê de volta:**
  - pirâmide de 80×32 a 8×8, entre 0x1E12E000 e 0x1E174000;
  - classificador de 640×240 em 0x1E1E5000.
- **O que foi preciso no Edge:**
  - manter os draws multipasso com memexport no pixel shader;
  - resolve sem vértices em vf0;
  - busca de textura no interpretador;
  - e a correção da submissão (seção 6).
- **Banco do rastreador:** o `Database.xmplr` descomprimido (LDI) tem 22.614.016 bytes, iguais no Edge e no código do knuckleslee.
- **Descartados como causa da falta de rastreamento:** espelhamento, `render_target_path=accuracy` e `enable_host_buffer=false`.

## 12. Kinect Sports (4D5308C9)

- A imagem foi despejada com o carregador XEX do Ghidra (XEXLoaderWV) e `DumpImage.java`, em `ks/ks.bin` no scratchpad.
- Os wrappers de XMsg ficam em 0x82A841A8 a 0x82A84570:
  - 0x2C001 (0x10 bytes), 0x2C003 e 0x2C004 (0x18), 0x2C005 (0xC);
  - 0x2C009 (callback, tamanho 0);
  - 0x2C00C;
  - **0x2C00D**;
  - 0x2A004 a 0x2A009.
- **0x2C00D** (`82A84340`), chamada por `82A0F2B0`, é a **resolução de identidade**:
  - o jogo compara os rostos com até 8 cadastros;
  - envia `{pontuações (0x128 bytes), 0, 0x111148 (8 bytes), &resultado, 0x128}` com tamanho 0;
  - o xam responde o cadastro reconhecido (0 a 7), 0xFFFFFFFE (ninguém) ou 0xFFFFFFFD.
  - O Xenia não tratava a mensagem (85 avisos em 150 s). Agora responde 0xFFFFFFFE (commit 7ae9c41d5).
- **Imports que o Nike não tem:** `XamShowNuiGuideUI` (o guia por gestos), `XamShowNuiControllerRequiredUI`, `XamShowNuiGamerCardUIForXUID` e `XamShowNuiMarketplaceUI`.
- **Falta no Xenia:** `IoDismountVolumeByFileHandle`, que não é do Kinect.

### Kinect Guide
- **No xam:** `XamShowNuiGuideUI(tracking_id, p2)` (0x816C3590) chama `FUN_816c1808`, que é o NuiHudCheck: `XamNuiHudIsEnabled`, senão 5; depois `SetEngagedTrackingID`, senão 0x65B. Em seguida chama `XamAppRequestLoadEx(4)`, que devolve 5 se falhar, e `XMsgSystemProcessCall(0xFE, 0x21030, {p1, p2, 0}, 0xC)`. O resultado passa por `FUN_816c16c8` (0, ou 0xE / 0x65B), e a função volta sem esperar o guia fechar.
- `XamShowGuideUI` é igual, com o app 1 e a mensagem 0x21015.
- **No Kinect Sports:** o wrapper 0x82A74D50 faz `li r4,0xFF; b XamShowNuiGuideUI`. É chamado pelo gesto de pausa e pela opção do menu do jogo (`param_2 == 4`).
- **No Xenia:** abre o menu do botão Guide e envia `XN_SYS_UI` true/false (commit 4c55c90b3).

## 16. Varredura dos dois jogos (2026-10-24)

Método:
- imports marcados `!!` no log do Xenia;
- `undefined extern call`;
- mensagens XMsg não implementadas;
- buscas dinâmicas (`XexGetProcedureAddress`) que falham;
- funções `kStub`/`kSketchy` chamadas, cruzadas com uma execução de 100 s em log nível 3.

**Do Kinect, chamadas e que faltavam:**

| Função | Jogo | Situação |
|---|---|---|
| `XamNuiGetDeviceSerialNumber` | Nike | implementada (91ac58116) |
| XMsg 0x2C00D | Kinect Sports | implementada (7ae9c41d5) |
| `XamVoiceSetMicArrayIdleUsers` (0x48C, busca dinâmica) | Kinect Sports | implementada (35019b0a4). Guarda a máscara de usuários ociosos e devolve 0 (xam 0x816FB9C0) |
| `XamNuiGetTrueColorInfo` (0x8DF, busca dinâmica) | os dois | **não implementada de propósito**: é o PsCam código 46 (0x2E) com um seletor, e a resposta do driver não é conhecida. Os jogos seguem sem ela, como num painel antigo |
| `XamNuiGetCameraIntrinsics` | nenhum dos dois | também usa o código 46, com o seletor 0x100 |

**Do Kinect, importadas mas não chamadas nos testes** (dependem de ação do jogador):
- `XamShowNuiControllerRequiredUI` e `XamShowNuiMarketplaceUI` (Kinect Sports);
- `XamShowNuiAchievementsUI` (Nike);
- `XamShowNuiGamerCardUIForXUID` (os dois).

**Verificadas e certas como estão:**
- `XamVoiceIsActiveProcess` → `XVoicedIsActiveProcess` do kernel (0x35C). No Kinect Sports só controla o abaixamento do som durante bate-papo de voz (82C34980 e outras); 0 está certo.
- `XamNuiCameraRememberFloor`: o Kinect Sports chama cerca de 25 vezes por segundo, e o Nike também. Responde sucesso; a inclinação automática é a do Xenia.
- `XamNuiCameraTiltSetCallback` só guarda o callback.

**Fora do Kinect** (não afetam o sensor):
- rede e Live: `NetDll_WSARecv`/`WSASend`, `XamFitnessInitialize` (Live 0xB0080; ver [XAM-Fitness](../xam/xam-fitness.md)), `XamXlfs*`;
- armazenamento: `Stfs*`, `IoDismountVolumeByFileHandle`, `XFileXctdCompressionInformation`;
- outros: `XAudioGetDucker*`, `XeCryptSha384*`;
- **Nike:** `XUsbcam*`, da câmera Xbox Live Vision e não do Kinect, e `RmcDeviceRequest`.
