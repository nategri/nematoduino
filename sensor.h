// Reference: http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/

#include "pin_config.h"

void SensorInit() {
  pinMode(SENSOR_T, OUTPUT);
  pinMode(SENSOR_R, INPUT);
}

long SensorDistance() {

  // Send sound pulse
  digitalWrite(SENSOR_T, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_T, LOW);

  // Read off length
  long pulseWidth = pulseIn(SENSOR_R, HIGH);

  // Calculate distance
  long distance = (pulseWidth/29.1) / 2;

  return distance;
}

