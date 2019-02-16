#ifndef WORM_HPP
#define WORM_HPP

#include "Arduino.h"

extern "C" {
#include "utility/connectome.h"
#include "utility/muscles.h"
};

class Worm {
  public:
    Worm();

    void chemotaxis();
    void noseTouch();

    int getLeftMuscle();
    int getRightMuscle();

  private:
    int _leftMuscle;
    int _rightMuscle;
    double _motorFireAvg;

    Connectome _connectome;
    void _update(const uint16_t*, int);
};

#endif
