#include "pid.h"

PID::PID(float kp, float kd)
{
    this->kp = kp;
    this->kd = kd;

    erroAnterior = 0;
}

float PID::calcular(float erro)
{
    float derivada = erro - erroAnterior;

    float correcao =
        kp * erro +
        kd * derivada;

    erroAnterior = erro;

    return constrain(correcao, -255, 255);
}

void PID::reset()
{
    erroAnterior = 0;
}