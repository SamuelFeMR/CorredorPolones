#include "defuzzy.h"
#include "funcreg.h"
#include "func.h"

void defuzzy::calcularPWM(const valoresFuzzy& regras, float base)
{
    // Velocidade base pode usar toda a faixa do PWM
    base = constrain(base, 0.0f, 255.0f);

    // Calcula a nova correcao fuzzy
    float correcaoNova = centroide(regras);

    // Suavizacao da correcao
    // Menor = mais suave
    // Maior = mais rapido
    constexpr float SUAVIZACAO = 0.25f;

    correcao =
        correcao +
        SUAVIZACAO * (correcaoNova - correcao);

    correcao = constrain(
        correcao,
        -255.0f,
        255.0f
    );

    // Controle diferencial
    pwmDir = base + correcao;
    pwmEsq = base - correcao;

    pwmDir = constrain(
        pwmDir,
        0.0f,
        255.0f
    );

    pwmEsq = constrain(
        pwmEsq,
        0.0f,
        255.0f
    );
}


float defuzzy::centroide(const valoresFuzzy& regras)
{
    float numerador = 0.0f;
    float denominador = 0.0f;

    for (int pwm = -255; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        // DIREITA
        float fullCD  = trapmf(pwm, 100, 200, 255, 255);
        float altoCD  = trimf(pwm, 20, 60, 100);
        float baixoCD = trimf(pwm, 0, 20, 40);

        // CENTRO
        float centro = trimf(pwm, -15, 0, 15);

        // ESQUERDA
        float baixoCE = trimf(pwm, -40, -20, 0);
        float altoCE  = trimf(pwm, -100, -60, -20);
        float fullCE  = trapmf(pwm, -255, -255, -200, -100);

        mu = max(mu, min(regras.VCD, fullCD));
        mu = max(mu, min(regras.VMD, altoCD));
        mu = max(mu, min(regras.VPD, baixoCD));

        mu = max(mu, min(regras.CEN, centro));

        mu = max(mu, min(regras.VPE, baixoCE));
        mu = max(mu, min(regras.VME, altoCE));
        mu = max(mu, min(regras.VCE, fullCE));

        numerador += pwm * mu;
        denominador += mu;
    }

    if (denominador == 0.0f)
        return 0.0f;

    return numerador / denominador;
}


float defuzzy::meanMax(const valoresFuzzy& regras)
{
    const float centros[7] =
    {
        255.0f,
        170.0f,
        80.0f,
        0.0f,
        -80.0f,
        -170.0f,
        -255.0f
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

    for (int i = 0; i < 7; i++)
    {
        if (ativacao[i] > maior)
            maior = ativacao[i];
    }

    if (maior <= 0.0f)
        return 0.0f;

    float soma = 0.0f;
    int quantidade = 0;

    const float tolerancia = 0.0001f;

    for (int i = 0; i < 7; i++)
    {
        if (fabs(ativacao[i] - maior) < tolerancia)
        {
            soma += centros[i];
            quantidade++;
        }
    }

    if (quantidade == 0)
        return 0.0f;

    return soma / quantidade;
}


float defuzzy::getCorrecao()
{
    return correcao;
}