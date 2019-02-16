#ifndef NEURALROM_H
#define NEURALROM_H

#include <stdint.h>

#include "defines.h"

// Word 0: Number of connected neurons (N_MAX)
// Word 1-(N_MAX): A address (offset from beginning) of each list of connections
// Word N_MAX+1: N_MAX plus the number of connections in the final neuron
// Word > (N_MAX+1): List of connections

// Connection words are formatted as follows:
// Bit 0-7: Lower part of neuron index
// Bit 8: High part of neuron index (9-bits reserved for index)
// Bit 9-15: Weight of connection

extern const uint16_t LARGE_CONST_ARR NEURAL_ROM[];

#endif
