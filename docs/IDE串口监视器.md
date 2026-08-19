# Arduino IDE 2 怎么打开串口监视器

Arduino IDE 2（你装的 2.3.10）和老版本不一样：
**不在「工具」菜单里**，在 **「查看」** 菜单里。

## 打开步骤

1. 用 Arduino IDE 打开  
   `桌面/esp8266-wifi-boat/firmware/wifi_boat/wifi_boat.ino`
2. 顶部选板型：**NodeMCU 1.0 (ESP-12E Module)**  
   选端口：**cu.usbserial-10**（或带 usbserial 的那一项）
3. 菜单 **查看 → 串口监视器**  
   快捷键：**Shift + Command + M**
4. 窗口**最下面**会弹出一块黑色面板，那就是监视器。
5. 面板右上角波特率选 **115200**。

也可以点编辑器右上角一排小图标里，**最右边那个像小终端/列表的按钮**。

## 仍然没有时

- 先关掉再打开 Arduino IDE（板支持包已经装好，要重启才认全）。
- 确认 USB 还插着，端口下拉里能看到 `usbserial`。
- 终端已经能看串口的话，说明板子正常。IDE 只是面板位置不同：

```bash
arduino-cli monitor -p /dev/cu.usbserial-10 -c baudrate=115200
```
