# Scripts do Kinect

Para este Xenia, com o suporte a Kinect. Ambos rodam a build Release de `$XENIA_DIR` (a raiz deste repositório, já compilada) e gravam o log em `logs/`, ao lado dos scripts.

```
XENIA_DIR=<checkout> ./live_test.sh <nome do log> <default.xex> [opções do Xenia...]
XENIA_DIR=<checkout> ./replay_test.sh <gravação> <nome do log> <segundos> <default.xex> [opções...]
```

- `live_test.sh`: usa o Kinect real. Com `KINECT_FIRMWARE` apontando para o firmware de áudio (o pacote `FFFE07DF00000001` de uma atualização de sistema do console), o motor, o LED e os microfones também funcionam.
- `replay_test.sh`: reproduz uma gravação feita com `--kinect_record_path` por alguns segundos e fecha. Serve para testar jogos sem o sensor e repetir exatamente a mesma sessão.
