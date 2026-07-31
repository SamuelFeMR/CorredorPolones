

/*
Variáveis do sistema fuzzy:

Variaveis de Entrada não nebulosas:
-Erro
-Variação do Erro

A partir dos erros determinamos as variáveis de entrada nebulosas:

Pertinência do Erro:
-Muito a Esquerda
-Pouco pra Esquerda
-Centro
-Pouco pra Direita
-Muito pra Direita

Pertinência do delta do Erro:
-Virando muito pra Esquerda
-Virando pouco pra Esquerda
-Centralizado
-Virando pouco pra Direita
-Virando muito pra Direita

Com esse conjunto de pertinências realizamos a inferência nebulosa a partir do conjunto de regras.

Nossas saídas nebulosas serão defuzzyficadas como PWMs para os dois motores

Saidas nebulosas:
-Virar completamente pra direita
-Virar muito pra direita
-Virar pouco pra direita
-Centralizar
-Virar pouco pra esquerda
-Virar muito pra esquerda
-Virar completamente pra esquerda

Defuzzificação:
-Saidas nebulosas > valores pwm

*/

#pragma once

#include <Arduino.h>
#include "inferencia.h"

class defuzzy
{
public:

    float centroide(const aidaFuzzy& regras);
    float meanMax(const aidaFuzzy& regras);

private:
    
    float fullCW(float x);
    float altoCW(float x);
    float baixoCW(float x);
    float centro(float x);
    float baixoCC(float x);
    float altoCC(float x);
    float fullCC(float x);
};