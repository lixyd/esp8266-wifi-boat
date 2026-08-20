# ESP8266 双电机 Wi-Fi 遥控船

**离线手册（采购 + 接线 + 程序）：** 用浏览器打开 [`docs/手册.html`](docs/手册.html)

GitHub：https://github.com/lixyd/esp8266-wifi-boat

手机连热点 **wifi-boat** / **12345678**，打开 http://192.168.4.1/

当前：双电机已接，网页调速，电池可给 L298N + ESP。

无人机在桌面 `无人机/`，**不要写进本仓库，也不要拆船上的件。**

## 硬件

| 模块 | 规格 |
|------|------|
| 开发板 | NodeMCU V3 ESP8266 CH340 USB-C（只给船） |
| 驱动 | L298N 双路 HW-095 |
| 动力 | 两个直流减速电机 |
| 电源 | 电池 → L298N 12V/GND，L298N 5V → ESP VIN |

## 开发

```bash
cd ~/Desktop/esp8266-wifi-boat
./scripts/compile.sh
./scripts/upload.sh
```

- Arduino IDE 2.3.10 + arduino-cli，FQBN `esp8266:esp8266:nodemcuv2`，烧录波特率 **57600**
- 串口 `/dev/cu.usbserial-10`，监视器在 IDE「查看」菜单，115200
- 改手册内容后可运行 `python3 scripts/build_handbook.py` 重新生成 HTML

## 目录

```
firmware/wifi_boat/     主程序
docs/手册.html          离线总手册
docs/img/               你拍的板和摄像头
hardware/               物料
scripts/                编译烧录
```
