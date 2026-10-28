# XamXStudioRequest

**Endereço:** 0x816FB3A0 · **Ordinal:** 1670 · **Área:** [Sensor e estado](README.md#sensor-e-estado)

## O que faz
Consulta o estado do dispositivo de voz (XVoiced) usado pelo status do sensor.

## Fatos do decompilado
- Códigos de erro: 0x80004005
- Globais: 0x81AACA04

## É chamada por
- [FUN_817FC750](FUN_817FC750.md) — Sensor: montar status

## Nas notas
- [Kinect-Ajustes-de-Camera](../../kinect/kinect-ajustes-de-camera.md): O jogo chama `PsCamDeviceRequest` por um wrapper (`82F476B8`), que tenta antes o `XamXStudioRequest(0x1003)`, ou seja, o Kinect Studio.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - **`XamXStudioRequest`:** repassa a um gancho que só o Kinect Studio (devkit) instala. Sem ele, devolve `0x80004005`. Os jogos chamam a cada quadro, e o wrapper do PsCam no Nike (82F476B8) tenta primeiro `XamXStudioRequest(0x1003)`.
