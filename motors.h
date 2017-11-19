#include <PID_v1.h>
#include "pin_config.h"

// Initial PID parameters
double Kp = 30;
double Ki = 0;
double Kd = 0;

double LKp = Kp;
double LKi = Ki;
double LKd = Kd;

double RKp = Kp;
double RKi = Ki;
double RKd = Kd;

// PID global variables
double LeftDesiredSpd, LeftEncoderIn, LeftMotorOut;
double RightDesiredSpd, RightEncoderIn, RightMotorOut;

// Global PID variables
PID LeftPID(&LeftEncoderIn, &LeftMotorOut, &LeftDesiredSpd, LKp, LKi, LKd, DIRECT);
PID RightPID(&RightEncoderIn, &RightMotorOut, &RightDesiredSpd, RKp, RKi, RKd, DIRECT);

const uint8_t turnTime = 0;

uint8_t LeftMin = 0;
uint8_t LeftMax = 255;

uint8_t RightMin = 0;
uint8_t RightMax = 255;

void MotorsInit() {
  pinMode(L_MOTOR_PWM, OUTPUT);
  pinMode(L_MOTOR_DIR, OUTPUT);
  pinMode(R_MOTOR_PWM, OUTPUT);
  pinMode(R_MOTOR_DIR, OUTPUT);
}

void MotorsInitPID() {
  MotorsInit();
  LeftEncoderIn = analogRead(L_ENCODER);
  RightEncoderIn = analogRead(R_ENCODER);

  LeftDesiredSpd = 100;
  RightDesiredSpd = 100;

  LeftPID.SetMode(AUTOMATIC);
  RightPID.SetMode(AUTOMATIC);
}

void MotorSimple(uint8_t spd) {
  digitalWrite(R_MOTOR_DIR, LOW);
  analogWrite(R_MOTOR_PWM, spd);
  digitalWrite(L_MOTOR_DIR, LOW);
  analogWrite(L_MOTOR_PWM, spd);
}

void rightMotorForward(uint8_t spd) {
  digitalWrite(R_MOTOR_DIR, LOW);
  //analogWrite(R_MOTOR_PWM, 255);
  //delay(6);
  analogWrite(R_MOTOR_PWM, spd);
}

void rightMotorBackward(uint8_t spd) {
  digitalWrite(R_MOTOR_DIR, HIGH);
  analogWrite(R_MOTOR_PWM, 255);
  delay(6);
  analogWrite(R_MOTOR_PWM, spd);
}

void rightMotorOff() {
  digitalWrite(R_MOTOR_PWM, 0);
}

void leftMotorForward(uint8_t spd) {
  digitalWrite(L_MOTOR_DIR, LOW);
  //analogWrite(L_MOTOR_PWM, 255);
  //delay(6);
  analogWrite(L_MOTOR_PWM, spd);
}

void leftMotorBackward(uint8_t spd) {
  digitalWrite(L_MOTOR_DIR, HIGH);
  analogWrite(L_MOTOR_PWM, 255);
  delay(6);
  analogWrite(L_MOTOR_PWM, spd);
}

void leftMotorOff() {
  digitalWrite(L_MOTOR_PWM, 0);
}

void RunMotorsPID(int16_t leftSpd, int16_t rightSpd, uint32_t count) {
  
  LeftDesiredSpd = leftSpd;
  RightDesiredSpd = rightSpd;

  //for(uint32_t i=count; i<count; i++) {
  while(1) {
    LeftEncoderIn = analogRead(L_ENCODER);
    RightEncoderIn = analogRead(R_ENCODER);

    Serial.println(RightEncoderIn);
    delay(10000);

    LeftPID.Compute();
    RightPID.Compute();
    
    // Left forward
    if(leftSpd > 0) {
      leftMotorForward(LeftMotorOut);
    }
    // Left backward
    else if(leftSpd < 0) {
      leftMotorBackward(LeftMotorOut);
    }
    else if(leftSpd == 0) {
      leftMotorOff();
    }
  
    // Right forward
    if(rightSpd > 0) {
      rightMotorForward(RightMotorOut);
    }
    // Right backward
    else if(rightSpd < 0) {
      rightMotorBackward(RightMotorOut);
    }
    else if(rightSpd == 0) {
      rightMotorOff();
    }
  
  
    if((rightSpd > 0) && (leftSpd < 0)) {
      delay(turnTime);
    }
    else if((rightSpd < 0) && (leftSpd > 0)) {
      delay(turnTime);
    }
  }
}

/*void RunMotors(int16_t leftSpd, int16_t rightSpd) {
  uint8_t leftSpdMotor;
  uint8_t rightSpdMotor;
  
  // Left forward
  if(leftSpd > 0) {
    leftSpdMotor = map(leftSpd, 0, 255, LeftMin, LeftMax);
    constrain(leftSpdMotor, LeftMin, LeftMax);
    leftMotorForward(leftSpdMotor);
  }
  // Left backward
  else if(leftSpd < 0) {
    leftSpdMotor = map(abs(leftSpd), 0, 255, LeftMin, LeftMax);
    constrain(leftSpdMotor, LeftMin, LeftMax);
    leftMotorBackward(leftSpdMotor);
  }
  else if(leftSpd == 0) {
    leftMotorOff();
  }

  // Right forward
  if(rightSpd > 0) {
    rightSpdMotor = map(rightSpd, 0, 255, RightMin, RightMax);
    constrain(rightSpdMotor, RightMin, RightMax);
    rightMotorForward(rightSpdMotor);
  }
  // Right backward
  else if(rightSpd < 0) {
    rightSpdMotor = map(abs(rightSpd), 0, 255, RightMin, RightMax);
    constrain(rightSpdMotor, RightMin, RightMax);
    rightMotorBackward(rightSpdMotor);
  }
  else if(rightSpd == 0) {
    rightMotorOff();
  }


  if((rightSpd > 0) && (leftSpd < 0)) {
    delay(turnTime);
  }
  else if((rightSpd < 0) && (leftSpd > 0)) {
    delay(turnTime);
  }
}*/

void MotorsOff() {
  leftMotorOff();
  rightMotorOff();
}

