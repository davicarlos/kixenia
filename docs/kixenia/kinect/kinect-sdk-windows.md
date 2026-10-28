# SDK do Kinect para Windows: referência cruzada com o Xbox

O SDK do Windows usa o mesmo sensor e quase as mesmas estruturas que o NUI do Xbox 360. Os nomes e valores dele explicam campos que no Xbox só têm números.

Relacionado: [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md) (seção 2), [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md), [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md).

## Binários e como foram lidos

| Arquivo | O que é | Como foi lido |
|---|---|---|
| `Microsoft.Kinect.dll` 1.8.0.0 | API .NET (139 tipos, 215 constantes) | REA `inspect_managed_*` e um leitor de metadados ECMA-335 próprio (`tools/cil_enums.py` no scratchpad), que lista os enums com valores |
| `Kinect10_x64.dll` | runtime nativo: imagem, esqueleto, mapeamento | exports, strings, constantes (REA/Ghidra) |
| `KinectAudio10_x64.dll` | DMO de áudio "KinectAEC" | REA/Ghidra; ver [Kinect-Audio-Firmware-e-CEMD](kinect-audio-firmware-e-cemd.md), seção 4 |
| `kinectcamera_x64.sys` | driver da câmera | REA; ver [Kinect-Engenharia-Reversa](kinect-engenharia-reversa.md), seção 2 |
| `KinectManagementService.exe`, `Microsoft.Speech.dll`, `sp*`/`ms*` | serviço e reconhecimento de fala | não analisados |

## Exports nativos

- **`Kinect10_x64.dll` (25):**
  - inicialização e sensores: `NuiInitialize`, `NuiShutdown`, `NuiCreateSensorById/ByIndex`, `NuiGetSensorCount`, `NuiSetDeviceStatusCallback`, `NuiSetFrameEndEvent`, `NuiGetAudioSource`;
  - inclinação: `NuiCameraElevationGetAngle/SetAngle`;
  - imagem: `NuiImageStreamOpen`, `NuiImageStreamGetNextFrame`, `NuiImageStreamReleaseFrame`, `NuiImageStreamGet/SetImageFrameFlags`;
  - mapeamento: `NuiImageGetColorPixelCoordinatesFromDepthPixel(AtResolution)`, `NuiCreateCoordinateMapperFromParameters`, `NuiCreateDepthFilter`;
  - esqueleto: `NuiSkeletonGetNextFrame`, `NuiSkeletonTrackingEnable/Disable`, `NuiSkeletonSetTrackedSkeletons`, `NuiSkeletonCalculateBoneOrientations`, `NuiTransformSmooth`.
- **`KinectAudio10_x64.dll` (6):** `NuiGetMicrophoneArrayDevices`, `NuiGetSpeakerDevices` e o registro COM.

## Enums e constantes (Microsoft.Kinect 1.8)

### Inicialização
- **`SensorOptions`** (equivalente aos flags de `NuiInitialize`): UseDepthAndPlayerIndex 1, UseColor 2, UseSkeletalTracking 8, UseDepth 0x20, UseAudio 0x10000000.
- O SDK nativo tem ainda `NUI_INITIALIZE_FLAG_USES_HIGH_QUALITY_COLOR` = **0x40**. É o flag que o Nike consulta (`DAT_83AF5900 & 0x40`) para a cor em alta qualidade (código 27 do PsCam, campo +0x04 dos ajustes).
- **`KinectStatus`:** Undefined 0, Disconnected 1, Connected 2, Initializing 3, Error 4, NotPowered 5, NotReady 6, DeviceNotGenuine 7, DeviceNotSupported 8, InsufficientBandwidth 9.
- **Erros:**

| Erro | Valor |
|---|---|
| `E_NUI_HARDWARE_FEATURE_UNAVAILABLE` | 0x8301000F |
| `E_NUI_NOTGENUINE` | 0x83010006 |
| `E_NUI_NOTSUPPORTED` | 0x83010008 |
| `E_NUI_DEVICE_IN_USE` | 0x83010009 |
| `E_NUI_BADINDEX` | 0x83010585 |

O prefixo **0x8301xxxx** é a mesma família dos códigos que o Nike devolve (0x83010005, 0x8301000B, 0x83010015, 0x83010016).

### Imagem
- **`ImageType`** / `NUI_IMAGE_TYPE`: DepthAndPlayerIndex 0, Color 1, ColorYuv 2, ColorRawYuv 3, Depth 4, ColorInfrared 5, ColorRawBayer 6.
- **`ImageResolution`:** 80×60 0, 320×240 1, 640×480 2, 1280×960 3 (Invalid −1).
- **`ColorImageFormat`:**
  - RGB: 640×480 a 30 fps 1, 1280×960 a 12 fps 2;
  - YUV: 640×480 a 15 fps 3, Raw 640×480 a 15 fps 4;
  - infravermelho: 640×480 a 30 fps 5;
  - Bayer cru: 640×480 a 30 fps 6, 1280×960 a 12 fps 7.
- **`DepthImageFormat`:** 640×480 a 30 fps 1, 320×240 2, 80×60 3.
- **Profundidade:** padrão de 800 a 4000 mm; near de 400 a 3000 mm. Valores especiais: muito perto 0, muito longe 4095, desconhecido −1. São 2 bytes por pixel, e os 3 bits baixos são o jogador (`PlayerIndexBitmask` 7).
- **Flags:** `NUI_IMAGE_STREAM_FLAG_SUPPRESS_NO_FRAME_DATA` 0x10000, `…ENABLE_NEAR_MODE` 0x20000, `…DISTINCT_OVERFLOW_DEPTH_VALUES` 0x40000; `NUI_IMAGE_FRAME_FLAG_NEAR_MODE_ENABLED` 0x20000.
- **Distância focal da profundidade:** **285,63 px** em 320×240 (e 1/285,63) aparece como constante no `Kinect10_x64.dll`. É o mesmo valor do Kinect Tuner do Xenia.

### Ajustes de câmera (só no Kinect for Windows, por UVC)
- **`PowerLineFrequency`:** Disabled 0, 50 Hz 1, 60 Hz 2.
- **`BacklightCompensationMode`:** AverageBrightness 0, CenterPriority 1, LowlightsPriority 2, CenterOnly 4.
- Os campos do Xbox com valores parecidos (propriedades 1 e 4 em [Kinect-Ajustes-de-Camera](kinect-ajustes-de-camera.md)) **podem** ser esses, mas a correspondência não está confirmada.

### Esqueleto
- **`JointType`** (20): HipCenter 0, Spine 1, ShoulderCenter 2, Head 3, ShoulderLeft 4, ElbowLeft 5, WristLeft 6, HandLeft 7, ShoulderRight 8, ElbowRight 9, WristRight 10, HandRight 11, HipLeft 12, KneeLeft 13, AnkleLeft 14, FootLeft 15, HipRight 16, KneeRight 17, AnkleRight 18, FootRight 19. É o mesmo índice que o Xenia usa (cabeça 3; pernas 13 a 15 e 17 a 19 na inclinação automática).
- **`SkeletonTrackingState`:** NotTracked 0, PositionOnly 1, Tracked 2. **`JointTrackingState`:** NotTracked 0, Inferred 1, Tracked 2.
- **`SkeletonTrackingMode`:** Default 0, Seated 1. São 6 esqueletos.
- **Flags:** `NUI_SKELETON_TRACKING_FLAG_SUPPRESS_NO_FRAME_DATA` 1, `TITLE_SETS_TRACKED_SKELETONS` 2 (o jogo escolhe quem rastrear, como os IDs em +0xACC no Xbox), `ENABLE_SEATED_SUPPORT` 4, `ENABLE_IN_NEAR_RANGE` 8.
- **`FrameEdges`:** Right 1, Left 2, Top 4, Bottom 8, que indicam onde o corpo foi cortado pela borda da imagem.

### Áudio
- **Feixe e fonte de som:** de **−50° a +50°**. É o mesmo leque dos 11 feixes da matriz `Lin4el` e a provável faixa de `XamVoiceSetMicArrayBeamAngle`.
- **`BeamAngleMode`:** Automatic 0, Adaptive 1, Manual 2.
- **`EchoCancellationMode`:** None 0, CancellationOnly 1, CancellationAndSuppression 2.
- **`_MIC_ARRAY_MODE`:** SingleChannel 0, SimpleSum 0x100, SingleBeam 0x200, FixedBeam 0x400, ExternalBeam 0x800, AdaptiveBeam 0x1100.
- **`_AEC_SYSTEM_MODE`:** ArrayOnly 2, ArrayAndEchoCancellation 4.
- **Propriedades do DMO (`MFPKEY_WMAAECMA_*`):** SYSTEM_MODE 2, DMO_SOURCE_MODE 3, DEVICE_INDEXES 4, FEATURE_MODE 5, FEATR_NS 8, FEATR_AGC 9, FEATR_AES 10, FEATR_MICARR_MODE 18.
- **Fluxo:** buffers de 50 ms (1600 bytes = 16 kHz × 16 bits), total entre 1 e 4 s, e 250 ms de dados velhos descartados.
