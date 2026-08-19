#!/bin/zsh
# 编译并烧录到当前插着的 NodeMCU。
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SKETCH="$ROOT/firmware/wifi_boat"
# 接了 L298N/电池后高速烧录容易失败，固定 57600
FQBN="esp8266:esp8266:nodemcuv2:baud=57600"

# 自动找 CH340 串口。如果插了多块板，请改成明确的设备名。
PORT="$(ls /dev/cu.usbserial* 2>/dev/null | head -n 1 || true)"
if [[ -z "${PORT}" ]]; then
  echo "找不到 /dev/cu.usbserial* ，请检查 USB 线和 CH340 是否被识别。"
  echo "排查：ls /dev/cu.*    以及拔掉再插一次数据线。"
  exit 1
fi

echo "编译并烧录"
echo "  sketch : $SKETCH"
echo "  FQBN   : $FQBN"
echo "  串口   : $PORT"

arduino-cli compile --fqbn "$FQBN" --warnings default "$SKETCH"
arduino-cli upload --fqbn "$FQBN" --port "$PORT" "$SKETCH"

echo "烧录完成。板载蓝灯应开始闪烁。"
echo "看串口输出：arduino-cli monitor -p $PORT -c baudrate=115200"
