#!/bin/zsh
# 第一阶段依赖下载：关 VPN 后运行。
# 断点续传、失败换镜像、下完按大小/SHA256 校验。
set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="$ROOT/downloads"
LOG="$DEST/download.log"
ARIA2C="/opt/homebrew/bin/aria2c"
CURL="/usr/bin/curl"

mkdir -p "$DEST"
exec > >(tee -a "$LOG") 2>&1

MIRRORS=(
  "https://ghproxy.net/"
  "https://gh.llkk.cc/"
  ""
)

echo "================================"
echo "ESP8266 遥控船 · 第一阶段自动下载"
echo "目录: $DEST"
echo "日志: $LOG"
echo "时间: $(date '+%Y-%m-%d %H:%M:%S')"
echo "================================"

if pgrep -xq "Karing" 2>/dev/null; then
  echo
  echo "警告：Karing 还在运行。请先在 Karing 里点「断开」。"
  echo "10 秒后仍继续（方便你已经断开但没退出 App 的情况）。"
  sleep 10
else
  echo "Karing 未在跑，按直连网络下载。"
fi

if [[ ! -x "$ARIA2C" ]]; then
  echo "未找到 aria2c，改用 curl 续传。"
fi

# name|filename|github_url|bytes|sha256
# bytes=0 或 sha256=- 表示不做该项校验
FILES=(
  "Arduino IDE 2.3.10|arduino-ide_2.3.10_macOS_arm64.dmg|https://github.com/arduino/arduino-ide/releases/download/2.3.10/arduino-ide_2.3.10_macOS_arm64.dmg|195168554|-"
  "ESP8266 板支持包 3.1.2|esp8266-3.1.2.zip|https://github.com/esp8266/Arduino/releases/download/3.1.2/esp8266-3.1.2.zip|39269515|b3f47686d7783c120c2f10bf82788f921c53db8642cc87012599abb6e335b182"
  "ESP8266 编译器|x86_64-apple-darwin14.xtensa-lx106-elf-e5f9fec.220621.tar.gz|https://github.com/earlephilhower/esp-quick-toolchain/releases/download/3.1.0-gcc10.3/x86_64-apple-darwin14.xtensa-lx106-elf-e5f9fec.220621.tar.gz|75923095|e58d166d4c9925585bea67b69dd82b295d86faaac2fe7c74b26ca61e87249e1d"
  "便携 Python3|python3-macosx-portable.tar.gz|https://github.com/earlephilhower/esp-quick-toolchain/releases/download/2.5.0-4/python3-macosx-portable.tar.gz|25494144|01a5bf1fa264c6f04cfaadf4c6e9f6caaacb6833ef40104dfbe953fcdb9bca1c"
  "mkspiffs|x86_64-apple-darwin14.mkspiffs-7fefeac.220621.tar.gz|https://github.com/earlephilhower/esp-quick-toolchain/releases/download/3.1.0-gcc10.3/x86_64-apple-darwin14.mkspiffs-7fefeac.220621.tar.gz|368767|2e88dacdb0f8c077af4e76883b5c394331065ffdda16971f8ea584ca40c56821"
  "mklittlefs|x86_64-apple-darwin14.mklittlefs-30b7fc1.220621.tar.gz|https://github.com/earlephilhower/esp-quick-toolchain/releases/download/3.1.0-gcc10.3/x86_64-apple-darwin14.mklittlefs-30b7fc1.220621.tar.gz|365752|928812182a240568ad03fc0c8fa68a38ca78bed89a2b3e5d66efa0e22c8f0593"
  "serial-discovery|serial-discovery_v1.5.2_macOS_ARM64.tar.gz|https://github.com/arduino/serial-discovery/releases/download/v1.5.2/serial-discovery_v1.5.2_macOS_ARM64.tar.gz|2086245|-"
  "serial-monitor|serial-monitor_v0.15.0_macOS_ARM64.tar.gz|https://github.com/arduino/serial-monitor/releases/download/0.15.0/serial-monitor_v0.15.0_macOS_ARM64.tar.gz|2276641|-"
)

ok_count=0
fail_count=0
skip_count=0
FAILED=()

file_size() {
  local f="$1"
  if [[ -f "$f" ]]; then
    stat -f%z "$f"
  else
    echo 0
  fi
}

is_complete() {
  local path="$1" expect_bytes="$2" expect_sha="$3"
  [[ -f "$path" ]] || return 1
  local actual
  actual="$(file_size "$path")"
  if [[ "$expect_bytes" != "0" && "$actual" != "$expect_bytes" ]]; then
    return 1
  fi
  if [[ "$expect_sha" != "-" ]]; then
    local got
    got="$(shasum -a 256 "$path" | awk '{print $1}')"
    [[ "$got" == "$expect_sha" ]] || return 1
  fi
  # 没有 sha 时，至少不能是明显的 HTML 错误页
  if [[ "$actual" -lt 2048 ]]; then
    return 1
  fi
  return 0
}

download_with_aria2() {
  local url="$1" out="$2" expect_sha="$3"
  local args=(
    -c
    -x 8
    -s 8
    -k 1M
    --file-allocation=none
    --auto-file-renaming=false
    --allow-overwrite=true
    --always-resume=true
    --remote-time=true
    --max-tries=20
    --retry-wait=3
    --timeout=60
    --connect-timeout=20
    --lowest-speed-limit=1024
    --max-file-not-found=3
    --summary-interval=10
    --console-log-level=notice
    -d "$DEST"
    -o "$out"
  )
  if [[ "$expect_sha" != "-" ]]; then
    args+=(--checksum="sha-256=$expect_sha")
  fi
  "$ARIA2C" "${args[@]}" "$url"
}

download_with_curl() {
  local url="$1" path="$2"
  "$CURL" -L --fail --retry 30 --retry-delay 3 --retry-all-errors \
    -C - \
    --speed-time 45 --speed-limit 1024 \
    --connect-timeout 20 \
    --max-time 0 \
    -o "$path" "$url"
}

download_one() {
  local name="$1" filename="$2" gh_url="$3" expect_bytes="$4" expect_sha="$5"
  local path="$DEST/$filename"

  echo
  echo "-------- $name --------"
  echo "文件: $filename"
  if [[ "$expect_bytes" != "0" ]]; then
    python3 -c "print('预期: {:.1f} MB'.format($expect_bytes/1024/1024))"
  fi

  if is_complete "$path" "$expect_bytes" "$expect_sha"; then
    echo "已完整，跳过。"
    skip_count=$((skip_count + 1))
    return 0
  fi

  if [[ -f "$path" ]]; then
    echo "发现未下完的文件 $(file_size "$path") 字节，从断点续传。"
  fi

  local mirror url rc
  for mirror in "${MIRRORS[@]}"; do
    url="${mirror}${gh_url}"
    echo "尝试: $url"
    rc=0
    if [[ -x "$ARIA2C" ]]; then
      download_with_aria2 "$url" "$filename" "$expect_sha" || rc=$?
    else
      download_with_curl "$url" "$path" || rc=$?
    fi
    if [[ $rc -eq 0 ]] && is_complete "$path" "$expect_bytes" "$expect_sha"; then
      echo "完成: $filename  ($(file_size "$path") 字节)"
      ok_count=$((ok_count + 1))
      return 0
    fi
    echo "这个镜像不行 (exit=$rc)，换下一个。"
  done

  echo "失败: $name"
  fail_count=$((fail_count + 1))
  FAILED+=("$name")
  return 1
}

for row in "${FILES[@]}"; do
  IFS='|' read -r name filename gh_url expect_bytes expect_sha <<<"$row"
  download_one "$name" "$filename" "$gh_url" "$expect_bytes" "$expect_sha" || true
done

echo
echo "================================"
echo "结果"
echo "  新下完: $ok_count"
echo "  原本就完整: $skip_count"
echo "  失败: $fail_count"
if (( fail_count > 0 )); then
  echo "  失败项: ${FAILED[*]}"
  echo
  echo "把这条命令再贴一次就会从断点继续，不用从头下。"
  echo "  $ROOT/scripts/download_deps.sh"
  echo "================================"
  exit 1
fi
echo
echo "8 个文件都齐了。回来跟我说一声，我接着装 IDE、装板支持包、烧 Blink。"
echo "================================"
exit 0
