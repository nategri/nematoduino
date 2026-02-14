#include "sensor.hpp"

Sensor::Sensor(long threshold)
  : _rangeFinder(RANGE_PIN),
    _distanceThreshold(threshold)
{
}

uint8_t Sensor::triggered() {
  // OseppRangeFinder returns distance in mm, convert to cm
  float distance_mm = _rangeFinder.ping();
  long distance_cm = distance_mm / 10;

  if (distance_cm < _distanceThreshold) {
    return 1;
  }
  return 0;
}
