# XamShowNuiTroubleshooterUI

**Endereço:** 0x816C5F68 · **Ordinal:** 1131 · **Área:** [Telas](README.md#telas) · **Xenia:** ⚪ stub

## O que faz
sem sensor: caixa de mensagem (textos 0x11F/0x4F); com sensor: engaja e abre o Tuner (0x21028), modo 2, 3 ou 5

## Fatos do decompilado
- Mensagens XMsg: app 0xFE msg 0x21028

## Chama
- [XamNuiHudSetEngagedTrackingID (816AC428)](XamNuiHudSetEngagedTrackingID-816AC428.md)
- [FUN_816C16C8](FUN_816C16C8.md) — Telas: resultado do pedido de UI
- [XamAppRequestLoadEx (816CAC60)](XamAppRequestLoadEx-816CAC60.md) — Telas: carregar app do sistema
- [XamNuiGetDeviceStatus (817FC990)](XamNuiGetDeviceStatus-817FC990.md)
- Outras: XMsgSystemProcessCall (816A2FF8), FUN_816B7C28, XamShowMessageBoxUI (816C5508), FUN_816CAC20, XamShowMessageBox (816CB168), XamLookupCommonStringByIndex (816E0BF0), XamPackageManagerGetExperienceMode (816E9AC0)

## É chamada por
- [FUN_816F9CB8](FUN_816F9CB8.md) — Despachante NUI (0x2B0xx)
- [XamNuiNatalCameraUpdateComplete (817FA270)](XamNuiNatalCameraUpdateComplete-817FA270.md)
- e mais 3 funções fora desta base (vizinhas): FUN_817F9550, FUN_817F96A8, FUN_817FABA8

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 1131 | 0x816C5F68 | `XamShowNuiTroubleshooterUI` | sem sensor: caixa de mensagem (textos 0x11F/0x4F); com sensor: engaja e abre o Tuner (0x21028), modo 2, 3 ou 5 | ⚪ stub |
- [Kinect-Xenia-Edge](../../kinect/kinect-xenia-edge.md): - **Kinect Tuner** (`XamShowNuiTroubleshooterUI`, e também pelo menu do botão Guide):
