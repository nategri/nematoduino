#include <avr/pgmspace.h>

#include "defines.h"
#include "neural_rom.h"

// Two state arrays
int8_t S0[N_TOTAL];
int8_t S1[N_TOTAL];

// Pointers to forward and back arrays
int8_t* CurrState = S0;
int8_t* NextState = S1;

// Array to track activations of neurons
uint8_t NeuralPings[N_TOTAL];

struct NeuralConnection {
  uint8_t id;
  int8_t weight;
};

NeuralConnection ParseROM(uint16_t romWord) {
  uint8_t* romByte;
  romByte = (uint8_t*)&romWord;

  uint16_t id = (uint16_t)romByte[1] + ((romByte[0] & 0b10000000) << 1);
  
  uint8_t weightBits = romByte[0] & 0b01111111;
  weightBits = weightBits + ((weightBits & 0b01000000) << 1);
  int8_t weight = (int8_t) weightBits;

  NeuralConnection neuralConn = {id, weight};

  //Serial.println(romWord, HEX);
  //Serial.println(romByte[1], HEX);
  //Serial.println(romByte[0], HEX);
  Serial.println(id);
  Serial.println(weight);
  Serial.println(romByte[0] & 0b10000000);
  Serial.println();

  return neuralConn;
}

void PingNeuron(uint16_t N_ID) {
  uint16_t address = pgm_read_word_near(NeuralROM+N_ID);
  uint16_t len = pgm_read_word_near(NeuralROM+N_ID+1) - pgm_read_word_near(NeuralROM+N_ID);
  
  //Serial.println(address);
  //Serial.println(len);
  //Serial.println();
  
  for(int i = 0; i<len; i++) {
    NeuralConnection neuralConn = ParseROM(pgm_read_word_near(NeuralROM+address+i));
    NextState[neuralConn.id] += neuralConn.weight; 
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize state arrays
  memset(S0, 0, sizeof(S0));
  memset(S1, 0, sizeof(S1));

  PingNeuron(N_AVL);

  //Serial.print(NeuralROM[15], HEX);

  /*for(int i = 0; i<N_TOTAL; i++) {
    Serial.print(i);
    Serial.println("");
    Serial.print(NextState[i]);
    Serial.println("");
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:

}
