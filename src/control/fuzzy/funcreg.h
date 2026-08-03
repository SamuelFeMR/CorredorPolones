#pragma once

#include <arduino.h>

class funcreg
{
public:
    float FullCW = 0;
    float AltoCW  = 0;
    float BaixoCW = 0;
    float Centro  = 0;
    float BaixoCC = 0;
    float AltoCC  = 0;
    float FullCC = 0;
private:

float EGRN, EPQN, EZER, EPQP, EGRP;
float VGRN, VPQN, VZER, VPQP, VGRP;
}