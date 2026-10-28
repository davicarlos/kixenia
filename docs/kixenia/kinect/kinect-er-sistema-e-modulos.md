# Kinect, engenharia reversa: pacotes e apps do sistema e o que os módulos decompilados revelaram

Parte de [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seções 9, 15, 16h).

## 9. Pacotes e aplicativos do Kinect na atualização 17559

- **`FFFE07DF00000001`:** o firmware `audios.bin`.
- **`FFFE07DF00000008`:** os modelos do sistema:
  - `NuiIdentityNN.bin.be` e `NuiIdentityPCA.bin.be`, os modelos de identidade;
  - `HeadPosition.mshdf`;
  - `Database.gmsodf`, de 12,6 MB, provavelmente a floresta do rastreador do sistema.
- **Aplicativos do sistema:**
  - `nuihud.xex`: a interface sobreposta e o guia por gestos;
  - `Guide.NuiTroubleshooter.xex`: o Kinect Tuner oficial;
  - `natalsu.xex`: a configuração inicial;
  - `dash.natalpregame.xex`: a tela "fique na frente do sensor";
  - `BiometricSetup.xex`: o cadastro do Kinect ID.
- **Jogos:** o Kinect Sports e o Nike trazem rastreador, identidade e modelos próprios (`database.xmplr`, `HeadPosition.xmplr`, `nuiidentity.bin.be`, `NuiHeadOrientation.bin.be`).

## 15. Aplicativos de sistema (só inventário)

| Arquivo | Tamanho |
|---|---|
| `dashnui.xex` | 3,2 MB |
| `BiometricSetup.xex` | 5,8 MB |
| `Dash.NuiTroubleshooter.lex` | 2,5 MB |
| `Guide.NuiTroubleshooter.xex` | 0,7 MB |
| `dash.natalpregame.xex` | 0,6 MB |
| `nuihud.xex` e `natalsu.xex` | 68 KB cada (provavelmente carregadores) |

Há também pacotes de mídia `.xzp` (`HudPurUI`, `NuiCommunity`, `NuiPurchase`). Nenhum foi decompilado.

## 16h. O que os módulos do sistema revelaram (2026-10-25)
Decompilados pelo Ghidra MCP: nuihud, BiometricSetup, Guide.NuiTroubleshooter, Dash.NuiTroubleshooter, Dash.FieldCalibration, natalsu, dash.natalpregame e Guide.Fitness.
- **Códigos do PsCamDeviceRequest que faltavam:**
  - **19 (0x13)**: comando cru à câmera, no protocolo de controle da PrimeSense {opcode, parâmetros, tamanhos, conclusão}. Quem usa é o natalsu.
  - **34 (0x22)**: versão do firmware da câmera; o natalsu só atualiza de 0x201 a 0x2FF.
  - **41 (0x29)**: lido pelo FieldCalibration; o resultado vira um float em `LogCameraSettings`.
  - Os três estão ausentes em `xboxkrnl_pscam.cc`. Os códigos 20, 40 e 44 continuam sem uso conhecido.
- **`XamNuiGetCameraIntrinsics`:** a saída tem **16 bytes**, provavelmente 4 floats (fx, fy, cx, cy, não confirmado), com um buffer auxiliar no 3º argumento (FieldCalibration).
- **Calibração de profundidade:**
  - o bloco tem A (escala, padrão 1,0) em +0 e B (deslocamento em mm, gravado com o sinal trocado) em +4, com 0x28 bytes no total;
  - o erro é checado a 3,5 m: `|(B + 3500)/A/3500 − 1|`;
  - `XamNuiGetLoadedDepthCalibration` (0x8DE) está ausente no Xenia.
- **AdjustTilt (0x2B00B)** e **0x2B00E:** resolvidos pelo Troubleshooter do guia (ver a tabela da seção 16e).
- **Modelos e pacotes do sistema:**
  - `NUIPACK:` (FFFE07DF00000008): identidade, rastreador e voz;
  - `NATALDEVICEPACKAGE:\`: firmware pmc, altair, detroit, cambridge, NuiCam;
  - os dois são achados por `XamPackageManagerFindPackageContainingIndexedXEX`, que está ausente no Xenia. Os jogos não dependem dele, porque trazem os próprios pacotes.
