# Jogos Kinect

Uma nota por jogo: executável, versão do runtime NUI, o que ele pede do sistema e como roda no Xenia Edge.

- [Nike-Kinect-Training](nike-kinect-training.md) — Nike+ Kinect Training (CryENGINE): estado, funções e tudo o que chama do sistema
- [Nike-Entradas](nike-entradas.md) — o que o Nike recebe do Kinect, do sistema e dos próprios arquivos; o estado do menu pelo Kinect
- [Kinect-Sports](kinect-sports.md) — Kinect Sports: estado, runtime NUI e tudo o que chama do sistema
- [Kinect-Adventures](kinect-adventures.md) — Kinect Adventures: estado, motor de inclinação do jogo, problemas corrigidos e tudo o que chama do sistema
- [Kinect-Joy-Ride](kinect-joy-ride.md) — Kinect Joy Ride: runtime NUI do Adventures, microfones com cancelamento de eco, campos do 'XtSs' que lê e tudo o que chama do sistema
- [Kinect-Star-Wars](kinect-star-wars.md) — Kinect Star Wars: estado com gravação, uso do 'XtSs' (chão guardado e gravidade do chão), voz pelo NUISpeechDLL e tudo o que chama do sistema
- [Kinect-Rush](kinect-rush.md) — Kinect Rush (Asobo, Disney-Pixar): estado com gravação, motor de inclinação e campos do 'XtSs', voz no próprio executável e tudo o que chama do sistema
- [Rapala-for-Kinect](rapala-for-kinect.md) — Rapala for Kinect (Activision): runtime NUI 2.0.20764 (motor de inclinação e 'XtSs' do Star Wars), fitness por ordinal, sem voz; estado com gravação
- [Kinect-Sesame-Street-TV](kinect-sesame-street-tv.md) — Kinect Sesame Street TV (dois discos, mesmo executável): runtime NUI 2.0.21119 (parecido com o Rush), câmera colorida, microfones e voz; estado com gravação
- [Fruit-Ninja-Kinect](fruit-ninja-kinect.md) — Fruit Ninja Kinect (Halfbrick, Arcade): runtime NUI 2.0.20500.0, motor de inclinação do Rush, sem voz; com a gravação para no login pelo Kinect (`XamShowNuiSigninUI` → registro no Kinect ID falha)
- [Doodle-Jump-Kinect](doodle-jump-kinect.md) — Doodle Jump for Kinect (Arcade): runtime NUI 2.0.21250.0 (o mais novo), calibração da inclinação em Lua pela API do runtime, sem voz; com a gravação roda 150 s sem travar
- [Michael-Jackson-The-Experience](michael-jackson-the-experience.md) — Michael Jackson The Experience (Ubisoft): runtime NUI 2.0.20353 (motor de inclinação do Rush), dança e canto; microfones do Kinect pelo motor de voz XHV2 (`XamVoiceCreate` stub → canto pelo Kinect não funciona); com a gravação para na tela de título, sem travar
- [Adrenalin-Misfits](adrenalin-misfits.md) — Adrenalin Misfits (Konami): runtime NUI 2.0.11775 com o motor de inclinação do Adventures, tela de solução de problemas e seletor de armazenamento pelo Kinect, avatares; estado com gravação
- [Dance-Dance-Revolution](dance-dance-revolution.md) — Dance Dance Revolution (Konami, tapete de dança): **não usa o Kinect** (sem runtime NUI; só a câmera USB Xbox Live Vision, `XUsbcam*`)
- [Motion-Explosion](motion-explosion.md) — Motion Explosion! (Majesco): runtime NUI 2.0.20764 (o do Rapala), link incremental (trampolins no .xidata), avatares como personagem, sem microfones do Kinect; estado com gravação
- [Harry-Potter-for-Kinect](harry-potter-for-kinect.md) — Harry Potter for Kinect (Eurocom): runtime NUI 2.0.21173.0 com os blocos do Rush (motor de inclinação, 'XtSs', voz no executável), avatares stubs, estado com gravação e o que testar ao vivo
- [Kinect-Sports-Season-Two](kinect-sports-season-two.md) — Kinect Sports: Season Two (4D5309D6): runtime NUI 2.0.20764 com o código do Rush (inclinação, 'XtSs', identidade, voz NUISPEECH no executável), avatares, contador de calorias; com a gravação roda 150 s, mas o jogador nunca passa de POSITION_ONLY (suspeita: o bit 0x10, só dele, nas flags do NuiInitialize)
- [Kinect-Rabbids-Invasion](kinect-rabbids-invasion.md) — Kinect Rabbids Invasion (Ubisoft, **Unity**): Kinect no plugin `unityKinect.xex` com o runtime NUI 2.0.21256 (igual ao do Rush), câmera colorida para fotos, volume do microfone; com a gravação **trava no vídeo de abertura** (plugin de vídeo `XAVpluginX360.xex`, não o Kinect)
- [Carnival-Games-Monkey-See-Monkey-Do](carnival-games-monkey-see-monkey-do.md) — Carnival Games: Monkey See, Monkey Do (Cat Daddy, 2K Play): runtime NUI 2.0.20353.1 (o do Michael Jackson) com o motor de inclinação e o 'XtSs' do Harry Potter, voz no executável, 10 avatares stubs, estado com gravação e o que testar ao vivo
- [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md) — os dois lado a lado: 68 funções NUI idênticas
