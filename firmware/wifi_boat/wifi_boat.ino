/*
 * wifi_boat.ino
 * ESP8266 双电机 Wi-Fi 遥控船 · 主程序入口
 *
 * 当前阶段：第二阶段
 *   开发板开 Wi-Fi 热点，手机浏览器打开控制页。
 *   按钮只在串口打印文字，不初始化电机 GPIO。
 *
 * 用法：
 *   1. 手机连热点 wifi-boat，密码 12345678
 *   2. 浏览器打开 http://192.168.4.1/
 *   3. 点前进/后退/左转/右转/掉头/停止
 *   4. 电脑串口监视器 115200 应出现 [CMD] 前进 等
 *
 * 板载蓝灯大约每秒闪一次，表示程序还在跑。
 */

#include <Arduino.h>
#include "config.h"
#include "src/web/WebControl.h"

WebControl web;
unsigned long lastBlinkMs = 0;
bool ledOn = false;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // 先灭（低电平才亮）

  web.begin();
}

void loop() {
  web.loop();

  // 非阻塞闪灯。这里不能再用 delay()，否则网页会卡死。
  const unsigned long now = millis();
  if (now - lastBlinkMs >= 500) {
    lastBlinkMs = now;
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn ? LOW : HIGH);
  }
}
