# Kinect Adventures (título 4D5308ED)

Kinect Adventures!, da Good Science Studio (Microsoft), o jogo que vinha com o Kinect. Executável `Default.xex` (26 MB) em `~/Downloads/Xbox360_extraido/Kinect Adventures [Kinect]/`, com `Database.xmplr` (modelos do rastreador), `NuiIdentity.bin.be` (identidade), `AvatarAssetPack` e `AvatarAwards`. Motor: Unreal Engine 3 (`SpringfieldGame`, `CookedXenon`). Runtime NUI **2.0.11775.7**, mais antigo que o do Nike e do Kinect Sports (2.0.21173.0).

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Sports](kinect-sports.md), [Nike-Kinect-Training](nike-kinect-training.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md), [Kinect-ID](../kinect-id.md).

## Estado no Xenia Edge

> [!WARNING]
> **Ainda não jogável com o Kinect real: o jogo entra na tela de ajuste do sensor e repete a varredura de inclinação sem achar o chão e o jogador.**

| Etapa | Situação | Detalhe |
|---|---|---|
| Abrir, logos, menu | ✅ | Com o Kinect real e com gravações. Rastreia o jogador no menu. |
| Entrar no menu sem travar | ✅ corrigido | Ver *Crash em `XamNuiCameraElevationGetAngle`* abaixo. |
| Aviso "o sensor foi movido" | ✅ corrigido (a confirmar ao vivo) | Ver *Ruído do acelerômetro* abaixo. |
| Tela de ajuste do sensor (busca do chão) | ❌ | Repete −25° → −17° → −7° → 3° → (alvo) e recomeça. O chão vem do rastreador; com o jogador quase nunca rastreado durante a busca, não termina. |
| Rastreamento | 🟡 | 27% das amostras com o jogador rastreado na gravação `kinect_rec_aovivo2` (o Kinect Sports tem 55% na mesma gravação). Ao vivo, quase só "0 tracked" durante a busca. |
| Microfones | — | O jogo não importa nenhuma função de microfone do Kinect (`XamVoiceGetMicArray*`, `McaDeviceRequest`): não usa comandos de voz. |
| Kinect ID (rostos) | 🟡 | `XamCacheOpenFile`/`XamCacheReset`/`XamCacheCloseFile` ausentes: o cache dos rostos falha (16 chamadas a `XamCacheOpenFile` nos logs). |
| Avatares | ⚪ | Toda a API de avatares (`XamAvatar*`, 12 funções) é stub no Xenia. |

### Como rodar
```
cd ~/Kixenia/kinect-tools
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema ./live_test.sh <log> "$HOME/Downloads/Xbox360_extraido/Kinect Adventures [Kinect]/Default.xex"
XENIA_DIR=~/Kixenia/xenia-edge-kinect-sistema ./replay_test.sh ~/kinect_rec_aovivo2 <log> 180 "<mesmo xex>"
```

## Problemas achados e corrigidos (2026-10-26)

### Crash em `XamNuiCameraElevationGetAngle` (loading infinito)
O jogo chama `XamNuiCameraElevationGetAngle` ao entrar no menu (o Nike nunca chama). O Xenia montava o parâmetro de `XamNuiCameraTiltGetStatus_entry` com `pointer_t<X_NUI_TILT_STATUS>(status_ptr)`; `pointer_t<T>` é `const TypedPointerParam<T>&`, uma **referência**, então o "cast" reinterpretava o inteiro na pilha como o objeto: guest `00000001`, host `7018EE687018EE68` (lixo). A leitura derrubava a thread principal ("Host-side crash … read at 0x0", uma General Protection Fault de endereço não canônico) e o jogo ficava carregando. Correção em `xam_nui.cc`: `shim::TypedPointerParam<X_NUI_TILT_STATUS>(status)`.

### Ruído do acelerômetro ("o sensor foi movido")
O ângulo que o xam devolve vem de `atan2(gravidade z, gravidade y)`, igual no console (0x816FAF68). O Xenia usava o acelerômetro **bruto** do bloco de status 0x8032 do firmware (palavras 4 a 6), que oscila de −2° a +2° com o sensor parado; o jogo tolera **2,0°** (0x82D2BCF0, 0x82D2CD20) e reiniciava o ajuste. O bloco traz o acelerômetro filtrado em dois estágios (palavras 9 a 11 e 12 a 14); parado em 0°, o filtrado fica entre 0° e −1° (o ângulo real está no limite do arredondamento). O Xenia agora usa as palavras 9 a 11 (`UsbAudio::State::gravity`) no status e no Kinect Tuner. Ainda não se sabe qual estágio o kernel do console usa (o 0x23 do `McaDeviceRequest` copia nove campos; ver [Kinect-Audio-Firmware-e-CEMD](../kinect-audio-firmware-e-cemd.md)).

| Leitura (sensor parado) | Ângulos vistos |
|---|---|
| Bruta, palavras 4–6 | −2°, −1°, 0°, 1°, 2° |
| Filtrada, palavras 9–11 | 0°, −1° |
| Filtrada, palavras 12–14 | 0°, −1° |

### Firmware do motor travado
Matar o Xenia (SIGKILL) no meio de um comando USB deixa o firmware de áudio do Kinect sem responder: LED, inclinação e status falham até em `xenia-kinect-camera-test --motor_test`, o motor sai como "(no motor)" e o jogo fica com tela preta (cada consulta espera o limite de 250 ms do USB, com o `audio_mutex_` preso). Resolve tirando o Kinect da tomada; o Xenia reenvia o firmware ao abrir.

## Motor de inclinação do jogo (como funciona)

O runtime NUI do jogo tem o seu próprio motor de inclinação (0x82D2B000–0x82D2D400), dirigido pela máquina de estados **0x82D2CD20**, chamada a cada quadro de esqueleto. Estado global em 0x83898E00–0x83899040 (estado em `0x83898E60`, ângulo-alvo em `0x83898E74`, contador de espera em `0x83898E64`, degraus restantes em `0x83898E68`, chão achado em `0x83898F9C`).

1. **Início** (0x82D2BA88): estado 2, espera de 60 quadros, inclina para −25°.
2. **Busca** (estado 2): enquanto não há chão (`f9c == 0`), a cada 60 quadros, se o ângulo lido do acelerômetro ainda é negativo, sobe um degrau: `23 − 10·n` com n = 4, 3, 2 (−17°, −7°, 3°).
3. Esgotados os degraus, calcula o alvo (0x82D2B728) pela altura pedida e pelo ângulo; se `|alvo − atual| > 2°`, inclina; conta uma tentativa (`0x83898FBC`, até 16) e passa ao estado 4 ou 6.
4. **Chão**: o plano do chão vem do quadro de esqueleto do rastreador (0x82D2BA18, na estrutura do quadro +0x20) e vai para o sistema com `XamNuiCameraRememberFloor`. Com o chão, o alvo é calculado pela distância ao chão (`atan2`, limitado a ±27°).
5. **Jogador** (estado 3): mira a cabeça do jogador (0x82D2C6A0/0x82D2CA08) e termina no estado 6.

Ou seja: **sem chão e sem jogador do rastreador, a busca nunca termina**. O próximo passo no Xenia é entender por que o rastreador não acha o chão durante a busca (profundidade, GPU ou a gravidade que o rastreador recebe pelo 'XtSs').

## Pendências
- Confirmar ao vivo que o aviso "sensor movido" sumiu com o acelerômetro filtrado.
- Rastreamento e plano do chão durante a busca (o motivo do loop).
- `XamCacheOpenFile`, `XamCacheReset`, `XamCacheCloseFile` (cache do Kinect ID).
- `EtxProducerRegister` e `XamXlfsInitializeUploadQueue`: chamadas, não afetam o Kinect.
- Avatares (stubs).

## Arquivos da análise
- Imagem plana do executável: `~/Kixenia/kinect-sdk-re/games/adventures_4D5308ED.img` (Ghidra XEXLoaderWV, `DumpImage.java`).
- Projetos Ghidra: `~/Kixenia/kinect-sdk-re/ghidra-projects/advvmx` (linguagem **PowerPC:BE:64:VMX128-32addr**, sem análise automática: é o que dá a decompilação completa) e `advfull` (mesma linguagem, com análise).
- Decompilação: `~/Kixenia/kinect-sdk-re/games/decompilado/adv/c_full/<endereço>.c`, com os thunks trocados pelos nomes das importações.
- Scripts: `adv_imports.py`, `adv_funcs.py`, `gen_adventures.py` (`games/decompilado/scripts/`), `FixNoReturn.java`, `Holes.java`, `FnGraph.java` (`~/Kixenia/kinect-tools/ghidra/`).

> [!TIP]
> **Lições da decompilação**
> - Sem a linguagem VMX128, o Ghidra para nas instruções vetoriais do Xenon (`0x1BE00774`, `0x1020F9C3`: "Unable to resolve constructor") e corta as funções.
> - Com a análise automática, o Ghidra marca `__savegprlr` (0x83191C80) e `__restfpr_NN` como "não retorna" e corta 20 mil funções; `FixNoReturn.java` desfaz.
> - A tabela `.pdata` (0x822C5600, 47 877 funções) dá o início e o tamanho de cada função sem análise.
> - A análise automática do Ghidra usa um núcleo só (14 min neste jogo); a decompilação em lote foi dividida em 6 processos com cópias do projeto.

## Chamadas ao sistema (análise de 2026-10-26)

> [!NOTE]
> **Seção gerada dos dados em `kinect-sdk-re/games/decompilado/adv/`.**

**364 importações** (167 do `xam.xex`, 197 do `xboxkrnl.exe`), 3084 chamadas diretas no código, 126 vistas em execução (gravação `kinect_rec_aovivo2`, log nível 3, sem as de alta frequência). Estado no Xenia Edge (worktree kinect-sistema): 238 implementadas, 29 aproximadas, 56 stubs, 40 ausentes.

**Como foi feito:** o XEX foi carregado pelo XEXLoaderWV sem análise e exportado como imagem plana; as importações e os thunks vêm da listagem do log do Xenia (`!!` = ausente); as chamadas são as instruções `bl`/`b` para cada thunk; a função que contém cada chamada vem da tabela `.pdata`; as funções foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128 (✅ = decompilada inteira).

### Funções do jogo que falam com o Kinect

#### Câmera (PsCam)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D1F408 | 96 | `PsCamDeviceRequest` | 0 | **O único ponto que chama a câmera.** Tenta primeiro `XamXStudioRequest(0x1003)` (reprodução do Kinect Studio do kit de desenvolvimento) e só se falhar chama `PsCamDeviceRequest`. | ✅ |

#### Motor de inclinação do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D2B3C8 | 284 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltReportStatus` | 1 | Fim de um pedido de inclinação: para o motor (`ElevationStopMovement`) e informa o status ao sistema (0x2B006/0x2B00C). | ✅ |
| 0x82D2B698 | 112 | `XamNuiCameraElevationSetAngle` | 2 | Inclina para um ângulo, se estiver entre −27° e +27° (`ElevationSetAngle`); fora disso devolve E_INVALIDARG. | ✅ |
| 0x82D2BA88 | 144 | `XamNuiCameraElevationSetAngle` | 3 | Começa a busca: estado 2, espera de 60 quadros e inclina para **−25°**. | ✅ |
| 0x82D2BB18 | 468 | `XamNuiCameraRememberFloor`, `XamNuiCameraTiltReportStatus` | 1 | A cada quadro: pega o plano do chão do quadro de esqueleto (0x82D2BA18) e o guarda no sistema (`XamNuiCameraRememberFloor`) durante a busca ou depois de 60 quadros estáveis. | ✅ |
| 0x82D2BCF0 | 680 | `XamNuiCameraElevationGetAngle` | 2 | **Pedido de ajuste automático** (distância, altura e flags). Flag 0x20: só confere. Flag 0x08: confere e, se `\|alvo − ângulo atual\| > 2,0°` (constante em 0x82001148), inclina de novo: é o "sensor foi movido". Sem flag: começa a busca (0x82D2BA88). | ✅ |
| 0x82D2C180 | 780 | `XamNuiCameraTiltReportStatus` | 0 | **Callback do motor de inclinação** registrado no xam: recebe AutoTilt, StopMovement, ReverseAutoTilt e AdjustTilt do sistema e responde com o status. | ✅ |
| 0x82D2C490 | 144 | `XamNuiCameraTiltGetStatus` | 2 | Lê o status do motor e do acelerômetro (`TiltGetStatus`, 'XtSs', 0x50 bytes) e converte a gravidade em vetor. | ✅ |
| 0x82D2C520 | 80 | `XamNuiCameraTiltSetCallback` | 1 | Registra o callback do motor de inclinação (`XamNuiCameraTiltSetCallback`). | ✅ |
| 0x82D2CD20 | 2456 | `XamNuiCameraElevationStopMovement`, `XamNuiCameraTiltGetStatus` | 1 | **Máquina de estados da inclinação**, a cada quadro de esqueleto. Estado 2 (busca): sem chão achado, espera 60 quadros em cada ângulo e, enquanto o ângulo lido for negativo, sobe um degrau (`23 − 10·n`: −17°, −7°, 3°); esgotados os degraus, calcula o alvo pela altura (0x82D2B728) e conta uma tentativa (até 16). Estado 3: com o jogador, mira a cabeça dele. Estados 4 e 6: terminado. | ✅ |

#### Motor de identidade e biometria (Kinect ID)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D2F5C8 | 392 | `XamNuiIdentityGetSessionId`, `XamUserNuiGetUserIndex` | 1 |  | ✅ |
| 0x82D2F750 | 340 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82D30650 | 392 | `XamUserNuiGetEnrollmentIndex` | 1 |  | ✅ |
| 0x82D30E18 | 300 | `XamUserNuiGetUserIndex` | 0 |  | ✅ |
| 0x82D31938 | 568 | `XamReadBiometricData`, `XamUserNuiEnableBiometric`, `XamWriteBiometricData` | 1 |  | ✅ |
| 0x82D31B70 | 3868 | `XamNuiIdentityGetSessionId` | 7 |  | ✅ |
| 0x82D331A8 | 244 | `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset` | 0 |  | — |
| 0x82D332A0 | 288 | `XamReadBiometricData` | 1 |  | ✅ |
| 0x82D33E88 | 672 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |
| 0x82D3A4F0 | 976 | `XamNuiIdentityGetSessionId` | 1 |  | ✅ |

#### Avatares

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82AA6368 | 1348 | `XamAvatarGenerateMipMaps`, `XamAvatarGetAssetsResultSize` | 0 |  | ✅ |
| 0x82AA6BE8 | 116 | `XamAvatarGetMetadataSignedOutProfileCount` | 0 |  | ✅ |
| 0x82AA8F08 | 2700 | `XamAvatarGetManifestLocalUser`, `XamAvatarGetMetadataSignedOutProfile` | 0 |  | ✅ |
| 0x82BF9998 | 168 | `XamAvatarGetMetadataRandom` | 2 |  | ✅ |
| 0x82BF9BD8 | 1972 | `XamAvatarGetAssets`, `XamAvatarGetAssetsResultSize`, `XamAvatarManifestGetBodyType` | 1 |  | ✅ |
| 0x82BFA3E0 | 712 | `XMsgCancelIORequest` | 1 |  | ✅ |
| 0x82BFA6A8 | 92 | `XMsgCancelIORequest` | 0 |  | ✅ |
| 0x82BFAD18 | 784 | `XamAvatarGenerateMipMaps` | 1 |  | ✅ |
| 0x82BFE0E0 | 284 | `XamAvatarGetAssets`, `XamAvatarGetAssetsResultSize` | 1 |  | ✅ |
| 0x82BFE460 | 284 | `XamAvatarLoadAnimation` | 1 |  | ✅ |
| 0x82BFFD50 | 424 | `XamAvatarInitialize` | 1 |  | ✅ |
| 0x82BFFEF8 | 320 | `XamAvatarShutdown` | 2 |  | ✅ |
| 0x82C074B8 | 560 | `XamAvatarManifestGetBodyType` | 1 |  | ✅ |

#### Mensagens aos apps do sistema (XMsg)

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D8B6D8 | 432 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D8B888 | 188 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8B948 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8B9F8 | 172 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8BAA8 | 172 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D8BB58 | 168 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8BC00 | 220 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8BCE0 | 156 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8BD80 | 176 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8BE30 | 232 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D8BF18 | 220 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8BFF8 | 164 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8C0A0 | 192 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D8C2B0 | 352 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82D8C410 | 304 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82D8C540 | 752 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8C830 | 384 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82D8C9B0 | 312 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x82D8CAE8 | 320 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82D8CC28 | 732 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x82D8CF08 | 648 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8D190 | 656 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8D420 | 572 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8D660 | 580 | `XMsgInProcessCall`, `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8D978 | 68 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x82D8DB20 | 244 | `XMsgInProcessCall` | 11 |  | ✅ |
| 0x82D8DD80 | 248 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8DE78 | 72 | `XMsgInProcessCall` | 3 |  | ✅ |
| 0x82D8DEC0 | 152 | `XMsgInProcessCall` | 3 |  | ✅ |
| 0x82D8DFB8 | 304 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8E0E8 | 204 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82D8FAB8 | 168 | `XMsgStartIORequestEx` | 2 |  | ✅ |
| 0x82D8FB60 | 104 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x831AD6F0 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x831AD768 | 120 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x831AD7E0 | 140 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x831AD878 | 184 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x831AD9D8 | 216 | `XMsgStartIORequest` | 3 |  | ✅ |
| 0x831ADF90 | 56 | `XMsgSystemProcessCall` | 7 |  | ✅ |
| 0x831AE010 | 40 | `XamNuiGetDeviceStatus` | 2 | Lê o estado do sensor (`XamNuiGetDeviceStatus`) e devolve só o campo de estado. | ✅ |
| 0x831AE988 | 28 | `XamShowNuiDirtyDiscErrorUI` | 6 |  | ⚠️ |
| 0x831B1818 | 68 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x831B21E0 | 60 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x831B2220 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x831B2268 | 72 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x831B22B0 | 64 | `XMsgSystemProcessCall` | 1 |  | ✅ |
| 0x831B22F0 | 56 | `XMsgInProcessCall` | 2 |  | ✅ |
| 0x831B2328 | 76 | `XMsgInProcessCall` | 9 |  | ✅ |
| 0x831B2378 | 68 | `XMsgInProcessCall` | 1 |  | ✅ |
| 0x831B57D0 | 80 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x831B5820 | 112 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x831B5890 | 100 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x831B58F8 | 80 | `XMsgStartIORequest` | 2 |  | ✅ |
| 0x831B5948 | 112 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x831B59B8 | 100 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x831B5B40 | 40 | `XMsgCancelIORequest` | 1 |  | ✅ |

#### Chat de voz

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x831ECD98 | 888 | `XamVoiceHeadsetPresent` | 1 |  | ✅ |
| 0x831ED3B0 | 316 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x831ED740 | 600 | `XamVoiceSubmitPacket` | 1 |  | ✅ |
| 0x831EDAA0 | 240 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 |  | ✅ |
| 0x831EDB90 | 192 | `XamVoiceClose`, `XamVoiceCreate` | 1 |  | ✅ |
| 0x831F1F38 | 208 | `XamVoiceSubmitPacket` | 2 |  | ✅ |

#### Outras funções do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82D93ED8 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 15904 | ✅ |
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 7 | 1 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 18 | 8064 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 33 | 9 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 24 | ✅ |
| `XamAvatarGenerateMipMaps` | xam 0x5E2 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 3 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfileCount` | xam 0x5EE | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 3 | 11 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 8 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ❌ |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | ❌ |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | ❌ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 11 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 1 | ✅ |
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendsUI` | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 1 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

### Todas as importações, por área

<details><summary><b>Threads, sincronização e tempo</b> (60)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 3 | 27 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 4 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedFlushSList` ⭐ | xboxkrnl 0x2B | 6 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 10 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 8 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 5 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 72 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 8 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 59 | 0 | ✅ |
| `KeInitializeApc` | xboxkrnl 0x6D | 1 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 2 | 🟡 |
| `KeInsertQueueApc` | xboxkrnl 0x7A | 1 | 0 | ✅ |
| `KeInsertQueueDpc` | xboxkrnl 0x7B | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 8 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 7 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 4 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 8 | 3 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 8 | 6782 | ✅ |
| `KeRestoreFloatingPointState` | xboxkrnl 0x90 | 1 | 3733 | 🟡 |
| `KeResumeThread` | xboxkrnl 0x92 | 2 | 1 | ✅ |
| `KeSaveFloatingPointState` | xboxkrnl 0x95 | 1 | 3733 | 🟡 |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 2 | 24 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 5 | 15 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 62 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 6 | 11 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 5 | 9 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 10 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 15 | 751 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 4 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 14 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 43 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 54 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 1 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` ⭐ | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 6030 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 6 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 6 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 749 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 81 | 214 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 8 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 902 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 6 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 6 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 117 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (59)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 1 | ⚪ stub |
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
| `NtClose` | xboxkrnl 0xCF | 23 | 418 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 4 | 50 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 6 | 80 | ✅ |
| `NtCreateIoCompletion` | xboxkrnl 0xD3 | 1 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 379 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 5 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 2 | ✅ |
| `NtDeviceIoControlFile` | xboxkrnl 0xD9 | 2 | 0 | ⚪ stub |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 1 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 2 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 8 | 2 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 1 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 2 | 20 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 3 | 12 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 5 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtRemoveIoCompletion` | xboxkrnl 0xF4 | 1 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 2 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 12 | 0 | ✅ |
| `NtSetIoCompletion` | xboxkrnl 0xF8 | 1 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 2 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 10 | 0 | ✅ |
| `ObCreateObject` | xboxkrnl 0x102 | 3 | 3 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 2 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 4 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 43 | 44 | ✅ |
| `ObIsTitleObject` | xboxkrnl 0x109 | 2 | 0 | ❌ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 3 | 3 | ✅ |
| `ObReferenceObject` | xboxkrnl 0x10F | 2 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 15 | 47 | ✅ |
| `StfsControlDevice` | xboxkrnl 0x25A | 0 | 0 | ⚪ stub |
| `StfsCreateDevice` | xboxkrnl 0x259 | 0 | 0 | ⚪ stub |
| `XamCacheCloseFile` | xam 0x2B5 | 1 | 0 | ❌ |
| `XamCacheOpenFile` | xam 0x2B4 | 1 | 0 | ❌ |
| `XamCacheReset` | xam 0x2B7 | 1 | 0 | ❌ |
| `XamContentClose` | xam 0x25A | 3 | 2 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 1 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 1 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 1 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentLaunchImage` ⭐ | xam 0x263 | 1 | 0 | ❌ |
| `XamContentOpenFile` | xam 0x269 | 4 | 4 | 🟡 |

</details>

<details><summary><b>Rede</b> (43)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 4313 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 1 | ✅ |
| `NetDll_XNetQosGetListenStats` ⭐ | xam 0x4D | 1 | 0 | ❌ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 105 | ✅ |
| `NetDll_XNetRegisterKey` ⭐ | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` ⭐ | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 1 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 2 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 1 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 4313 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 3 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 1 | ✅ |
| `XNetLogonGetTitleID` | xam 0x136 | 2 | 0 | ✅ |
| `XamXlfsInitializeUploadQueue` | xam 0x482 | 1 | 0 | ❌ |
| `XamXlfsMountUploadQueueInstance` | xam 0x484 | 1 | 0 | ❌ |
| `XamXlfsUninitializeUploadQueue` | xam 0x483 | 1 | 0 | ❌ |
| `XamXlfsUnmountUploadQueueInstance` | xam 0x485 | 1 | 0 | ❌ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (34)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamProfileCreateEnumerator` | xam 0x231 | 1 | 2 | ✅ |
| `XamProfileEnumerate` | xam 0x232 | 1 | 4 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 13 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowCustomPlayerListUI` ⭐ | xam 0x2E6 | 1 | 0 | ❌ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowGoldUpgradeUI` | xam 0x6A5 | 1 | 0 | ⚪ stub |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowPlayerReviewUI` ⭐ | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowPlayersUI` ⭐ | xam 0x2C8 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 3 | 🟡 |
| `XamUserCreateAchievementEnumerator` ⭐ | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 2 | 0 | 🟡 |
| `XamUserGetDeviceContext` | xam 0x208 | 1 | 130235 | ⚪ stub |
| `XamUserGetIndexFromXUID` | xam 0x211 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 1 | ✅ |
| `XamUserGetSigninInfo` | xam 0x227 | 1 | 2 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 5 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 2 | 15 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 12 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 1 | ✅ |
| `XamWriteGamerTile` ⭐ | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Sistema e console</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 15 | 56 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 12 | 3 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetGameRegion` | xam 0x3CC | 4 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamEnableInactivityProcessing` | xam 0x1A0 | 1 | 1 | ⚪ stub |
| `XamGetCurrentTitleId` | xam 0x1CF | 6 | 1 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 8 | 5 | ✅ |
| `XamLoaderGetLaunchData` ⭐ | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` ⭐ | xam 0x1A7 | 1 | 1 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` ⭐ | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XamResetInactivity` | xam 0x1A1 | 1 | 0 | ⚪ stub |
| `XamXStudioRequest` | xam 0x686 | 10 | 18624 | ✅ |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 5 | 1 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 4 | 7 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 1 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 4 | 12 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 1 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (27)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 1 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 1 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 2 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 1 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 4810 | ⚪ stub |
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
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 4811 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 1 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 1 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 1 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 2 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetAVPack` | xam 0x3CB | 1 | 1 | ⚪ stub |
| `XGetVideoMode` | xam 0x3D1 | 4 | 5 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 10 | 0 | ✅ |
| `EtxProducerLog` | xboxkrnl 0x332 | 3 | 0 | ❌ |
| `EtxProducerRegister` | xboxkrnl 0x334 | 1 | 0 | ❌ |
| `EtxProducerUnregister` | xboxkrnl 0x335 | 1 | 0 | ❌ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 2 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 2 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlCompareStringN` | xboxkrnl 0x11D | 2 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 1 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 2 | 1 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 25 | 86 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 21 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 3 | 0 | ❌ |
| `RtlUpcaseUnicodeChar` | xboxkrnl 0x149 | 2 | 0 | ✅ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_snprintf` | xboxkrnl 0x13A | 10 | 0 | ✅ |
| `_vsnprintf` | xboxkrnl 0x14D | 2 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolTypeWithTag` | xboxkrnl 0xB | 2 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 7 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 359 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 31 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 16 | 134 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 4 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 8 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` ⭐ | xboxkrnl 0xC7 | 1 | 81 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 13 | 5857 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 13 | 5434 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 13 | 1 | ✅ |
| `XamFree` | xam 0x1EC | 17 | 1 | ✅ |

</details>

<details><summary><b>Avatares</b> (12)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamAvatarGenerateMipMaps` ⭐ | xam 0x5E2 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssets` | xam 0x5E1 | 2 | 0 | ⚪ stub |
| `XamAvatarGetAssetsResultSize` | xam 0x5E0 | 3 | 0 | ⚪ stub |
| `XamAvatarGetManifestLocalUser` | xam 0x5DE | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataRandom` | xam 0x5EB | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfile` | xam 0x5EF | 1 | 0 | ⚪ stub |
| `XamAvatarGetMetadataSignedOutProfileCount` ⭐ | xam 0x5EE | 1 | 0 | ⚪ stub |
| `XamAvatarInitialize` | xam 0x5DC | 1 | 1 | ⚪ stub |
| `XamAvatarLoadAnimation` | xam 0x5E7 | 1 | 0 | ⚪ stub |
| `XamAvatarManifestGetBodyType` | xam 0x5F1 | 3 | 11 | ⚪ stub |
| `XamAvatarSetCustomAsset` | xam 0x5E3 | 1 | 8 | ⚪ stub |
| `XamAvatarShutdown` | xam 0x5DD | 1 | 1 | ⚪ stub |

</details>

<details><summary><b>Kinect: câmera e sensor</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `PsCamDeviceRequest` | xboxkrnl 0x30D | 1 | 15904 | ✅ |
| `XamNuiCameraElevationGetAngle` | xam 0x881 | 2 | 0 | ✅ |
| `XamNuiCameraElevationSetAngle` | xam 0x880 | 2 | 11 | ✅ |
| `XamNuiCameraElevationStopMovement` | xam 0x883 | 2 | 0 | ✅ |
| `XamNuiCameraRememberFloor` | xam 0x865 | 2 | 0 | ✅ |
| `XamNuiCameraTiltGetStatus` | xam 0x870 | 2 | 0 | ✅ |
| `XamNuiCameraTiltReportStatus` | xam 0x895 | 7 | 0 | ✅ |
| `XamNuiCameraTiltSetCallback` | xam 0x86D | 1 | 1 | ✅ |
| `XamNuiGetDeviceStatus` | xam 0x885 | 1 | 1 | ✅ |
| `XamUserNuiGetEnrollmentIndex` | xam 0x86A | 1 | 0 | ✅ |
| `XamUserNuiGetUserIndex` | xam 0x869 | 2 | 0 | ✅ |

</details>

<details><summary><b>Kinect: HUD, esqueleto e telas Nui</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamShowNuiAchievementsUI` | xam 0x46E | 1 | 0 | ❌ |
| `XamShowNuiControllerRequiredUI` | xam 0x47B | 1 | 0 | ❌ |
| `XamShowNuiDeviceSelectorUI` | xam 0x470 | 1 | 0 | 🟡 |
| `XamShowNuiDirtyDiscErrorUI` ⭐ | xam 0x471 | 1 | 0 | ⚪ stub |
| `XamShowNuiFriendsUI` ⭐ | xam 0x472 | 1 | 0 | ❌ |
| `XamShowNuiGamerCardUIForXUID` | xam 0x474 | 1 | 0 | ❌ |
| `XamShowNuiGuideUI` | xam 0x356 | 1 | 0 | ✅ |
| `XamShowNuiHardwareRequiredUI` ⭐ | xam 0x47D | 1 | 0 | ✅ |
| `XamShowNuiMarketplaceUI` | xam 0x476 | 1 | 0 | ❌ |
| `XamShowNuiSigninUI` | xam 0x47A | 1 | 0 | 🟡 |
| `XamShowNuiTroubleshooterUI` | xam 0x46B | 1 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (8)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 5 | 0 | ✅ |
| `XNotifyPositionUI` ⭐ | xam 0x28C | 9 | 1 | ⚪ stub |
| `XamCreateEnumeratorHandle` | xam 0x24E | 1 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 3 | 4 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 3 | ✅ |
| `XamTaskCloseHandle` | xam 0x1B1 | 1 | 0 | ✅ |
| `XamTaskSchedule` | xam 0x1AF | 1 | 0 | 🟡 |
| `XamTaskShouldExit` | xam 0x1B3 | 1 | 0 | ⚪ stub |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 2 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 2 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 2 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptRandom` | xboxkrnl 0x18A | 4 | 0 | ⚪ stub |
| `XeCryptSha` | xboxkrnl 0x192 | 7 | 0 | ✅ |
| `XeKeysConsolePrivateKeySign` | xboxkrnl 0x256 | 1 | 0 | 🟡 |
| `XeKeysConsoleSignatureVerification` | xboxkrnl 0x257 | 1 | 0 | ❌ |
| `XeKeysGetConsoleID` | xboxkrnl 0x246 | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 1 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 7 | 1 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 18 | 8064 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 33 | 9 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 1 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 5 | 24 | ✅ |

</details>

<details><summary><b>Descompressão (LDI/LZX)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `LDICreateDecompression` | xboxkrnl 0xB6 | 2 | 1 | ✅ |
| `LDIDecompress` | xboxkrnl 0xB7 | 3 | 691 | ✅ |
| `LDIDestroyDecompression` | xboxkrnl 0xB8 | 6 | 1 | ✅ |
| `LDIResetDecompression` | xboxkrnl 0x2E2 | 1 | 0 | ✅ |

</details>

<details><summary><b>Kinect ID e biometria</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamNuiIdentityGetSessionId` | xam 0x873 | 6 | 1 | ✅ |
| `XamReadBiometricData` | xam 0x849 | 2 | 1 | ✅ |
| `XamUserNuiEnableBiometric` | xam 0x863 | 1 | 0 | ✅ |
| `XamWriteBiometricData` | xam 0x84A | 1 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (3)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 2 | 4 | 🟡 |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 1 | 4313 | ✅ |

</details>

⭐ = só este jogo importa (nem o Nike nem o Kinect Sports).

**Ausentes no Xenia (40):** `EtxProducerLog`, `EtxProducerRegister`, `EtxProducerUnregister`, `IoCheckShareAccess`, `IoCompleteRequest`, `IoDismountVolume`, `IoDismountVolumeByFileHandle`, `IoInvalidDeviceRequest`, `IoRemoveShareAccess`, `IoSetShareAccess`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetServerToInAddr`, `NetDll_XnpLogonGetStatus`, `ObIsTitleObject`, `RtlCaptureContext`, `RtlUnwind`, `XamCacheCloseFile`, `XamCacheOpenFile`, `XamCacheReset`, `XamContentLaunchImage`, `XamShowCustomPlayerListUI`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowNuiAchievementsUI`, `XamShowNuiControllerRequiredUI`, `XamShowNuiFriendsUI`, `XamShowNuiGamerCardUIForXUID`, `XamShowNuiMarketplaceUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamXlfsInitializeUploadQueue`, `XamXlfsMountUploadQueueInstance`, `XamXlfsUninitializeUploadQueue`, `XamXlfsUnmountUploadQueueInstance`, `XeKeysConsoleSignatureVerification`, `__C_specific_handler`.
