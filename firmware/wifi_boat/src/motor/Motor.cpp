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
  Serial.println(F("Motor: 特慢10  慢40  快90"));
}

void Motor::loop() {}

void Motor::setSpeed(int speed) {
  if (speed < 0) speed = 0;
  if (speed > 100) speed = 100;
  speed_ = speed;
  Serial.print(F("Motor: 档 "));
  Serial.println(speed_);
}

int Motor::hwPwm(int pct) const {
  if (pct <= 0) return 0;
  if (pct >= 100) return MOTOR_PWM_MAX;
  return pct * MOTOR_PWM_MAX / 100;
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

void Motor::setMix(int leftDir, int leftPct, int rightDir, int rightPct) {
  leftDir_ = leftDir;
  rightDir_ = rightDir;
  leftPct_ = leftPct;
  rightPct_ = rightPct;
  driveLeft(leftDir_, leftPct_);
  driveRight(rightDir_, rightPct_);
}

int Motor::turnInner() const {
  return speed_ * MOTOR_TURN_INNER / 100;
}

int Motor::turnOuter() const {
  return speed_ * MOTOR_TURN_OUTER / 100;
}

void Motor::forward() {
  setMix(1, speed_, 1, speed_);
}

void Motor::backward() {
  setMix(-1, speed_, -1, speed_);
}

void Motor::left() {
  setMix(1, turnInner(), 1, turnOuter());
}

void Motor::right() {
  setMix(1, turnOuter(), 1, turnInner());
}

void Motor::uturn() {
  setMix(-1, turnInner(), 1, turnOuter());
}

void Motor::stop() {
  lastMove_ = "stop";
  leftDir_ = 0;
  rightDir_ = 0;
  leftPct_ = 0;
  rightPct_ = 0;
  driveLeft(0, 0);
  driveRight(0, 0);
}

void Motor::apply(const String& cmd) {
  if (cmd == "crawl") {
    setSpeed(MOTOR_SPEED_CRAWL);
    if (lastMove_ != "stop") apply(lastMove_);
    return;
  }
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

  if (cmd == "forward") {
    lastMove_ = cmd;
    forward();
  } else if (cmd == "backward") {
    lastMove_ = cmd;
    backward();
  } else if (cmd == "left") {
    lastMove_ = cmd;
    left();
  } else if (cmd == "right") {
    lastMove_ = cmd;
    right();
  } else if (cmd == "uturn") {
    lastMove_ = cmd;
    uturn();
  } else {
    stop();
  }

  Serial.print(F("  L="));
  Serial.print(leftPct_);
  Serial.print(F("% R="));
  Serial.print(rightPct_);
  Serial.println('%');
}
