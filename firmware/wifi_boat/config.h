/*
 * config.h
 * 全项目配置集中放这里，避免引脚号、Wi-Fi 名称散落在各文件。
 *
 * 第一阶段：只用到串口波特率。
 * 电机引脚、网页端口、PWM 等会在第三阶段再填，现在先占位并注释掉，
 * 防止误接到还没接线的 L298N。
 */

#ifndef WIFI_BOAT_CONFIG_H
#define WIFI_BOAT_CONFIG_H

// 串口监视器波特率。Arduino IDE 右上角「串口监视器」也要选同一个数。
#define SERIAL_BAUD 115200

/*
 * ---------- 以下内容第三阶段再启用 ----------
 *
 * NodeMCU 丝印 → ESP8266 GPIO 对照（以后接线时用）：
 *   D1 = GPIO5
 *   D2 = GPIO4
 *   D3 = GPIO0   （注意：GPIO0 和启动模式有关，尽量不当电机脚）
 *   D4 = GPIO2   （板载 LED，也和启动模式有关）
 *   D5 = GPIO14
 *   D6 = GPIO12
 *   D7 = GPIO13
 *   D8 = GPIO15  （启动时不要拉高）
 *
 * L298N 建议（第三阶段才会写进正式宏）：
 *   左电机：ENA, IN1, IN2
 *   右电机：ENB, IN3, IN4
 *
 * 第二阶段才会用到：
 *   #define WIFI_AP_SSID     "wifi-boat"
 *   #define WIFI_AP_PASSWORD "12345678"
 *   #define WEB_SERVER_PORT  80
 */

#endif
