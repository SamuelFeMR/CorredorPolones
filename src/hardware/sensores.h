#pragma once

#include <Arduino.h>
#include <QTRSensors.h>

#define NUM_SENSORS 8
#define SensorDireita 26
#define SensorEsquerda 2

#define esqC1 23
#define esqC2 21
#define dirC1 19
#define dirC2 14

class sensores
{
public:
    void begin();
    void calibrate();

    void update();
    bool direitaDetected() const;
    bool esquerdaDetected() const;

    bool linhaDetectada() const;
    
    int calculaErro() const;

    const uint16_t* getValues() const;
    float getVelocidadeEsq() const;
    float getVelocidadeEsq() const;

private:
    QTRSensors qtr;
    uint16_t sensorValues[NUM_SENSORS];
    uint16_t sensoresLaterais[2];
    bool detectDireita;
    bool detectEsquerda;
    float velDir, velEsq;
    int erro = 0;
    int thersholdDirMin = 4095, thersholdDirMax = 0, thersholdEsqMin = 4095, thersholdEsqMax = 0;
    volatile uint32_t pulsosEsq = 0;
    volatile uint32_t pulsosDir = 0;
};