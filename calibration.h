#include "pin_config.h"

#include <math.h>
#include "Statistic.h"

#define START_SPD 100
#define MAX_TRIES 20
#define MAX_DATA

// Global object for statistics calcs
Statistic stats;

typedef struct {
  uint8_t encoder;
  void (*run)(uint8_t);
  float aParm;
  float bParm;
} MotorCal;

MotorCal leftMotorForwardCal = {L_ENCODER, leftMotorForward, 0, 0};
MotorCal leftMotorBackwardCal = {L_ENCODER, leftMotorBackward, 0, 0};
MotorCal rightMotorForwardCal = {R_ENCODER, rightMotorForward, 0, 0};
MotorCal rightMotorBackwardCal = {R_ENCODER, rightMotorBackward, 0, 0};

/*float average(double* a) {
  float sum = 0;
  uint8_t n = 0;

  double ret;

  while(true) {
    if(a[idx] != 0) {
      sum += a[n];
      n++;
    }
    else {
      ret = sum / n;
    }
  }

  return ret;
}*/

/*float linear_regression(MotocrCal* motor, double* x, double* y) {
  uint8_t idx;

  // Convert to log
  idx = 0;
  while(x[idx] != 0) {
    x[idx] = log(x[idx]);
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
  }*/



void CalibrateMotorsInit() {
  pinMode(L_ENCODER, INPUT);
  pinMode(R_ENCODER, INPUT);

  MotorsInit();
}

/*void CalibrateMotor(MotorCal* motor) {
  double avg;
  double sigma;

  double x[MAX_DATA] = { 0 };
  double y[MAX_DATA] = { 0 };

  CalibrateMotorsInit();

  Serial.begin(9600);

  // Variables used to determine if motor actually running
  uint8_t tries = 0;
  float pulseSum = 0;

  // Loop through motor speeds
  int idx = 0; // Index for data array
  for(int spd=START_SPD; spd<=250; spd+=10) {
    // Run motor until sufficcient data quality is achieved
    while(true) {
      motor->run(spd);

      double pulseWidth = pulseIn(motor->encoder, HIGH, 0.1);
      double freq = (1.0 / (pulseWidth/1.0e6))/20.0;

      avg = stats.average();
      sigma = stats.pop_stdev();

      if(stats.count() > 100) {
        if(sigma < 0.15*avg) {
          // Success!
          // Record data
          idx++;
          x[idx] = spd;
          y[idx] = avg;
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
      tries++;
      // Move on to next speed if it's not moving after MAX_TRIES
      if((tries==MAX_TRIES) && (pulseSum == 0)) {
        break;
      }
    }

    // Perform linear regression
    float xAvg, yAvg;
    for(int i=0; i<MAX_DATA; i++) {
      // Break if both data points are zero (untouched)
      if((x[i] == 0) and (y[i] == 0)) {
        break;
      }
    }
  }

  leftMotorOff();
  stats.clear();
  delay(2000);
}*/
