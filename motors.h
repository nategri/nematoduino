#include "pin_config.h"

const uint8_t spdBoost = 0;
const uint8_t turnTime = 0;

void MotorsInit() {
  pinMode(L_MOTOR_PWM, OUTPUT);
  pinMode(L_MOTOR_DIR, OUTPUT);
  pinMode(R_MOTOR_PWM, OUTPUT);
  pinMode(R_MOTOR_DIR, OUTPUT);
}

void rightMotorForward(uint8_t spd) {
  digitalWrite(R_MOTOR_DIR, LOW);
  analogWrite(R_MOTOR_PWM, spd);
}

void rightMotorBackward(uint8_t spd) {
  digitalWrite(R_MOTOR_DIR, HIGH);
  analogWrite(R_MOTOR_PWM, spd);
}

void rightMotorOff() {
  digitalWrite(R_MOTOR_PWM, 0);
}

void leftMotorForward(uint8_t spd) {
  digitalWrite(L_MOTOR_DIR, LOW);
  analogWrite(L_MOTOR_PWM, spd);
}

void leftMotorBackward(uint8_t spd) {
  digitalWrite(L_MOTOR_DIR, HIGH);
  analogWrite(L_MOTOR_PWM, spd);
}

void leftMotorOff() {
  digitalWrite(L_MOTOR_PWM, 0);
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

