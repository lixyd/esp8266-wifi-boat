/*
 * config.h
 * 全项目配置集中放这里，避免 Wi-Fi 名称、引脚号散落在各文件。
 *
 * 当前阶段：第二阶段
 *   只用到串口和热点参数。
 *   电机 GPIO 仍然不启用，避免误碰到还没接线的 L298N。
 */

#ifndef WIFI_BOAT_CONFIG_H
#define WIFI_BOAT_CONFIG_H

// 串口监视器波特率。Arduino IDE 右上角也要选同一个数。
#define SERIAL_BAUD 115200

// 开发板自己开的 Wi-Fi 热点。手机搜这个名字。
#define WIFI_AP_SSID     "wifi-boat"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_AP_CHANNEL  1
#define WIFI_AP_MAX_CONN 4

// 网页端口。手机浏览器打开 http://192.168.4.1/
#define WEB_SERVER_PORT  80

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
 */

#endif
