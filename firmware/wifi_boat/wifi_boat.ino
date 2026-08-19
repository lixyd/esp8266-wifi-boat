/*
 * wifi_boat.ino
 * ESP8266 双电机 Wi-Fi 遥控船 · 主程序入口
 *
 * 当前阶段：第一阶段
 *   只验证开发板能编译、烧录、运行。
 *   不初始化电机 GPIO，不启动 Web Server。
 *
 * 板载 LED：
 *   NodeMCU V3 的 LED_BUILTIN 是 GPIO2（丝印 D4）。
 *   这颗灯是低电平点亮（写 LOW 灯亮，写 HIGH 灯灭）。
 *
 * 成功标志：
 *   烧录完成后，开发板上的蓝灯大约每秒闪一次。
 *   串口监视器（115200）会反复打印 "Blink OK"。
 */

#include <Arduino.h>
#include "config.h"

void setup() {
  // 打开串口，方便在电脑上看板子是否在跑
  Serial.begin(SERIAL_BAUD);
  delay(200);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println();
  Serial.println(F("================================"));
  Serial.println(F("wifi_boat  第一阶段  Blink 验证"));
  Serial.println(F("板子: NodeMCU V3 (ESP8266)"));
  Serial.println(F("本阶段不控制电机、不启动网页"));
  Serial.println(F("================================"));
}

void loop() {
  // 点亮板载 LED（低电平有效）
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("Blink OK  LED ON"));
  delay(500);

  // 熄灭板载 LED
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(F("Blink OK  LED OFF"));
  delay(500);
}
