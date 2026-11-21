# Kinect no Windows: compilar, rodar e o que ainda falha

Guia para usar o Kinect do Xbox 360 (modelo 1473, o que liga na tomada) no Xenia Edge com Kinect, no **Windows 10/11**. Foi testado no Windows 11 com um sensor real, compilando com o Visual Studio 2022 Build Tools, e com o *Nike+ Kinect Training*.

Para o que cada opção faz, veja [docs/kinect.md](../../kinect.md). Para o protocolo do firmware de áudio, veja [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md).

## O que funciona e o que não

| Parte | Estado no Windows |
|---|---|
| Câmera de profundidade e de cor | Funciona, 30 fps, pelo UsbDk |
| Esqueleto | Funciona com `--gpu=vulkan` (veja abaixo) |
| Motor, ângulo, LED, acelerômetro | Funciona, com o firmware de áudio carregado |
| Microfones | **Não funciona direito**: os pacotes chegam em rajadas curtas |

## 1. O que você precisa

- Windows 10 ou 11 de 64 bits, com os [requisitos de compilação](../../building.md) (Visual Studio 2022 ou os Build Tools, Python 3.6+ e CMake).
- O [UsbDk](https://github.com/daynix/UsbDk) instalado (a câmera passa por ele).
- Os drivers do Kinect para Windows (os do SDK 1.8 da Microsoft). A câmera usa o driver `kinectcamera.sys` deles.
- O Kinect ligado na **tomada** (fonte própria) e na USB.
- Para o motor e o microfone: o firmware de áudio da Microsoft, que **este projeto não distribui**. Veja a seção 4.

## 2. Compilar

Use o Python diretamente, em vez de `xb.bat`: o `xb.bat` pode escolher uma instalação de Python que não funciona.

```powershell
python xenia-build.py slang
python xenia-build.py setup
python xenia-build.py build --config=release
```

O executável fica em `build\bin\Windows\Release\xenia_edge.exe`.

Atenção:

- O build executa ferramentas que ele mesmo compila (como `xenia-shader-cc.exe`). Com o **Smart App Control** ligado, o Windows bloqueia esse arquivo e o build falha. Desligue o Smart App Control antes de compilar (não dá para religar sem reinstalar o Windows) ou compile em outra máquina.
- O primeiro `setup` leva alguns minutos (configura o CMake e compila o Mesa).

## 3. Rodar com a câmera

```powershell
.\xenia_edge.exe --gpu=vulkan --allow_kinect_camera=true --allow_nui_initialization=true "C:\caminho\do\jogo\default.xex"
```

**Use `--gpu=vulkan`.** No Windows o padrão é o D3D12, e nele o rastreador de esqueleto dos jogos falha: o log enche de `Unsupported resolve vertex buffer format` e `PM4_DRAW_INDX_2 ... Failed in backend`. O suporte de GPU do rastreador está no Vulkan.

Se a pasta do jogo tiver um espaço no fim do nome, o Windows o descarta e o emulador não acha o arquivo. Copie o jogo para uma pasta de nome limpo.

## 4. Motor e firmware de áudio

O sensor liga sem firmware de áudio (como o dispositivo `045e:02ad`) e o Xenia envia o firmware (`kinect_audio_firmware_path`, veja o [docs/kinect.md](../../kinect.md)). Só que o **serviço `KinectManagement`** do SDK da Microsoft faz o mesmo assim que o sensor liga, com **outro firmware** (`UAC.bin`). Depois disso o sensor aparece como `045e:02bb`, aceita LED e inclinação, mas não responde à leitura de estado nem aos microfones, porque esse firmware não fala o protocolo do Xenia.

Para o Xenia carregar o firmware dele, pare o serviço antes de ligar o sensor:

1. Num PowerShell **como administrador**: `Stop-Service KinectManagement -Force`.
2. Tire a energia do Kinect por uns 10 segundos e religue. Ele volta como `02AD`.
3. Abra o jogo com `--kinect_audio_firmware_path="C:\caminho\FFFE07DF00000001"` (o pacote do sistema do console, ou o `audios.bin`).
4. O serviço volta sozinho na próxima vez que o Windows iniciar.

Sem o firmware, a câmera e o esqueleto funcionam, mas o motor e o microfone não.

## 5. Problemas conhecidos

- **Câmera não abre (`LIBUSB_ERROR_OTHER`) depois de encerrar o emulador à força.** O UsbDk pode ficar com o dispositivo preso. Feche o emulador sempre pela janela; reiniciar o Windows resolve.
- **O microfone entrega só rajadas curtas** (de 0,3 a 1,2 s de áudio em 5 s), com o backend WinUSB e com o UsbDk. O nível do microfone no Kinect Tuner se mexe de forma aleatória. No Linux o mesmo código entrega o áudio de forma contínua, então a causa deve estar no agendamento isócrono do Windows. Falta uma captura de USB para comparar.
- **O número de série do sensor:** a câmera deste modelo devolve só zeros. O Xenia usa o serial do dispositivo de áudio quando isso acontece.
- A calibração da matriz de microfones (`kinect_audio_calibration_path`) continua sem arquivo para testar.

## 6. O que foi corrigido para compilar no Windows

- `xenia-build.py`: o caminho do `vswhere.exe` usava barras e o Windows não o achava.
- `xam_nui.cc`: `M_PI` não existe no MSVC sem `_USE_MATH_DEFINES`.
- `gpu/d3d12/pipeline_cache.cc`: faltava o terceiro argumento de `IsRasterizationPotentiallyDone`.
- Serial do sensor: lido uma vez, com a câmera já aberta (o UsbDk não deixa abrir a mesma câmera duas vezes).
