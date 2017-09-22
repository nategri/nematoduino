#include "defines.h"

const uint16_t PROGMEM LeftNeckMuscles[] = {
N_MDL05, N_MDL06, N_MDL07, N_MDL08, N_MVL05, N_MVL06, N_MVL07, N_MVL08
};

const uint16_t PROGMEM RightNeckMuscles[] = {
N_MDR05, N_MDR06, N_MDR07, N_MDR08, N_MVR05, N_MVR06, N_MVR07, N_MVR08
};

const uint16_t PROGMEM LeftBodyMuscles[] = {
N_MDL09, N_MDL10, N_MDL11, N_MDL12, N_MDL13, N_MDL14, N_MDL15,
N_MDL16, N_MDL17, N_MDL18, N_MDL19, N_MDL20, N_MDL21, N_MDL22, N_MDL23,
N_MVL09, N_MVL10, N_MVL11, N_MVL12, N_MVL13, N_MVL14, N_MVL15,
N_MVL16, N_MVL17, N_MVL18, N_MVL19, N_MVL20, N_MVL21, N_MVL22, N_MVL23
};

const uint16_t PROGMEM RightBodyMuscles[] = {
N_MDR09, N_MDR10, N_MDR11, N_MDR12, N_MDR13, N_MDR14, N_MDR15,
N_MDR16, N_MDR17, N_MDR18, N_MDR19, N_MDR20, N_MDR21, N_MDR22, N_MDR23,
N_MVR09, N_MVR10, N_MVR11, N_MVR12, N_MVR13, N_MVR14, N_MVR15,
N_MVR16, N_MVR17, N_MVR18, N_MVR19, N_MVR20, N_MVR21, N_MVR22, N_MVR23
};

/*
const uint16_t PROGMEM MotorNeuronsB[] = {
N_DB1, N_DB2, N_DB3, N_DB4, N_DB5, N_DB6, N_DB7, N_VB1, N_VB2, N_VB3, N_VB4,
N_VB5, N_VB6, N_VB7, N_VB8, N_VB9, N_VB10, N_VB11
};

const uint16_t PROGMEM MotorNeuronsA[] = {
N_DA1, N_DA2, N_DA3, N_DA4, N_DA5, N_DA6, N_DA7, N_DA8, N_DA9,
N_VA1, N_VA2, N_VA3, N_VA4, N_VA5, N_VA6, N_VA7, N_VA8, N_VA9, N_VA10, N_VA11,
N_VA12
};
*/

// Significant motor neurons (e.g. ones that are good indicators for locomotion direction)
const uint16_t PROGMEM SigMotorNeuronsB[] = {
N_VB2, N_VB3, N_VB4, N_VB5, N_VB6
};

const uint16_t PROGMEM SigMotorNeuronsA[] = {
N_VA1, N_VA2
};

