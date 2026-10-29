# Need for Speed Most Wanted 2005 XBOX 360 (título 454107D9)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `Need for Speed Most Wanted 2005 XBOX 360` (pacote GoD (cabeçalho em `Content/…/00007000/`)). **Executável:** 13120 KB de imagem, base 0x82000000, entrada 0x8262E768, 22679 funções no `.pdata`, `.text` 0x820e0000–0x828AB01C.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 7 importações (`XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamShowVoiceMailUI`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`); 4 funções próprias (0x82854000, 0x828888F0, 0x82888EF0, 0x828890F8) e 3 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamShowVoiceMailUI`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`**.
- **Mensagens aos apps do sistema (XMsg):** 5 importações (`XMsgCancelIORequest`, `XMsgInProcessCall`, `XMsgStartIORequest`, `XMsgStartIORequestEx`, `XMsgSystemProcessCall`); 59 funções próprias (0x8262CED0, 0x8262CF28, 0x8262CFA8, 0x8262D038 …) e 145 do runtime.

## Chamadas ao sistema

**366 importações** (173 do `xam.xex`, 193 do `xboxkrnl.exe`), 2272 chamadas diretas no código. Estado no Xenia Edge (kixenia): 248 implementadas, 23 aproximadas, 39 stubs, 55 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 209 funções que chamam o sistema, **147 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **62 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

#### Código próprio do jogo

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x8262CED0 | 88 | `XMsgStartIORequest` | 2 | XMsg: 0xB0006 ao app 0xFB (Live) | ✅ |
| 0x8262CF28 | 96 | `XMsgStartIORequest` | 0 | XMsg: 0xB0007 ao app 0xFB (Live) | ✅ |
| 0x8262CFA8 | 128 | `XMsgStartIORequest` | 0 | XMsg: 0xB0020 ao app 0xFB (Live) | ✅ |
| 0x8262D038 | 232 | `XMsgStartIORequest` | 0 | XMsg: 0xB0021 ao app 0xFB (Live) | ✅ |
| 0x826D5A40 | 104 | `XMsgSystemProcessCall` | 0 | XMsg: 0x7003B ao app 0xFA | ✅ |
| 0x826D5BD0 | 124 | `XMsgInProcessCall` | 0 | XMsg: 0x7003D ao app 0xFA | ✅ |
| 0x826D5C50 | 356 | `XMsgStartIORequest` | 0 | XMsg: 0x7002E ao app 0xFA; 0x7000D ao app 0xFA | ✅ |
| 0x826D5EA8 | 136 | `XMsgStartIORequest` | 0 | XMsg: 0x7000E ao app 0xFA | ✅ |
| 0x826D5F30 | 92 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70002 ao app 0xFA | ✅ |
| 0x826D5FC8 | 80 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70004 ao app 0xFA | ✅ |
| 0x826D60C0 | 144 | `XMsgStartIORequest` | 0 | XMsg: 0x70008 ao app 0xFA | ✅ |
| 0x826D6150 | 144 | `XMsgStartIORequest` | 0 | XMsg: 0x70029 ao app 0xFA | ✅ |
| 0x826D61E0 | 128 | `XMsgStartIORequest` | 0 | XMsg: 0x70009 ao app 0xFA | ✅ |
| 0x826D6260 | 128 | `XMsgStartIORequest` | 0 | XMsg: 0x7000B ao app 0xFA | ✅ |
| 0x826D62E0 | 124 | `XMsgStartIORequest` | 0 | XMsg: 0x7000C ao app 0xFA | ✅ |
| 0x826D6408 | 136 | `XMsgStartIORequest` | 2 | XMsg: 0x7001B ao app 0xFA | ✅ |
| 0x826D6528 | 60 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70003 ao app 0xFA | ✅ |
| 0x826D6578 | 60 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70005 ao app 0xFA | ✅ |
| 0x826D65B8 | 60 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70006 ao app 0xFA | ✅ |
| 0x826D65F8 | 60 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70007 ao app 0xFA | ✅ |
| 0x8280E0B0 | 772 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x8280E538 | 312 | `XMsgInProcessCall` | 1 | XMsg: 0x58037 ao app 0xFC | ✅ |
| 0x8280E7B0 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x5801F ao app 0xFC | ✅ |
| 0x8280EB58 | 756 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8280EE50 | 736 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8280F668 | 660 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8280FE80 | 124 | `XMsgInProcessCall` | 0 | XMsg: 0x58032 ao app 0xFC | ✅ |
| 0x82810028 | 228 | `XMsgInProcessCall` | 147 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x828365C8 | 780 | `XMsgStartIORequestEx` | 0 |  | ✅ |
| 0x828385A8 | 664 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82838E38 | 664 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283A1C8 | 980 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283AC90 | 628 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283AF08 | 696 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283B1C0 | 688 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283B470 | 612 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283B6D8 | 640 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283BBB0 | 940 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283DD40 | 616 | `XMsgStartIORequestEx` | 0 |  | ✅ |
| 0x8283F5E0 | 560 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283FC40 | 896 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283FFC0 | 868 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828416C8 | 620 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82841938 | 772 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828423D8 | 668 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843338 | 740 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846768 | 560 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828480F8 | 396 | `XMsgStartIORequest` | 1 | XMsg: 0xB0010 ao app 0xFB (Live) | ✅ |
| 0x82848288 | 160 | `XMsgStartIORequest` | 0 | XMsg: 0xB0018 ao app 0xFB (Live) | ✅ |
| 0x82848898 | 240 | `XMsgStartIORequest` | 2 | XMsg: 0xB0016 ao app 0xFB (Live) | ✅ |
| 0x82848988 | 244 | `XMsgStartIORequest` | 0 | XMsg: 0xB001C ao app 0xFB (Live) | ✅ |
| 0x82848A80 | 228 | `XMsgStartIORequest` | 0 | XMsg: 0xB001B ao app 0xFB (Live) | ✅ |
| 0x82848B68 | 92 | `XMsgInProcessCall` | 0 | XMsg: 0xB0019 ao app 0xFB (Live) | ✅ |
| 0x82848BC8 | 160 | `XMsgStartIORequest` | 0 | XMsg: 0xB0026 ao app 0xFB (Live) | ✅ |
| 0x82848DA8 | 80 | `XMsgInProcessCall` | 3 | XMsg: 0x5800E ao app 0xFC | ✅ |
| 0x82848DF8 | 80 | `XMsgInProcessCall` | 0 | XMsg: 0x5800C ao app 0xFC | ✅ |
| 0x82848E48 | 80 | `XMsgInProcessCall` | 0 | XMsg: 0x5800D ao app 0xFC | ✅ |
| 0x8284E790 | 144 | `XMsgStartIORequest` | 0 | XMsg: 0x2000D ao app 0xFE (xam) | ✅ |
| 0x82854000 | 1092 | `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask` | 0 |  | ✅ |
| 0x828888F0 | 876 | `XamVoiceHeadsetPresent` | 1 | `XamVoiceHeadsetPresent`: kernel XVoiced* | ✅ |
| 0x82888EF0 | 280 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x828890F8 | 772 | `XamVoiceSubmitPacket` | 1 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |

<details><summary><b>Runtime NUI embutido</b> (147 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x8262CE50 | 128 | `XMsgStartIORequest` | 1 | XMsg: 0xB0008 ao app 0xFB (Live) | ✅ |
| 0x826D5988 | 80 | `XMsgStartIORequestEx` | 0 |  | ✅ |
| 0x826D5AA8 | 292 | `XMsgInProcessCall` | 0 | XMsg: 0x70025 ao app 0xFA | ✅ |
| 0x826D5DB8 | 240 | `XMsgStartIORequestEx` | 0 | XMsg: 0x70013 ao app 0xFA | ✅ |
| 0x826D5F90 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70003 ao app 0xFA | ✅ |
| 0x826D6018 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70005 ao app 0xFA | ✅ |
| 0x826D6050 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70006 ao app 0xFA | ✅ |
| 0x826D6088 | 56 | `XMsgSystemProcessCall` | 0 | XMsg: 0x70007 ao app 0xFA | ✅ |
| 0x826D6360 | 168 | `XMsgStartIORequestEx` | 2 | XMsg: 0x7001A ao app 0xFA | ✅ |
| 0x8280DE20 | 352 | `XMsgInProcessCall` | 1 | XMsg: 0x58020 ao app 0xFC | ✅ |
| 0x8280DF80 | 304 | `XMsgInProcessCall` | 3 | XMsg: 0x58023 ao app 0xFC | ✅ |
| 0x8280E3B8 | 384 | `XMsgInProcessCall` | 0 | XMsg: 0x58019 ao app 0xFC | ✅ |
| 0x8280E670 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x5801E ao app 0xFC | ✅ |
| 0x8280E8F0 | 612 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8280F130 | 652 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8280F3C0 | 676 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8280F900 | 576 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x8280FC60 | 68 | `XMsgInProcessCall` | 0 | XMsg: 0x58004 ao app 0xFC | ✅ |
| 0x8280FDC0 | 68 | `XMsgInProcessCall` | 2 | XMsg: 0x58006 ao app 0xFC | ✅ |
| 0x8280FE08 | 116 | `XMsgInProcessCall` | 2 | XMsg: 0x58007 ao app 0xFC | ✅ |
| 0x828368D8 | 452 | `XMsgInProcessCall` | 0 | XMsg: 0x58038 ao app 0xFC | ✅ |
| 0x82836AA0 | 488 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82836C88 | 352 | `XMsgInProcessCall` | 0 | XMsg: 0x58030 ao app 0xFC | ✅ |
| 0x82836DE8 | 352 | `XMsgInProcessCall` | 0 | XMsg: 0x58031 ao app 0xFC | ✅ |
| 0x82836F48 | 752 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82837238 | 544 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82837458 | 336 | `XMsgInProcessCall` | 0 | XMsg: 0x5802E ao app 0xFC | ✅ |
| 0x828375A8 | 368 | `XMsgInProcessCall` | 0 | XMsg: 0x5802C ao app 0xFC | ✅ |
| 0x82837718 | 336 | `XMsgInProcessCall` | 0 | XMsg: 0x5802B ao app 0xFC | ✅ |
| 0x82837868 | 728 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82837B40 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x58024 ao app 0xFC | ✅ |
| 0x82837C80 | 384 | `XMsgInProcessCall` | 0 | XMsg: 0x5802A ao app 0xFC | ✅ |
| 0x82837E00 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x58025 ao app 0xFC | ✅ |
| 0x82837F40 | 596 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x82838198 | 688 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82838448 | 352 | `XMsgInProcessCall` | 0 | XMsg: 0x5802D ao app 0xFC | ✅ |
| 0x82838840 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82838A38 | 328 | `XMsgInProcessCall` | 0 | XMsg: 0x58015 ao app 0xFC | ✅ |
| 0x82838B80 | 692 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828390D0 | 524 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828392E0 | 540 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82839500 | 532 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82839718 | 524 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82839928 | 564 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82839B60 | 548 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82839D88 | 532 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82839FA0 | 548 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283A5A0 | 596 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283A7F8 | 604 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283AA58 | 564 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283B958 | 596 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283BF60 | 572 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283C1A0 | 588 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283C3F0 | 572 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283C630 | 680 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283C8D8 | 728 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283CBB0 | 572 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283CDF0 | 572 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283D030 | 476 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283D210 | 280 | `XMsgInProcessCall` | 0 | XMsg: 0x58016 ao app 0xFC | ✅ |
| 0x8283D328 | 556 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283D558 | 320 | `XMsgInProcessCall` | 0 | XMsg: 0x5801C ao app 0xFC | ✅ |
| 0x8283D698 | 352 | `XMsgInProcessCall` | 0 | XMsg: 0x5801D ao app 0xFC | ✅ |
| 0x8283D7F8 | 344 | `XMsgInProcessCall` | 0 | XMsg: 0x58014 ao app 0xFC | ✅ |
| 0x8283D950 | 452 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283DB18 | 552 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283DFA8 | 604 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283E208 | 468 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283E3E0 | 468 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283E5B8 | 572 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283E7F8 | 436 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283E9B0 | 556 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283EBE0 | 588 | `XMsgInProcessCall`, `XMsgStartIORequest` | 0 | XMsg: 0x58017 ao app 0xFC | ✅ |
| 0x8283EE30 | 516 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283F038 | 516 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283F240 | 472 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283F418 | 452 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283F810 | 536 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x8283FA28 | 536 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82840328 | 480 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82840508 | 480 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828406E8 | 436 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828408A0 | 468 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82840A78 | 456 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82840C40 | 480 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82840E20 | 516 | `XMsgStartIORequest` | 1 |  | ✅ |
| 0x82841028 | 516 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82841230 | 484 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82841418 | 688 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82841C40 | 468 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82841E18 | 456 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82841FE0 | 492 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828421D0 | 516 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82842678 | 576 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828428B8 | 488 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82842AA0 | 508 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82842CA0 | 564 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82842ED8 | 636 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843158 | 480 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843620 | 504 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843818 | 508 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843A18 | 492 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843C08 | 548 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82843E30 | 624 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828440A0 | 588 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828442F0 | 600 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82844548 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82844740 | 484 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82844928 | 672 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82844BC8 | 672 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82844E68 | 548 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845090 | 632 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845308 | 528 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845518 | 632 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845790 | 544 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828459B0 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845BA8 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845DA0 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82845F98 | 484 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846180 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846378 | 484 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846560 | 520 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846998 | 436 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846B50 | 472 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846D28 | 452 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82846EF0 | 436 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828470A8 | 576 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828472E8 | 468 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828474C0 | 516 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828476C8 | 500 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x828478C0 | 524 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82847AD0 | 528 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82847CE0 | 528 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82847EF0 | 516 | `XMsgStartIORequest` | 0 |  | ✅ |
| 0x82848328 | 172 | `XMsgStartIORequest` | 3 | XMsg: 0xB0011 ao app 0xFB (Live) | ✅ |
| 0x828483D8 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x82848480 | 168 | `XMsgStartIORequest` | 1 | XMsg: 0xB0012 ao app 0xFB (Live) | ✅ |
| 0x82848528 | 164 | `XMsgStartIORequest` | 0 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x828485D0 | 164 | `XMsgStartIORequest` | 1 | XMsg: 0xB0013 ao app 0xFB (Live) | ✅ |
| 0x82848678 | 216 | `XMsgStartIORequest` | 1 | XMsg: 0xB001A ao app 0xFB (Live) | ✅ |
| 0x82848750 | 152 | `XMsgStartIORequest` | 2 | XMsg: 0xB0014 ao app 0xFB (Live) | ✅ |
| 0x828487E8 | 172 | `XMsgStartIORequest` | 1 | XMsg: 0xB0015 ao app 0xFB (Live) | ✅ |
| 0x82848CE8 | 192 | `XMsgStartIORequest` | 1 | XMsg: 0xB0025 ao app 0xFB (Live) | ✅ |
| 0x8284E728 | 100 | `XMsgStartIORequest` | 0 | XMsg: 0x2000B ao app 0xFE (xam) | ✅ |
| 0x82889508 | 248 | `XMsgCancelIORequest`, `XamVoiceClose` | 2 | `XamVoiceClose`: voz (kernel XVoiced*) | ✅ |
| 0x82889600 | 180 | `XamVoiceClose`, `XamVoiceCreate` | 1 | `XamVoiceClose`: voz (kernel XVoiced*); `XamVoiceCreate`: voz (kernel XVoiced*) | ✅ |
| 0x8288EB00 | 208 | `XamVoiceSubmitPacket` | 2 | `XamVoiceSubmitPacket`: kernel XVoiced* | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XMsgCancelIORequest` | xam 0x1F8 | 2 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 54 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 155 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 5 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 11 | 0 | ✅ |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

### Todas as importações, por área

<details><summary><b>Rede</b> (91)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `NetDll_WSACancelOverlappedIO` | xam 0x11 | 1 | 0 | ❌ |
| `NetDll_WSACleanup` | xam 0x2 | 1 | 0 | ✅ |
| `NetDll_WSACloseEvent` | xam 0x1E | 1 | 0 | ✅ |
| `NetDll_WSACreateEvent` | xam 0x1D | 1 | 0 | ✅ |
| `NetDll_WSAEventSelect` | xam 0x23 | 1 | 0 | ✅ |
| `NetDll_WSAGetLastError` | xam 0x1B | 1 | 0 | ✅ |
| `NetDll_WSAGetOverlappedResult` | xam 0x10 | 1 | 0 | ✅ |
| `NetDll_WSARecv` | xam 0x13 | 1 | 0 | ❌ |
| `NetDll_WSARecvFrom` | xam 0x15 | 1 | 0 | ✅ |
| `NetDll_WSAResetEvent` | xam 0x20 | 1 | 0 | ✅ |
| `NetDll_WSASend` | xam 0x17 | 1 | 0 | ❌ |
| `NetDll_WSASendTo` | xam 0x19 | 1 | 0 | ✅ |
| `NetDll_WSASetEvent` | xam 0x1F | 1 | 0 | ✅ |
| `NetDll_WSASetLastError` | xam 0x1C | 1 | 0 | ✅ |
| `NetDll_WSAStartup` | xam 0x1 | 1 | 0 | ✅ |
| `NetDll_WSAWaitForMultipleEvents` | xam 0x21 | 1 | 0 | ✅ |
| `NetDll_XNetCleanup` | xam 0x34 | 1 | 0 | ✅ |
| `NetDll_XNetConnect` | xam 0x41 | 1 | 0 | ✅ |
| `NetDll_XNetCreateKey` | xam 0x36 | 1 | 0 | ✅ |
| `NetDll_XNetDnsLookup` | xam 0x43 | 1 | 0 | ✅ |
| `NetDll_XNetDnsRelease` | xam 0x44 | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetBroadcastVersionStatus` | xam 0x4C | 1 | 0 | ✅ |
| `NetDll_XNetGetConnectStatus` | xam 0x42 | 1 | 0 | ✅ |
| `NetDll_XNetGetDebugXnAddr` | xam 0x4A | 1 | 0 | ⚪ stub |
| `NetDll_XNetGetEthernetLinkStatus` | xam 0x4B | 1 | 0 | ✅ |
| `NetDll_XNetGetOpt` | xam 0x4E | 1 | 0 | 🟡 |
| `NetDll_XNetGetTitleXnAddr` | xam 0x49 | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToServer` | xam 0x3D | 1 | 0 | ❌ |
| `NetDll_XNetInAddrToString` | xam 0x3E | 1 | 0 | ✅ |
| `NetDll_XNetInAddrToXnAddr` | xam 0x3C | 1 | 0 | ✅ |
| `NetDll_XNetQosGetListenStats` | xam 0x4D | 1 | 0 | ❌ |
| `NetDll_XNetQosListen` | xam 0x45 | 1 | 0 | ✅ |
| `NetDll_XNetQosLookup` | xam 0x46 | 1 | 0 | ✅ |
| `NetDll_XNetQosRelease` | xam 0x48 | 1 | 0 | ✅ |
| `NetDll_XNetQosServiceLookup` | xam 0x47 | 1 | 0 | ⚪ stub |
| `NetDll_XNetRandom` | xam 0x35 | 1 | 0 | ✅ |
| `NetDll_XNetRegisterKey` | xam 0x37 | 1 | 0 | ✅ |
| `NetDll_XNetServerToInAddr` | xam 0x3A | 1 | 0 | ❌ |
| `NetDll_XNetSetOpt` | xam 0x4F | 1 | 0 | ❌ |
| `NetDll_XNetStartup` | xam 0x33 | 1 | 0 | ✅ |
| `NetDll_XNetTsAddrToInAddr` | xam 0x3B | 1 | 0 | ❌ |
| `NetDll_XNetUnregisterInAddr` | xam 0x3F | 1 | 0 | ✅ |
| `NetDll_XNetUnregisterKey` | xam 0x38 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToInAddr` | xam 0x39 | 1 | 0 | ✅ |
| `NetDll_XNetXnAddrToMachineId` | xam 0x40 | 1 | 0 | ✅ |
| `NetDll_XnpCapture` | xam 0x6C | 1 | 0 | ❌ |
| `NetDll_XnpConfig` | xam 0x68 | 1 | 0 | ❌ |
| `NetDll_XnpConfigUPnP` | xam 0x67 | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptRecv` | xam 0x6F | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptSetCallbacks` | xam 0x6D | 1 | 0 | ❌ |
| `NetDll_XnpEthernetInterceptXmit` | xam 0x6E | 1 | 0 | ❌ |
| `NetDll_XnpGetActiveSocketList` | xam 0x7A | 1 | 0 | ❌ |
| `NetDll_XnpGetConfigStatus` | xam 0x69 | 1 | 0 | ❌ |
| `NetDll_XnpGetVlanXboxName` | xam 0x78 | 1 | 0 | ❌ |
| `NetDll_XnpLoadConfigParams` | xam 0x65 | 1 | 0 | ❌ |
| `NetDll_XnpLoadMachineAccount` | xam 0x6A | 1 | 0 | ❌ |
| `NetDll_XnpLogonClearQEvent` | xam 0x74 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQFlags` | xam 0x71 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetQVals` | xam 0x75 | 1 | 0 | ❌ |
| `NetDll_XnpLogonGetStatus` | xam 0x70 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetPState` | xam 0x77 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQEvent` | xam 0x73 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQFlags` | xam 0x72 | 1 | 0 | ❌ |
| `NetDll_XnpLogonSetQVals` | xam 0x76 | 1 | 0 | ❌ |
| `NetDll_XnpNoteSystemTime` | xam 0x7B | 1 | 0 | ❌ |
| `NetDll_XnpRegisterKeyForCallerType` | xam 0x7C | 1 | 0 | ❌ |
| `NetDll_XnpSaveConfigParams` | xam 0x66 | 1 | 0 | ❌ |
| `NetDll_XnpSaveMachineAccount` | xam 0x6B | 1 | 0 | ❌ |
| `NetDll_XnpSetVlanXboxName` | xam 0x79 | 1 | 0 | ❌ |
| `NetDll_XnpUnregisterKeyForCallerType` | xam 0x7D | 1 | 0 | ❌ |
| `NetDll___WSAFDIsSet` | xam 0x22 | 1 | 0 | ✅ |
| `NetDll_accept` | xam 0xE | 1 | 0 | ✅ |
| `NetDll_bind` | xam 0xB | 1 | 0 | ✅ |
| `NetDll_closesocket` | xam 0x4 | 1 | 0 | ✅ |
| `NetDll_connect` | xam 0xC | 1 | 0 | ✅ |
| `NetDll_getpeername` | xam 0xA | 1 | 0 | ✅ |
| `NetDll_getsockname` | xam 0x9 | 1 | 0 | ✅ |
| `NetDll_getsockopt` | xam 0x8 | 1 | 0 | ✅ |
| `NetDll_inet_addr` | xam 0x1A | 1 | 0 | ✅ |
| `NetDll_ioctlsocket` | xam 0x6 | 1 | 0 | ✅ |
| `NetDll_listen` | xam 0xD | 1 | 0 | ✅ |
| `NetDll_recv` | xam 0x12 | 1 | 0 | ✅ |
| `NetDll_recvfrom` | xam 0x14 | 1 | 0 | ✅ |
| `NetDll_select` | xam 0xF | 1 | 0 | ✅ |
| `NetDll_send` | xam 0x16 | 1 | 0 | ✅ |
| `NetDll_sendto` | xam 0x18 | 1 | 0 | ✅ |
| `NetDll_setsockopt` | xam 0x7 | 1 | 0 | ✅ |
| `NetDll_shutdown` | xam 0x5 | 1 | 0 | ✅ |
| `NetDll_socket` | xam 0x3 | 1 | 0 | ✅ |
| `XNetLogonGetMachineID` | xam 0x135 | 29 | 0 | ⚪ stub |
| `XNetLogonGetTitleID` | xam 0x136 | 38 | 0 | ✅ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (58)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 2 | 0 | ✅ |
| `ExEventObjectType` | xboxkrnl 0xE | 0 | 0 | ✅ |
| `ExSemaphoreObjectType` | xboxkrnl 0x17 | 0 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `ExTimerObjectType` | xboxkrnl 0x1C | 0 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 97 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 5 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 8 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnableFpuExceptions` | xboxkrnl 0x5D | 1 | 0 | ⚪ stub |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 1 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 55 | 0 | ✅ |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 1 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KePulseEvent` | xboxkrnl 0x7F | 1 | 0 | ✅ |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 1 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 3 | 0 | ✅ |
| `KeRaiseIrqlToDpcLevel` | xboxkrnl 0x85 | 98 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 113 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 6 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 2 | 0 | ✅ |
| `KeSetDisableBoostThread` | xboxkrnl 0x9C | 1 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 13 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 3 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 0 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 2 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 3 | 0 | ✅ |
| `KeTryToAcquireSpinLockAtRaisedIrql` | xboxkrnl 0xAE | 2 | 0 | 🟡 |
| `KeUnlockL2` | xboxkrnl 0x6C | 2 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 1 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 12 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 6 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 8 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtQueueApcThread` | xboxkrnl 0xE3 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtSignalAndWaitForSingleObjectEx` | xboxkrnl 0xFB | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 3 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 2 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 116 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 22 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 322 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 4 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 7 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (45)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscGetCacheElementCount` | xboxkrnl 0x20 | 1 | 0 | ⚪ stub |
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
| `NtClose` | xboxkrnl 0xCF | 15 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 2 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 3 | 0 | ✅ |
| `NtCreateIoCompletion` | xboxkrnl 0xD3 | 1 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 8 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 2 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 2 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 7 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 4 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 3 | 0 | ✅ |
| `NtRemoveIoCompletion` | xboxkrnl 0xF4 | 1 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 13 | 0 | ✅ |
| `NtSetIoCompletion` | xboxkrnl 0xF8 | 1 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 3 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 21 | 0 | ✅ |
| `ObLookupThreadByThreadId` | xboxkrnl 0x10B | 1 | 0 | ✅ |
| `ObOpenObjectByName` | xboxkrnl 0x10D | 3 | 0 | ✅ |
| `ObOpenObjectByPointer` | xboxkrnl 0x10E | 1 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 8 | 0 | ✅ |
| `XamContentClose` | xam 0x25A | 1 | 0 | ✅ |
| `XamContentCreate` | xam 0x258 | 1 | 0 | ✅ |
| `XamContentCreateDeviceEnumerator` | xam 0x25D | 1 | 0 | ✅ |
| `XamContentCreateEnumerator` | xam 0x25C | 1 | 0 | ✅ |
| `XamContentCreateEx` | xam 0x259 | 1 | 0 | ✅ |
| `XamContentDelete` | xam 0x25B | 1 | 0 | ✅ |
| `XamContentFlush` | xam 0x267 | 1 | 0 | ⚪ stub |
| `XamContentGetCreator` | xam 0x262 | 1 | 0 | ✅ |
| `XamContentGetDeviceData` | xam 0x25E | 1 | 0 | ✅ |
| `XamContentGetDeviceName` | xam 0x25F | 1 | 0 | ✅ |
| `XamContentGetDeviceState` | xam 0x265 | 2 | 0 | ⚪ stub |
| `XamContentGetLicenseMask` | xam 0x266 | 1 | 0 | ⚪ stub |
| `XamContentGetThumbnail` | xam 0x261 | 1 | 0 | ✅ |
| `XamContentInstall` | xam 0x26A | 1 | 0 | ❌ |
| `XamContentLaunchImage` | xam 0x263 | 1 | 0 | ❌ |
| `XamContentSetThumbnail` | xam 0x260 | 1 | 0 | ✅ |

</details>

<details><summary><b>Usuários, perfis e Live</b> (34)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamParseGamerTileKey` | xam 0x30A | 1 | 0 | ✅ |
| `XamSessionCreateHandle` | xam 0x316 | 1 | 0 | ✅ |
| `XamSessionRefObjByHandle` | xam 0x317 | 12 | 0 | ✅ |
| `XamShowAchievementsUI` | xam 0x2C5 | 1 | 0 | ✅ |
| `XamShowDeviceSelectorUI` | xam 0x2CB | 1 | 0 | ✅ |
| `XamShowDirtyDiscErrorUI` | xam 0x2D9 | 1 | 0 | ✅ |
| `XamShowFriendRequestUI` | xam 0x2CE | 1 | 0 | ❌ |
| `XamShowFriendsUI` | xam 0x2BF | 1 | 0 | ❌ |
| `XamShowGameInviteUI` | xam 0x2CD | 1 | 0 | ❌ |
| `XamShowGamerCardUIForXUID` | xam 0x2D5 | 1 | 0 | ❌ |
| `XamShowKeyboardUI` | xam 0x2C1 | 1 | 0 | ✅ |
| `XamShowMarketplaceUI` | xam 0x2C7 | 1 | 0 | 🟡 |
| `XamShowMessageBoxUI` | xam 0x2CA | 1 | 0 | ✅ |
| `XamShowMessageBoxUIEx` | xam 0x2DC | 1 | 0 | ✅ |
| `XamShowMessageComposeUI` | xam 0x2CC | 1 | 0 | ❌ |
| `XamShowMessagesUI` | xam 0x2C0 | 1 | 0 | ❌ |
| `XamShowPlayerReviewUI` | xam 0x2C6 | 1 | 0 | ❌ |
| `XamShowPlayersUI` | xam 0x2C8 | 1 | 0 | ❌ |
| `XamShowQuickChatUI` | xam 0x2C2 | 1 | 0 | ❌ |
| `XamShowSigninUI` | xam 0x2BC | 1 | 0 | ✅ |
| `XamShowSigninUIp` | xam 0x2BE | 1 | 0 | ✅ |
| `XamUserAreUsersFriends` | xam 0x213 | 2 | 0 | 🟡 |
| `XamUserCheckPrivilege` | xam 0x212 | 3 | 0 | 🟡 |
| `XamUserCreateAchievementEnumerator` | xam 0x2EE | 1 | 0 | 🟡 |
| `XamUserCreatePlayerEnumerator` | xam 0x309 | 1 | 0 | ❌ |
| `XamUserCreateStatsEnumerator` | xam 0x2F7 | 3 | 0 | 🟡 |
| `XamUserGetMembershipTierFromXUID` | xam 0x217 | 1 | 0 | ✅ |
| `XamUserGetName` | xam 0x20E | 1 | 0 | ✅ |
| `XamUserGetOnlineCountryFromXUID` | xam 0x218 | 7 | 0 | ✅ |
| `XamUserGetSigninState` | xam 0x210 | 3 | 0 | ✅ |
| `XamUserGetXUID` | xam 0x20A | 5 | 0 | ✅ |
| `XamUserReadProfileSettings` | xam 0x219 | 4 | 0 | ✅ |
| `XamUserWriteProfileSettings` | xam 0x21A | 1 | 0 | ✅ |
| `XamWriteGamerTile` | xam 0x2F0 | 1 | 0 | 🟡 |

</details>

<details><summary><b>Vídeo e GPU</b> (30)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 0 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 0 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 2 | 0 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 1 | 0 | ⚪ stub |
| `VdGetGraphicsAsicID` | xboxkrnl 0x1BC | 6 | 0 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 0 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEDRAM` | xboxkrnl 0x268 | 1 | 0 | ❌ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 0 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 0 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 0 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 0 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 0 | 🟡 |
| `VdQuerySystemCommandBuffer` | xboxkrnl 0x1C8 | 1 | 0 | ❌ |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBuffer` | xboxkrnl 0x1D8 | 1 | 0 | ❌ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 1 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |
| `XGetVideoMode` | xam 0x3D1 | 2 | 0 | 🟡 |
| `XamReadTileToTexture` | xam 0x2F5 | 3 | 0 | ⚪ stub |

</details>

<details><summary><b>Áudio</b> (23)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioQueryDriverPerformance` | xboxkrnl 0x1FD | 1 | 0 | ⚪ stub |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 3 | 0 | ✅ |
| `XMABlockWhileInUse` | xboxkrnl 0x235 | 2 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 5 | 0 | ✅ |
| `XMADisableContext` | xboxkrnl 0x228 | 9 | 0 | ✅ |
| `XMAEnableContext` | xboxkrnl 0x227 | 8 | 0 | ✅ |
| `XMAGetOutputBufferReadOffset` | xboxkrnl 0x22B | 3 | 0 | ✅ |
| `XMAGetOutputBufferWriteOffset` | xboxkrnl 0x229 | 3 | 0 | ✅ |
| `XMAInitializeContext` | xboxkrnl 0x225 | 2 | 0 | ✅ |
| `XMAIsInputBuffer0Valid` | xboxkrnl 0x22F | 4 | 0 | ✅ |
| `XMAIsInputBuffer1Valid` | xboxkrnl 0x231 | 4 | 0 | ✅ |
| `XMAIsOutputBufferValid` | xboxkrnl 0x22D | 2 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 5 | 0 | ✅ |
| `XMASetInputBuffer0` | xboxkrnl 0x232 | 2 | 0 | ✅ |
| `XMASetInputBuffer0Valid` | xboxkrnl 0x22E | 2 | 0 | ✅ |
| `XMASetInputBuffer1` | xboxkrnl 0x233 | 2 | 0 | ✅ |
| `XMASetInputBuffer1Valid` | xboxkrnl 0x230 | 2 | 0 | ✅ |
| `XMASetInputBufferReadOffset` | xboxkrnl 0x237 | 1 | 0 | ✅ |
| `XMASetLoopData` | xboxkrnl 0x236 | 2 | 0 | ✅ |
| `XMASetOutputBufferReadOffset` | xboxkrnl 0x22A | 2 | 0 | ✅ |
| `XMASetOutputBufferValid` | xboxkrnl 0x22C | 4 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (18)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 7 | 0 | ✅ |
| `RtlCaptureContext` | xboxkrnl 0x119 | 1 | 0 | ❌ |
| `RtlCompareMemory` | xboxkrnl 0x11A | 1 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 10 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 2 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 19 | 0 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 2 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 6 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 4 | 0 | ⚪ stub |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 2 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 2 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_vsnprintf` | xboxkrnl 0x14D | 4 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 3 | 0 | ✅ |
| `vsprintf` | xboxkrnl 0x14E | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExAllocatePoolWithTag` | xboxkrnl 0xA | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 3 | 0 | ✅ |
| `MmAllocatePhysicalMemory` | xboxkrnl 0xB9 | 1 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 2 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 3 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 1 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 1 | 0 | ✅ |
| `MmQueryAllocationSize` | xboxkrnl 0xC5 | 1 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 14 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 11 | 0 | ✅ |
| `NtProtectVirtualMemory` | xboxkrnl 0xE1 | 2 | 0 | ✅ |
| `XamAlloc` | xam 0x1EA | 148 | 0 | ✅ |
| `XamFree` | xam 0x1EC | 169 | 0 | ✅ |

</details>

<details><summary><b>Criptografia e chaves do console</b> (15)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XeCryptAesEcb` | xboxkrnl 0x15A | 1 | 0 | ✅ |
| `XeCryptAesKey` | xboxkrnl 0x159 | 1 | 0 | ✅ |
| `XeCryptBnQwNeRsaKeyGen` | xboxkrnl 0x16B | 1 | 0 | ❌ |
| `XeCryptBnQwNeRsaPrvCrypt` | xboxkrnl 0x16C | 1 | 0 | ❌ |
| `XeCryptBnQwNeRsaPubCrypt` | xboxkrnl 0x16D | 1 | 0 | ✅ |
| `XeCryptBnQw_SwapDwQwLeBe` | xboxkrnl 0x170 | 21 | 0 | ✅ |
| `XeCryptRandom` | xboxkrnl 0x18A | 3 | 0 | ⚪ stub |
| `XeCryptRc4Ecb` | xboxkrnl 0x18C | 1 | 0 | ✅ |
| `XeCryptRc4Key` | xboxkrnl 0x18B | 1 | 0 | ✅ |
| `XeCryptShaFinal` | xboxkrnl 0x191 | 1 | 0 | ✅ |
| `XeCryptShaInit` | xboxkrnl 0x18F | 1 | 0 | ✅ |
| `XeCryptShaUpdate` | xboxkrnl 0x190 | 1 | 0 | ✅ |
| `XeKeysGetKey` | xboxkrnl 0x244 | 1 | 0 | 🟡 |
| `XeKeysGetKeyProperties` | xboxkrnl 0x241 | 1 | 0 | ✅ |
| `XeKeysQwNeRsaPrvCrypt` | xboxkrnl 0x248 | 1 | 0 | ❌ |

</details>

<details><summary><b>Sistema e console</b> (14)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 12 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XGetLanguage` | xam 0x3CD | 1 | 0 | ✅ |
| `XamGetExecutionId` | xam 0x280 | 1 | 0 | ✅ |
| `XamGetPrivateEnumStructureFromHandle` | xam 0x24F | 1 | 0 | ✅ |
| `XamGetSystemVersion` | xam 0x282 | 3 | 0 | ✅ |
| `XamLoaderGetLaunchData` | xam 0x1A8 | 1 | 0 | 🟡 |
| `XamLoaderGetLaunchDataSize` | xam 0x1A7 | 3 | 0 | 🟡 |
| `XamLoaderLaunchTitle` | xam 0x1A4 | 1 | 0 | 🟡 |
| `XamLoaderSetLaunchData` | xam 0x1A6 | 1 | 0 | 🟡 |
| `XamLoaderTerminateTitle` | xam 0x1A9 | 2 | 0 | 🟡 |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 1 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (7)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |
| `XAudioGetVoiceCategoryVolumeChangeMask` | xboxkrnl 0x1F7 | 1 | 0 | ⚪ stub |
| `XamShowVoiceMailUI` | xam 0x2C3 | 1 | 0 | ❌ |
| `XamVoiceClose` | xam 0x30F | 3 | 0 | ⚪ stub |
| `XamVoiceCreate` | xam 0x30C | 1 | 0 | ⚪ stub |
| `XamVoiceHeadsetPresent` | xam 0x30D | 1 | 0 | ⚪ stub |
| `XamVoiceSubmitPacket` | xam 0x30E | 2 | 0 | ⚪ stub |

</details>

<details><summary><b>Mensagens XMsg (apps do sistema)</b> (5)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XMsgCancelIORequest` | xam 0x1F8 | 2 | 0 | ✅ |
| `XMsgInProcessCall` | xam 0x1F4 | 54 | 0 | ✅ |
| `XMsgStartIORequest` | xam 0x1F7 | 155 | 0 | ✅ |
| `XMsgStartIORequestEx` | xam 0x1FC | 5 | 0 | ✅ |
| `XMsgSystemProcessCall` | xam 0x1F6 | 11 | 0 | ✅ |

</details>

<details><summary><b>Tarefas, enumeração e notificações</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XNotifyGetNext` | xam 0x28B | 12 | 0 | ✅ |
| `XamCreateEnumeratorHandle` | xam 0x24E | 2 | 0 | ✅ |
| `XamEnumerate` | xam 0x250 | 2 | 0 | ✅ |
| `XamNotifyCreateListener` | xam 0x28A | 2 | 0 | ✅ |

</details>

<details><summary><b>Entrada (controle, teclado)</b> (4)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XamInputGetCapabilities` | xam 0x190 | 1 | 0 | 🟡 |
| `XamInputGetKeystrokeEx` | xam 0x198 | 2 | 0 | ✅ |
| `XamInputGetState` | xam 0x191 | 1 | 0 | ✅ |
| `XamInputSetState` | xam 0x192 | 2 | 0 | ✅ |

</details>

<details><summary><b>Outras</b> (2)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `KfLowerIrql` | xboxkrnl 0xB3 | 114 | 0 | ✅ |
| `Refresh` | xam 0x1D7 | 2 | 0 | ⚪ stub |

</details>

**Ausentes no Xenia (55):** `NetDll_WSACancelOverlappedIO`, `NetDll_WSARecv`, `NetDll_WSASend`, `NetDll_XNetInAddrToServer`, `NetDll_XNetQosGetListenStats`, `NetDll_XNetServerToInAddr`, `NetDll_XNetSetOpt`, `NetDll_XNetTsAddrToInAddr`, `NetDll_XnpCapture`, `NetDll_XnpConfig`, `NetDll_XnpConfigUPnP`, `NetDll_XnpEthernetInterceptRecv`, `NetDll_XnpEthernetInterceptSetCallbacks`, `NetDll_XnpEthernetInterceptXmit`, `NetDll_XnpGetActiveSocketList`, `NetDll_XnpGetConfigStatus`, `NetDll_XnpGetVlanXboxName`, `NetDll_XnpLoadConfigParams`, `NetDll_XnpLoadMachineAccount`, `NetDll_XnpLogonClearQEvent`, `NetDll_XnpLogonGetQFlags`, `NetDll_XnpLogonGetQVals`, `NetDll_XnpLogonGetStatus`, `NetDll_XnpLogonSetPState`, `NetDll_XnpLogonSetQEvent`, `NetDll_XnpLogonSetQFlags`, `NetDll_XnpLogonSetQVals`, `NetDll_XnpNoteSystemTime`, `NetDll_XnpRegisterKeyForCallerType`, `NetDll_XnpSaveConfigParams`, `NetDll_XnpSaveMachineAccount`, `NetDll_XnpSetVlanXboxName`, `NetDll_XnpUnregisterKeyForCallerType`, `RtlCaptureContext`, `RtlUnwind`, `VdInitializeEDRAM`, `VdQuerySystemCommandBuffer`, `VdSetSystemCommandBuffer`, `XamContentInstall`, `XamContentLaunchImage`, `XamShowFriendRequestUI`, `XamShowFriendsUI`, `XamShowGameInviteUI`, `XamShowGamerCardUIForXUID`, `XamShowMessageComposeUI`, `XamShowMessagesUI`, `XamShowPlayerReviewUI`, `XamShowPlayersUI`, `XamShowQuickChatUI`, `XamShowVoiceMailUI`, `XamUserCreatePlayerEnumerator`, `XeCryptBnQwNeRsaKeyGen`, `XeCryptBnQwNeRsaPrvCrypt`, `XeKeysQwNeRsaPrvCrypt`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`, `XAudioGetVoiceCategoryVolumeChangeMask`, `XamShowVoiceMailUI`, `XamVoiceClose`, `XamVoiceCreate`, `XamVoiceHeadsetPresent`, `XamVoiceSubmitPacket`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/needspeemost/` (`needspeemost.img`, `needspeemost_imports.json`, `needspeemost_kinect.json`, `needspeemost_fngraph.json`, `c_full/<endereço>.c`: 241 funções).
- Projeto Ghidra: `games/decompilado/needspeemost/ghidra/needspeemost` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
