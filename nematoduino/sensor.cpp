#include "sensor.hpp"

Sensor::Sensor(long threshold) {
  this->_distanceThreshold = threshold;

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

uint8_t Sensor::triggered() {
  long d = this->_detectDistance();

  if(d<this->_distanceThreshold) {
    return 1;
  }
  else {
    return 0;
  }
}

long Sensor::_detectDistance() {
  // Send sound pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read off length
  long pulseWidth = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance
  long distance = (pulseWidth/29.1) / 2;

  return distance;
}
