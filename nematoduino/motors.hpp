#ifndef MOTORS_HPP
#define MOTORS_HPP

#include <avr/pgmspace.h>
#include <Arduino.h>

#define LSPD_PIN 6
#define RSPD_PIN 5

#define LFWD_PIN 14
#define LBCK_PIN 15

#define RFWD_PIN 17
#define RBCK_PIN 16

#define SPDBOOST 0
#define TURN_TIME 0

class Motors {
  public:
    Motors();
    void run(int16_t, int16_t);
    void off();

  private:
    void _leftForward(uint8_t);
    void _leftBackward(uint8_t);
    void _leftOff();

    void _rightForward(uint8_t);
    void _rightBackward(uint8_t);
    void _rightOff();
};

#endif
