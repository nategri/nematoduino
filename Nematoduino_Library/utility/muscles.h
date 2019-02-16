#ifndef MUSCLES_H
#define MUSCLES_H

#include <stdint.h>

#include "defines.h"

extern const uint16_t LARGE_CONST_ARR left_neck_muscle[];
extern const uint16_t LARGE_CONST_ARR right_neck_muscle[];

extern const uint16_t LARGE_CONST_ARR left_body_muscle[];
extern const uint16_t LARGE_CONST_ARR right_body_muscle[];

extern const uint16_t LARGE_CONST_ARR motor_neuron_b[];
extern const uint16_t LARGE_CONST_ARR motor_neuron_a[];

// Significant motor neurons (e.g. ones that are good indicators for locomotion direction)
extern const uint16_t LARGE_CONST_ARR sig_motor_neuron_b[];
extern const uint16_t LARGE_CONST_ARR sig_motor_neuron_a[];

#endif
