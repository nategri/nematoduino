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

void MotorsForward(uint8_t spd) {
  leftMotorForward(spd);
  rightMotorForward(spd);
}

void MotorsBackward(uint8_t spd) {
  leftMotorBackward(spd);
  rightMotorBackward(spd);
}

void MotorsOff() {
  leftMotorOff();
  rightMotorOff();
}

void MotorsLeftTurn(uint8_t spd) {
  leftMotorForward(spd);
  rightMotorBackward(spd);
}

void MotorsRightTurn(uint8_t spd) {
  rightMotorForward(spd);
  leftMotorBackward(spd);
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

