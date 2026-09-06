#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID
{
private:
    float kp;
    float kd;
    float erroAnterior;

public:
    PID(float kp, float kd);

    float calcular(float erro);
    void reset();
};

#endif