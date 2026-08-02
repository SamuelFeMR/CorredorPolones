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
    
    float fullCW(float x);
    float altoCW(float x);
    float baixoCW(float x);
    float centro(float x);
    float baixoCC(float x);
    float altoCC(float x);
    float fullCC(float x);
};