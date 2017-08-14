#include "pin_config.h"

// Initialize button
// And loop until pressed

void ButtonInit() {
  pinMode(buttonPin, INPUT_PULLUP);
}

bool ButtonPress() {
  if(digitalRead(buttonPin) == LOW) {
    return true;
  }
  else {
    return false;
  }
}
