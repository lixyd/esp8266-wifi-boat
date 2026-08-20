/*
 * Motor.h
 * 特慢≤10%  慢≤40%  快=90%（≥80）。
 * 转弯为当前档的 50%/80%，不会超过该档上限。
 */

#ifndef WIFI_BOAT_MOTOR_H
#define WIFI_BOAT_MOTOR_H

#include <Arduino.h>
#include "../../config.h"

class Motor {
 public:
  void begin();
  void loop();
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
  String lastMove_ = "stop";
  int leftDir_ = 0;
  int rightDir_ = 0;
  int leftPct_ = 0;
  int rightPct_ = 0;

  void setMix(int leftDir, int leftPct, int rightDir, int rightPct);
  void driveLeft(int dir, int pwm);
  void driveRight(int dir, int pwm);
  int hwPwm(int pct) const;
  void writeEnable(uint8_t pin, int hw);
  int turnInner() const;
  int turnOuter() const;
};

#endif
