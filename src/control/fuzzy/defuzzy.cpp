#include "defuzzy.h"
#include "funcreg.h"
#include "func.h"

void defuzzy::calcularPWM(const valoresFuzzy& regras, float base)
{
    base = constrain(base, 0.0f, 255.0f);

    float correcaoFuzzy = centroide(regras);

    // Normaliza a correção fuzzy
    float correcaoNova =
        (correcaoFuzzy / 255.0f) * 255.0f;

    // Suavização exponencial
    correcao =
        correcao + SUAVIZACAO * (correcaoNova - correcao);

    // Limita a correção
    correcao = constrain(correcao, -255.0f, 255.0f);

    pwmDir = base + correcao;
    pwmEsq = base - correcao;

    pwmDir = constrain(pwmDir, 0.0f, 255.0f);
    pwmEsq = constrain(pwmEsq, 0.0f, 255.0f);
}

float defuzzy::centroide(const valoresFuzzy& regras)
{

    /*
        Universo da saída:

        -255                0                +255
          |----------------|----------------|
        esquerda          centro            direita

        A saída fuzzy representa CORREÇÃO,
        não PWM absoluto.
    */

    float numerador = 0.0f;
    float denominador = 0.0f;

    for (int pwm = -255; pwm <= 255; pwm++)
    {
        float mu = 0.0f;

        // -------------------------------
        // Correção para a DIREITA
        float fullCD  = trapmf(pwm, 170, 220, 255, 255);
        float altoCD  = trimf(pwm, 100, 170, 220);
        float baixoCD = trimf(pwm, 0, 80, 140);

        // -------------------------------
        // Centro
        float centro = trimf(pwm, -40, 0, 40);

        // -------------------------------
        // Correção para a ESQUERDA
        float baixoCE = trimf(pwm, -140, -80, 0);
        float altoCE  = trimf(pwm, -220, -170, -100);
        float fullCE  = trapmf(pwm, -255, -255, -220, -170);

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
    /*
        Mean of Maximum.

        Os centros representam CORREÇÃO:

        VCD = +255
        VMD = +170
        VPD = +80
        CEN = 0
        VPE = -80
        VME = -170
        VCE = -255
    */

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