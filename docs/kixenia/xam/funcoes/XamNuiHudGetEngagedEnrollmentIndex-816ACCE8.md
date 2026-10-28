# XamNuiHudGetEngagedEnrollmentIndex

**Endereço:** 0x816ACCE8 · **Ordinal:** 1307 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** ✅ implementada

## O que faz
cópia do quadro do HUD: cadastro do engajado, ou −2

## Fatos do decompilado
- Kernel: ExReleaseReadWriteLock (xboxkrnl 0x16)
- Globais: 0x81AA07D0

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1307 | 0x816ACCE8 | `XamNuiHudGetEngagedEnrollmentIndex` | cópia do quadro do HUD: cadastro do engajado, ou −2 | ✅ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `XamNuiHudGetEngagedEnrollmentIndex`: na cópia do quadro do HUD, procura o esqueleto com o ID engajado (+0x118) e devolve o índice de cadastro; senão −2.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Implementado no commit e19c98e7e:** `XamNuiHudInterpretFrame` (escolha dos 2 esqueletos, rebaixamento para PositionOnly, aceno de 4 meias-ondas com os parâmetros do xam, gesto do guia por pose aproximada, opção `kinect_guide_gesture`, `0x8301000B` com UI do sistema aberta), `XamNuiHudGetEngagedEnrollmentIndex` e `XamNuiHudEnableInputFilter` (só guarda o valor).
