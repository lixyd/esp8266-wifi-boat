/*
 * config.h
 * 全项目配置集中放这里。
 *
 * 当前阶段：第三阶段
 *   已定义 L298N 引脚和 Motor 类，网页指令会改 GPIO。
 *   还没有接线，电机不会转。第四阶段才接 L298N。
 */

#ifndef WIFI_BOAT_CONFIG_H
#define WIFI_BOAT_CONFIG_H

#define SERIAL_BAUD 115200

#define WIFI_AP_SSID     "wifi-boat"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_AP_CHANNEL  1
#define WIFI_AP_MAX_CONN 4
#define WEB_SERVER_PORT  80

/*
 * L298N 接线表（第四阶段才动手，现在只写进程序）
 *
 * NodeMCU 丝印    GPIO     L298N
 * D5              14       ENA   左电机速度（PWM）
 * D1               5       IN1   左电机方向
 * D2               4       IN2   左电机方向
 * D6              12       ENB   右电机速度（PWM）
 * D7              13       IN3   右电机方向
 * D8              15       IN4   右电机方向
 *
 * 不用这些脚的原因：
 *   D3/GPIO0  烧录按键，启动模式相关
 *   D4/GPIO2  板载 LED，启动模式相关
 *   D0/GPIO16 NodeMCU 上经常拉不出高电平，反转会变成停止
 */

#define PIN_ENA 14
#define PIN_IN1 5
#define PIN_IN2 4
#define PIN_ENB 12
#define PIN_IN3 13
#define PIN_IN4 15

// 网页仍按 0–100。硬件 PWM 用 0–1023。
// L298N 压降大，低于约 75% 常只蜂鸣不转，所以 50–100 会映射到 780–1023。
#define MOTOR_PWM_MAX        1023
#define MOTOR_PWM_FREQ_HZ    1000
// 玩具双桨船常见两档：巡航约 70%，高速 100%。
#define MOTOR_SPEED_SLOW     70
#define MOTOR_SPEED_FAST     100
#define MOTOR_SPEED_DEFAULT  MOTOR_SPEED_SLOW
#define MOTOR_HW_MIN_SPIN    780

#define MOTOR_TURN_INNER     50
#define MOTOR_TURN_OUTER     80

#endif
