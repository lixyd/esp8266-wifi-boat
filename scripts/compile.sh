#!/bin/zsh
# 只编译，不烧录。用来确认代码能通过。
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SKETCH="$ROOT/firmware/wifi_boat"
FQBN="esp8266:esp8266:nodemcuv2"

echo "编译 sketch: $SKETCH"
echo "板型 FQBN : $FQBN"
arduino-cli compile --fqbn "$FQBN" --warnings default "$SKETCH"
echo "编译成功"
