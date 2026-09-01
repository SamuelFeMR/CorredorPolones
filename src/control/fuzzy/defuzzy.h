#pragma once

#include <Arduino.h>
#include "funcreg.h"
#include "func.h"

class defuzzy
{
public:

    void calcularPWM(const valoresFuzzy& regras, float base);
    float getCorrecao();
    int pwmEsq = 0;
    int pwmDir = 0;

private:

    float correcao = 0.0f;

    float centroide(const valoresFuzzy& regras);

    float meanMax(const valoresFuzzy& regras);
};