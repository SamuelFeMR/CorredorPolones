#pragma once

#include <Arduino.h>

#include "hardware/sensores.h"
#include "control/fuzzy/funcreg.h"
#include "control/fuzzy/defuzzy.h"

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

    // CONTROLE
    int erroAnterior = 0;
    bool primeiroCiclo = true;

    static constexpr float PWM_BASE = 170.0f;

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