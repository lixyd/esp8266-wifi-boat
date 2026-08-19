/*
 * Motor.h
 * L298N 双路电机。转向用更低 PWM，避免原地甩得太猛。
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

  // dir: 1 正转, -1 反转, 0 停；pwm 为这一侧油门
  void driveLeft(int dir, int pwm);
  void driveRight(int dir, int pwm);
  int turnPwm() const;
};

#endif
