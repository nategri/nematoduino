#include "oseppRobot.h"

OseppTBMotor::OseppTBMotor(uint8_t dir_pin, uint8_t pwm_pin, uint8_t forward_level)
{
    _dir_pin = dir_pin;
    _pwm_pin = pwm_pin;
    _forward_level = forward_level;
    pinMode(_dir_pin, OUTPUT);
    digitalWrite(_pwm_pin, LOW);
}
void OseppTBMotor::forward(int pwm)
{
    if (pwm < 0)
    {
        digitalWrite(_dir_pin, !_forward_level);
        pwm = -pwm;
    }
    else
    {
        digitalWrite(_dir_pin, _forward_level);
    }
    analogWrite(_pwm_pin, pwm > 255 ? 255 : pwm);
}
void OseppTBMotor::backward(int pwm)
{
    forward(-pwm);
}

OseppRangeFinder::OseppRangeFinder(uint8_t sig_Pin)
{
    _sig_Pin = sig_Pin;
    _lastEntry = -26;
    _lastPing = 0;
}
float OseppRangeFinder::ping(unsigned int max_mm)
{
    if (millis() - _lastEntry < 25)
        return _lastPing;
    _lastEntry = millis();
    pinMode(_sig_Pin, INPUT);
    if (digitalRead(_sig_Pin) == HIGH)
        return _lastPing;
    pinMode(_sig_Pin, OUTPUT);
    digitalWrite(_sig_Pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_sig_Pin, LOW);
    pinMode(_sig_Pin, INPUT);
    unsigned long duration = pulseIn(_sig_Pin, HIGH, max_mm * 5.5 + 200);
    if (duration == 0)
        _lastPing = max_mm;
    else
        _lastPing = (float)duration / 5.8;
    return _lastPing;
}

OseppRemote::OseppRemote(long baudrate)
{
    letter = 0;
    number = 0;
    minusSign = 0;
    mButtonState = 0;
    tempButton = 0;
    mLast_Data_millis = millis();
    if (baudrate)
    {
        Serial.begin(115200);
        update();
    }
}
void OseppRemote::update()
{
    while (Serial.available())
    {
        char data = Serial.read();
        mLast_Data_millis = millis();
        if ((data >= '0') && (data <= '9'))
        {
            number *= 10;
            number += data - '0';
        }
        else if (data == '-')
        {
            minusSign = 1;
        }
        else
        {
            if (minusSign)
                number = -number;
            switch (letter)
            {
            case 'u':
                mChannel[0] = number;
                break;
            case 'v':
                mChannel[1] = number;
                break;
            case 'w':
                mChannel[2] = number;
                break;
            case 'a':
                mChannel[3] = number;
                break;
            case 'x':
                mChannel[4] = number;
                break;
            case 'y':
                mChannel[5] = number;
                break;
            case 'z':
                mChannel[6] = number;
                break;
            default:
                break;
            }
            number = 0;
            minusSign = 0;
            switch (data)
            {
            case 'L':
                tempButton |= 1;
                break;
            case 'R':
                tempButton |= 2;
                break;
            case 'U':
                tempButton |= 4;
                break;
            case 'D':
                tempButton |= 8;
                break;
            case 'A':
                tempButton |= 16;
                break;
            case 'B':
                tempButton |= 32;
                break;
            case 'X':
                tempButton |= 64;
                break;
            case 'Y':
                tempButton |= 128;
                break;
            case '\n':
                mButtonState = tempButton;
                tempButton = 0;
                break;
            default:
                break;
            }
            letter = data;
        }
    }
}
uint8_t OseppRemote::getButtonState(uint8_t mask)
{
    update();
    return mButtonState & mask;
}
int OseppRemote::getChannalData(uint8_t channel)
{
    update();
    if (channel > sizeof(mChannel) / sizeof(mChannel[0]))
        return 0;
    return mChannel[channel];
}
bool OseppRemote::isTimeoutFor(unsigned long howlong)
{
    update();
    if (millis() - mLast_Data_millis > howlong)
        return true;
    return false;
}

OseppUltrasonic::OseppUltrasonic(uint8_t trig_Pin, uint8_t echo_pin)
{
    _trig_Pin = trig_Pin;
    _echo_pin = echo_pin;
    _lastEntry = -26;
    _lastPing = 0;
}
float OseppUltrasonic::ping(unsigned int max_mm)
{
    if (millis() - _lastEntry < 25)
        return _lastPing;
    _lastEntry = millis();
    pinMode(_trig_Pin, OUTPUT);
    digitalWrite(_trig_Pin, LOW);
    pinMode(_echo_pin, INPUT);
    digitalWrite(_echo_pin, LOW);
    digitalWrite(_trig_Pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig_Pin, LOW);
    unsigned long duration = pulseIn(_echo_pin, HIGH, max_mm * 5.5 + 200);
    if (duration == 0)
        _lastPing = max_mm;
    else
        _lastPing = (float)duration / 5.8;
    return _lastPing;
}
