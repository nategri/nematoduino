#include <avr/pgmspace.h>
#include <EEPROM.h>

#include "motors.h"
#include "sensor.h"

#include "defines.h"
#include "neural_rom.h"
#include "muscles.h"

//#include "led_shield.h"

#include "calibration.h"

//
// Global constants
//

// Total number of connected neurons (first word in ROM)
uint16_t const N_MAX = (uint16_t)NeuralROM[0];

// Running average of activity for 'significant' motor neurons
float SigMotorNeuronAvg = 1.0;

// Global object for LED control
//LedShield* screen;

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
    if(val > 127) {
      NextConnectedState[N_ID] = 127;
    }
    else if(val < -128) {
      NextConnectedState[N_ID] = -128;
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
    if (GetCurrState(i) > N_THRESHOLD) {
      DischargeNeuron(i);
    }
  }

  ActivateMuscles();
  HandleIdleNeurons();
  CopyStates();

  //screen->showMotorNeurons(CurrConnectedState);
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
  uint16_t bodyTotal = 0;

  // Gather totals on left and right side muscles
  for(int i = 0; i < N_NBODYMUSCLES; i++) {
    uint16_t leftId = pgm_read_word_near(LeftBodyMuscles+i);
    uint16_t rightId = pgm_read_word_near(RightBodyMuscles+i);

    int16_t leftVal = GetNextState(leftId);
    int16_t rightVal = GetNextState(rightId);

    if(leftVal < 0) {
      leftVal = 0;
    }
    if(rightVal < 0) {
      rightVal = 0;
    }

    bodyTotal += (leftVal + rightVal);

    SetNextState(leftId, 0.0);
    SetNextState(rightId, 0.0);
  }

  // Gather total for neck muscles
  uint16_t leftNeckTotal = 0;
  uint16_t rightNeckTotal = 0;
  for(int i = 0; i < N_NNECKMUSCLES; i++) {
    //uint16_t leftId = LeftBodyMuscles[i];
    //uint16_t rightId = RightBodyMuscles[i]; 

    uint16_t leftId = pgm_read_word_near(LeftNeckMuscles+i);
    uint16_t rightId = pgm_read_word_near(RightNeckMuscles+i);

    int16_t leftVal = GetNextState(leftId);
    int16_t rightVal = GetNextState(rightId);

    if(leftVal < 0) {
      leftVal = 0;
    }
    if(rightVal < 0) {
      rightVal = 0;
    }

    leftNeckTotal += leftVal;
    rightNeckTotal += rightVal;

    SetNextState(leftId, 0.0);
    SetNextState(rightId, 0.0);
  }


  uint16_t normBodyTotal = 255.0 * ((float) bodyTotal) / 600.0;
  //Serial.println(normBodyTotal);

  // Log A and B type motor neuron activity
  uint8_t motorNeuronASum = 0;
  uint8_t motorNeuronBSum = 0;

  for(int i = 0; i < N_SIGMOTORB; i++) {
    uint8_t motorBId = pgm_read_word_near(SigMotorNeuronsB+i);
    if(GetCurrState(motorBId) > N_THRESHOLD) {
      motorNeuronBSum += 1;
    }
  }

  for(int i = 0; i < N_SIGMOTORA; i++) {
    uint8_t motorAId = pgm_read_word_near(SigMotorNeuronsA+i);
    if(GetCurrState(motorAId) > N_THRESHOLD) {
      motorNeuronASum += 1;
    }
  }

  // Plot on the shield
  //screen->plotSigMotorNeurons(motorNeuronASum, motorNeuronBSum);
  
  // Sum (with weights) and add contribution to running average of significant activity
  float motorNeuronSumTotal = (-1*motorNeuronASum) + motorNeuronBSum;

  SigMotorNeuronAvg = (motorNeuronSumTotal + (5.0*SigMotorNeuronAvg))/(5.0 + 1.0);

  // Set left and right totals, scale neck muscle contribution
  int32_t leftTotal = (6*leftNeckTotal) + normBodyTotal;
  int32_t rightTotal = (6*rightNeckTotal) + normBodyTotal;

  // Re-normalize for appropriate signal to motors
  int32_t normLeftTotal = ((float) leftTotal) / 1.5;
  int32_t normRightTotal = ((float) rightTotal) / 1.5;

  //Serial.println(4*leftNeckTotal);
  //Serial.println(4*rightNeckTotal);
  //Serial.println();

  if(SigMotorNeuronAvg < 0.27) { // Magic number read off from c_matoduino simulation
    RunMotors(-1*normRightTotal, -1*normLeftTotal);
    //RunMotorsPID(-1*normLeftTotal, -1*normRightTotal, 1000);
  }
  else {
    RunMotors(normRightTotal, normLeftTotal);
    //RunMotorsPID(normLeftTotal, normRightTotal, 1000);
  }

  //screen->showMuscles(leftTotal, rightTotal, bodyTotal);
  
  delay(180);
}

//
// Standard Arduino setup and loop functions
//

void setup() {
  // put your setup code here, to run once:
  
  //Uncomment for serial debugging
  Serial.begin(9600);

  Serial.println("Hello");

  /*CalibrateMotor(&leftMotorForwardCal);
  delay(100000);*/

  // Initialize motors
  //MotorsInitPID();

  //RunMotors(255, 255);
  //delay(10);
  //MotorSimple(80);
  //delay(100000);

  InitCalibrationJumper();
  Serial.println("Cal");
  PerformMotorCalibration();
  LoadMotorCalibration();

  MotorsInit();

  uint8_t leftSpd = CalFunction(&leftMotorForwardCal, 9.0);
  uint8_t rightSpd = CalFunction(&rightMotorForwardCal, 9.0);
  Serial.println(leftSpd);
  Serial.println(rightSpd);

  leftMotorForward(leftSpd);
  rightMotorForward(rightSpd);
  //leftMotorForward(100);
  //rightMotorForward(100);

  //MeasureMotor(&leftMotorForwardCal, leftSpd);
  //MeasureMotor(&rightMotorForwardCal, rightSpd);

  delay(100000);

  /*if(CalibrationJumperPresent()) {
    Serial.println("Cal");
    PerformMotorCalibration();
  }
  else {
    Serial.println("No cal");
    LoadMotorCalibration();
  }*/

  //screen = new LedShield();

  // initialize state arrays
  StatesInit();

  // Initialize sensor
  SensorInit();

  // Uses EEPROM to implement reset switch as
  // toggle for running/not running
  if (EEPROM.read(0x0) == 0) {
    EEPROM.write(0x0, 1);
    while(true) {
      delay(100);
    }
  }
  else {
    EEPROM.write(0x0, 0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  long dist = SensorDistance();
  //long dist = 30.0;

  if(dist < 25.0) {
    //screen->oneLedOn();
        
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
    //screen->oneLedOff();
    
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
