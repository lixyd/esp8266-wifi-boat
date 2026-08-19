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

  Serial.println(F("Motor: 默认慢速，转向再减半"));
}

void Motor::setSpeed(int speed) {
  if (speed < 0) speed = 0;
  if (speed > MOTOR_PWM_MAX) speed = MOTOR_PWM_MAX;
  speed_ = speed;
  Serial.print(F("Motor: 速度 "));
  Serial.println(speed_);
}

int Motor::turnPwm() const {
  // 转向大约用直行的一半，船才转得过来、又不会原地疯转
  int t = speed_ / 2;
  if (t < 280) t = (speed_ < 280) ? speed_ : 280;
  return t;
}

void Motor::driveLeft(int dir, int pwm) {
  if (dir > 0) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, pwm);
  } else if (dir < 0) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    analogWrite(PIN_ENA, pwm);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, 0);
  }
}

void Motor::driveRight(int dir, int pwm) {
  if (dir > 0) {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, pwm);
  } else if (dir < 0) {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    analogWrite(PIN_ENB, pwm);
  } else {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, 0);
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
  const int t = turnPwm();
  driveLeft(-1, t);
  driveRight(1, t);
}

void Motor::right() {
  const int t = turnPwm();
  driveLeft(1, t);
  driveRight(-1, t);
}

void Motor::uturn() {
  const int t = turnPwm();
  driveLeft(1, t);
  driveRight(-1, t);
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
  if (cmd == "mid") {
    setSpeed(MOTOR_SPEED_MID);
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

  Serial.print(F("  IN3="));
  Serial.print(digitalRead(PIN_IN3));
  Serial.print(F(" IN4="));
  Serial.print(digitalRead(PIN_IN4));
  Serial.print(F(" spd="));
  Serial.println(speed_);
}
