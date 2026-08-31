#pragma once

#include <Arduino.h>
#include "funcreg.h"

class defuzzy
{
public:

    void calcularPWM(const valoresFuzzy& regras, float base);

    int pwmEsq;
    int pwmDir;

private:

    float correcao = 0.0f;

    float centroide(const valoresFuzzy& regras);

    float meanMaxEsq(const valoresFuzzy& regras);
    float meanMaxDir(const valoresFuzzy& regras);
};