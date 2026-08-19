#!/bin/zsh
# 关闭 VPN 之后运行：测本机带宽，并对比「国内 CDN vs GitHub」
set -euo pipefail

echo "================================"
echo "测速前先确认 VPN 已断开"
echo "================================"

if pgrep -xq "Karing" 2>/dev/null; then
  echo "警告：Karing 还在运行。请打开 Karing 点「断开」，不要只关窗口。"
  echo "断开后再跑一次本脚本。下面仍会测，结果可能是 VPN 里的速度。"
else
  echo "Karing 进程未在跑，按直连网络测。"
fi

echo
echo "1) 系统自带测速（Apple networkQuality，大约 15–30 秒）"
echo "   这是整机带宽，联通宽带好的话下行常见几十到几百 Mbps。"
networkQuality -v || true

mbps() {
  # curl 的 speed_download 是 字节/秒 → Mbps
  python3 -c "print(round($1 * 8 / 1_000_000, 2))"
}

sample() {
  local name="$1"
  local url="$2"
  echo
  echo "2) 抽样下载：$name"
  echo "   $url"
  echo "   最多测 12 秒，看平均速度。"
  local out
  out="$(curl -L --max-time 12 -o /dev/null -s -w '%{http_code} %{size_download} %{speed_download}' "$url" || true)"
  local code size speed
  code="$(echo "$out" | awk '{print $1}')"
  size="$(echo "$out" | awk '{print $2}')"
  speed="$(echo "$out" | awk '{print $3}')"
  if [[ -z "$speed" || "$speed" == "0" ]]; then
    echo "   失败 http=$code  可能被墙、超时或地址变了。"
    return
  fi
  echo "   HTTP $code  已拉 ${size} 字节  约 $(mbps "$speed") Mbps"
}

# 国内：阿里云镜像，联通走国内骨干，代表「VPN 关掉后正常网速」
sample "国内阿里云镜像" "https://mirrors.aliyun.com/ubuntu-releases/24.04/ubuntu-24.04.2-desktop-amd64.iso"

# GitHub Release：这就是我们下 Arduino 卡住的那类地址
sample "GitHub Release（Arduino IDE）" "https://github.com/arduino/arduino-ide/releases/download/2.3.10/arduino-ide_2.3.10_macOS_arm64.dmg"

echo
echo "怎么看："
echo "  - 系统测速 / 国内镜像 都很快，GitHub 很慢 → 不是联通带宽差，是 GitHub 被堵，该用镜像。"
echo "  - 三个都慢 → 先查 Wi-Fi/网线，或联通本身限速。"
echo
echo "联通节点浏览器测速（可选）："
echo "  https://www.speedtest.cn   选「中国联通」服务器"
echo "================================"
