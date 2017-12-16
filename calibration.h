#include <EEPROM.h>

#include "pin_config.h"

#include <math.h>
#include "Statistic.h"

#define START_SPD 35
#define STOP_SPD 255

#define MAX_TRIES 5
#define MAX_DATA 26

#define JUMPER_PIN 8

// Global object for statistics calcs
Statistic stats;

typedef struct {
  void (*runMotor)(uint8_t);
  float aParm;
  float bParm;
} MotorCal;

MotorCal leftMotorForwardCal = {leftMotorForward, 0, 0};
MotorCal leftMotorBackwardCal = {leftMotorBackward, 0, 0};
MotorCal rightMotorForwardCal = {rightMotorForward, 0, 0};
MotorCal rightMotorBackwardCal = {rightMotorBackward, 0, 0};

MotorCal* motorCalArr[] = {
                             &leftMotorForwardCal, &leftMotorBackwardCal,
                             &rightMotorForwardCal, &rightMotorBackwardCal
                          };

void InitCalibrationJumper() {
  pinMode(JUMPER_PIN, INPUT_PULLUP);
}

bool CalibrationJumperPresent() {
  if(digitalRead(JUMPER_PIN) == LOW) {
    return true;
  }
  else {
    return false;
  }
}


float average(double* a) {
  float sum = 0;
  uint8_t n = 0;

  while(a[n] != 0) {
    sum += a[n];
    n++;
  }

  return sum / n;
}

void linear_regression(MotorCal* motor, double* x, double* y) {
  Serial.println("wtf");

  uint8_t idx;

  // Convert to log
  idx = 0;
  while(x[idx] != 0) {
    x[idx] = log(x[idx]);
    idx++;
  }

  // Compute averages
  float xAvg = average(x);
  float yAvg = average(y);

  // Perform linear regression
  float top = 0;
  float bottom = 0;
  idx = 0;
  while((x[idx] != 0) && (y[idx] != 0)) {
    top += (x[idx] - xAvg)*(y[idx] - yAvg);
    bottom += (x[idx] - xAvg)*(x[idx] - xAvg);
    idx++;
  }

  motor->bParm = top / bottom;
  motor->aParm = yAvg - motor->bParm*xAvg;

  Serial.println("Constants");
  Serial.println(top/bottom);
  Serial.println(yAvg - motor->bParm*xAvg);
}



void CalibrateMotorsInit() {
  pinMode(ROT_ENCODER, INPUT);

  MotorsInit();
}

void CalibrateMotor(MotorCal* motor) {
  double avg;
  double sigma;

  double x[MAX_DATA] = { 0 };
  double y[MAX_DATA] = { 0 };

  CalibrateMotorsInit();

  //Serial.begin(9600);

  // Variables used to determine if motor actually running
  uint8_t tries;
  float pulseSum;

  // Loop through motor speeds
  int idx = 0; // Index for data array
  for(int spd=START_SPD; spd<=STOP_SPD; spd+=10) {
    tries = 0;
    pulseSum = 0;
    // Run motor until sufficcient data quality is achieved
    motor->runMotor(spd);
    delay(500);
    while(true) {

      double pulseWidth = pulseIn(ROT_ENCODER, HIGH);
      double freq = (1.0 / (pulseWidth/1.0e6))/20.0;

      avg = stats.average();
      sigma = stats.pop_stdev();

      if(stats.count() > 100) {
        if(sigma < 0.12*avg) {
          // Success!
          // Record data
          x[idx] = spd;
          y[idx] = avg;
          idx++;
          Serial.print(spd);
          Serial.print(" ");
          Serial.print(avg);
          Serial.print("\n");
          break;
        }
        else if(abs(freq-avg) < 3*sigma) {
          stats.add(freq);
        }
      }
      else if(freq < 5e3) {
        stats.add(freq);
      }

      // Track tries
      pulseSum += pulseWidth;
      //Serial.println(pulseSum);
      tries++;
      // Move on to next speed if it's not moving after MAX_TRIES
      if((tries==MAX_TRIES) && (pulseSum == 0)) {
        Serial.println("break triggered");
        break;
      }
    }
    MotorsOff();
    stats.clear();
    delay(2000);
  }

  Serial.println("Shouldnt see me");

  // Perform linear regression
  linear_regression(motor, x, y);
}

void MeasureMotor(MotorCal* motor, uint8_t spd) {
  double avg;
  double sigma;

  motor->runMotor(spd);
  delay(2000);
  while(true) {

    double pulseWidth = pulseIn(ROT_ENCODER, HIGH);
    double freq = (1.0 / (pulseWidth/1.0e6))/20.0;

    avg = stats.average();
    sigma = stats.pop_stdev();


    if(stats.count() > 100) {
      if(sigma < 0.12*avg) {
        // Success!
        // Record data
        Serial.print(spd);
        Serial.print(" ");
        Serial.print(avg);
        Serial.print("\n");
        Serial.print("\n");
        break;
      }
      else if(abs(freq-avg) < 3*sigma) {
        Serial.println(freq);
        stats.add(freq);
      }
    }
    else if(freq < 5e3) {
      Serial.println(freq);
      stats.add(freq);
    }
  }

  MotorsOff();
  stats.clear();
  delay(2000);
}

void PerformMotorCalibration() {
  for(uint8_t i=0; i<4; i++) {
    CalibrateMotor(motorCalArr[i]);
    EEPROM.put(0x1+(8*i), motorCalArr[i]->aParm);
    EEPROM.put(0x1+(8*i)+4, motorCalArr[i]->bParm);
    if(i==1) {
      tone(BUZZER, 440, 500);
      delay(250);
      tone(BUZZER, 440, 500);
      delay(15000);
    }
  }
}

void LoadMotorCalibration() {
  for(uint8_t i=0; i<4; i++) {
    EEPROM.get(0x1+(8*i), motorCalArr[i]->aParm);
    EEPROM.get(0x1+(8*i)+4, motorCalArr[i]->bParm);
    Serial.println(motorCalArr[i]->aParm);
    Serial.println(motorCalArr[i]->bParm);
  }
}

uint8_t CalFunction(MotorCal* motor, float y) {
  float A = motor->aParm;
  float B = motor->bParm;

  return (uint8_t) (exp((y - A) / B) + 0.5);
}
