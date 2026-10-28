# Kixenia

Um Xenia com **Kinect** e **avatares** do Xbox 360.

Kixenia é o [Xenia Edge](https://github.com/has207/xenia-edge) com o suporte ao Kinect v1 (o sensor do Xbox 360, ligado no PC pelo USB) e o início do suporte aos avatares, mais as notas da engenharia reversa que levaram a isso. O README original do Xenia Edge está em [README.xenia-edge.md](README.xenia-edge.md).

*Kixenia is Xenia Edge with Xbox 360 Kinect support and early avatar support, plus the reverse-engineering notes (in Portuguese) behind them.*

> [!NOTE]
> Projeto **não oficial**, sem ligação com o projeto Xenia nem com a Microsoft. Tudo vem de engenharia reversa de jogos, do sistema do console (xam da atualização 17559) e de hardware obtidos legalmente. **Nada vem das XDKs.**

## O que muda em relação ao Xenia Edge

Cada mudança é um commit com a explicação completa na mensagem: o problema, o que foi feito e em quais arquivos. Para ver: `git log edge..main`, ou a lista de commits no GitHub.

| Área | O que faz |
|---|---|
| **Sensor** (`src/xenia/hid/kinect/`) | Câmera de profundidade e de cor, microfones, motor, LED e acelerômetro pelo USB (libusb, sem dependências no Linux); gravação e reprodução de sessões |
| **Sistema** (`src/xenia/kernel/kinect/`, `xboxkrnl_pscam.cc`, `xam_nui*.cc`, `nui.cc`) | A câmera para os jogos (PsCamDeviceRequest), o estado NUI do xam, inclinação e chão, HUD e gestos, Kinect ID, áudio com cancelamento de eco |
| **GPU** (`draw_util.cc`, `shader_interpreter.cc`, Vulkan) | O que o rastreador de esqueleto precisa: resolves sem vértices, leitura de texturas no interpretador, draws multipass com exportação de memória |
| **Kinect Tuner** (`xam/ui/kinect_tuner_ui.cc`) | Janela com o que o sensor vê, os esqueletos, a inclinação e os microfones; abre pelo menu do botão central |
| **Fitness** (`xam_fitness.cc`) | O perfil corporal que os jogos de exercício leem |
| **Avatares** (`xam_avatar_assets.cc`) | Monta as peças do avatar a partir do pacote de avatares dos jogos |
| **Kernel** | KeInitializeMutant, KeSetTimer, tipos de objeto dos jogos, XCTD, LDI e mais |

## Usar

1. Compile como o Xenia Edge: [docs/building.md](docs/building.md).
2. Configure o Kinect: [docs/kinect.md](docs/kinect.md) explica o sensor, as permissões no Linux, as opções (`allow_kinect_camera`, `kinect_audio_firmware_path`, gravações) e o Kinect Tuner.
3. Para os avatares, ligue `allow_avatar_initialization` (opção em `[Kernel]`). O jogo precisa trazer o arquivo `AvatarAssetPack` na pasta dele, e precisa estar extraído numa pasta (não numa imagem de disco).

## Estado

- Jogos Kinect testados com o sensor real e com gravações: veja [docs/kixenia/kinect/jogos/](docs/kixenia/kinect/jogos/README.md).
- Avatares: corpo, cabeça, cabelo e roupas aparecem; faltam as texturas do rosto, as formas e o esqueleto/animação do corpo.

## Pastas novas

| Pasta | Conteúdo |
|---|---|
| [docs/kixenia/](docs/kixenia/README.md) | As notas de engenharia reversa: Kinect, jogos, avatares e funções do xam |
| [tools/kixenia/](tools/kixenia/README.md) | Ferramentas: avatar, scripts do Ghidra, testes com o Kinect e a exportação das notas |

## Créditos

- O [Xenia](https://github.com/xenia-project/xenia), o [Xenia Canary](https://github.com/xenia-canary/xenia-canary) e o [Xenia Edge](https://github.com/has207/xenia-edge), sobre os quais tudo isto foi feito.
- [knuckleslee](https://github.com/knuckleslee): o primeiro trabalho com o Kinect no Canary; dois commits de GPU aqui são dele.
- [ryantimjohn](https://github.com/ryantimjohn): as notas de NUI e o commit dos draws multipass.
- O protocolo da câmera segue o que o [libfreenect](https://github.com/OpenKinect/libfreenect) documentou.

## Licença

O código segue a licença do Xenia ([LICENSE](LICENSE), BSD). As notas em `docs/kixenia/` estão sob [CC BY 4.0](docs/kixenia/LICENSE.md) e as ferramentas em `tools/kixenia/` sob [MIT](tools/kixenia/LICENSE).
