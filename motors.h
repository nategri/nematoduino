#include "pin_config.h"

const uint8_t spdBoost = 0;
const uint8_t turnTime = 0;

void MotorsInit() {
  pinMode(leftMotorPinPwm, OUTPUT);
  pinMode(leftMotorPinDir, OUTPUT);
  pinMode(rightMotorPinPwm, OUTPUT);
  pinMode(rightMotorPinDir, OUTPUT);
}

void rightMotorForward(uint8_t spd) {
  digitalWrite(rightMotorPinDir, LOW);
  analogWrite(rightMotorPinPwm, spd);
}

void rightMotorBackward(uint8_t spd) {
  digitalWrite(rightMotorPinDir, HIGH);
  analogWrite(rightMotorPinPwm, spd);
}

void rightMotorOff() {
  digitalWrite(rightMotorPinPwm, 0);
}

void leftMotorForward(uint8_t spd) {
  digitalWrite(leftMotorPinDir, LOW);
  analogWrite(leftMotorPinPwm, spd);
}

void leftMotorBackward(uint8_t spd) {
  digitalWrite(leftMotorPinDir, HIGH);
  analogWrite(leftMotorPinPwm, spd);
}

void leftMotorOff() {
  digitalWrite(leftMotorPinPwm, 0);
}

void RunMotors(int16_t leftSpd, int16_t rightSpd) {
  uint8_t leftSpdMotor;
  uint8_t rightSpdMotor;
  
  // Left forward
  if(leftSpd >= 0) {
    leftSpd += spdBoost;
    if(leftSpd > 255) {
      leftSpdMotor = 255;
    }
    else {
      leftSpdMotor = leftSpd;
    }
    leftMotorForward(leftSpdMotor);
  }
  // Left backward
  else if(leftSpd < 0) {
    leftSpd -= spdBoost;
    if(abs(leftSpd) > 255) {
      leftSpdMotor = 255;
    }
    else {
      leftSpdMotor = abs(leftSpd);
    }
    leftMotorBackward(leftSpdMotor);
  }

    // Right forward
  if(rightSpd >= 0) {
    rightSpd += spdBoost;
    if(rightSpd > 255) {
      rightSpdMotor = 255;
    }
    else {
      rightSpdMotor = rightSpd;
    }
    rightMotorForward(rightSpdMotor);
  }
  // Right backward
  else if(rightSpd < 0) {
    rightSpd -= spdBoost;
    if(abs(rightSpd) > 255) {
      rightSpdMotor = 255;
    }
    else {
      rightSpdMotor = abs(rightSpd);
    }
    rightMotorBackward(rightSpdMotor);
  }

  if((rightSpd > 0) && (leftSpd < 0)) {
    delay(turnTime);
  }
  else if((rightSpd < 0) && (leftSpd > 0)) {
    delay(turnTime);
  }
}

void MotorsOff() {
  leftMotorOff();
  rightMotorOff();
}

