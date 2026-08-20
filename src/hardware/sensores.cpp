#include "sensores.h"

void sensores::begin()
{
    pinMode(2, OUTPUT);
    pinMode(SensorDireita, INPUT);
    pinMode(SensorEsquerda, INPUT);

    qtr.setTypeAnalog();

    qtr.setSensorPins(
        (const uint8_t[]){39, 34, 35, 32, 33, 25, 26, 27},
         
        NUM_SENSORS
    );
}

void sensores::calibrate()
{
    digitalWrite(2, HIGH);

    for (int i = 0; i < 400; i++)
    {
        qtr.calibrate();

        int direita = analogRead(SensorDireita);
        int esquerda = analogRead(SensorEsquerda);

        thersholdDirMin = min(thersholdDirMin, direita);
        thersholdDirMax = max(thersholdDirMax, direita);

        thersholdEsqMin = min(thersholdEsqMin, esquerda);
        thersholdEsqMax = max(thersholdEsqMax, esquerda);

        delay(5);
    }

    digitalWrite(2, LOW);
}

void sensores::update()
{
    uint16_t position = qtr.readLineBlack(sensorValues);

    erro = (int)position - 3500;

    
    int leituraDireita = analogRead(SensorDireita);
    leituraDireita = constrain(leituraDireita,thersholdDirMin,thersholdDirMax);
    detectDireita = map(leituraDireita,thersholdDirMin,thersholdDirMax,0,1000) < 500;

    int leituraEsquerda = analogRead(SensorEsquerda);
    leituraEsquerda = constrain(leituraEsquerda,thersholdEsqMin,thersholdEsqMax);
    detectEsquerda = map(leituraEsquerda,thersholdEsqMin,thersholdEsqMax,0,1000) < 500;
}

bool sensores::direitaDetected() const
{
    return detectDireita;
}

bool sensores::esquerdaDetected() const
{
    return detectEsquerda;
}

int sensores::calculaErro() const
{
    return erro;
}

const uint16_t* sensores::getValues() const
{
    return sensorValues;
}