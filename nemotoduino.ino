#include <avr/pgmspace.h>

#include "motors.h"
#include "sensor.h"

#include "defines.h"
#include "neural_rom.h"
#include "muscles.h"

#include "status_led.h"
#include "button.h"

//
// Global constants
//

// Total number of connected neurons (first word in ROM)
uint16_t const N_MAX = (uint16_t)NeuralROM[0];

//
// Structs
//

// Struct for representing a neuron connection
struct NeuralConnection {
  uint16_t id;
  int8_t weight;
};

//
// Three sets of neural state arrays
//

// One set for neurons that are connected to others
int8_t CurrConnectedState[N_NTOTAL];
int8_t NextConnectedState[N_NTOTAL];

// Another set for muscles that aren't connected to other cells
int16_t* CurrMuscleState = malloc((N_NTOTAL-N_MAX)*sizeof(int16_t));
int16_t* NextMuscleState = malloc((N_NTOTAL-N_MAX)*sizeof(int16_t));

// Final set to track how many cycles a neuron has been idle
uint8_t* IdleCycles = malloc(N_MAX*sizeof(uint8_t));

//
// Functions for getting and setting these states
//

void StatesInit() {
  memset(CurrConnectedState, 0, sizeof(CurrConnectedState));
  memset(NextConnectedState, 0, sizeof(NextConnectedState));
  memset(CurrMuscleState, 0, (N_NTOTAL-N_MAX)*sizeof(CurrMuscleState[0]));
  memset(NextMuscleState, 0, (N_NTOTAL-N_MAX)*sizeof(NextMuscleState[0]));

  memset(IdleCycles, 0, N_MAX*sizeof(NextConnectedState[0]));
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

// Copy 'next' state into 'current' state
void CopyStates() {
  memcpy(CurrConnectedState, NextConnectedState, sizeof(NextConnectedState));
  memcpy(CurrMuscleState, NextMuscleState, (N_NTOTAL-N_MAX)*sizeof(NextMuscleState[0]));
}

//
// Functions for handling connectome simulation
//

// Parse a word of the ROM into a neuron id and connection weight
NeuralConnection ParseROM(uint16_t romWord) {
  uint8_t* romByte;
  romByte = (uint8_t*)&romWord;

  uint16_t id = romByte[1] + ((romByte[0] & 0b10000000) << 1);
  
  uint8_t weightBits = romByte[0] & 0b01111111;
  weightBits = weightBits + ((weightBits & 0b01000000) << 1);
  int8_t weight = (int8_t)weightBits;

  NeuralConnection neuralConn = {id, weight};

  return neuralConn;
}

// Propagate each neuron connection weight into the next state
void PingNeuron(uint16_t N_ID) {
  uint16_t address = pgm_read_word_near(NeuralROM+N_ID+1);
  uint16_t len = pgm_read_word_near(NeuralROM+N_ID+1+1) - pgm_read_word_near(NeuralROM+N_ID+1);

  //Serial.println(len);
  
  for(int i = 0; i<len; i++) {
    NeuralConnection neuralConn = ParseROM(pgm_read_word_near(NeuralROM+address+i));

    AddToNextState(neuralConn.id, neuralConn.weight);
  }
}

void DischargeNeuron(uint16_t N_ID) {
  PingNeuron(N_ID);
  SetNextState(N_ID, 0);
}

// Complete one cycle ('tick') of the nematode neural system
void NeuralCycle() {
  for(int i = 0; i < N_MAX; i++) {
    if (fabs(GetCurrState(i)) > N_THRESHOLD) {
      DischargeNeuron(i);
    }
  }

  ActivateMuscles();
  HandleIdleNeurons();
  CopyStates();
}

// Flush neurons that have been idle for a while
void HandleIdleNeurons() {
  for(int i = 0; i < N_MAX; i++) {
    if(GetNextState(i) == GetCurrState(i)) {
      IdleCycles[i] += 1;
    }
    if(IdleCycles[i] > 10) {
      SetNextState(i, 0);
      IdleCycles[i] = 0;
    }
  }
}

//
// Function for determinining how muscle weights map to motors
//

void ActivateMuscles() {
  int32_t leftTotal = 0;
  int32_t rightTotal = 0;

  // Gather totals on left and right side muscles
  for(int i = 0; i < N_NMUSCLES; i++) {
    uint16_t leftId = pgm_read_word_near(LeftMuscles+i);
    uint16_t rightId = pgm_read_word_near(RightMuscles+i); 

    leftTotal += GetNextState(leftId);
    rightTotal += GetNextState(rightId);

    SetNextState(leftId, 0.0);
    SetNextState(rightId, 0.0);
  }

  int8_t motorNeuronASum = 0;
  int8_t motorNeuronBSum = 0;

  for(int i = 0; i < N_MOTORB; i++) {
    uint8_t motorBId = pgm_read_word_near(MotorNeuronsB+i);
    if(GetCurrState(motorBId) > N_THRESHOLD) {
      motorNeuronBSum += 1;
    }
  }

  for(int i = 0; i < N_MOTORA; i++) {
    uint8_t motorAId = pgm_read_word_near(MotorNeuronsA+i);
    if(GetCurrState(motorAId) > N_THRESHOLD) {
      motorNeuronASum += 1;
    }
  }

  // Set speed for the motors
  //uint16_t muscleTotal = abs(leftTotal) + abs(rightTotal);

  Serial.println(motorNeuronBSum);
  Serial.println(motorNeuronASum);
  Serial.println();

  //float motorNeuronRatio = ((float) motorNeuronASum) / ((float) motorNeuronBSum);

  if(motorNeuronASum >= (2.7*motorNeuronBSum)) {
    RunMotors(-1*rightTotal, -1*leftTotal);
  }
  else {
    RunMotors(rightTotal, leftTotal);
  }
  delay(20);
}

//
// Standard Arduino setup and loop functions
//

void setup() {
  // put your setup code here, to run once:

  
  //Uncomment for serial debugging
  Serial.begin(9600);
  

  // initialize state arrays
  StatesInit();

  // Initialize motors
  MotorsInit();

  // Initialize sensor
  SensorInit();

  // Initialize status LED
  StatusLedInit();

  // Initialize button
  ButtonInit();

  // Loop until pressed
  while(true) {
    if(ButtonPress()) {
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  long dist = SensorDistance();

  if(dist < 50.0) {
    // Status LED on
    StatusLedOn();
    
    // Nose touch neurons
    PingNeuron(N_FLPR);
    PingNeuron(N_FLPL);
    PingNeuron(N_ASHL);
    PingNeuron(N_ASHR);
    PingNeuron(N_IL1VL);
    PingNeuron(N_IL1VR);
    PingNeuron(N_OLQDL);
    PingNeuron(N_OLQDR);
    PingNeuron(N_OLQVR);
    PingNeuron(N_OLQVL);
    NeuralCycle();
  }
  else {
    // Status LED off
    StatusLedOff();
    
    // Chemotaxis neurons
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
}
