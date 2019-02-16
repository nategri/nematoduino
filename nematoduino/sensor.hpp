// Reference: http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/
#include <Arduino.h>

#define ECHO_PIN 2 // RX
#define TRIG_PIN 3 // TX

class Sensor {
  public:
    Sensor(long);
    uint8_t triggered();

  private:
    long _detectDistance();
    long _distanceThreshold;
};
