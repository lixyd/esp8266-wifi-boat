# 小船（ESP8266 Wi-Fi 遥控船）

这是 **小船** 仓库。无人机在 `~/Desktop/无人机/` 和 `Obsidian/AgentSharedMemory/项目/无人机/`，**不要混进本仓，不要拆船上的件去飞。**

## 先读

1. `docs/手册.html` — 采购、接线、油门、程序、照片（离线打开）
2. `firmware/wifi_boat/` — 当前固件（`config.h` 引脚和速度）
3. GitHub（公开）：https://github.com/lixyd/esp8266-wifi-boat

## 干活时

- Shell 必须走 RTK（`rtk ls` / `rtk read` / `rtk grep` / `rtk git`）。全局 Codex 钩子会改写。
- 改代码后如需更新手册：`python3 scripts/build_handbook.py`
- 烧录：`./scripts/compile.sh` / `./scripts/upload.sh`（波特率 57600）

## 现状摘要

热点 `wifi-boat` / `12345678` → http://192.168.4.1/  
双电机 + L298N HW-095 已接；特慢脉冲 / 慢 40% / 快 90%。摄像头 MW1350 不能接这块 ESP。
