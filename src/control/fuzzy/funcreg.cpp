/*Regras definidas para o erro e para a variacao do erro:
EGRN = Trapezoidal      VGRN = Trapezoidal
EPQN = Triangular       VPQN = Triangular
EZER = Triangular       VZER = Triangular
EPQP = Triangular       VPQP = Triangular
EGRP = Trapezoidal      VGRP = Trapezoidal
*/
#include <funcreg.h>
#include <func.h>

void fuzzy::funcoes(float erro, float varerro)
{
EGRN = trapmf(erro,-3500,-3500,-2000,-1500); //analisar na calibragem do sensor se os valores estao corretos
EPQN = trimf(erro,-1750,-1125,-500);
EZER = trimf(erro,-700,0,700);
EPQP = trimf(erro,500,1125,1750);
EGRP = trapmf(erro,1500,2000,3500,3500);

VGRN = trapmf(varerro,-3500,-3500,-2000,-1500);
VPQN = trimf(varerro,-1750,-1125,-500);
VZER = trimf(varerro,-700,0,700);
VPQP = trimf(varerro,500,1125,1750);
VGRP = trapmf(varerro,1500,2000,3500,3500);
}

//Regras (25 regras)

float regra1 = min(EGRN,VGRN);