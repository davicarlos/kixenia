# Fitness no XAM (XamFitness*)

O Xbox 360 guarda um **perfil de fitness** por usuário: dados do corpo, eventos de exercício (calorias, MET, duração) e resumos por jogo. Esses dados são sincronizados com o Xbox Live. Quase tudo no XAM é só uma ponte: monta um pedido e o envia ao **serviço da Live** (XMsg app **0xFB**, mensagens 0xB0079 a 0xB0089). Sem Live, o serviço não responde.

Os 22 decompilados estão em `~/Kixenia/kinect-sdk-re/xam-nui/decomp-fitness/`.

Relacionado: [Kinect-Engenharia-Reversa](../kinect/kinect-engenharia-reversa.md), [Nike-Kinect-Training](../kinect/jogos/nike-kinect-training.md), [XAM-Rede-e-Login](xam-rede-e-login.md).

## Padrão comum

- **Usuário:** o índice deve ser menor que 4 (senão `0x80070057`) e o perfil precisa estar conectado (`FUN_816dd118` é o estado de login: 0 nenhum, 1 local, 2 Live; com 0 a resposta é `0x80004005`).
- **Envio:** `FUN_816a3128(0xFB, msg, buffer, tamanho, overlapped)` envia ao serviço da Live (assíncrono, com XOVERLAPPED).
- **Gravações** (AddBodyProfileRecord, AddFitnessEvent):
  - `FUN_8168f068` aloca um pacote (tag de pool 0x18000000) do tamanho dos dados + 0x1C e copia os dados;
  - cabeçalho `{u32 usuário, u32 handle da alocação, u16 tamanho, u32 1}`;
  - primeiro uma chamada síncrona de verificação `FUN_816a2f88(0xFB, msg, pacote, 0)`; depois zera o flag e faz o envio de verdade;
  - `0x800703E5` (pendente) vira sucesso.

## Funções

| Ordinal | Função | Endereço | Mensagem 0xFB | Pedido / comportamento |
|---|---|---|---|---|
| 1228 | XamFitnessInitialize | 0x8168F4E0 | 0xB0080 | `{ponteiro de saída}` (4 bytes), síncrono; ponteiro nulo → `0x80070057` |
| 1315 | XamFitnessInitializeForOneUser | 0x8168F470 | 0xB0086 | `{usuário (≤ 6), ponteiro}` (8 bytes) |
| 1246 | XamFitnessUnInitialize | 0x8168F530 | 0xB0081 | sem dados; síncrono ou com overlapped |
| 1210 | XamFitnessAddBodyProfileRecord | 0x8168FA00 | 0xB0079 | pacote `{usuário, handle, tamanho, 1}` + registro do corpo |
| 1218 | XamFitnessGetCurrentBodyProfileRecord | 0x8168F168 | 0xB007B | `{usuário, buffer, u16 tamanho}` (0xC bytes), síncrono |
| 1219 | XamFitnessGetAllBodyProfileRecords | 0x8168F328 | 0xB007C | `{usuário, u16, buffer, u32, u16}` (0x14 bytes) |
| 1217 | XamFitnessClearBodyProfileRecords | 0x81698BF8 | — | **não fala com a Live**: preenche uma estrutura de 0x10 bytes (`[0] == 0x10`) com 3 valores em cache (`81A815C4`, `81A9F6AC`, `81A815C0`), lidos uma vez por `FUN_81698B78`. O nome do export parece não bater com o que ela faz |
| 1222 | XamFitnessAddFitnessEvent | 0x8168FB40 | 0xB007A | pacote como no AddBodyProfileRecord + evento |
| 1227 | XamFitnessCreateFitnessEventEnumerator | 0x8168FCF0 | 0xB007F (via 0xB000B), 0xB0009 | cria um enumerador de eventos a partir de uma consulta (abaixo) |
| 1220 | XamFitnessGetAllTitleSummaries | 0x8168F2C8 | 0xB007D | `{usuário, 0, 0, u16, …}` (0x1C bytes) |
| 1444 | XamFitnessGetTitleSummaries | 0x8168F2E8 | 0xB007D | `{usuário, 0, p2, p3, p4, p5, u16}` |
| 1445 | XamFitnessGetTitleSummary | 0x8168F308 | 0xB007D | `{usuário, título, 0xFFFFFFFF, 1, 0, buffer, u16}` |
| 1221 | XamFitnessGetOverallSummary | 0x8168F3D8 | 0xB007E | `{usuário, buffer, u16}` (0xC bytes) |
| 1274 | XamFitnessClearAll | 0x8168F0E0 | 0xB0084 | `{usuário, p2}` (8 bytes) |
| 1314 | XamFitnessGetSyncStatus | 0x8168F830 | 0xB0085 | `{usuário, 0x10, p3, p4}`; exige p2 == 0x10 |
| 1447 | XamFitnessContainsFitnessData | 0x8168F990 | 0xB0089 | `{usuário, ponteiro}` (8 bytes) |
| 1275 | XamFitnessGetPrivacySettings | 0x8168F688 | — | ver abaixo |
| 1316 | XamFitnessGetSuggestedPrivacySettings | 0x8168F8C0 | — | ver abaixo |
| 1240 | XamFitnessConvertByteMetToFloatMet | 0x8168F568 | — | `float = byte × 0,1` |
| 1250 | XamFitnessConvertFloatMetToByteMet | 0x8168F5B0 | — | `byte = (int)(float × 10)` |
| 1242 | XamFitnessMsgTimeToSystemTime | 0x8168FC78 | — | `FILETIME = (MsgTime + 0x2F422C080) × 10⁷`, depois `FileTimeToSystemTime` |
| 1243 | XamFitnessSystemTimeToMsgTime | 0x8168F5E8 | — | o inverso: `(FILETIME − 0x01C2B128BA2B4000) / 10⁷` |
| 1209 | XamShowFitnessBodyProfileUI | 0x816C5C50 | UI 0x2103A | `FUN_8168ef70(p2)` valida, depois `FUN_816c5a80({usuário, p2, p3, 0x2103A})` abre a tela |
| 1244 | XamShowFitnessWarnAboutPrivacyUI | 0x816C5D20 | UI 0x2103C | igual, com 0x2103C |

**MsgTime** = segundos desde **1º de janeiro de 2003**: 0x2F422C080 s depois de 1601 é 2003-01-01. **MET** em byte = MET × 10, em décimos.

### Privacidade

- **XamFitnessGetPrivacySettings(usuário, &valor):**
  - sem perfil Live (`FUN_816d8590` falha) → 0;
  - perfil local (estado ≠ 2) → 1;
  - com Live: lê a configuração de perfil **0x10040000** (painel 0xFFFE07D1, 0x30 bytes) e o privilégio 6, e escolhe entre os valores 2 a 6 pelos bits 0x200, 0x400 e 0x800 da configuração e pelo bit 2 do privilégio.
- **XamFitnessGetSuggestedPrivacySettings(usuário, &valor):**
  - por padrão 6;
  - se a região do perfil (configuração 6) não for a de comparação em 0x815F4400 (5 bytes), ou se a leitura falhar, consulta `FUN_816d9ce0` e responde 3 ou 4.

### Consulta do enumerador de eventos (`CreateFitnessEventEnumerator`)

A estrutura tem tamanho ≥ 0x38:
- +0x04: flags (0x100 = início definido, 0x200 = fim definido, 0x400, 0x800 e 0x1000 com campos próprios; 0x1800 não pode ser nem 0 nem 0x1800);
- +0x05: tipo de período (1 a 5; o 5 exige contagem 1). Os períodos 2, 3 e 4 parecem ser dia, mês e ano, ajustados em `SYSTEMTIME`;
- +0x08 / +0x0C: início e fim em MsgTime;
- +0x10: valor de 64 bits (flag 0x1000);
- +0x18: valor < 4 (flag 0x800);
- +0x28: contagem; +0x2C: ponteiro de tamanho; +0x30: ponteiro do handle.
- Cria um enumerador XAM com itens de 0x24 bytes, cada um com cópia de 0x38 bytes da consulta.

## Como o Nike usa

O Nike resolve as funções **na primeira chamada**, por ordinal (`Function_82D9C468(xam, ordinal)`). Se faltarem, devolve **`0x80004001`** (E_NOTIMPL).

São nove: 1209 (BodyProfileUI), 1218, 1222, 1228, 1243, 1244 (WarnAboutPrivacyUI), 1246, 1250 e 1275. Os wrappers ficam em 0x82D47D50 a 0x82D48198.

- **Inicialização** (`82F55A08`): aloca 8 blocos e chama `XamFitnessInitialize(&saida)`. Se falhar, registra "XamFitnessInitialize: 0x%x", desliga o fitness (`DAT_83B04539 = 0`) e segue. No Xenia hoje o log mostra `XamFitnessInitialize: 0x80004001`.
- **Privacidade** (`82F55508`): `GetPrivacySettings`. O valor 0 vira o erro 0x83010015 e o 1 vira 0x83010016; os demais viram o enum do Nike (2 → 1, 3 → 3, 4 → 4, 5 → 5, 6 → 2).
- **Perfil do corpo** (`82F55718`): exige o privilégio de executável 0x2A e chama `GetCurrentBodyProfileRecord(usuário, buf, 0x10)`. Do registro tira: bytes 2 a 5, 6 a 9 e 10 a 11, e o byte 1 == 2 vira um flag (provavelmente peso, altura e sexo).
- **Fim de exercício** (`82F55E48`): monta um evento com:
  - horário de início (`SystemTimeToMsgTime`);
  - duração em segundos (ms / 1000);
  - calorias (double dividido em dois u16);
  - MET em byte (`ConvertFloatMetToByteMet`);
  - tipo (0x20, ou 0x10 conforme o modo).
  
  Depois chama `XamFitnessAddFitnessEvent`.

## No Xenia

Desde **5125708b2** (2026-10-25), `src/xenia/kernel/xam/xam_fitness.cc` implementa as funções que o [Nike](../kinect/jogos/nike-kinect-training.md) usa:
- **`XamFitnessInitialize`** grava 1 na saída. Com isso o Nike marca o serviço como pronto (`DAT_83b04539`).
- **`XamFitnessUnInitialize`** responde sucesso.
- **`XamFitnessGetCurrentBodyProfileRecord`** devolve o registro de 0x10 bytes guardado em `storage_root/fitness/<XUID>.body`:
  - +1: sexo (2 = feminino);
  - +2: altura em metros (float);
  - +6: peso em kg (float);
  - +0xA: ano de nascimento (u16).
  Sem perfil, responde `0x80070490`, e o jogo usa os padrões.
- **`XamFitnessAddFitnessEvent`** acrescenta o evento a `<XUID>.events` e o registra no log. O evento do Nike tem 0xC bytes: início em MsgTime, duração em s, calorias (u32), tipo (0x10 a 0x40) e MET × 10.
- **`XamFitnessGetPrivacySettings`** responde 1 para perfil local e 2 para perfil na Live sem configuração, como o xam.
- **Conversão de tempo e de MET:** `SystemTimeToMsgTime`, `MsgTimeToSystemTime` e `ConvertFloatMetToByteMet`.
- **`XamShowFitnessBodyProfileUI`** abre um diálogo para preencher sexo, altura, peso e ano de nascimento. `XamShowFitnessWarnAboutPrivacyUI` só responde sucesso.

**Por que importa:** sem o perfil do corpo, a biblioteca de calorias do Kinect no Nike (`Function_82F55B80`) usa **175 cm, 75 kg e 28 anos** para qualquer jogador (constantes em 0x821C3CD0, 0x820186E0 e 0x82058274; idade mínima 4).

**Ainda não feitas:** enumeradores, resumos, `ClearAll`, `GetSyncStatus` e a conversão de MET de byte para float (o shim do Xenia não tem retorno em ponto flutuante). O Nike não usa nenhuma delas.

**Teste ao vivo pendente:**
1. Abrir o perfil do corpo pelo menu do Nike.
2. Fazer um treino.
3. Ver `Fitness: workout of …` no log.
