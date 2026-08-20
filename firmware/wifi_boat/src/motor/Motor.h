/*
 * Motor.h
 * 三档：特慢≈1/100 脉冲、慢=巡航、快=拉满。
 * 转弯按当前档的 50%/80%，两轮正转。
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
  bool crawl_ = false;
  String lastMove_ = "stop";
  int leftDir_ = 0;
  int rightDir_ = 0;
  int leftPct_ = 0;
  int rightPct_ = 0;

  void setMix(int leftDir, int leftPct, int rightDir, int rightPct);
  void refresh();
  void driveLeft(int dir, int pwm);
  void driveRight(int dir, int pwm);
  int hwPwm(int pct) const;
  void writeEnable(uint8_t pin, int hw);
};

#endif
