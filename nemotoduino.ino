#include <avr/pgmspace.h>

#include "defines.h"
#include "neural_rom.h"
#include "muscles.h"

// Total number of connected neurons (first word in ROM)
uint16_t N_MAX;

// Two state arrays
int8_t S0[N_TOTAL];
int8_t S1[N_TOTAL];

// Pointers to forward and back arrays
int8_t* CurrState = S0;
int8_t* NextState = S1;

// Array to track how many cycles neurons have been inactive
uint8_t NumCyclesInactive[N_TOTAL];

struct NeuralConnection {
  uint8_t id;
  int8_t weight;
};

NeuralConnection ParseROM(uint16_t romWord) {
  uint8_t* romByte;
  romByte = (uint8_t*)&romWord;

  uint16_t id = romByte[1] + ((romByte[0] & 0b10000000) << 1);
  
  uint8_t weightBits = romByte[0] & 0b01111111;
  weightBits = weightBits + ((weightBits & 0b01000000) << 1);
  int8_t weight = (int8_t)weightBits;

  NeuralConnection neuralConn = {id, weight};

  //Serial.println(id);
  //Serial.println(weight);
  //Serial.println();

  return neuralConn;
}

void PingNeuron(uint16_t N_ID) {
  uint16_t address = pgm_read_word_near(NeuralROM+N_ID+1);
  uint16_t len = pgm_read_word_near(NeuralROM+N_ID+1+1) - pgm_read_word_near(NeuralROM+N_ID+1);
  
  for(int i = 0; i<len; i++) {
    NeuralConnection neuralConn = ParseROM(pgm_read_word_near(NeuralROM+address+i));

    if (NextState[neuralConn.id] <= N_THRESHOLD) {
      NextState[neuralConn.id] += neuralConn.weight;
      //Serial.println(NextState[neuralConn.id]);
    }
    
  }
}

void DischargeNeuron(uint16_t N_ID) {
  PingNeuron(N_ID);
  NextState[N_ID] = 0;
}

void NeuralCycle() {
  for(int i = 0; i < N_MAX; i++) {
    if (CurrState[i] > N_THRESHOLD) {
      DischargeNeuron(i);
    }
  }
  memcpy(CurrState, NextState, N_TOTAL);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize state arrays
  memset(S0, 0, sizeof(S0));
  memset(S1, 0, sizeof(S1));

  N_MAX = (uint16_t)NeuralROM[0];

  PingNeuron(N_AVL);

  NeuralCycle();

  Serial.println("Done");

  // Dump state
  for(int i = 0; i < N_TOTAL; i++) {
    Serial.println(i);
    Serial.println(CurrState[i]);
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
