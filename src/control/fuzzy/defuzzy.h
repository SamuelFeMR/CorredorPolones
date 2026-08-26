

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
#include "funcreg.h"

class defuzzy
{
public:

    float pwmMotorEsq(const valoresFuzzy& regras);
    float pwmMotorDir(const valoresFuzzy& regras);

private:

    float centroideEsq(const valoresFuzzy& regras);
    float centroideDir(const valoresFuzzy& regras);


    
    float meanMaxEsq(const valoresFuzzy& regras);
    float meanMaxDir(const valoresFuzzy& regras);

};