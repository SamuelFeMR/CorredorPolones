#pragma once

#include <Arduino.h>

class PID
{
public:

    PID(float kp, float kd);

    // Calcula a correção diferencial dos motores.
    //
    // Entrada:
    // erro = aproximadamente -600 até +600
    //
    // Saída:
    // aproximadamente -255 até +255
    float calcular(float erro);

    void configurar(float kp, float kd);

    void reset();

    float getKp() const;
    float getKd() const;

    float getProporcional() const;
    float getDerivada() const;
    float getCorrecao() const;

private:

    float kp;
    float kd;

    float erroAnterior = 0.0f;
    float derivadaFiltrada = 0.0f;

    float proporcionalAtual = 0.0f;
    float derivadaAtual = 0.0f;
    float correcaoAtual = 0.0f;

    unsigned long tempoAnterior = 0;

    bool primeiroCiclo = true;

    // Erro máximo esperado pelo QTR
    static constexpr float ERRO_MAX = 1500.0f;

    // Correção máxima
    static constexpr float SAIDA_MAX = 255.0f;

    // Limite da derivada normalizada.
    //
    // Evita que uma leitura muito rápida
    // gere uma correção absurda.
    static constexpr float DERIVADA_MAX = 20.0f;

    // Filtro passa-baixa da derivada.
    //
    // 0.0 = sem filtro
    // 1.0 = extremamente filtrado
    static constexpr float FILTRO_DERIVADA = 0.70f;
};