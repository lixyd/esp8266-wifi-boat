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

  Serial.println(F("Motor: GPIO 已初始化  IN4=D8"));
}

void Motor::setSpeed(int speed) {
  if (speed < 0) speed = 0;
  if (speed > MOTOR_PWM_MAX) speed = MOTOR_PWM_MAX;
  speed_ = speed;
}

void Motor::driveLeft(int dir) {
  if (dir > 0) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, speed_);
  } else if (dir < 0) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    analogWrite(PIN_ENA, speed_);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, 0);
  }
}

void Motor::driveRight(int dir) {
  if (dir > 0) {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, speed_);
  } else if (dir < 0) {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    analogWrite(PIN_ENB, speed_);
  } else {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, 0);
  }
}

void Motor::forward() {
  driveLeft(1);
  driveRight(1);
}

void Motor::backward() {
  driveLeft(-1);
  driveRight(-1);
}

void Motor::left() {
  // 差速原地左转：左桨反转，右桨正转
  driveLeft(-1);
  driveRight(1);
}

void Motor::right() {
  driveLeft(1);
  driveRight(-1);
}

void Motor::uturn() {
  // 掉头先按原地右转处理，第四阶段再按实船调方向
  driveLeft(1);
  driveRight(-1);
}

void Motor::stop() {
  driveLeft(0);
  driveRight(0);
}

void Motor::apply(const String& cmd) {
  if (cmd == "forward") forward();
  else if (cmd == "backward") backward();
  else if (cmd == "left") left();
  else if (cmd == "right") right();
  else if (cmd == "uturn") uturn();
  else stop();

  Serial.print(F("  IN3="));
  Serial.print(digitalRead(PIN_IN3));
  Serial.print(F(" IN4="));
  Serial.println(digitalRead(PIN_IN4));
}
