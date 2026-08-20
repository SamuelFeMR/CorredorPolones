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

    for (int pwm = 0; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        float fullCW = (pwm >= 240) ? 1.0f : 0.0f;

        float altoCW  = trimf(pwm, 180, 210, 240);
        float baixoCW = trimf(pwm, 160, 190, 220);
        float centro  = trimf(pwm, 140, 170, 200);
        float baixoCC = trimf(pwm, 100, 130, 160);
        float altoCC  = trimf(pwm, 50, 80, 110);

        float fullCC = (pwm <= 20) ? 1.0f : 0.0f;

        mu = max(mu, min(regras.VCD, fullCW));
        mu = max(mu, min(regras.VMD, altoCW));
        mu = max(mu, min(regras.VPD, baixoCW));
        mu = max(mu, min(regras.CEN, centro));
        mu = max(mu, min(regras.VPE, baixoCC));
        mu = max(mu, min(regras.VME, altoCC));
        mu = max(mu, min(regras.VCE, fullCC));

        numerador += pwm * mu;
        denominador += mu;
    }

    if (denominador == 0.0f)
        return 0;

    return numerador / denominador;
}

float defuzzy::centroideDir(const saidaFuzzy& regras,
                            const Inferencia& inf)
{
    float numerador = 0.0f;
    float denominador = 0.0f;

    for (int pwm = 0; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        float fullCW = (pwm <= 20) ? 1.0f : 0.0f;

        float altoCW  = trimf(pwm, 50, 80, 110);
        float baixoCW = trimf(pwm, 100, 130, 160);
        float centro  = trimf(pwm, 140, 170, 200);
        float baixoCC = trimf(pwm, 160, 190, 220);
        float altoCC  = trimf(pwm, 180, 210, 240);

        float fullCC = (pwm >= 240) ? 1.0f : 0.0f;

        mu = max(mu, min(regras.VCD, fullCW));
        mu = max(mu, min(regras.VMD, altoCW));
        mu = max(mu, min(regras.VPD, baixoCW));
        mu = max(mu, min(regras.CEN, centro));
        mu = max(mu, min(regras.VPE, baixoCC));
        mu = max(mu, min(regras.VME, altoCC));
        mu = max(mu, min(regras.VCE, fullCC));

        numerador += pwm * mu;
        denominador += mu;
    }

    if (denominador == 0.0f)
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
        regras.VCD,
        regras.VMD,
        regras.VPD,
        regras.CEN,
        regras.VPE,
        regras.VME,
        regras.VCE
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
        regras.VCD,
        regras.VMD,
        regras.VPD,
        regras.CEN,
        regras.VPE,
        regras.VME,
        regras.VCE
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