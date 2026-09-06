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

    PID controladorPD;

    enum TipoControle
    {
        CONTROLE_FUZZY,
        CONTROLE_PD
    };

    static constexpr TipoControle CONTROLE_ATUAL = CONTROLE_PD;


    // CONTROLE
    int erroAnterior = 0;
    bool primeiroCiclo = true;

    static constexpr float PWM_BASE = 160.0f;

    static constexpr float PD_KP = 180.0f;
    static constexpr float PD_KD = 8.0f;

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