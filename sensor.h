// Reference: http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/

#include "pin_config.h"

void SensorInit() {
  pinMode(tPin, OUTPUT);
  pinMode(rPin, INPUT);
}

long SensorDistance() {

  // Send sound pulse
  digitalWrite(tPin, LOW);
  delayMicroseconds(2);
  digitalWrite(tPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(tPin, LOW);

  // Read off length
  long pulseWidth = pulseIn(rPin, HIGH);

  // Calculate distance
  long distance = (pulseWidth/29.1) / 2;

  return distance;
}

