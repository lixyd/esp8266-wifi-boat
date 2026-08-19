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

  Serial.println(F("Motor: 直行跟慢中快，转弯内50外80"));
}

void Motor::setSpeed(int speed) {
  if (speed < 0) speed = 0;
  if (speed > MOTOR_PWM_MAX) speed = MOTOR_PWM_MAX;
  speed_ = speed;
  Serial.print(F("Motor: 直行速度 "));
  Serial.println(speed_);
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

  Serial.print(F("  Lpwm="));
  Serial.print((cmd == "left") ? MOTOR_TURN_INNER :
               (cmd == "right") ? MOTOR_TURN_OUTER :
               (cmd == "uturn") ? MOTOR_TURN_INNER : speed_);
  Serial.print(F(" Rpwm="));
  Serial.println((cmd == "left") ? MOTOR_TURN_OUTER :
                 (cmd == "right") ? MOTOR_TURN_INNER :
                 (cmd == "uturn") ? MOTOR_TURN_OUTER : speed_);
}
