#include <avr/pgmspace.h>

#include <Worm.hpp>

#include "motors.hpp"
#include "sensor.hpp"

Worm worm;
Motors motors;
Sensor sensor(25.0);

void setup() {
}

void loop() {

  if(sensor.triggered()) {
    worm.noseTouch();
  }
  else {
    worm.chemotaxis();
  }

  int16_t leftMuscle = worm.getLeftMuscle();
  int16_t rightMuscle = worm.getRightMuscle();

  motors.run(leftMuscle, rightMuscle);

  delay(100);
}
