# Texto à mão da nota do Sonic Free Riders (5345084D).
NOME = "Sonic Free Riders"
RELACIONADO = ["Kinect-Xenia-Edge", "XamVoiceGetMicArrayAudioEx (816FBCF8)"]
TEXTO = r"""
Sonic Free Riders (Sega, 2010, título de lançamento do Kinect): corrida em pranchas voadoras controlada com o corpo (inclinar para virar, agachar, braços). Jogo só de Kinect, com reconhecimento de voz: carrega `nuispeech:\speech\en-us\*.lxa` e lê a matriz de microfones a cada quadro (`XamVoiceGetMicArrayAudioEx` com `XamVoiceGetMicArrayUnderrunStatus`).

**Defeito achado e corrigido (2026-10-29):** o jogo guarda **dois pedidos estáticos de janela de áudio de 0x44 bytes**, um atrás do outro, em 0x82AFE1B8 e 0x82AFE1FC (`00000044`, 5 ponteiros de buffer, tamanhos 0x200 e 4× 0x400). O xam só escreve o tic em +0x40 quando o pedido declara mais de 0x40 bytes e a hora em +0x44/+0x48 quando declara mais de 0x64; o `KinectAudio::CompleteRequest` do Xenia escrevia os três sempre, e a hora caía em cima do tamanho e do ponteiro do segundo pedido. A partir daí cada janela de áudio (16 ms) era gravada num endereço lixo: constantes de fetch com tipo errado desde o quadro 6125, shaders com opcodes ALU desconhecidos no 6824 e crash em 0x82497E38 (liberador genérico de buffer do motor, com o cabeçalho do buffer sobrescrito) no 7062, ~160 s depois de abrir. Corrigido em 77342ab64 (o tic e a hora só com espaço declarado): 8 385 quadros limpos na rodada seguinte.

Para passar da tela inicial é preciso o gesto de engajamento (corpo inteiro no quadro, mão direita levantada) e depois o cursor de mão.
"""
