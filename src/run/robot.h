#pragma once

#include <Arduino.h>

#include "hardware/sensores.h"
#include "control/fuzzy/funcreg.h"
#include "control/fuzzy/defuzzy.h"
#include "control/pid/pid.h"

#define PWM_MAX 255

class Robot
{
public:
    void begin();
    void update();

private:

    // =================================================
    // MODULOS
    // =================================================

    sensores sensoresRobot;
    fuzzy controladorFuzzy;
    defuzzy controladorDefuzzy;

    PID controladorPID{PID_KP, PID_KD};


    // =================================================
    // TIPO DE CONTROLE
    // =================================================

    enum TipoControle
    {
        CONTROLE_FUZZY,
        CONTROLE_PID
    };

    static constexpr TipoControle CONTROLE_ATUAL =
        CONTROLE_PID;


    // =================================================
    // RECUPERACAO DE LINHA
    // =================================================

    int ultimoPwmEsq = 0;
    int ultimoPwmDir = 0;

    bool linhaPerdida = false;
    unsigned long inicioLinhaPerdida = 0;

    static constexpr unsigned long TEMPO_RECUPERACAO_MS = 500;


    // =================================================
    // CONTROLE
    // =================================================

    static constexpr float PWM_BASE = 150.0f;

    static constexpr float PID_KP = 0.062f;
    static constexpr float PID_KD = 0.3f;

    //Funcionou com 150 0.058 0.1
    //Funcionou com 180 0.078 0.2

    // =================================================
    // DEBUG
    // =================================================

    static constexpr unsigned long INTERVALO_DEBUG_MS = 100;

    unsigned long ultimoDebug = 0;

    const unsigned long TEMPO_CALIBRACAO = 5000;

    // ================================================= 
    // MOTOR ESQUERDO
    // =================================================

    static constexpr uint8_t AIN1 = 18;
    static constexpr uint8_t AIN2 = 5;
    static constexpr uint8_t PWMA = 13;


    // =================================================
    // MOTOR DIREITO
    // =================================================

    static constexpr uint8_t BIN1 = 16;
    static constexpr uint8_t BIN2 = 17;
    static constexpr uint8_t PWMB = 4;


    // =================================================
    // PWM
    // =================================================

    static constexpr uint32_t PWM_FREQ = 1000;
    static constexpr uint8_t PWM_RES = 8;

    static constexpr uint8_t PWM_CHANNEL_A = 0;
    static constexpr uint8_t PWM_CHANNEL_B = 1;


    // =================================================
    // FUNCOES
    // =================================================

    void configurarMotores();

    void motorEsquerdo(int pwm);
    void motorDireito(int pwm);

    void giroCalib();
    void pararMotores();


    // =================================================
    // PARADA
    // =================================================

    int contadorDeteccoesDireita = 0;

    bool direitaAnterior = false;
    bool contandoParada = false;
    bool roboParado = false;

    static constexpr int DETECCOES_NECESSARIAS = 7;

    unsigned long inicioContagemParada = 0;
    static constexpr unsigned long TEMPO_ATE_PARADA_MS = 3000;
    
    unsigned long inicioPista = 0;
    bool pistaIniciada = false;

    static constexpr unsigned long TEMPO_PISTA_MS = 29000;
};