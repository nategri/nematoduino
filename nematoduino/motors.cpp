#include "motors.hpp"

Motors::Motors()
  : _leftMotor(LEFT_DIR_PIN, LEFT_PWM_PIN, LEFT_FWD_LEVEL),
    _rightMotor(RIGHT_DIR_PIN, RIGHT_PWM_PIN, RIGHT_FWD_LEVEL)
{
}

void Motors::run(int16_t inputLeftSpd, int16_t inputRightSpd) {
  // Apply speed boost if configured
  int16_t leftSpd = inputLeftSpd;
  int16_t rightSpd = inputRightSpd;

  if (inputLeftSpd != 0) {
    int8_t leftSign = (inputLeftSpd > 0) ? 1 : -1;
    leftSpd = leftSign * min(abs(inputLeftSpd) + SPDBOOST, 255);
  }

  if (inputRightSpd != 0) {
    int8_t rightSign = (inputRightSpd > 0) ? 1 : -1;
    rightSpd = rightSign * min(abs(inputRightSpd) + SPDBOOST, 255);
  }

  // OseppTBMotor::forward() handles direction based on sign
  _leftMotor.forward(leftSpd);
  _rightMotor.forward(rightSpd);

  // Delay when turning (opposite directions)
  if ((leftSpd * rightSpd) < 0) {
    delay(TURN_TIME);
  }
}

void Motors::off() {
  _leftMotor.forward(0);
  _rightMotor.forward(0);
}
