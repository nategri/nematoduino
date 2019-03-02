#include "sensor.hpp"

Sensor::Sensor(long threshold) {
  this->_ultrasonicDistanceThreshold = threshold;

  Wire.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

uint8_t Sensor::triggered() {
  long d = this->_ultrasonicDetectDistance();

  if((d < this->_ultrasonicDistanceThreshold) || (this->_infraredTriggered())) {
    return 1;
  }
  else {
    return 0;
  }
}

uint8_t Sensor::_infraredI2CRead() {
  Wire.requestFrom(I2C_ADDR, 1);

  if(Wire.available()) {
    return Wire.read();
  }
  else {
    return 0xFF;
  }
}

void Sensor::_infraredI2CWrite(uint8_t data) {
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t Sensor::_infraredTriggered() {
  uint8_t data = 0xC0 | this->_infraredI2CRead();
  this->_infraredI2CWrite(data);

  uint8_t sensorValue = 0x3F | this->_infraredI2CRead();

  if(sensorValue != 0xFF) {
    return 1;
  }
  else {
    return 0;
  }
}

long Sensor::_ultrasonicDetectDistance() {
  // Send sound pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read off length
  long pulseWidth = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in centimeters
  long distance = (pulseWidth/29.1) / 2;

  return distance;
}
