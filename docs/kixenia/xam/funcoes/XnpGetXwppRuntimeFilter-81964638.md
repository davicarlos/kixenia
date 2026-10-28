# XnpGetXwppRuntimeFilter

**Endereço:** 0x81964638 · **Ordinal:** 199 · **Área:** [Sensor e estado](README.md#sensor-e-estado)

## O que faz
Função que só devolve 0. O xam a reaproveita para várias exports (`XamIsNuiAutomationEnabled`, `XamIsNatalPlaybackEnabled` e outras, como `XnpGetXwppRuntimeFilter`): automação e reprodução de gravações do Kinect ficam sempre desligadas no console de varejo.

## É chamada por
- [FUN_817FD108](FUN_817FD108.md) — HUD: cursor
- e mais 37 funções fora desta base (vizinhas): FUN_816C00A8, XamRequestToken (816C0438), FUN_8172BE40, FUN_81732F20, FUN_81742010, FUN_81773940, FUN_8177F760, FUN_8178E7C0, FUN_817BAF30, FUN_817BB058, FUN_817FEDF0, FUN_8181C470, FUN_81839780, FUN_81839C60, D3DDevice_DrawVertices (8183A220), FUN_81855460, FUN_8189C810, FUN_818CA108, FUN_818CCCA0, FUN_818CCD70, FUN_818D7568, FUN_819022A0, FUN_81902C18, FUN_81954F70, FUN_8195E7C0, FUN_8195EB88, FUN_819618D0, FUN_81962950, FUN_819A51F8, FUN_819A5878 …

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2171 | 0x81964638 | `XamIsNatalPlaybackEnabled` | sempre 0 (mesma função de 0x81964638) | ⚪ stub |
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2170 | 0x81964638 | `XamIsNuiAutomationEnabled` | sempre 0 (0x81964638 devolve 0) | ⚪ stub |
