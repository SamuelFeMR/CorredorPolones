#pragma once

#include <Arduino.h>

#include "hardware/sensores.h"
#include "control/fuzzy/funcreg.h"
#include "control/fuzzy/defuzzy.h"
#include "control/pid/pid.h"
class Robot
{
public:
    void begin();
    void update();

private:

    // MÓDULOS

    sensores sensoresRobot;
    fuzzy controladorFuzzy;
    defuzzy controladorDefuzzy;

    PID controladorPID{PID_KP, PID_KD};

    enum TipoControle
    {
        CONTROLE_FUZZY,
        CONTROLE_PID
    };

    static constexpr TipoControle CONTROLE_ATUAL = CONTROLE_PID;


    // CONTROLE
    int erroAnterior = 0;
    bool primeiroCiclo = true;

    static constexpr float PWM_BASE = 150.0f;

    static constexpr float PID_KP = 60.0f;
    static constexpr float PID_KD = 0.0f;

    static constexpr unsigned long INTERVALO_DEBUG_MS = 100;
    unsigned long ultimoDebug = 0;
    
    // MOTOR ESQUERDO
    static constexpr uint8_t AIN1 = 18;
    static constexpr uint8_t AIN2 = 5;
    static constexpr uint8_t PWMA = 13;


    // MOTOR DIREITO
    static constexpr uint8_t BIN1 = 16;
    static constexpr uint8_t BIN2 = 17;
    static constexpr uint8_t PWMB = 4;


    // PWM
    static constexpr uint32_t PWM_FREQ = 1000;
    static constexpr uint8_t PWM_RES = 8;

    static constexpr uint8_t PWM_CHANNEL_A = 0;
    static constexpr uint8_t PWM_CHANNEL_B = 1;


    // FUNÇÕES
    void configurarMotores();

    void motorEsquerdo(int pwm);
    void motorDireito(int pwm);

    void giroCalib();
    void pararMotores();

    // PARADA
    int contadorDeteccoesDireita = 0;

    bool direitaAnterior = false;
    bool contandoDistancia = false;
    bool roboParado = false;

    static constexpr int DETECCOES_NECESSARIAS = 10;

    static constexpr float DISTANCIA_POS_DETECCAO_MM = 200.0f;
};