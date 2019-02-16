#ifndef CONNECTOME_H
#define CONNECTOME_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "neural_rom.h"

//
// Struct that contains cell states
//
// Simulation projects current state in next,
// then copies next into current and repeats
//

typedef struct {
  // Point to _neuron_current
  // and _muscle_current arrays
  int8_t* neuron_state;
  int16_t* muscle_state;

  // Total number of neuron type cells
  uint16_t _neurons_tot;
  
  // Total number of muscle type cells
  uint8_t _muscles_tot;

  // Current state
  int8_t* _neuron_current;
  int16_t* _muscle_current;

  // Next state
  int8_t* _neuron_next;
  int16_t* _muscle_next;

  // Meta array holds information about how long a neuron
  // has been idle in the lower 7 bits (i.e. how many
  // ticks since it last discharged) and whether or not it
  // discharged this tick in the high bit
  uint8_t* _meta;

} Connectome;

//
// Functions that provide primary interface to
// connectome emulation
//

// Function for initializing connectome struct
void ctm_init(Connectome* const);

// Propagates each neuron connection weight into the next state
void ctm_ping_neuron(Connectome* const, const uint16_t);

// Propagates connections and sets state to zero, simulating
// a neuron discharge
void ctm_discharge_neuron(Connectome* const, const uint16_t);

// Completes one cycle ('tick') of the nematode neural system
// accepts an array of neurons to stimulate and the length of
// that list---otherwise NULL, 0
void ctm_neural_cycle(Connectome* const, const uint16_t*, const uint16_t);

// Utility functions

// Functions for returning cell weights
int16_t ctm_get_weight(Connectome* const, const uint16_t);
void ctm_weight_query(Connectome* const, const uint16_t*, uint16_t*, const uint16_t);

// Check whether or not one or more neurons discharged 
// in the last tick
uint8_t ctm_get_discharge(Connectome* const, const uint16_t);
void ctm_discharge_query(Connectome* const, const uint16_t*, uint8_t*, const uint16_t);

#endif
