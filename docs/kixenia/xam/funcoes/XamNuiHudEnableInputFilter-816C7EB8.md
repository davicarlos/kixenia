# XamNuiHudEnableInputFilter

**Endereço:** 0x816C7EB8 · **Ordinal:** 2173 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos) · **Xenia:** 🟡 aproximada

## O que faz
com HUD: FUN_81692C48(0x81AA2160, p)

## Fatos do decompilado
- Globais: 0x81AA2160

## Chama
- [FUN_81692C48](FUN_81692C48.md) — HUD: pedido de filtro de entrada
- [XamNuiHudIsEnabled (816AC4B8)](XamNuiHudIsEnabled-816AC4B8.md)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2173 | 0x816C7EB8 | `XamNuiHudEnableInputFilter` | com HUD: FUN_81692C48(0x81AA2160, p) | 🟡 |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): 3. Se o filtro de entrada está ligado (0x81AA2188, `XamNuiHudEnableInputFilter`), devolve **0x8301000B**: o título não deve usar esse quadro, porque o sistema está com a entrada.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `XamNuiHudEnableInputFilter(p)`: com o HUD ligado e o objeto 0x81AA2160, `FUN_81692C48(obj, p)` (que chama `FUN_816920F0` se obj+0x38).
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **Implementado no commit e19c98e7e:** `XamNuiHudInterpretFrame` (escolha dos 2 esqueletos, rebaixamento para PositionOnly, aceno de 4 meias-ondas com os parâmetros do xam, gesto do guia por pose aproximada, opção `kinect_guide_gesture`, `0x8301000B` com UI do sistema aberta), `XamNuiHudGetEngagedEnrollmentIndex` e `XamNuiHudEnableInputFilter` (só guarda o valor).
