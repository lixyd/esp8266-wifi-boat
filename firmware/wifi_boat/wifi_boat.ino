/*
 * wifi_boat.ino
 * ESP8266 双电机 Wi-Fi 遥控船 · 主程序入口
 *
 * 当前阶段：第三阶段
 *   网页按钮会调用 Motor，GPIO 按 L298N 定义动作。
 *   还没有接线，电机不会转。不要接 L298N。
 *
 * 手机：连 wifi-boat / 12345678 → http://192.168.4.1/
 * 电脑串口：Arduino IDE 菜单「查看」→「串口监视器」，115200
 */

#include <Arduino.h>
#include "config.h"
#include "src/motor/Motor.h"
#include "src/web/WebControl.h"

Motor motors;
WebControl web;
unsigned long lastBlinkMs = 0;
bool ledOn = false;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  motors.begin();
  web.begin(&motors);
}

void loop() {
  web.loop();

  const unsigned long now = millis();
  if (now - lastBlinkMs >= 500) {
    lastBlinkMs = now;
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn ? LOW : HIGH);
  }
}
