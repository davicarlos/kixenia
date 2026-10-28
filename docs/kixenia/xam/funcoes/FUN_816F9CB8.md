# FUN_816F9CB8

**Endereço:** 0x816F9CB8 · **Área:** [HUD, esqueleto e gestos](README.md#hud-esqueleto-e-gestos)

## O que faz
Despachante das mensagens 0x2B001 a 0x2B00E do app 0xFE: quadro de esqueleto (→ serviço de identidade e HUD), início e fim do título, repasse dos pedidos de inclinação ao motor do título (callback 0x81A85C40), status do motor ao callback 0x81A85C20, registro desses callbacks e 0x81AAC9A8.

## Fatos do decompilado
- Constantes de mensagem: 0x2B001
- Códigos de erro: 0x80004005, 0x8000FFFF

## Chama
- [FUN_816AC460](FUN_816AC460.md) — Mensagem 0x2B001
- [XamNuiHudInterpretFrame (816ACC68)](XamNuiHudInterpretFrame-816ACC68.md)
- [XamShowNuiTroubleshooterUI (816C5F68)](XamShowNuiTroubleshooterUI-816C5F68.md)
- [XamIsNuiUIActive (816C7D88)](XamIsNuiUIActive-816C7D88.md)
- [XamNuiGetDeviceStatus (817FC990)](XamNuiGetDeviceStatus-817FC990.md)
- [FUN_8180DB10](FUN_8180DB10.md) — Identidade: quadro
- Outras: XMsgCompleteIORequest (816A2588), FUN_816AD130, FUN_816AD198, XamAppUnloadStack (816CAF08), FUN_816F9F78, FUN_816F9FC0, FUN_816F9FD0, FUN_816F9FF0, FUN_816FA038, FUN_816FA900, FUN_816FA970, FUN_816FA988, FUN_816FA9A0, FUN_816FAA00, FUN_8180DB68

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): **0x2B0xx (despachante `FUN_816F9CB8`, `switch (msg − 0x2B001)`)**
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): O despachante das mensagens 0x2B0xx do app 0xFE é **`FUN_816F9CB8`** (`switch (msg − 0x2B001)`):
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - O despachante 0x2B0xx do xam (`FUN_816F9CB8`) passa **cada quadro de esqueleto que o título publica (0x2B002, versão 5)** por `XamNuiHudInterpretFrame`. É ali que o console vê o gesto do guia e o aceno de engajamento, em **todos** os jogos. O Xenia só guardava o quadro para o Kinect Tuner; agora chama o HUD.
