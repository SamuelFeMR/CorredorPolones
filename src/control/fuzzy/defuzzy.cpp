#include "defuzzy.h"

float defuzzy::pwmMotorEsq(const saidaFuzzy& regras, const Inferencia& inf)
{
    return centroideEsq(regras, inf);
}

float defuzzy::pwmMotorDir(const saidaFuzzy& regras, const Inferencia& inf)
{
    return centroideDir(regras, inf);
}

float defuzzy::centroideEsq(const saidaFuzzy& regras,
                            const Inferencia& inf)
{
    float numerador = 0.0f;
    float denominador = 0.0f;

    for(int pwm = 0; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        mu = max(mu, min(regras.FullCW , inf.fullCW(pwm)));
        mu = max(mu, min(regras.AltoCW , inf.altoCW(pwm)));
        mu = max(mu, min(regras.BaixoCW, inf.baixoCW(pwm)));
        mu = max(mu, min(regras.Centro , inf.centro(pwm)));
        mu = max(mu, min(regras.BaixoCC, inf.baixoCC(pwm)));
        mu = max(mu, min(regras.AltoCC , inf.altoCC(pwm)));
        mu = max(mu, min(regras.FullCC , inf.fullCC(pwm)));

        numerador += pwm * mu;
        denominador += mu;
    }

    if(denominador == 0.0f)
        return 0;

    return numerador / denominador;
}

float defuzzy::centroideDir(const saidaFuzzy& regras,
                            const Inferencia& inf)
{
    float numerador = 0.0f;
    float denominador = 0.0f;

    for(int pwm = 0; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        mu = max(mu, min(regras.FullCW , inf.fullCW(pwm)));
        mu = max(mu, min(regras.AltoCW , inf.altoCW(pwm)));
        mu = max(mu, min(regras.BaixoCW, inf.baixoCW(pwm)));
        mu = max(mu, min(regras.Centro , inf.centro(pwm)));
        mu = max(mu, min(regras.BaixoCC, inf.baixoCC(pwm)));
        mu = max(mu, min(regras.AltoCC , inf.altoCC(pwm)));
        mu = max(mu, min(regras.FullCC , inf.fullCC(pwm)));

        numerador += pwm * mu;
        denominador += mu;
    }

    if(denominador == 0.0f)
        return 0;

    return numerador / denominador;
}

float defuzzy::meanMaxEsq(const saidaFuzzy& regras)
{
    const float centros[7] =
    {
        255,
        200,
        200,
        230,
        170,
        150,
        0
    };

    const float ativacao[7] =
    {
        regras.FullCW,
        regras.AltoCW,
        regras.BaixoCW,
        regras.Centro,
        regras.BaixoCC,
        regras.AltoCC,
        regras.FullCC
    };

    float maior = 0.0f;

    for(int i = 0; i < 7; i++)
        if(ativacao[i] > maior)
            maior = ativacao[i];

    float soma = 0.0f;
    int quantidade = 0;

    for(int i = 0; i < 7; i++)
    {
        if(ativacao[i] == maior)
        {
            soma += centros[i];
            quantidade++;
        }
    }

    if(quantidade == 0)
        return 0;

    return soma / quantidade;
}

float defuzzy::meanMaxDir(const saidaFuzzy& regras)
{
    const float centros[7] =
    {
        0,
        140,
        180,
        230,
        200,
        200,
        255
    };

    const float ativacao[7] =
    {
        regras.FullCW,
        regras.AltoCW,
        regras.BaixoCW,
        regras.Centro,
        regras.BaixoCC,
        regras.AltoCC,
        regras.FullCC
    };

    float maior = 0.0f;

    for(int i = 0; i < 7; i++)
        if(ativacao[i] > maior)
            maior = ativacao[i];

    float soma = 0.0f;
    int quantidade = 0;

    for(int i = 0; i < 7; i++)
    {
        if(ativacao[i] == maior)
        {
            soma += centros[i];
            quantidade++;
        }
    }

    if(quantidade == 0)
        return 0;

    return soma / quantidade;
}