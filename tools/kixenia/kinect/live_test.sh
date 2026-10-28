#!/bin/bash
# Roda um jogo no Xenia Edge (com o suporte a Kinect) usando o Kinect real.
# uso: live_test.sh <nome do log> <xex> [opções extras do Xenia...]
# XENIA_DIR: checkout do Xenia com a build Release (padrão: diretório atual)
# KINECT_FIRMWARE: firmware de áudio do Kinect (o pacote FFFE07DF00000001 de uma
#   atualização de sistema do console); sem ele, motor, LED e microfones ficam simulados
# LOGLEVEL: nível do log (padrão 2)
T="$(cd "$(dirname "$0")" && pwd)"
[ $# -ge 2 ] || { echo "uso: $0 <nome do log> <xex> [opções...]"; exit 2; }
L="$T/logs/$1.log"; XEX="$2"; shift 2
mkdir -p "$T/logs"
cd "${XENIA_DIR:-.}"
FW=(); [ -n "$KINECT_FIRMWARE" ] && FW=(--kinect_audio_firmware_path="$KINECT_FIRMWARE")
exec ./build/bin/Linux/Release/xenia_edge --storage_root="$T/storage" --allow_kinect_camera=true \
  --allow_nui_initialization=true --protect_zero=false --log_level=${LOGLEVEL:-2} --log_file="$L" \
  "${FW[@]}" "$@" "$XEX"
