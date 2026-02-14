#ifndef MOTORS_HPP
#define MOTORS_HPP

#include <avr/pgmspace.h>
#include <Arduino.h>
#include "oseppRobot.h"

// OSEPP motor pin configuration
// Left motor: dir=12, pwm=11, forward=HIGH
// Right motor: dir=8, pwm=3, forward=LOW
#define LEFT_DIR_PIN 12
#define LEFT_PWM_PIN 11
#define LEFT_FWD_LEVEL HIGH

#define RIGHT_DIR_PIN 8
#define RIGHT_PWM_PIN 3
#define RIGHT_FWD_LEVEL LOW

#define SPDBOOST 0
#define TURN_TIME 0

class Motors {
  public:
    Motors();
    void run(int16_t, int16_t);
    void off();

  private:
    OseppTBMotor _leftMotor;
    OseppTBMotor _rightMotor;
};

#endif
