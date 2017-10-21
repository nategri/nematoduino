#ifndef pin_config_h
#define pin_config_h

// Motor pins
const uint8_t rightMotorPinPwm = 3;
const uint8_t leftMotorPinPwm = 11;
const uint8_t rightMotorPinDir = 12;
const uint8_t leftMotorPinDir = 13;

// Button pin (configured so that on is LOW)
const uint8_t buttonPin = 13;

// Transmit and receive pins for distance sensor
const uint8_t tPin = 2;
const uint8_t rPin = 4;

// Pin for status LED (turns on when nose touch neurons stimulated)
const uint8_t statusPin = 12;

#endif
