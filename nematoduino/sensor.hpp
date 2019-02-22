#ifndef SENSOR_HPP
#define SENSOR_HPP

// References:
// http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/
// https://www.waveshare.com/wiki/AlphaBot2-Ar (demo code)

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR 0x20

#define ECHO_PIN 2 // RX
#define TRIG_PIN 3 // TX

class Sensor {
  public:
    Sensor(long);
    uint8_t triggered();

  private:
    uint8_t _infraredI2CRead();
    void _infraredI2CWrite(uint8_t);
    uint8_t _infraredTriggered();

    long _ultrasonicDetectDistance();
    long _ultrasonicDistanceThreshold;
};

#endif
