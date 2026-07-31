#pragma once

#include <Arduino.h>

struct saidaFuzzy
{
    float FullCW = 0;
    float AltoCW  = 0;
    float BaixoCW = 0;
    float Centro  = 0;
    float BaixoCC = 0;
    float AltoCC  = 0;
    float FullCC = 0;
};

class Inferencia
{
public:
    saidaFuzzy executar(const entradaFuzzy& entrada);
};