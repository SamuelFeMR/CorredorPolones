#include "pid.h"

PID::PID(float kp, float ki, float kd)
{
    configurar(kp, ki, kd);
}

void PID::configurar(float novoKp, float novoKi, float novoKd)
{
    kp = novoKp;
    ki = novoKi;
    kd = novoKd;

    reset();
}

void PID::reset()
{
    integral = 0.0f;
    erroAnterior = 0.0f;
    derivadaFiltrada = 0.0f;

    tempoAnterior = millis();

    primeiroCiclo = true;
}

float PID::calcular(float erro)
{
    unsigned long agora = millis();

    float dt =
        (agora - tempoAnterior) / 1000.0f;

    tempoAnterior = agora;

    // Evita divisão por zero
    if (dt <= 0.0f)
    {
        dt = 0.001f;
    }

    // -------------------------------------------------
    // NORMALIZAÇÃO DO ERRO
    // -------------------------------------------------
    //
    // -3500 -> -1
    //     0 ->  0
    // +3500 -> +1
    //
    float erroNormalizado =
        erro / ERRO_MAX;

    erroNormalizado =
        constrain(
            erroNormalizado,
            -1.0f,
            1.0f
        );


    // -------------------------------------------------
    // DERIVADA
    // -------------------------------------------------

    float derivada = 0.0f;

    if (!primeiroCiclo)
    {
        derivada =
            (erroNormalizado - erroAnterior) / dt;
    }

    primeiroCiclo = false;


    // Filtro passa-baixa na derivada

    derivadaFiltrada =
        FILTRO_DERIVADA * derivadaFiltrada
        +
        (1.0f - FILTRO_DERIVADA) * derivada;


    // -------------------------------------------------
    // INTEGRAL
    // -------------------------------------------------

    integral +=
        erroNormalizado * dt;

    integral =
        constrain(
            integral,
            -INTEGRAL_MAX,
            INTEGRAL_MAX
        );


    // -------------------------------------------------
    // PID
    // -------------------------------------------------

    float P =
        kp * erroNormalizado;

    float I =
        ki * integral;

    float D =
        kd * derivadaFiltrada;


    float saida =
        P + I + D;


    // -------------------------------------------------
    // LIMITAÇÃO
    // -------------------------------------------------

    saida =
        constrain(
            saida,
            -SAIDA_MAX,
            SAIDA_MAX
        );


    // Guarda erro atual
    erroAnterior =
        erroNormalizado;


    return saida;
}

float PID::getKp() const
{
    return kp;
}

float PID::getKi() const
{
    return ki;
}

float PID::getKd() const
{
    return kd;
}

float PID::getIntegral() const
{
    return integral;
}

float PID::getDerivada() const
{
    return derivadaFiltrada;
}