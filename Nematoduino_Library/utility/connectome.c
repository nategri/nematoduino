#include <stdio.h>
#include "connectome.h"

//
// Utilities for parsing a ROM word into a connection
// with an id and weight
//

// Struct for representing a neuron connection
typedef struct {
  uint16_t id;
  int8_t weight;
} NeuronConnection;

static const NeuronConnection parse_rom_word(uint16_t rom_word) {
  uint8_t* rom_byte;
  rom_byte = (uint8_t*)&rom_word;

  uint16_t id = rom_byte[1] + ((rom_byte[0] & 0b10000000) << 1);
  
  uint8_t weight_bits = rom_byte[0] & 0b01111111;
  weight_bits = weight_bits + ((weight_bits & 0b01000000) << 1);
  int8_t weight = (int8_t)weight_bits;

  NeuronConnection neuron_conn = {id, weight};

  return neuron_conn;
}

//
// Getter and setter type functions for interfacing with
// 'current' and 'next' states
//

static int16_t ctm_get_current_state(Connectome* const c, const uint16_t id) {
  if(id < c->_neurons_tot) {
    return c->_neuron_current[id];
  }
  else {
    return c->_muscle_current[id - c->_neurons_tot];
  }
}

static void ctm_set_next_state(Connectome* const c, const uint16_t id, const int16_t val) {
  if(id < c->_neurons_tot) {
    if(val > 127) {
      c->_neuron_next[id] = 127;
    }
    else if(val < -128) {
      c->_neuron_next[id] = -128;
    }
    else {
      c->_neuron_next[id] = val;
    }
    
  }
  else {
    c->_muscle_next[id - c->_neurons_tot] = val;
  }
}

static int16_t ctm_get_next_state(Connectome* const c, const uint16_t id) {
  if(id < c->_neurons_tot) {
    return c->_neuron_next[id];
  }
  else {
    return c->_muscle_next[id - c->_neurons_tot];
  }
}

static void ctm_add_to_next_state(Connectome* const c, const uint16_t id, const int8_t val) {
  int16_t curr_val = ctm_get_next_state(c, id);
  ctm_set_next_state(c, id, curr_val + val);
}

// Copy 'next' state into 'current' state,
// flush the muscles in 'next' state
static void ctm_iterate_state(Connectome* const c) {
  memcpy(c->_neuron_current, c->_neuron_next, c->_neurons_tot*sizeof(c->_neuron_next[0]));
  memcpy(c->_muscle_current, c->_muscle_next, c->_muscles_tot*sizeof(c->_muscle_next[0]));

  memset(c->_muscle_next, 0, c->_muscles_tot*sizeof(c->_muscle_next[0]));
}

//
// Functions for handling meta/logging type information
//

// Set flag in meta array to indicate if neuron discharged
static void ctm_meta_flag_discharge(Connectome* const c, const uint16_t id, const uint8_t val) {
  if(val == 0) {
    c->_meta[id] = 0b01111111 & c->_meta[id];
  }
  else if(val == 1) {
    //c->_meta[id] = 0b10000000 | c->_meta[id];
    c->_meta[id] = 0b10000000;
  }
}

// Flush neurons that have been idle for a while
static void ctm_meta_handle_idle_neurons(Connectome* const c) {
  // _meta array high bit indicates whether neuron discharged
  // during previous tick, lower seven give number of ticks
  // that neuron was idle
  for(uint16_t i = 0; i < c->_neurons_tot; i++) {
    uint8_t low_val = c->_meta[i] & 0b01111111;
    uint8_t high_val = c->_meta[i] & 0b10000000;

    uint8_t idle_ticks = low_val;

    if(ctm_get_next_state(c, i) == ctm_get_current_state(c, i)) {
      // Doesnt matter if high bit is set as long as MAX_IDLE < 127
      c->_meta[i] += 1;
      idle_ticks += 1;
    }
    else {
      c->_meta[i] = high_val;
    }

    if(idle_ticks > MAX_IDLE) {
      ctm_set_next_state(c, i, 0);
      // Set number of idle ticks to zero (i.e. only preserve high bit)
      c->_meta[i] = high_val;
    }
  }
}

//
// Functions that provide primary interface to
// connectome emulation
//

// Function for initializing connectome struct
void ctm_init(Connectome* const c) {
  // Set number of neuron type cells
  c->_neurons_tot = (uint16_t)NEURAL_ROM[0];
  c->_muscles_tot = (uint8_t)(CELLS - c->_neurons_tot);

  // Allocate neuron state arrays
  c->_neuron_current = malloc(c->_neurons_tot*sizeof(int8_t));
  c->_neuron_next = malloc(c->_neurons_tot*sizeof(int8_t));

  // Allocate muscle state arrays
  c->_muscle_current = malloc(c->_muscles_tot*sizeof(int16_t));
  c->_muscle_next = malloc(c->_muscles_tot*sizeof(int16_t));

  // Allocate metastate array
  c->_meta = malloc(c->_neurons_tot*sizeof(uint8_t));

  // Set up pointers for public interface members
  c->neuron_state = c->_neuron_current;
  c->muscle_state = c->_muscle_current;

  // Initialize arrays to zero
  memset(c->_neuron_current, 0, c->_neurons_tot*sizeof(c->_neuron_current[0]));
  memset(c->_neuron_next, 0, c->_neurons_tot*sizeof(c->_neuron_next[0]));
  memset(c->_muscle_current, 0, c->_muscles_tot*sizeof(c->_muscle_current[0]));
  memset(c->_muscle_next, 0, c->_muscles_tot*sizeof(c->_muscle_next[0]));
  memset(c->_meta, 0, c->_neurons_tot*sizeof(c->_meta[0]));
}

// Propagate each neuron connection weight into the next state
void ctm_ping_neuron(Connectome* const c, const uint16_t id) {
  const uint16_t address = READ_WORD(NEURAL_ROM, id + 1);
  const uint16_t len = READ_WORD(NEURAL_ROM, id + 2) - READ_WORD(NEURAL_ROM, id + 1);
  
  for(int8_t i = 0; i < len; i++) {
    NeuronConnection neuron_conn = parse_rom_word(READ_WORD(NEURAL_ROM, address + i));

    ctm_add_to_next_state(c, neuron_conn.id, neuron_conn.weight);
  }
}

// Propagate connections and set state to zero (i.e. simulate a neuron
// discharge)
void ctm_discharge_neuron(Connectome* const c, const uint16_t id) {
  ctm_ping_neuron(c, id);
  ctm_set_next_state(c, id, 0);
}

// Complete one cycle ('tick') of the nematode neural system;
// accepts an array of neurons to stimulate and the length of
// that list---otherwise NULL, 0
void ctm_neural_cycle(Connectome* const c, const uint16_t* stim_neuron, const uint16_t len) {

  // Iterate through list of neurons to
  // stimulate this tick, if any
  if(stim_neuron != NULL) {
    for(uint16_t i = 0; i < len; i++) {
      uint16_t id = stim_neuron[i];
      ctm_ping_neuron(c, id);
    }
  }

  // Discharge any neurons over threshold
  for(uint16_t i = 0; i < c->_neurons_tot; i++) {
    if(ctm_get_current_state(c, i) > THRESHOLD) {
      ctm_discharge_neuron(c, i);
      ctm_meta_flag_discharge(c, i, 1);
    }
    else {
      ctm_meta_flag_discharge(c, i, 0);
    }
  }

  ctm_meta_handle_idle_neurons(c);
  ctm_iterate_state(c);
}

// Utility functions

// Functions for returning cell weights
int16_t ctm_get_weight(Connectome* const c, const uint16_t id) {
  int16_t weight = ctm_get_current_state(c, id);

  return weight;
}

void ctm_weight_query(Connectome* const c, const uint16_t* input_id, uint16_t* query_result, const uint16_t len_query) {
  for(uint16_t i = 0; i < len_query; i++) {
    uint16_t id = input_id[i];
    int16_t weight = ctm_get_current_state(c, id);
    query_result[i] = weight;
  }
}

// Check whether or not one or more neurons discharged 
// in the last tick
uint8_t ctm_get_discharge(Connectome* const c, const uint16_t id) {
  uint8_t discharged = c->_meta[id] >> 7;

  return discharged;
}

void ctm_discharge_query(Connectome* const c, const uint16_t* input_id, uint8_t* query_result, const uint16_t len_query) {
  for(uint16_t i = 0; i < len_query; i++) {
    uint16_t id = input_id[i];
    uint8_t discharged = c->_meta[id] >> 7;
    query_result[i] = discharged;
  }
}
