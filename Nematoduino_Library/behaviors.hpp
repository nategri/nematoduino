#ifndef BEHAVIORS_H
#define BEHAVIORS_H

#define CHEMOTAXIS_LEN 8
#define NOSE_TOUCH_LEN 10

#include <stdint.h>

extern "C" {
#include "utility/defines.h"
}

extern const uint16_t NOSE_TOUCH[];
extern const uint16_t CHEMOTAXIS[];

#endif
