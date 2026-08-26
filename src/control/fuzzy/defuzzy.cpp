#include "defuzzy.h"

void defuzzy::calcularPWM(
    const valoresFuzzy& regras,
    float base)
{
    base = constrain(base, 0.0f, 255.0f);

    // Calcula a correção fuzzy
    float pwmFuzzy = centroide(regras);

    // Converte o centroide para uma correção
    correcao = (pwmFuzzy / 255.0f) * base;

    if (correcao > 0)
    {
        // Correção para a direita
        pwmEsq = base;
        pwmDir = base - correcao;
    }
    else
    {
        // Correção para a esquerda
        pwmEsq = base + correcao;
        pwmDir = base;
    }

    pwmEsq = constrain(pwmEsq, 0, 255);
    pwmDir = constrain(pwmDir, 0, 255);
}

float defuzzy::centroide(const valoresFuzzy& regras)
{
    float numerador = 0.0f;
    float denominador = 0.0f;

    for (int pwm = -255; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        float fullCW  = trapmf(pwm, -255, 230, 20, 50);
        float altoCW  = trimf(pwm, 40, 75, 110);
        float baixoCW = trimf(pwm, 80, 115, 145);
        float centro  = trimf(pwm, 115, 145, 175);
        float baixoCC = trimf(pwm, 145, 180, 210);
        float altoCC  = trimf(pwm, 180, 210, 240);
        float fullCC  = trapmf(pwm, 220, 240, 255, 255);

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

    float pwmFuzzy = numerador / denominador;
    
    //Normalizando o valor de correção para o intervalo correto
    correcao = (base/255) * pwmFuzzy;


    return correcao;
}

float defuzzy::meanMaxEsq(const valoresFuzzy& regras)
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

float defuzzy::meanMaxDir(const valoresFuzzy& regras)
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