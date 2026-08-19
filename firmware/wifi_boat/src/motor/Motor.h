/*
 * Motor.h
 * 直行两档：慢=巡航 70%，快=拉满。转弯内侧 50、外侧 80，两轮正转。
 */

#ifndef WIFI_BOAT_MOTOR_H
#define WIFI_BOAT_MOTOR_H

#include <Arduino.h>
#include "../../config.h"

class Motor {
 public:
  void begin();
  void setSpeed(int speed);
  int speed() const { return speed_; }
  void forward();
  void backward();
  void left();
  void right();
  void uturn();
  void stop();
  void apply(const String& cmd);

 private:
  int speed_ = MOTOR_SPEED_DEFAULT;
  String lastMove_ = "stop";

  void driveLeft(int dir, int pwm);
  void driveRight(int dir, int pwm);
  int hwPwm(int pct) const;
  void writeEnable(uint8_t pin, int hw);
};

#endif
