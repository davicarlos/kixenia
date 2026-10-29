# Bulletstorm (título 454108EF)

> [!NOTE]
> **Jogo **sem Kinect** (nenhuma importação do sensor). Está aqui pelas importações de avatar, voz e mensagens aos apps do sistema e pelo que falta no Xenia.**

**No HD:** `Bulletstorm` (pacote GoD (cabeçalho em `Content/…/00007000/`)). **Executável:** 21568 KB de imagem, base 0x82000000, entrada 0x82D33328, 38788 funções no `.pdata`, `.text` 0x82280000–0x83019A54.

Relacionado: [Jogos-Kinect-Comparacao](../jogos-kinect-comparacao.md), [Kinect-Xenia-Edge](../kinect-xenia-edge.md), [Kinect-Mapa-XAM](../kinect-mapa-xam.md).

## Estado no Xenia Edge

> [!NOTE]
> **Ainda não rodado na bateria ao vivo; só a análise estática.**

## Como o jogo usa o Kinect

Não usa o sensor. O que importa do mesmo grupo de funções:

- **Voz e microfones:** 1 importações (`XAudioGetVoiceCategoryVolume`); 0 funções próprias e 1 do runtime; **faltam/stub: `XAudioGetVoiceCategoryVolume`**.

## Chamadas ao sistema

**272 importações** (0 do `xam.xex`, 150 do `xboxkrnl.exe`), 1128 chamadas diretas no código. Estado no Xenia Edge (kixenia): 119 implementadas, 8 aproximadas, 20 stubs, 124 ausentes.

**Como foi feito:** XEX decifrado (`mex_decifra.py`) e descomprimido (`xexinfo`); importações e thunks da tabela do XEX; chamadas = `bl`/`b` para cada thunk; a função de cada chamada vem do `.pdata`; essas funções (e quem as chama) foram decompiladas pelo Ghidra 12.1.4 com a linguagem VMX128, sem análise automática (`jogos/pipeline.py`).

### Funções do jogo que falam com o Kinect

Das 1 funções que chamam o sistema, **1 são o runtime NUI embutido** (a biblioteca da Microsoft que todo título Kinect leva: a mesma função, com o mesmo tamanho, as mesmas importações e as mesmas mensagens, aparece em 3 ou mais jogos) e **0 são código próprio do jogo**. As próprias vêm primeiro; o runtime fica recolhido no fim.

<details><summary><b>Runtime NUI embutido</b> (1 funções)</summary>

| Função | Tamanho | Chama | Chamada por | O que faz | Decompilada |
|---|---|---|---|---|---|
| 0x82C98850 | 224 | `XAudioGetVoiceCategoryVolume` | 0 |  | ✅ |

</details>

### Importações do Kinect, NUI, voz e avatares

| Importação | Biblioteca, ordinal | Chamadas no código | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |

### Todas as importações, por área

<details><summary><b>Outras</b> (122)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `0_1` | 0 0x1 | 1 | 0 | ❌ |
| `0_10` | 0 0x10 | 1 | 0 | ❌ |
| `0_12` | 0 0x12 | 1 | 0 | ❌ |
| `0_13` | 0 0x13 | 1 | 0 | ❌ |
| `0_135` | 0 0x135 | 1 | 0 | ❌ |
| `0_136` | 0 0x136 | 3 | 0 | ❌ |
| `0_14` | 0 0x14 | 1 | 0 | ❌ |
| `0_15` | 0 0x15 | 1 | 0 | ❌ |
| `0_16` | 0 0x16 | 1 | 0 | ❌ |
| `0_18` | 0 0x18 | 1 | 0 | ❌ |
| `0_190` | 0 0x190 | 2 | 0 | ❌ |
| `0_191` | 0 0x191 | 1 | 0 | ❌ |
| `0_192` | 0 0x192 | 1 | 0 | ❌ |
| `0_1A` | 0 0x1A | 1 | 0 | ❌ |
| `0_1A4` | 0 0x1A4 | 1 | 0 | ❌ |
| `0_1A6` | 0 0x1A6 | 1 | 0 | ❌ |
| `0_1A7` | 0 0x1A7 | 1 | 0 | ❌ |
| `0_1A8` | 0 0x1A8 | 1 | 0 | ❌ |
| `0_1A9` | 0 0x1A9 | 2 | 0 | ❌ |
| `0_1B` | 0 0x1B | 1 | 0 | ❌ |
| `0_1C` | 0 0x1C | 1 | 0 | ❌ |
| `0_1D` | 0 0x1D | 1 | 0 | ❌ |
| `0_1E` | 0 0x1E | 1 | 0 | ❌ |
| `0_1EA` | 0 0x1EA | 13 | 0 | ❌ |
| `0_1EC` | 0 0x1EC | 17 | 0 | ❌ |
| `0_1F` | 0 0x1F | 1 | 0 | ❌ |
| `0_1F4` | 0 0x1F4 | 13 | 0 | ❌ |
| `0_1F7` | 0 0x1F7 | 27 | 0 | ❌ |
| `0_1F8` | 0 0x1F8 | 3 | 0 | ❌ |
| `0_1FC` | 0 0x1FC | 1 | 0 | ❌ |
| `0_2` | 0 0x2 | 1 | 0 | ❌ |
| `0_20` | 0 0x20 | 1 | 0 | ❌ |
| `0_208` | 0 0x208 | 1 | 0 | ❌ |
| `0_20A` | 0 0x20A | 3 | 0 | ❌ |
| `0_20E` | 0 0x20E | 1 | 0 | ❌ |
| `0_21` | 0 0x21 | 1 | 0 | ❌ |
| `0_210` | 0 0x210 | 5 | 0 | ❌ |
| `0_212` | 0 0x212 | 3 | 0 | ❌ |
| `0_213` | 0 0x213 | 2 | 0 | ❌ |
| `0_219` | 0 0x219 | 3 | 0 | ❌ |
| `0_21A` | 0 0x21A | 1 | 0 | ❌ |
| `0_227` | 0 0x227 | 1 | 0 | ❌ |
| `0_24E` | 0 0x24E | 1 | 0 | ❌ |
| `0_24F` | 0 0x24F | 1 | 0 | ❌ |
| `0_250` | 0 0x250 | 1 | 0 | ❌ |
| `0_259` | 0 0x259 | 1 | 0 | ❌ |
| `0_25A` | 0 0x25A | 1 | 0 | ❌ |
| `0_25B` | 0 0x25B | 1 | 0 | ❌ |
| `0_25C` | 0 0x25C | 1 | 0 | ❌ |
| `0_25E` | 0 0x25E | 1 | 0 | ❌ |
| `0_262` | 0 0x262 | 1 | 0 | ❌ |
| `0_265` | 0 0x265 | 1 | 0 | ❌ |
| `0_280` | 0 0x280 | 1 | 0 | ❌ |
| `0_282` | 0 0x282 | 7 | 0 | ❌ |
| `0_28A` | 0 0x28A | 2 | 0 | ❌ |
| `0_28B` | 0 0x28B | 14 | 0 | ❌ |
| `0_28C` | 0 0x28C | 9 | 0 | ❌ |
| `0_2BC` | 0 0x2BC | 1 | 0 | ❌ |
| `0_2BF` | 0 0x2BF | 1 | 0 | ❌ |
| `0_2C0` | 0 0x2C0 | 1 | 0 | ❌ |
| `0_2C1` | 0 0x2C1 | 1 | 0 | ❌ |
| `0_2C2` | 0 0x2C2 | 1 | 0 | ❌ |
| `0_2C5` | 0 0x2C5 | 1 | 0 | ❌ |
| `0_2C6` | 0 0x2C6 | 1 | 0 | ❌ |
| `0_2C7` | 0 0x2C7 | 2 | 0 | ❌ |
| `0_2C8` | 0 0x2C8 | 1 | 0 | ❌ |
| `0_2CB` | 0 0x2CB | 1 | 0 | ❌ |
| `0_2CC` | 0 0x2CC | 1 | 0 | ❌ |
| `0_2CD` | 0 0x2CD | 1 | 0 | ❌ |
| `0_2CE` | 0 0x2CE | 1 | 0 | ❌ |
| `0_2D5` | 0 0x2D5 | 1 | 0 | ❌ |
| `0_2D9` | 0 0x2D9 | 1 | 0 | ❌ |
| `0_2DC` | 0 0x2DC | 1 | 0 | ❌ |
| `0_2E6` | 0 0x2E6 | 1 | 0 | ❌ |
| `0_2E7` | 0 0x2E7 | 1 | 0 | ❌ |
| `0_2EE` | 0 0x2EE | 1 | 0 | ❌ |
| `0_2F0` | 0 0x2F0 | 1 | 0 | ❌ |
| `0_2F5` | 0 0x2F5 | 1 | 0 | ❌ |
| `0_2F7` | 0 0x2F7 | 2 | 0 | ❌ |
| `0_3` | 0 0x3 | 1 | 0 | ❌ |
| `0_30C` | 0 0x30C | 1 | 0 | ❌ |
| `0_30D` | 0 0x30D | 1 | 0 | ❌ |
| `0_30E` | 0 0x30E | 2 | 0 | ❌ |
| `0_30F` | 0 0x30F | 3 | 0 | ❌ |
| `0_316` | 0 0x316 | 1 | 0 | ❌ |
| `0_317` | 0 0x317 | 14 | 0 | ❌ |
| `0_33` | 0 0x33 | 1 | 0 | ❌ |
| `0_34` | 0 0x34 | 1 | 0 | ❌ |
| `0_35` | 0 0x35 | 1 | 0 | ❌ |
| `0_36` | 0 0x36 | 1 | 0 | ❌ |
| `0_37` | 0 0x37 | 1 | 0 | ❌ |
| `0_38` | 0 0x38 | 1 | 0 | ❌ |
| `0_39` | 0 0x39 | 1 | 0 | ❌ |
| `0_3A` | 0 0x3A | 1 | 0 | ❌ |
| `0_3CB` | 0 0x3CB | 1 | 0 | ❌ |
| `0_3CC` | 0 0x3CC | 2 | 0 | ❌ |
| `0_3CD` | 0 0x3CD | 1 | 0 | ❌ |
| `0_3D1` | 0 0x3D1 | 2 | 0 | ❌ |
| `0_3F` | 0 0x3F | 1 | 0 | ❌ |
| `0_4` | 0 0x4 | 1 | 0 | ❌ |
| `0_41` | 0 0x41 | 1 | 0 | ❌ |
| `0_42` | 0 0x42 | 1 | 0 | ❌ |
| `0_43` | 0 0x43 | 1 | 0 | ❌ |
| `0_44` | 0 0x44 | 1 | 0 | ❌ |
| `0_45` | 0 0x45 | 1 | 0 | ❌ |
| `0_46` | 0 0x46 | 1 | 0 | ❌ |
| `0_47` | 0 0x47 | 1 | 0 | ❌ |
| `0_48` | 0 0x48 | 1 | 0 | ❌ |
| `0_49` | 0 0x49 | 1 | 0 | ❌ |
| `0_4B` | 0 0x4B | 1 | 0 | ❌ |
| `0_4D` | 0 0x4D | 1 | 0 | ❌ |
| `0_5` | 0 0x5 | 1 | 0 | ❌ |
| `0_6` | 0 0x6 | 1 | 0 | ❌ |
| `0_7` | 0 0x7 | 1 | 0 | ❌ |
| `0_8` | 0 0x8 | 1 | 0 | ❌ |
| `0_9` | 0 0x9 | 1 | 0 | ❌ |
| `0_A` | 0 0xA | 1 | 0 | ❌ |
| `0_B` | 0 0xB | 1 | 0 | ❌ |
| `0_C` | 0 0xC | 1 | 0 | ❌ |
| `0_D` | 0 0xD | 1 | 0 | ❌ |
| `0_E` | 0 0xE | 1 | 0 | ❌ |
| `0_F` | 0 0xF | 1 | 0 | ❌ |

</details>

<details><summary><b>Threads, sincronização e tempo</b> (53)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExCreateThread` | xboxkrnl 0xD | 1 | 0 | ✅ |
| `ExTerminateThread` | xboxkrnl 0x19 | 2 | 0 | ✅ |
| `ExThreadObjectType` | xboxkrnl 0x1B | 0 | 0 | ✅ |
| `InterlockedFlushSList` | xboxkrnl 0x2B | 6 | 0 | ✅ |
| `InterlockedPopEntrySList` | xboxkrnl 0x2C | 10 | 0 | ✅ |
| `KeAcquireSpinLockAtRaisedIrql` | xboxkrnl 0x4D | 1 | 0 | ✅ |
| `KeBugCheck` | xboxkrnl 0x52 | 7 | 0 | ✅ |
| `KeBugCheckEx` | xboxkrnl 0x53 | 4 | 0 | ⚪ stub |
| `KeCertMonitorData` | xboxkrnl 0x266 | 0 | 0 | ✅ |
| `KeDebugMonitorData` | xboxkrnl 0x59 | 0 | 0 | ✅ |
| `KeDelayExecutionThread` | xboxkrnl 0x5A | 1 | 0 | ✅ |
| `KeEnterCriticalRegion` | xboxkrnl 0x5F | 3 | 0 | ✅ |
| `KeGetCurrentProcessType` | xboxkrnl 0x66 | 53 | 0 | ✅ |
| `KeInitializeDpc` | xboxkrnl 0x6F | 1 | 0 | 🟡 |
| `KeLeaveCriticalRegion` | xboxkrnl 0x7D | 3 | 0 | ✅ |
| `KeLockL2` | xboxkrnl 0x6B | 1 | 0 | 🟡 |
| `KeQueryBasePriorityThread` | xboxkrnl 0x81 | 1 | 0 | ✅ |
| `KeQueryPerformanceFrequency` | xboxkrnl 0x83 | 3 | 0 | ✅ |
| `KeQuerySystemTime` | xboxkrnl 0x84 | 4 | 0 | ✅ |
| `KeReleaseSpinLockFromRaisedIrql` | xboxkrnl 0x89 | 1 | 0 | ✅ |
| `KeResetEvent` | xboxkrnl 0x8F | 1 | 0 | ✅ |
| `KeSetAffinityThread` | xboxkrnl 0x97 | 1 | 0 | ✅ |
| `KeSetBasePriorityThread` | xboxkrnl 0x99 | 2 | 0 | ✅ |
| `KeSetCurrentProcessType` | xboxkrnl 0x9A | 2 | 0 | ✅ |
| `KeSetEvent` | xboxkrnl 0x9D | 10 | 0 | ✅ |
| `KeTimeStampBundle` | xboxkrnl 0xAD | 0 | 0 | ✅ |
| `KeTlsAlloc` | xboxkrnl 0x152 | 6 | 0 | ✅ |
| `KeTlsFree` | xboxkrnl 0x153 | 4 | 0 | ✅ |
| `KeTlsGetValue` | xboxkrnl 0x154 | 10 | 0 | ✅ |
| `KeTlsSetValue` | xboxkrnl 0x155 | 15 | 0 | ✅ |
| `KeUnlockL2` | xboxkrnl 0x6C | 1 | 0 | 🟡 |
| `KeWaitForMultipleObjects` | xboxkrnl 0xAF | 2 | 0 | ✅ |
| `KeWaitForSingleObject` | xboxkrnl 0xB0 | 3 | 0 | ✅ |
| `KfAcquireSpinLock` | xboxkrnl 0xB1 | 8 | 0 | ✅ |
| `KfReleaseSpinLock` | xboxkrnl 0xB4 | 10 | 0 | ✅ |
| `KiApcNormalRoutineNop` | xboxkrnl 0x1DF | 1 | 0 | ⚪ stub |
| `NtCancelTimer` | xboxkrnl 0xCD | 1 | 0 | ✅ |
| `NtClearEvent` | xboxkrnl 0xCE | 1 | 0 | ✅ |
| `NtPulseEvent` | xboxkrnl 0xE2 | 1 | 0 | ✅ |
| `NtReleaseMutant` | xboxkrnl 0xF2 | 1 | 0 | ✅ |
| `NtReleaseSemaphore` | xboxkrnl 0xF3 | 1 | 0 | ✅ |
| `NtResumeThread` | xboxkrnl 0xF5 | 1 | 0 | ✅ |
| `NtSuspendThread` | xboxkrnl 0xFC | 1 | 0 | ✅ |
| `NtWaitForMultipleObjectsEx` | xboxkrnl 0xFE | 1 | 0 | ✅ |
| `NtWaitForSingleObjectEx` | xboxkrnl 0xFD | 3 | 0 | ✅ |
| `NtYieldExecution` | xboxkrnl 0x101 | 1 | 0 | ✅ |
| `RtlEnterCriticalSection` | xboxkrnl 0x125 | 146 | 0 | ✅ |
| `RtlInitializeCriticalSection` | xboxkrnl 0x12E | 31 | 0 | ✅ |
| `RtlInitializeCriticalSectionAndSpinCount` | xboxkrnl 0x12F | 3 | 0 | ✅ |
| `RtlLeaveCriticalSection` | xboxkrnl 0x130 | 173 | 0 | ✅ |
| `RtlTimeFieldsToTime` | xboxkrnl 0x13F | 5 | 0 | ✅ |
| `RtlTimeToTimeFields` | xboxkrnl 0x140 | 5 | 0 | ✅ |
| `RtlTryEnterCriticalSection` | xboxkrnl 0x141 | 76 | 0 | ✅ |

</details>

<details><summary><b>Conteúdo e armazenamento</b> (26)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `FscSetCacheElementCount` | xboxkrnl 0x21 | 1 | 0 | ⚪ stub |
| `MmMapIoSpace` | xboxkrnl 0xC2 | 1 | 0 | ✅ |
| `NtClose` | xboxkrnl 0xCF | 10 | 0 | ✅ |
| `NtCreateEvent` | xboxkrnl 0xD1 | 2 | 0 | ✅ |
| `NtCreateFile` | xboxkrnl 0xD2 | 2 | 0 | ✅ |
| `NtCreateMutant` | xboxkrnl 0xD4 | 1 | 0 | ✅ |
| `NtCreateSemaphore` | xboxkrnl 0xD5 | 1 | 0 | ✅ |
| `NtCreateTimer` | xboxkrnl 0xD7 | 1 | 0 | ✅ |
| `NtDuplicateObject` | xboxkrnl 0xDA | 1 | 0 | ✅ |
| `NtFlushBuffersFile` | xboxkrnl 0xDB | 1 | 0 | ✅ |
| `NtOpenFile` | xboxkrnl 0xDF | 6 | 0 | ✅ |
| `NtQueryDirectoryFile` | xboxkrnl 0xE4 | 0 | 0 | ✅ |
| `NtQueryFullAttributesFile` | xboxkrnl 0xE7 | 1 | 0 | ✅ |
| `NtQueryInformationFile` | xboxkrnl 0xE8 | 1 | 0 | ✅ |
| `NtQueryVirtualMemory` | xboxkrnl 0xEE | 3 | 0 | ✅ |
| `NtQueryVolumeInformationFile` | xboxkrnl 0xEF | 1 | 0 | ✅ |
| `NtReadFile` | xboxkrnl 0xF0 | 0 | 0 | ✅ |
| `NtReadFileScatter` | xboxkrnl 0xF1 | 0 | 0 | ✅ |
| `NtSetEvent` | xboxkrnl 0xF6 | 1 | 0 | ✅ |
| `NtSetInformationFile` | xboxkrnl 0xF7 | 9 | 0 | ✅ |
| `NtSetTimerEx` | xboxkrnl 0xFA | 1 | 0 | ✅ |
| `NtWriteFile` | xboxkrnl 0xFF | 3 | 0 | ✅ |
| `ObCreateSymbolicLink` | xboxkrnl 0x103 | 1 | 0 | ✅ |
| `ObDeleteSymbolicLink` | xboxkrnl 0x104 | 1 | 0 | ✅ |
| `ObDereferenceObject` | xboxkrnl 0x105 | 19 | 0 | ✅ |
| `ObReferenceObjectByHandle` | xboxkrnl 0x110 | 5 | 0 | ✅ |

</details>

<details><summary><b>Vídeo e GPU</b> (24)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `VdCallGraphicsNotificationRoutines` | xboxkrnl 0x1B1 | 1 | 0 | 🟡 |
| `VdEnableDisableClockGating` | xboxkrnl 0x1B4 | 2 | 0 | ⚪ stub |
| `VdEnableRingBufferRPtrWriteBack` | xboxkrnl 0x1B6 | 1 | 0 | ✅ |
| `VdGetCurrentDisplayGamma` | xboxkrnl 0x1B9 | 1 | 0 | ⚪ stub |
| `VdGetCurrentDisplayInformation` | xboxkrnl 0x1BA | 3 | 0 | ⚪ stub |
| `VdGetSystemCommandBuffer` | xboxkrnl 0x1BD | 1 | 0 | ⚪ stub |
| `VdGlobalDevice` | xboxkrnl 0x1BE | 0 | 0 | ✅ |
| `VdGlobalXamDevice` | xboxkrnl 0x1BF | 0 | 0 | ✅ |
| `VdGpuClockInMHz` | xboxkrnl 0x1C0 | 0 | 0 | ✅ |
| `VdHSIOCalibrationLock` | xboxkrnl 0x1C1 | 0 | 0 | ✅ |
| `VdInitializeEngines` | xboxkrnl 0x1C2 | 1 | 0 | ⚪ stub |
| `VdInitializeRingBuffer` | xboxkrnl 0x1C3 | 1 | 0 | ✅ |
| `VdInitializeScalerCommandBuffer` | xboxkrnl 0x1C5 | 1 | 0 | 🟡 |
| `VdIsHSIOTrainingSucceeded` | xboxkrnl 0x1C6 | 1 | 0 | ⚪ stub |
| `VdPersistDisplay` | xboxkrnl 0x1C7 | 1 | 0 | 🟡 |
| `VdQueryVideoFlags` | xboxkrnl 0x1C9 | 1 | 0 | ⚪ stub |
| `VdQueryVideoMode` | xboxkrnl 0x1CA | 3 | 0 | ⚪ stub |
| `VdRetrainEDRAM` | xboxkrnl 0x269 | 2 | 0 | ⚪ stub |
| `VdRetrainEDRAMWorker` | xboxkrnl 0x26A | 1 | 0 | ⚪ stub |
| `VdSetDisplayMode` | xboxkrnl 0x1D3 | 1 | 0 | ⚪ stub |
| `VdSetGraphicsInterruptCallback` | xboxkrnl 0x1D5 | 2 | 0 | ✅ |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | xboxkrnl 0x1D9 | 3 | 0 | ⚪ stub |
| `VdShutdownEngines` | xboxkrnl 0x1DC | 2 | 0 | ⚪ stub |
| `VdSwap` | xboxkrnl 0x25B | 1 | 0 | ✅ |

</details>

<details><summary><b>Runtime (Rtl, strings, exceções, log)</b> (16)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `DbgBreakPoint` | xboxkrnl 0x1 | 1 | 0 | ⚪ stub |
| `DbgPrint` | xboxkrnl 0x3 | 5 | 0 | ✅ |
| `RtlCompareMemoryUlong` | xboxkrnl 0x11B | 7 | 0 | ✅ |
| `RtlFillMemoryUlong` | xboxkrnl 0x126 | 1 | 0 | ✅ |
| `RtlImageXexHeaderField` | xboxkrnl 0x12B | 1 | 0 | ✅ |
| `RtlInitAnsiString` | xboxkrnl 0x12C | 16 | 0 | ✅ |
| `RtlInitUnicodeString` | xboxkrnl 0x12D | 1 | 0 | ✅ |
| `RtlMultiByteToUnicodeN` | xboxkrnl 0x133 | 1 | 0 | 🟡 |
| `RtlNtStatusToDosError` | xboxkrnl 0x135 | 4 | 0 | ❔ |
| `RtlRaiseException` | xboxkrnl 0x136 | 3 | 0 | ⚪ stub |
| `RtlUnicodeStringToAnsiString` | xboxkrnl 0x142 | 1 | 0 | ✅ |
| `RtlUnicodeToMultiByteN` | xboxkrnl 0x143 | 1 | 0 | 🟡 |
| `RtlUnwind` | xboxkrnl 0x147 | 2 | 0 | ❌ |
| `__C_specific_handler` | xboxkrnl 0x1A5 | 0 | 0 | ❌ |
| `_vsnprintf` | xboxkrnl 0x14D | 1 | 0 | ✅ |
| `sprintf` | xboxkrnl 0x13B | 7 | 0 | ✅ |

</details>

<details><summary><b>Sistema e console</b> (13)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExGetXConfigSetting` | xboxkrnl 0x10 | 13 | 0 | ✅ |
| `ExLoadedCommandLine` | xboxkrnl 0x1AE | 0 | 0 | ✅ |
| `ExRegisterTitleTerminateNotification` | xboxkrnl 0x15 | 9 | 0 | ✅ |
| `HalReturnToFirmware` | xboxkrnl 0x28 | 1 | 0 | ⚪ stub |
| `XboxHardwareInfo` | xboxkrnl 0x156 | 0 | 0 | ✅ |
| `XboxKrnlVersion` | xboxkrnl 0x158 | 0 | 0 | ✅ |
| `XexCheckExecutablePrivilege` | xboxkrnl 0x194 | 3 | 0 | ✅ |
| `XexExecutableModuleHandle` | xboxkrnl 0x193 | 0 | 0 | ✅ |
| `XexGetModuleHandle` | xboxkrnl 0x195 | 4 | 0 | ✅ |
| `XexGetModuleSection` | xboxkrnl 0x196 | 1 | 0 | ✅ |
| `XexGetProcedureAddress` | xboxkrnl 0x197 | 4 | 0 | ✅ |
| `XexLoadImage` | xboxkrnl 0x199 | 1 | 0 | ✅ |
| `XexUnloadImage` | xboxkrnl 0x1A1 | 1 | 0 | ✅ |

</details>

<details><summary><b>Memória</b> (11)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `ExAllocatePool` | xboxkrnl 0x9 | 1 | 0 | ✅ |
| `ExFreePool` | xboxkrnl 0xF | 2 | 0 | ✅ |
| `MmAllocatePhysicalMemoryEx` | xboxkrnl 0xBA | 4 | 0 | ✅ |
| `MmFreePhysicalMemory` | xboxkrnl 0xBD | 5 | 0 | ✅ |
| `MmGetPhysicalAddress` | xboxkrnl 0xBE | 11 | 0 | ✅ |
| `MmQueryAddressProtect` | xboxkrnl 0xC4 | 2 | 0 | ✅ |
| `MmQueryStatistics` | xboxkrnl 0xC6 | 1 | 0 | ✅ |
| `MmSetAddressProtect` | xboxkrnl 0xC7 | 1 | 0 | ✅ |
| `NtAllocateVirtualMemory` | xboxkrnl 0xCC | 12 | 0 | ✅ |
| `NtFreeVirtualMemory` | xboxkrnl 0xDC | 7 | 0 | ✅ |
| `RtlFreeAnsiString` | xboxkrnl 0x127 | 1 | 0 | ✅ |

</details>

<details><summary><b>Áudio</b> (6)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetSpeakerConfig` | xboxkrnl 0x1FF | 1 | 0 | ✅ |
| `XAudioRegisterRenderDriverClient` | xboxkrnl 0x1F3 | 1 | 0 | ✅ |
| `XAudioSubmitRenderDriverFrame` | xboxkrnl 0x1F5 | 1 | 0 | ✅ |
| `XAudioUnregisterRenderDriverClient` | xboxkrnl 0x1F4 | 1 | 0 | ✅ |
| `XMACreateContext` | xboxkrnl 0x224 | 1 | 0 | ✅ |
| `XMAReleaseContext` | xboxkrnl 0x226 | 1 | 0 | ✅ |

</details>

<details><summary><b>Voz e microfones</b> (1)</summary>

| Importação | Biblioteca, ordinal | Chamadas | Em execução | Xenia |
|---|---|---|---|---|
| `XAudioGetVoiceCategoryVolume` | xboxkrnl 0x1F8 | 1 | 0 | ⚪ stub |

</details>

**Ausentes no Xenia (124):** `0_1`, `0_10`, `0_12`, `0_13`, `0_135`, `0_136`, `0_14`, `0_15`, `0_16`, `0_18`, `0_190`, `0_191`, `0_192`, `0_1A`, `0_1A4`, `0_1A6`, `0_1A7`, `0_1A8`, `0_1A9`, `0_1B`, `0_1C`, `0_1D`, `0_1E`, `0_1EA`, `0_1EC`, `0_1F`, `0_1F4`, `0_1F7`, `0_1F8`, `0_1FC`, `0_2`, `0_20`, `0_208`, `0_20A`, `0_20E`, `0_21`, `0_210`, `0_212`, `0_213`, `0_219`, `0_21A`, `0_227`, `0_24E`, `0_24F`, `0_250`, `0_259`, `0_25A`, `0_25B`, `0_25C`, `0_25E`, `0_262`, `0_265`, `0_280`, `0_282`, `0_28A`, `0_28B`, `0_28C`, `0_2BC`, `0_2BF`, `0_2C0`, `0_2C1`, `0_2C2`, `0_2C5`, `0_2C6`, `0_2C7`, `0_2C8`, `0_2CB`, `0_2CC`, `0_2CD`, `0_2CE`, `0_2D5`, `0_2D9`, `0_2DC`, `0_2E6`, `0_2E7`, `0_2EE`, `0_2F0`, `0_2F5`, `0_2F7`, `0_3`, `0_30C`, `0_30D`, `0_30E`, `0_30F`, `0_316`, `0_317`, `0_33`, `0_34`, `0_35`, `0_36`, `0_37`, `0_38`, `0_39`, `0_3A`, `0_3CB`, `0_3CC`, `0_3CD`, `0_3D1`, `0_3F`, `0_4`, `0_41`, `0_42`, `0_43`, `0_44`, `0_45`, `0_46`, `0_47`, `0_48`, `0_49`, `0_4B`, `0_4D`, `0_5`, `0_6`, `0_7`, `0_8`, `0_9`, `0_A`, `0_B`, `0_C`, `0_D`, `0_E`, `0_F`, `RtlUnwind`, `__C_specific_handler`.

## Pendências

- Importações do Kinect ausentes ou stub: `XAudioGetVoiceCategoryVolume`.

## Arquivos da análise

- Dados: `~/Kixenia/kinect-sdk-re/games/decompilado/bull/` (`bull.img`, `bull_imports.json`, `bull_kinect.json`, `bull_fngraph.json`, `c_full/<endereço>.c`: 1 funções).
- Projeto Ghidra: `games/decompilado/bull/ghidra/bull` (PowerPC:BE:64:VMX128-32addr, sem análise automática).
