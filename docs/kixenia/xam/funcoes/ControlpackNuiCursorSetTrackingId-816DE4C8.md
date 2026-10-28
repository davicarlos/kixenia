# ControlpackNuiCursorSetTrackingId

**Endereço:** 0x816DE4C8 · **Ordinal:** 2174 · **Área:** [Kinect ID](README.md#kinect-id) · **Xenia:** ❌ ausente

## O que faz
Lê os dados biométricos do perfil (armazém 0x18, até 0x32000 bytes, até 0x66 registros) e compara a data de cada rosto com a atual; diz se o cadastro está velho. A tabela do Xenia chama este ordinal de ControlpackNuiCursorSetTrackingId, o que parece errado. o endereço do exports.csv cai numa checagem de data dos dados biométricos; rótulo provavelmente errado

## Chama
- [XamCacheFetchFile (816793F0)](XamCacheFetchFile-816793F0.md) — Armazém de cache: ler
- Outras: XamAlloc (816E67A0), XamFree (816E6AB0), GetSystemTime (8171CA10), SystemTimeToFileTime (81722098), `memset` (0x8172D4F0), `memcpy` (0x8172D590)

## Nas notas
- [Kinect-Mapa-XAM](../../kinect/kinect-mapa-xam.md): | 2174 | 0x816DE4C8 | `ControlpackNuiCursorSetTrackingId` | função não definida pelo Ghidra (prólogo em 0x816DE4C8): lê os dados biométricos do perfil (armazém 0x18) e confere a data dos rostos cadastrados; o nome na tabela do Xenia parece errado | ❌ |
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - `ControlpackNuiCursorSetTrackingId`: o endereço do exports.csv (0x816DE4C8) é uma verificação de data dos dados biométricos; o rótulo deve estar errado.
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): ### Ordinal 2174 (`ControlpackNuiCursorSetTrackingId` na tabela do Xenia)
- [Kinect-Engenharia-Reversa](../../kinect/kinect-engenharia-reversa.md): - 0x816DE4C8 é uma função de verdade (prólogo `mflr r12`) que o Ghidra não tinha definido. O REA confirma: o endereço não pertence a nenhuma função analisada.
