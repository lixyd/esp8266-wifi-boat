#include "Motor.h"

void Motor::begin() {
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  analogWriteRange(MOTOR_PWM_MAX);
  analogWriteFreq(MOTOR_PWM_FREQ_HZ);

  speed_ = MOTOR_SPEED_DEFAULT;
  stop();

  Serial.println(F("Motor: 快=拉满高电平  转弯映射到能转的区间"));
}

void Motor::setSpeed(int speed) {
  if (speed < 0) speed = 0;
  if (speed > 100) speed = 100;
  speed_ = speed;
  Serial.print(F("Motor: 直行速度 "));
  Serial.println(speed_);
}

int Motor::hwPwm(int pct) const {
  if (pct <= 0) return 0;
  if (pct >= 100) return MOTOR_PWM_MAX;
  if (pct < 50) pct = 50;
  return MOTOR_HW_MIN_SPIN + (MOTOR_PWM_MAX - MOTOR_HW_MIN_SPIN) * (pct - 50) / 50;
}

void Motor::writeEnable(uint8_t pin, int hw) {
  if (hw <= 0) {
    analogWrite(pin, 0);
    digitalWrite(pin, LOW);
  } else if (hw >= MOTOR_PWM_MAX) {
    analogWrite(pin, 0);
    digitalWrite(pin, HIGH);
  } else {
    analogWrite(pin, hw);
  }
}

void Motor::driveLeft(int dir, int pwm) {
  const int hw = hwPwm(pwm);
  if (dir > 0) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    writeEnable(PIN_ENA, hw);
  } else if (dir < 0) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    writeEnable(PIN_ENA, hw);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    writeEnable(PIN_ENA, 0);
  }
}

void Motor::driveRight(int dir, int pwm) {
  const int hw = hwPwm(pwm);
  if (dir > 0) {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    writeEnable(PIN_ENB, hw);
  } else if (dir < 0) {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    writeEnable(PIN_ENB, hw);
  } else {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    writeEnable(PIN_ENB, 0);
  }
}

void Motor::forward() {
  driveLeft(1, speed_);
  driveRight(1, speed_);
}

void Motor::backward() {
  driveLeft(-1, speed_);
  driveRight(-1, speed_);
}

void Motor::left() {
  // 左慢右快，都正转，弧线左转
  driveLeft(1, MOTOR_TURN_INNER);
  driveRight(1, MOTOR_TURN_OUTER);
}

void Motor::right() {
  driveLeft(1, MOTOR_TURN_OUTER);
  driveRight(1, MOTOR_TURN_INNER);
}

void Motor::uturn() {
  // 掉头仍限 50/80：内侧反转 50，外侧正转 80
  driveLeft(-1, MOTOR_TURN_INNER);
  driveRight(1, MOTOR_TURN_OUTER);
}

void Motor::stop() {
  driveLeft(0, 0);
  driveRight(0, 0);
}

void Motor::apply(const String& cmd) {
  if (cmd == "slow") {
    setSpeed(MOTOR_SPEED_SLOW);
    if (lastMove_ != "stop") apply(lastMove_);
    return;
  }
  if (cmd == "fast") {
    setSpeed(MOTOR_SPEED_FAST);
    if (lastMove_ != "stop") apply(lastMove_);
    return;
  }

  if (cmd == "forward") forward();
  else if (cmd == "backward") backward();
  else if (cmd == "left") left();
  else if (cmd == "right") right();
  else if (cmd == "uturn") uturn();
  else stop();

  if (cmd == "forward" || cmd == "backward" || cmd == "left" ||
      cmd == "right" || cmd == "uturn" || cmd == "stop") {
    lastMove_ = cmd;
  }

  Serial.print(F("  Lpwm="));
  Serial.print((cmd == "left") ? MOTOR_TURN_INNER :
               (cmd == "right") ? MOTOR_TURN_OUTER :
               (cmd == "uturn") ? MOTOR_TURN_INNER : speed_);
  Serial.print(F(" Rpwm="));
  Serial.println((cmd == "left") ? MOTOR_TURN_OUTER :
                 (cmd == "right") ? MOTOR_TURN_INNER :
                 (cmd == "uturn") ? MOTOR_TURN_OUTER : speed_);
}
