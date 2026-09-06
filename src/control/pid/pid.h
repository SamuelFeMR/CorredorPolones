#pragma once

#include <Arduino.h>

class PID
{
public:

    PID(float kp, float ki, float kd);

    // Calcula a correção diferencial dos motores.
    //
    // Entrada:
    // erro = aproximadamente -3500 até +3500
    //
    // Saída:
    // aproximadamente -255 até +255
    float calcular(float erro);

    void configurar(float kp, float ki, float kd);

    void reset();

    float getKp() const;
    float getKi() const;
    float getKd() const;

    float getIntegral() const;
    float getDerivada() const;

private:

    float kp;
    float ki;
    float kd;

    float integral = 0.0f;
    float erroAnterior = 0.0f;
    float derivadaFiltrada = 0.0f;

    unsigned long tempoAnterior = 0;

    bool primeiroCiclo = true;

    // Erro máximo esperado pelo QTR
    static constexpr float ERRO_MAX = 3500.0f;

    // Correção máxima dos motores
    static constexpr float SAIDA_MAX = 255.0f;

    // Limite da integral
    static constexpr float INTEGRAL_MAX = 2.0f;

    // Filtro da derivada
    //
    // 0.0 = sem histórico
    // 1.0 = totalmente filtrado
    static constexpr float FILTRO_DERIVADA = 0.70f;
};