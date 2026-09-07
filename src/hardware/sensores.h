#pragma once

#include <Arduino.h>
#include <QTRSensors.h>

#define NUM_SENSORS 8
#define SensorDireita 2
#define SensorEsquerda 36

#define esqC1 23
#define esqC2 21
#define dirC1 19
#define dirC2 14
#define ENCODER_PULSOS_POR_VOLTA 72.0f
#define INTERVALO_VELOCIDADE_MS 200

#define DIAMETRO_RODA_MM 22.0f

class sensores
{
public:
    void begin();
    void calibrate(unsigned long TEMPO_CALIBRACAO = 5000);

    void update();
    bool direitaDetected() const;
    bool esquerdaDetected() const;
    int getLeituraDireita() const;
    int getLeituraEsquerda() const;

    int getMinDireita() const;
    int getMaxDireita() const;

    int getMinEsquerda() const;
    int getMaxEsquerda() const;

    bool linhaDetectada() const;
    
    int calculaErro() const;

    const uint16_t* getValues() const;
    void atualizarVelocidade();
    float getVelocidadeEsq() const;
    float getVelocidadeDir() const;

    void zerarDistancia();
    float getDistanciaEsqMm() const;
    float getDistanciaDirMm() const;

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
    int32_t pulsosDistanciaEsq = 0;
    int32_t pulsosDistanciaDir = 0;
};