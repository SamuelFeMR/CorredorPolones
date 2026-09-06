#include "pid.h"

PD::PD(float novoKp, float novoKd)
{
    configurar(novoKp, novoKd);
}


void PD::configurar(float novoKp, float novoKd)
{
    kp = novoKp;
    kd = novoKd;

    reset();
}


void PD::reset()
{
    erroAnterior = 0.0f;

    derivadaFiltrada = 0.0f;

    proporcionalAtual = 0.0f;
    derivadaAtual = 0.0f;
    correcaoAtual = 0.0f;

    tempoAnterior = millis();

    primeiroCiclo = true;
}


float PD::calcular(float erro)
{
    unsigned long agora = millis();

    float dt =
        (agora - tempoAnterior) / 1000.0f;

    tempoAnterior = agora;

    // Proteção contra divisão por zero
    if (dt <= 0.0f)
    {
        dt = 0.001f;
    }


    // =================================================
    // NORMALIZAÇÃO DO ERRO
    // =================================================

    // -3500 -> -1
    //     0 ->  0
    // +3500 -> +1

    float erroNormalizado =
        erro / ERRO_MAX;

    erroNormalizado =
        constrain(
            erroNormalizado,
            -1.0f,
            1.0f
        );


    // =================================================
    // TERMO PROPORCIONAL
    // =================================================

    proporcionalAtual =
        kp * erroNormalizado;


    // =================================================
    // TERMO DERIVATIVO
    // =================================================

    float derivada = 0.0f;

    if (!primeiroCiclo)
    {
        derivada =
            (erroNormalizado - erroAnterior) / dt;
    }

    primeiroCiclo = false;


    // Limita a derivada antes do filtro

    derivada =
        constrain(
            derivada,
            -DERIVADA_MAX,
            DERIVADA_MAX
        );


    // Filtro passa-baixa

    derivadaFiltrada =
        FILTRO_DERIVADA * derivadaFiltrada
        +
        (1.0f - FILTRO_DERIVADA) * derivada;


    derivadaAtual =
        kd * derivadaFiltrada;


    // =================================================
    // PD
    // =================================================

    correcaoAtual =
        proporcionalAtual
        +
        derivadaAtual;


    // =================================================
    // LIMITAÇÃO DA SAÍDA
    // =================================================

    correcaoAtual =
        constrain(
            correcaoAtual,
            -SAIDA_MAX,
            SAIDA_MAX
        );


    // Guarda erro para o próximo ciclo

    erroAnterior =
        erroNormalizado;


    return correcaoAtual;
}


float PD::getKp() const
{
    return kp;
}


float PD::getKd() const
{
    return kd;
}


float PD::getProporcional() const
{
    return proporcionalAtual;
}


float PD::getDerivada() const
{
    return derivadaAtual;
}


float PD::getCorrecao() const
{
    return correcaoAtual;
}