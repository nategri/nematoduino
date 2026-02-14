#ifndef OSEPPROBOT_H
#define OSEPPROBOT_H

#include <arduino.h>

#define button_L (1)
#define button_R (2)
#define button_U (4)
#define button_D (8)
#define button_A (16)
#define button_B (32)
#define button_X (64)
#define button_Y (128)

#define left_X (0)
#define left_Y (1)
#define right_X (2)
#define right_Y (3)
#define gravity_X (4)
#define gravity_Y (5)
#define gravity_Z (6)

class OseppTBMotor
{
private:
    uint8_t _dir_pin, _pwm_pin;
    uint8_t _forward_level;

public:
    OseppTBMotor(uint8_t dir_pin, uint8_t pwm_pin, uint8_t forward_level = HIGH);
    void forward(int pwm);
    void backward(int pwm);
};

class OseppRangeFinder
{
private:
    uint8_t _sig_Pin;
    float _lastPing;
    unsigned long _lastEntry;

public:
    OseppRangeFinder(uint8_t sig_Pin);
    float ping(unsigned int max_mm = 4000);
};

class OseppRemote
{
private:
    char letter;
    int number;
    char minusSign;
    char tempButton;
    char mButtonState;
    int mChannel[7];
    unsigned long mLast_Data_millis;

public:
    OseppRemote(long baudrate = 0);
    void update();
    uint8_t getButtonState(uint8_t mask);
    int getChannalData(uint8_t channel);
    bool isTimeoutFor(unsigned long howlong);
};

class OseppUltrasonic
{
private:
    uint8_t _trig_Pin, _echo_pin;
    float _lastPing;
    unsigned long _lastEntry;
public:
    OseppUltrasonic(uint8_t trig_Pin, uint8_t echo_pin);
    float ping(unsigned int max_mm = 4000);
};

#endif