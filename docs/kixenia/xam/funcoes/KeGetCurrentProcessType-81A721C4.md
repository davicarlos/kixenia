# KeGetCurrentProcessType

**Endereço:** 0x81A721C4 · **Área:** [Kernel (importações)](README.md#kernel-importações)

## O que faz
Importação do kernel: `KeGetCurrentProcessType (xboxkrnl 0x66)`. O xam chama por este thunk.

## Fatos do decompilado
- Kernel: KeGetCurrentProcessType (xboxkrnl 0x66)

## É chamada por
- [XMsgProcessRequest (816A3128)](XMsgProcessRequest-816A3128.md) — XMsg assíncrono
- [XamAppRequestLoadEx (816CAC60)](XamAppRequestLoadEx-816CAC60.md) — Telas: carregar app do sistema
- [XamVoiceHeadsetPresent (816FB418)](XamVoiceHeadsetPresent-816FB418.md)
- [XamVoiceGetDirectionalData (816FB450)](XamVoiceGetDirectionalData-816FB450.md)
- [XamVoiceSubmitPacket (816FB498)](XamVoiceSubmitPacket-816FB498.md)
- [XamVoiceCreate (816FC098)](XamVoiceCreate-816FC098.md)
- e mais 105 funções fora desta base (vizinhas): FUN_81671140, FUN_816782E0, FUN_816786C8, FUN_81678E38, XamGetLiveHiveValueW (8169AED8), XamGetLiveHiveValueA (8169AF50), FUN_8169E088, FUN_816A0798, XamLoaderLaunchTitleEx (816A0D70), FUN_816A31C8, XamNetworkStorageGetLastSyncTime (816A6988), FUN_816A8A48, XamNotifyCreateListener (816A9378), FUN_816B73E8, FUN_816B7F30, FUN_816B80C8, FUN_816B8138, XamExecutingOnBehalfOfTitle (816B8190), FUN_816B8208, FUN_816B9890, FUN_816B9D68, FUN_816B9F08, FUN_816CAD78, XamShowMessageBox (816CB168), XamAppAllocateInterappWorkspace (816CDFE0), FUN_816D52E8, FUN_816D53A8, FUN_816D5448, FUN_816D6C98, FUN_816D7000 …

## Nas notas
- [Kinect-Audio-Firmware-e-CEMD](../../kinect/kinect-audio-firmware-e-cemd.md): | `0x81A721C4` | KeGetCurrentProcessType (0x66) |
