#include "pin_config.h"

void StatusLedInit() {
  pinMode(statusPin, OUTPUT);
}

void StatusLedOn() {
  digitalWrite(statusPin, HIGH);
}

void StatusLedOff() {
  digitalWrite(statusPin, LOW);
}

