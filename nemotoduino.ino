#include <avr/pgmspace.h>

#include "defines.h"
#include "neural_rom.h"
#include "muscles.h"

// Total number of connected neurons (first word in ROM)
uint16_t const N_MAX = (uint16_t)NeuralROM[0];

struct NeuralConnection {
  uint16_t id;
  int8_t weight;
};

// Two sets of state arrays

// One set for neurons that are connected to others
int8_t CurrConnectedState[N_NTOTAL];
int8_t NextConnectedState[N_NTOTAL];

// Another set for muscles that aren't connected to other cells
int16_t* CurrMuscleState = malloc((N_NTOTAL-N_MAX)*sizeof(int16_t));
int16_t* NextMuscleState = malloc((N_NTOTAL-N_MAX)*sizeof(int16_t));

// Functions for getting and setting these states
void InitStates() {
  memset(CurrConnectedState, 0, sizeof(CurrConnectedState));
  memset(NextConnectedState, 0, sizeof(NextConnectedState));
  memset(CurrMuscleState, 0, (N_NTOTAL-N_MAX)*sizeof(CurrMuscleState[0]));
  memset(NextMuscleState, 0, (N_NTOTAL-N_MAX)*sizeof(NextMuscleState[0]));
}

void SetCurrState(uint16_t N_ID, int8_t val) {
  if(N_ID < N_MAX) {
    CurrConnectedState[N_ID] = val;
  }
  else {
    CurrMuscleState[N_ID-N_MAX] = val;
  }
}

int16_t GetCurrState(uint16_t N_ID) {
  if(N_ID < N_MAX) {
    return CurrConnectedState[N_ID];
  }
  else {
    return CurrMuscleState[N_ID-N_MAX];
  }
}

void SetNextState(uint16_t N_ID, int16_t val) {
  if(N_ID < N_MAX) {
    if(val > N_THRESHOLD) {
      NextConnectedState[N_ID] = N_THRESHOLD+1;
    }
    else if(val < (-1*N_THRESHOLD)) {
      NextConnectedState[N_ID] = -1*N_THRESHOLD-1;
    }
    else {
      NextConnectedState[N_ID] = val;
    }
    
  }
  else {
    NextMuscleState[N_ID-N_MAX] = val;
  }
}

int16_t GetNextState(uint16_t N_ID) {
  if(N_ID < N_MAX) {
    return NextConnectedState[N_ID];
  }
  else {
    return NextMuscleState[N_ID-N_MAX];
  }
}

void AddToNextState(uint16_t N_ID, int8_t val) {
  int16_t currVal = GetNextState(N_ID);
  SetNextState(N_ID, currVal + val);
}

void CopyStates() {
  memcpy(CurrConnectedState, NextConnectedState, sizeof(NextConnectedState));
  memcpy(CurrMuscleState, NextMuscleState, (N_NTOTAL-N_MAX)*sizeof(NextMuscleState[0]));
}

// Array to track how many cycles neurons have been inactive
uint8_t NumCyclesInactive[N_NTOTAL];

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

  //Serial.println(len);
  
  for(int i = 0; i<len; i++) {
    NeuralConnection neuralConn = ParseROM(pgm_read_word_near(NeuralROM+address+i));

    //if (fabs(NextState[neuralConn.id]) <= N_THRESHOLD) {
      //NextState[neuralConn.id] += neuralConn.weight;
    //}
    //NextState[neuralConn.id] += neuralConn.weight;

    AddToNextState(neuralConn.id, neuralConn.weight);

    /*if(fabs(GetNextState(neuralConn.id)) > 128.0) {
      Serial.println(neuralConn.id);
      Serial.println(GetNextState(neuralConn.id));
      Serial.println();
    }*/
    
  }
}

void DischargeNeuron(uint16_t N_ID) {
  PingNeuron(N_ID);
  SetNextState(N_ID, 0.0);
}

void NeuralCycle() {
  for(int i = 0; i < N_MAX; i++) {
    if (fabs(GetCurrState(i)) > N_THRESHOLD) {
      DischargeNeuron(i);
    }
  }

  ActivateMuscles();
  
  CopyStates();
}

void ActivateMuscles() {
  int32_t leftTotal = 0;
  int32_t rightTotal = 0;
  
  for(int i = 0; i < N_NMUSCLES; i++) {
    uint16_t leftId = pgm_read_word_near(LeftMuscles+i);
    uint16_t rightId = pgm_read_word_near(RightMuscles+i); 

    leftTotal += GetNextState(leftId);
    rightTotal += GetNextState(rightId);

    SetNextState(leftId, 0.0);
    SetNextState(rightId, 0.0);
  }

  Serial.println(rightTotal);
  Serial.println(leftTotal);
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize state arrays
  InitStates();
  
  // Test chemotaxis neurons
  for(int i = 0; i < 100; i++) {
    
    PingNeuron(N_ADFL);
    PingNeuron(N_ADFR);
    PingNeuron(N_ASGR);
    PingNeuron(N_ASGL);
    PingNeuron(N_ASIL);
    PingNeuron(N_ASIR);
    PingNeuron(N_ASJR);
    PingNeuron(N_ASJL);
    
    NeuralCycle();
  }
  Serial.println("Done");

  /*for(int i=0; i < N_NTOTAL; i++) {
    Serial.println(i);
    Serial.println(GetCurrState(i));
    Serial.println();
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:


}
