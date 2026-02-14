#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <Arduino.h>
#include "oseppRobot.h"

// OSEPP range finder pin
#define RANGE_PIN 2

class Sensor {
  public:
    Sensor(long threshold);
    uint8_t triggered();

  private:
    OseppRangeFinder _rangeFinder;
    long _distanceThreshold;  // in cm
};

#endif
