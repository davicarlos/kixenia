# Texto à mão da nota do The Gunstringer (4D5309DF).
NOME = "The Gunstringer"
TEXTO = r"""
The Gunstringer (Twisted Pixel, 2011): tiro em terceira pessoa com um boneco de marionete; a mão esquerda guia o boneco e a direita mira e atira (gesto de "puxar"). Quase tudo o que fala com o sistema é o runtime NUI embutido (80 de 94 funções). O código próprio cuida de três coisas: a **câmera Xbox Live Vision** (`XUsbcam*`, 9 funções em 0x8269D6B0–0x8269F900: o jogo filma o jogador para o modo de foto; sem a câmera Vision nada é chamado, por isso as importações ausentes não atrapalham), o **cancelamento de eco** para a voz (`XAudio*MECClient`, 0x82813B78) e a identificação do jogador (0x826B5488, `XamNuiIdentityGetSessionId` + `XamUserNuiGetUserIndex`).

Ao vivo (2026-10-29, 5 min): sem crash; 20 de 91 amostras com jogador rastreado. O cursor de mão tremia: o rastreador do jogo roda a 20–28 Hz irregulares (medido pela contagem de esqueletos), com a thread do jogo que faz o rastreamento a ~75–100% de uma CPU; a câmera sozinha entrega 30,0 fps sem perda (`xenia-kinect-camera-test`, 20 s). Investigação em [[Kinect-Xenia-Edge]].
"""
