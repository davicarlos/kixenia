#!/bin/bash
# Reproduz uma gravação do Kinect (--kinect_record_path) num jogo por um tempo e fecha.
# uso: replay_test.sh <gravação> <nome do log> <segundos> <xex> [opções extras do Xenia...]
# XENIA_DIR: checkout do Xenia com a build Release (padrão: diretório atual)
T="$(cd "$(dirname "$0")" && pwd)"
[ $# -ge 4 ] || { echo "uso: $0 <gravação> <nome do log> <segundos> <xex> [opções...]"; exit 2; }
REC="$1"; L="$T/logs/$2.log"; SEGS="$3"; XEX="$4"; shift 4
mkdir -p "$T/logs"
cd "${XENIA_DIR:-.}"
./build/bin/Linux/Release/xenia_edge --storage_root="$T/storage-replay" --allow_kinect_camera=true \
  --allow_nui_initialization=true --protect_zero=false --log_level=${LOGLEVEL:-2} --log_file="$L" \
  --kinect_replay_path="$REC" "$@" "$XEX" >/dev/null 2>&1 &
P=$!; sleep "$SEGS"; kill $P; sleep 4; kill -9 $P 2>/dev/null
echo "log $L"
