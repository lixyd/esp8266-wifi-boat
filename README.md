# ESP8266 双电机 Wi-Fi 遥控船

基于 **NodeMCU V3（ESP8266 + CH340 + USB-C）** 的双电机遥控船。
手机连上开发板热点后，用网页控制前进、后退、左转、右转、掉头和停止。

当前阶段：**第二阶段 · Wi-Fi 热点 + 网页按钮（只打印文字）**
（未接 L298N，未接电机）

---

## 硬件清单

| 模块 | 型号 | 本阶段状态 |
|------|------|------------|
| 开发板 | NodeMCU V3（ESP8266，CH340，USB-C） | 已用数据线连上 Mac |
| 电机驱动 | L298N 双路驱动 | 本阶段不接 |
| 动力 | 两个直流减速电机 + 双桨 | 本阶段不接 |
| 电源 | 电池 | 本阶段不接（板子供电走 USB） |
| 舵机 | — | 暂时不用 |

## 开发环境

- 电脑：Mac（Apple Silicon）
- 语言：Arduino 框架（C++）
- 工具：Arduino IDE 2.x（图形界面）+ arduino-cli（命令行编译烧录）
- 版本管理：Git
- 板支持包：`esp8266:esp8266`（选 **NodeMCU 1.0 (ESP-12E Module)**）

## 目录结构

```
esp8266-wifi-boat/
├── README.md
├── docs/                      阶段说明与操作手册
├── firmware/wifi_boat/        Arduino 工程（.ino 必须与文件夹同名）
│   ├── wifi_boat.ino          主程序入口
│   ├── config.h               引脚 / Wi-Fi / PWM 配置（后续阶段填写）
│   └── src/
│       ├── motor/             第三阶段：电机类
│       ├── web/               第二阶段：网页控制
│       ├── sensors/           以后：GPS / 摄像头 / 避障
│       └── ai/                以后：自动巡航 / 自动返航
├── hardware/                  物料、接线表
└── scripts/                   编译 / 烧录脚本
```

Arduino 会递归编译 `src/` 下的 `.cpp` / `.h`，所以以后加模块时，直接往对应目录丢文件即可。

## 分阶段计划

| 阶段 | 内容 | 状态 |
|------|------|------|
| 1 | 安装环境、Git 仓库、Blink 烧录，确认开发板活着 | **完成**（蓝灯约每秒闪一次） |
| 2 | ESP8266 Web Server，手机打开控制页，按钮只打印文字，不控 GPIO | **进行中** |
| 3 | 加入 Motor 类和 GPIO 定义，仍不接线 | 未开始 |
| 4 | 接 NodeMCU + L298N + 双电机，单电机 / 双电机 / 方向 / PWM 测试 | 未开始 |
| 以后 | AI 巡航、GPS、摄像头、自动返航、避障 | 未开始 |

**每完成一个阶段，等你确认后再做下一阶段。**

## 常用命令（第一阶段之后可用）

```bash
cd ~/Desktop/esp8266-wifi-boat
./scripts/compile.sh      # 只编译
./scripts/upload.sh       # 编译并烧录到开发板
```

串口设备当前是：`/dev/cu.usbserial-10`
（拔掉再插可能会变成 `usbserial-11` 之类，以 `ls /dev/cu.usbserial*` 为准）

## 本阶段不要做的事

- 不要接 L298N
- 不要接电机
- 不要给电机驱动板上电
- 不要改 GPIO 去驱动电机
