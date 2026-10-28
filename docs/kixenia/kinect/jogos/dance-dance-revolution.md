# Dance Dance Revolution (título 4B4E0807)

> [!WARNING]
> ****Não usa o Kinect.** Por isso não houve decompilação, execução com a gravação nem teste ao vivo.**

Dance Dance Revolution, da Konami (Xbox 360, 2010), jogado com o tapete de dança. Contêiner GOD em `<HD>/Xbox 360/Dance Dance Revolution [RF]/Content/0000000000000000/4B4E0807/` (HD externo), extraído com `xenia-vfs-dump` em `~/Kixenia/kinect-sdk-re/games/extraidos/ddr/` (2,7 GB): `default.xex` (6,2 MB), `data/` (`arc`, `dance/sq3`, `font`, `mdb`, `movie`, `sound`), `AvatarAssetPack`, `nxeart` e `$SystemUpdate`.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Michael-Jackson-The-Experience](michael-jackson-the-experience.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md).

## Por que não usa o Kinect
Conferido em 2026-10-26 com o XEX carregado no Ghidra (XEXLoaderWV, sem análise; projeto `~/Kixenia/kinect-sdk-re/ghidra-projects/ddrvmx`, imagem `~/Kixenia/kinect-sdk-re/games/ddr.img`):

- **Nenhuma importação `XamNui*`, `PsCam*`, `XamVoice*MicArray*`, `XamFitness*` nem `XamAvatar*`**, entre as 238 importações (225 thunks; 35 do `xam.xex`).
- O cabeçalho do XEX (bibliotecas estáticas) **não tem o runtime `NUI`** nem `NUISP`/`XHV2`: só bibliotecas do XDK **2.0.11626** (`XAPILIB`, `XAUDIO2`, `XACT3LTC`, `D3D9LTCG`, `XONLINE`…) e **`XCAM`**.
- `XCAM` é a biblioteca da **Xbox Live Vision**, a câmera USB anterior ao Kinect: o jogo importa `XUsbcamCreate`, `XUsbcamDestroy`, `XUsbcamGetState`, `XUsbcamSetConfig`, `XUsbcamSetCaptureMode` e `XUsbcamReadFrame`, com 8 chamadas diretas, todas em 0x822FFAB4–0x82300CC0 (um único módulo de câmera). Nenhuma string de Kinect, Natal, NUI ou modelos do rastreador (`*.xmplr`) no executável.

No Xenia (worktree kinect-sistema), `XUsbcamCreate` e `XUsbcamGetState` são stubs (`xboxkrnl_usbcam.cc`); as outras quatro estão só na tabela de exports (`xboxkrnl_table.inc`), sem implementação. Isso só afeta a câmera Vision (provavelmente o vídeo do jogador ao fundo), não o Kinect.

## Teste com o Kinect real: não se aplica
O jogo não fala com o Kinect. Se for testado, o que importa é o tapete de dança (controle comum) e, opcionalmente, a câmera Xbox Live Vision.
