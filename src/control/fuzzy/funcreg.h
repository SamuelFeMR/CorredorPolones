#pragma once

#include <arduino.h>

class funcreg
{
public:

private:
float EGRN, EPQN, EZER, EPQP, EGRP;
float VGRN, VPQN, VZER, VPQP, VGRP;
}

struct RegrasSaida
{
    float mesquerda, esquerda, zero, direita, mdireita;
}