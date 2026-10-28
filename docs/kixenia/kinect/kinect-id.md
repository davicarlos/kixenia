# Kinect ID

O **Kinect ID** é o reconhecimento facial do Kinect no Xbox 360. Ele faz duas coisas:
- **Cadastro:** grava o rosto de um perfil.
- **Identificação:** depois, reconhece quem está na frente do sensor e entra no perfil dessa pessoa sozinho, inclusive durante jogos Kinect.

O cadastro é feito no aplicativo **BiometricSetup.xex** do painel. O aplicativo pede para a pessoa andar pela sala e grava vários ângulos do rosto, com iluminações diferentes. Strings do próprio aplicativo:
- *"Kinect ID helps your console recognize you and signs you in while you're in Kinect games and experiences."*
- *"Kinect ID works best after it's recorded several angles of your face in varied lighting conditions."*

Relacionado: [Nike-Kinect-Training](jogos/nike-kinect-training.md), xam.xex, dashnui.xex, [XAM-Rede-e-Login](../xam/xam-rede-e-login.md).

## Quem faz o reconhecimento

O reconhecimento não acontece no `xam.xex`: as funções do XAM só fazem a ponte. Quem roda o motor é o **runtime NUI que vem dentro de cada aplicativo ou jogo Kinect**, o mesmo que faz o rastreamento de esqueleto. O motor aparece em vários lugares:
- no Nike (`DEFAULT.xex`);
- no `BiometricSetup.xex`, no `dash.xex`, no `dashnui.xex`, no `AvatarEditor.xex` e no `Title.Zune.xex`.

O caminho de um pedido:

```
Título → XamNuiIdentityIdentifyWithBiometric / EnrollForSignIn / Abort
       → XMsgProcessRequest(0xFE, 0x2C0xx)  (serviço de identidade dentro do XAM)
       → FUN_8180e750: chama o callback guardado em serviço+0x24
       → callback registrado pelo runtime NUI do título com a mensagem 0x2C009
       → motor de identidade do título (modelos abaixo)
```

- Sem callback registrado, o serviço responde `0x8000FFFF`.
- Antes de identificar (0x2C006/7/8), o serviço pede ao callback os dados do cadastro (0x2C013).
- Todas as funções de identidade recusam o pedido (`0x80004005`) se `XamNuiHudIsEnabled()` for falso.
- Endereços: `xam.xex` da atualização 17559, base 0x815F0000.

## Mensagens do serviço (app 0xFE)

| Mensagem | Quem envia | O que é |
|---|---|---|
| 0x2C006, 0x2C007, 0x2C008 | `XamNuiIdentityIdentifyWithBiometric` (0x2C007, buffer de 0x10) | identificar; antes consulta 0x2C013 |
| 0x2C009 | runtime NUI do título | registrar o callback de identidade |
| 0x2C00E | `XamNuiIdentityAbort` (4 bytes) | cancelar |
| 0x2C010 | `XamNuiIdentityEnrollForSignIn` (0xC bytes: usuário, param2, param4) | cadastrar para o login |
| 0x2C013 | `XamNuiIdentityGetEnrollmentInfo` | dados do cadastro (resposta de 0x10 bytes; devolve 2 dwords) |
| 0x2A009 | `XamUserNuiEnableBiometric` ao desligar | avisar que a biometria foi desligada (0x18 bytes, zerados) |

## Funções do XAM (ordinal, endereço)

| Função | Ordinal | Endereço | O que faz |
|---|---|---|---|
| XamNuiIdentityIdentifyWithBiometric | 2164 | 0x8180DA50 | envia 0x2C007 com {usuário, p3, p4, 0} |
| XamNuiIdentityEnrollForSignIn | 2162 | 0x8180D970 | envia 0x2C010 com {usuário, p2, p4} |
| XamNuiIdentityAbort | 2199 | 0x8180D910 | envia 0x2C00E |
| XamNuiIdentityGetEnrollmentInfo | 2138 | 0x8180E048 | 0x2C013 → dois dwords |
| XamNuiIdentityGetSessionId | 2163 | 0x8180DAC8 | id de sessão aleatório de 8 bytes, criado na primeira chamada |
| XamNuiIdentityGetQualityFlags / ...Message | 2161 / 2156 | 0x8180D9E8 / 0x816F9B30 | qualidade da imagem do rosto (luz, rosto coberto) |
| XamNuiIdentityGetColorTexture | 2140 | 0x8180DF08 | imagem colorida do rosto (para a interface) |
| XamNuiIdentityUnenroll | 2139 | 0x8180DFE0 | apagar o cadastro |
| XamUserNuiGetEnrollmentIndex | 2154 | 0x816DADA8 | tabela de 4 cadastros × 0x40 bytes: +0x30 ativo, +0x34 índice; sem cadastro devolve `0xFFFFFFFE` e `0x80070525` |
| XamUserNuiGetUserIndex | 2153 | 0x816DAE00 | percorre os 4 cadastros procurando o usuário; não achou: `0x80070525` |
| XamUserNuiEnableBiometric | 2147 | 0x816DE380 | grava a configuração de perfil **0x1004004E**; ao desligar, apaga os dados biométricos e envia 0x2A009 |
| XamUserNuiIsBiometricEnabled | 2146 | 0x816DAEE0 | lê essa configuração (perfil do painel, título 0xFFFE07D1) |
| XamReadBiometricData / XamWriteBiometricData | 2121 / 2122 | 0x816B1D88 / 0x816B1E58 | dados biométricos via XMsg 0xFB / 0xB0076 (Live) |

Nomes de dados no XAM: `NuiV1FaceHiRes`, `NuiV1FaceLoRes`, `NuiV1UpFaceHiRes`, `NuiV1UpFaceLoRes`, `NuiIdCfgVer`. Provavelmente são as imagens do rosto guardadas com o cadastro, em alta e baixa resolução (o "Up" deve ser o rosto de cima).

## Motor de identidade (no título)

Strings do Nike e do BiometricSetup: `NuiIdentityIdentify` (com o ID de rastreamento do esqueleto), `RequestNuiIdentitySignIn`, `NuiIdentityFaceVec0`, "Identifier got positive Identification after %u ms", "Identify timed out", "Enroll failed, profile not matched".

O que os modelos sugerem sobre o pipeline (deduzido dos nomes; o código ainda não foi lido):
1. O esqueleto dá a posição da cabeça (`HeadPosition.mshdf`).
2. Um detector acha o rosto na imagem colorida (`detector_pruned_rot_9.bin.be`).
3. Modelos estimam a orientação da cabeça: yaw, pitch e roll, cada um com `*_pca_lda` e `*_knn`.
4. O rosto vira um vetor (`NuiIdentityFaceVec`) por PCA (`NuiIdentityPCA.bin.be`) e uma rede neural (`NuiIdentityNN.bin.be`).
5. O vetor é comparado com os vetores cadastrados.

Mensagens de qualidade que o usuário vê: rosto não visível, sala escura demais, sala clara demais, luz no sensor, mãos na frente do rosto ("Error_CannotSeeFace", "Error_FaceCovered").

## Arquivos de modelo

O título monta dispositivos com estes arquivos. O Nike procura em `nuiidentity:\`, `headposition:\`, `game:\` e `update:\`.

| Arquivo | Onde | Tamanho | SHA-1 (início) |
|---|---|---|---|
| NuiIdentityNN.bin.be | pacote do sistema `FFFE07DF00000008` e `NuiIdentity.bin.be` do disco (PIRS) | 1.374.068 | 0121dd3fae707a68 |
| NuiIdentityPCA.bin.be | idem | 1.816.400 | 6834bb608b8fd3b6 |
| NuiIdentityDbVersion.txt | idem | 5 (`1.0.0`) | — |
| HeadPosition.mshdf | `FFFE07DF00000008` e `HeadPosition.xmplr` do disco | 393.236 | 9936a2803a550a0a |
| Database.gmsodf | `FFFE07DF00000008` (12,6 MB); `Database.xmplr` no disco | — | — |
| detector_pruned_rot_9.bin.be | `NuiHeadOrientation.bin.be` do disco | 1.000.944 | — |
| yaw/pitch/roll `_pca_lda`, `_knn` | `NuiHeadOrientation.bin.be` do disco | 1,5 KB a 714 KB | — |

O pacote do sistema e o do disco do Nike têm os mesmos modelos (mesma versão 1.0.0, mesmos hashes). Os arquivos `.bin.be` estão em big-endian.

## Onde o título guarda os rostos (Nike, descoberto em 2026-10-23)

O Nike guarda os rostos cadastrados num formato próprio, os "dados biométricos" de cada perfil. Existem duas cópias:
- **cache do XAM**, que dura até desligar o console, acessado por XMsg do app 0xFE;
- **cópia do perfil na Live**, acessada por `XamReadBiometricData` e `XamWriteBiometricData`.

| Chamada | Buffer / argumentos | Significado |
|---|---|---|
| XMsg 0x2A008 (Nike `838C2920`) | `{xuid (8), &versão (ponteiro em 8 bytes; a versão é int64), dados, &tamanho}` | lê o cache. Versão **0** = o XAM ainda não tem; **0x80000000** = o perfil não tem dados. Sem ponteiro de dados, só consulta a versão |
| XMsg 0x2A007 (Nike `838C2990`) | `{xuid, versão (8), dados, tamanho}` | grava no cache. Sem dados: versão 0x80000000 e tamanho 1 |
| `XamReadBiometricData(xuid, &versão, buf, &tam ≤ 0x32000, ov)` | | 0 e versão ≠ 0 = há dados |
| `XamWriteBiometricData(xuid, &versão, buf, &tam, ov)` | | grava na Live; o XAM manda tamanho 0 para apagar |

Fluxo do Nike:
- **Ao entrar um perfil** (`82F6A6F8`): consulta o cache. Se a versão for 0, lê a Live e repassa ao cache.
- **Ao carregar** (`82F6AFC8`, `82F6A138`): lê o cache até 0x32000 bytes e descomprime. São 20 rostos de 0x7D4 bytes, que vão para a base de identidade.
- **Ao cadastrar** (`82F6AD68`): lê a Live, soma 1 à versão e monta os dados novos. Depois chama `0x2A007`, `XamUserNuiEnableBiometric(usuário, 1)` e `XamWriteBiometricData`.
- `XamUserNuiGetEnrollmentIndex` < 0 também faz o Nike buscar os dados no cache.

## Situação no Xenia (Edge, branch kinect-linux)

**Implementado** (commits faaaa59af e 46a0badf5):
- `XamNuiIdentityGetSessionId`.
- Cache da sessão: as XMsg 0x2A007 e 0x2A008 ficam no `NUIController`. A versão 0 faz o jogo buscar a cópia do perfil.
- `XamReadBiometricData` e `XamWriteBiometricData`: a cópia "Live" fica em `content/<xuid>/FFFE07D1/NuiBiometric.bin`.
- `XamUserNuiEnableBiometric` grava a configuração de perfil 0x1004004E; desligar apaga os dados. `XamUserNuiIsBiometricEnabled` lê essa configuração.
- `XamUserNuiGetEnrollmentIndex` responde como o xam quando não há cadastro (índice −2, `0x80070525`).

- XMsg **0x2C00C** com op 1 (converter os dados para a versão 0x10001) copia a entrada (commit 6445d2a1f). Sem isso, o Nike nunca carregava os rostos lidos. O op 2 (parâmetros por nome) devolve vazio, e o jogo usa os padrões.
- XMsg **0x2C00D** (resolução de identidade do Kinect Sports) responde 0xFFFFFFFE, "ninguém", porque não há cadastros (commit 7ae9c41d5).
- Formato dos dados do Nike: [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md), seção 13.

**Ainda stubs:**
- `EnrollForSignIn`, `IdentifyWithBiometric`, `Abort` e `GetEnrollmentInfo`.
- O callback da 0x2C009 só aparece no log.
- `XamUserNuiGetUserIndex` sempre diz que não há ninguém.

**Falta para o Kinect ID do painel:**
1. Guardar o callback registrado pela 0x2C009 e repassar a ele as mensagens 0x2C006 a 0x2C013 (como `FUN_8180e750`).
2. Manter a tabela de 4 cadastros do xam.
3. Montar `nuiidentity:` e `headposition:`.

**Testes:** com um arquivo falso de teste, o Nike leu a cópia do perfil e repassou ao cache (versão 5, 16 bytes) sem travar. Falta testar ao vivo cadastrar um rosto no Nike, reabrir o jogo e ser reconhecido.

## Fontes

- **XAM decompilado:** `~/Kixenia/kinect-sdk-re/xam-nui/`
  - `decomp/`: 93 funções;
  - `exports.csv`: ordinal → endereço;
  - `xam_17559.exe`: PE com o layout de seções corrigido.
- **Análise:** Ghidra 12.1.4 headless, com os scripts `XamFixFlow` e `XamNuiDecomp`. O REA não abre XEX.
- **Pacotes:** `~/Downloads/SystemUpdate_17559/extracted/$SystemUpdate/` (`FFFE07DF00000008`, `BiometricSetup.xex`) e o disco do Nike.

## Em aberto

- O formato dos arquivos `.bin.be` e o tamanho do vetor do rosto.
- O que são os parâmetros p2, p3 e p4 de `EnrollForSignIn` e `IdentifyWithBiometric`.
- Onde ficam guardados os vetores cadastrados (perfil? `XamWriteBiometricData`?).
- O código do motor dentro do título, a partir de `NuiIdentityIdentify`.

## Cadastro do rosto, pelo BiometricSetup (2026-10-25)
O app de cadastro do sistema (BiometricSetup) usa o mesmo runtime NUI dos jogos, então o caminho do cadastro é o mesmo do Nike. Resumo (detalhes e endereços na nota do módulo):
1. A cada quadro de identidade, o esqueleto do jogador precisa estar rastreado.
2. As flags de qualidade (distância, rotação da cabeça, ombros) são calculadas; os limites vêm de `NuiV1QualDist/Pose/Clip`.
3. O rosto é extraído: a cabeça é projetada na imagem colorida, depois vêm o detector de rosto, a normalização, os olhos e o vetor de 2.004 bytes.
4. No modo de cadastro, o vetor entra numa fila de até 500 amostras, reduzida a 200 representativas.
5. Com 200 ou mais, grava o cache (0x2A009, 0x2A007), liga a biometria e grava no perfil.
- Os modelos (`NuiIdentityNN.bin.be`, `NuiIdentityPCA.bin.be`) vêm do pacote do sistema `NUIPACK:` (FFFE07DF00000008) nos apps do sistema e do `NuiIdentity.bin.be` do disco nos jogos. Nos logs do Xenia eles **abrem** no Nike e no Kinect Sports; a falha do cadastro é anterior às 200 amostras (esqueleto, qualidade, rosto na imagem colorida ou olhos).
- O guia do Kinect (nuihud) identifica quem está na frente com `XamNuiIdentityIdentifyWithBiometric` e troca o dono do guia com `XamHudSetUserIndex`; as duas estão ausentes no Xenia.

### Endereços do motor de identidade no Nike (2026-10-25)
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
