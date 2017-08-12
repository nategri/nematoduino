const uint8_t rightPinA = 9;
const uint8_t rightPinB = 10;

const uint8_t leftPinA = 5;
const uint8_t leftPinB = 6;

void MotorsInit() {
  pinMode(leftPinA, OUTPUT);
  pinMode(leftPinB, OUTPUT);
  pinMode(rightPinA, OUTPUT);
  pinMode(rightPinB, OUTPUT);
}

void rightMotorForward(uint8_t spd) {
  analogWrite(rightPinA, spd);
  digitalWrite(rightPinB, LOW);
}

void rightMotorBackward(uint8_t spd) {
  digitalWrite(rightPinA, LOW);
  analogWrite(rightPinB, spd);
}

void rightMotorOff() {
  digitalWrite(rightPinA, LOW);
  digitalWrite(rightPinB, LOW);
}

void leftMotorForward(uint8_t spd) {
  analogWrite(leftPinA, spd);
  digitalWrite(leftPinB, LOW);
}

void leftMotorBackward(uint8_t spd) {
  digitalWrite(leftPinA, LOW);
  analogWrite(leftPinB, spd);
}

void leftMotorOff() {
  digitalWrite(leftPinA, LOW);
  digitalWrite(leftPinB, LOW);
}

void MotorsOff() {
  leftMotorOff();
  rightMotorOff();
}

void pwmDigital(void (*motorsOn)(uint8_t), uint16_t spd, uint8_t timeUnits) {
  
  float normSpd = (float) spd / 255.0;

  uint8_t onDelay = (uint8_t) 30*normSpd;
  if(onDelay < 15) {
    onDelay = 15;
  }
  
  uint8_t offDelay = 30 - onDelay;

  uint8_t motorSpd = 255;
  if(spd < 0) {
    motorSpd = 0;
  }
  
  for(int i = 0; i < timeUnits; i++) {
    motorsOn(motorSpd);
    delay(onDelay);
    MotorsOff();
    delay(offDelay);
  }
}

void MotorsForward(uint8_t spd) {
  leftMotorForward(spd);
  rightMotorForward(spd);
}

void MotorsForwardPwm(uint8_t spd, uint8_t timeUnits, bool use) {
  if(use == true) {
    pwmDigital(MotorsForward, spd, timeUnits);
  }
  else {
    MotorsForward(spd);
    delay(300);
  }
}

void MotorsBackward(uint8_t spd) {
  leftMotorBackward(spd);
  rightMotorBackward(spd);
}

void MotorsBackwardPwm(uint8_t spd, uint8_t timeUnits, bool use) {
  if(use == true) {
    pwmDigital(MotorsBackward, spd, timeUnits);
  }
  else {
    MotorsBackward(spd);
    delay(300);
  }
}

void MotorsLeftTurn(uint8_t spd) {
  leftMotorForward(spd);
  rightMotorBackward(spd);
}

void MotorsLeftTurnPwm(uint8_t spd, uint8_t timeUnits, bool use) {
  if(use == true) {
    pwmDigital(MotorsLeftTurn, spd, timeUnits);
  }
  else {
    MotorsLeftTurn(spd);
    delay(500);
  }
}

void MotorsRightTurn(uint8_t spd) {
  rightMotorForward(spd);
  leftMotorBackward(spd);
}

void MotorsRightTurnPwm(uint8_t spd, uint8_t timeUnits, bool use) {
  if(use == true) {
    pwmDigital(MotorsRightTurn, spd, timeUnits);
  }
  else {
    MotorsRightTurn(spd);
    delay(500);
  }
}

void MotorsTest() {
  MotorsForward(180);
  delay(1000);
  MotorsLeftTurn(180);
  delay(1000);
  MotorsRightTurn(180);
  delay(1000);
  MotorsBackward(180);
  delay(1000);
  MotorsOff();
}

