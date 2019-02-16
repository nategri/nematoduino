#include "motors.hpp"

Motors::Motors() {
  pinMode(LSPD_PIN, OUTPUT);
  pinMode(RSPD_PIN, OUTPUT);

  pinMode(LFWD_PIN, OUTPUT);
  pinMode(LBCK_PIN, OUTPUT);

  pinMode(RFWD_PIN, OUTPUT);
  pinMode(RBCK_PIN, OUTPUT);
}

void Motors::_leftForward(uint8_t spd) {
  digitalWrite(LBCK_PIN, LOW);
  digitalWrite(LFWD_PIN, HIGH);

  analogWrite(LSPD_PIN, spd);
}

void Motors::_leftBackward(uint8_t spd) {
  digitalWrite(LFWD_PIN, LOW);
  digitalWrite(LBCK_PIN, HIGH);

  analogWrite(LSPD_PIN, spd);
}

void Motors::_leftOff() {
  digitalWrite(LFWD_PIN, LOW);
  digitalWrite(LBCK_PIN, LOW);

  analogWrite(LSPD_PIN, 0);
}

void Motors::_rightForward(uint8_t spd) {
  digitalWrite(RBCK_PIN, LOW);
  digitalWrite(RFWD_PIN, HIGH);

  analogWrite(RSPD_PIN, spd);
}

void Motors::_rightBackward(uint8_t spd) {
  digitalWrite(RFWD_PIN, LOW);
  digitalWrite(RBCK_PIN, HIGH);

  analogWrite(RSPD_PIN, spd);
}

void Motors::_rightOff() {
  digitalWrite(RFWD_PIN, LOW);
  digitalWrite(RBCK_PIN, LOW);

  analogWrite(RSPD_PIN, 0);
}

void Motors::run(int16_t inputLeftSpd, int16_t inputRightSpd) {
  int8_t leftSign = inputLeftSpd / abs(inputLeftSpd);
  int8_t rightSign = inputRightSpd / abs(inputRightSpd);

  uint8_t outputLeftSpd = min( abs(inputLeftSpd)+SPDBOOST, 255);
  uint8_t outputRightSpd = min( abs(inputRightSpd)+SPDBOOST, 255);

  if(leftSign > 0) {
    this->_leftForward(outputLeftSpd);
  }
  else if(leftSign < 0) {
    this->_leftBackward(outputLeftSpd);
  }
  
  if(rightSign > 0) {
    this->_rightForward(outputRightSpd);
  }
  else if(rightSign < 0) {
    this->_rightBackward(outputRightSpd);
  }

  if((leftSign*rightSign) < 0) {
    delay(TURN_TIME);
  }
}

void Motors::off() {
  this->_leftOff();
  this->_rightOff();
}
