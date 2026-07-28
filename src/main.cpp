/*
Calibração-
Inicialização
Calculo de Erros/
Regras de Inferencia
Funções de Pertinência
Desfuzzificação
Atuação
Debug
Leituras dos Sensores-
Leitura Bluetooth

Obs *
Trajetoria

Função só de controle:
- Fuzzy
- PID

*/
#include <Arduino.h>
#include "app/Robot.h"

Robot robot;

void setup()
{
    robot.begin();
}

void loop()
{
    robot.update();
}