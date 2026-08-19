/*
 * Motor.h
 * 第三阶段：L298N 双路电机封装。
 *
 * 现在就会改 GPIO 电平，但板子还没接到 L298N，
 * 所以桨不会转。第四阶段按 config.h 接线后再测。
 */

#ifndef WIFI_BOAT_MOTOR_H
#define WIFI_BOAT_MOTOR_H

#include <Arduino.h>
#include "../../config.h"

class Motor {
 public:
  void begin();
  void setSpeed(int speed);
  void forward();
  void backward();
  void left();
  void right();
  void uturn();
  void stop();
  void apply(const String& cmd);

 private:
  int speed_ = MOTOR_SPEED_DEFAULT;

  // dir: 1 正转, -1 反转, 0 停
  void driveLeft(int dir);
  void driveRight(int dir);
};

#endif
