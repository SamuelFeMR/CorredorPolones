#pragma once

#include <Arduino.h>
#include <QTRSensors.h>

#define NUM_SENSORS 8
#define SensorDireita 22
#define SensorEsquerda 36

class Sensors
{
public:
    void begin();
    void calibrate();

    void update();
    void direitaDetected();
    void esquerdaDetected();

    int calculaErro() const;

    const uint16_t* getValues() const;

private:
    QTRSensors qtr;
    uint16_t sensorValues[NUM_SENSORS];
    uint16_t sensoresLaterais[2];
    bool detectDireita;
    bool detectEsquerda;
    int erro = 0;
    int thersholdDirMin = 4095, thersholdDirMax = 0, thersholdEsqMin = 4095, thersholdEsqMax = 0;
};