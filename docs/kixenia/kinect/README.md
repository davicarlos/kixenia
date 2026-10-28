# Kinect

Como o Kinect do Xbox 360 funciona por dentro (USB, firmware, câmera, áudio, inclinação, NUI no xam, Kinect ID) e o que foi preciso para rodá-lo no Xenia Edge.

Os jogos testados estão em [jogos/](jogos/README.md).

## Kinect no Xenia Edge

- [Kinect-Xenia-Edge](kinect-xenia-edge.md) — **comece aqui**: o que funciona, como usar, testes e o que falta
- [Kinect-ID](kinect-id.md) — reconhecimento facial: API do xam, mensagens 0x2C0xx, o caminho do cadastro e os endereços no Nike
- [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md) — PsCamDeviceRequest 35/36: exposição, ganhos, janela de medição
- [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md) — pacote de firmware (cambridge/altair/detroit/pmc), protocolo CEMD, calibração da matriz

## Engenharia reversa do Kinect

- [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) — visão geral, com as partes por tema:
  - [Kinect-ER-Ferramentas-e-SDK](kinect-er-ferramentas-e-sdk.md) · [Kinect-ER-USB-e-Protocolos](kinect-er-usb-e-protocolos.md) · [Kinect-ER-xam](kinect-er-xam.md) · [Kinect-ER-Jogos-e-Rastreador](kinect-er-jogos-e-rastreador.md) · [Kinect-ER-Sistema-e-Modulos](kinect-er-sistema-e-modulos.md) · [Kinect-ER-Pendencias](kinect-er-pendencias.md)
- [Kinect-Mapa-XAM](kinect-mapa-xam.md) — as 136 exports do Kinect no xam, o que fazem e o estado no Xenia
- [Kinect-SDK-Windows](kinect-sdk-windows.md) — o SDK 1.8 do Windows cruzado com o Xbox
