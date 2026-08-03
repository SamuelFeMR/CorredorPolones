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
float regra2 = min(EGRN,VPQN);
float regra3 = min(EGRN,VZER);
float regra4 = min(EGRN,VPQP);
float regra5 = min(EGRN,VGRP);
float regra6 = min(EPQN,VGRN);
float regra7 = min(EPQN,VPQN);
float regra8 = min(EPQN,VZER);
float regra9 = min(EPQN,VPQP);
float regra10 = min(EPQN,VGRP);
float regra11 = min(EZER,VGRN);
float regra12 = min(EZER,VPQN);
float regra13 = min(EZER,VZER);
float regra14 = min(EZER,VPQP);
float regra15 = min(EZER,VGRP);
float regra16 = min(EPQP,VGRN);
float regra17 = min(EPQP,VPQN);
float regra18 = min(EPQP,VZER);
float regra19 = min(EPQP,VPQP);
float regra20 = min(EPQP,VGRP);
float regra21 = min(EGRP,VGRN);
float regra22 = min(EGRP,VPQN);
float regra23 = min(EGRP,VZER);
float regra24 = min(EGRP,VPQP);
float regra25 = min(EGRP,VGRP);

FullCW = max(regra1,regra2,regra3,regra4,regra5);  //Fazer max do max e corrigir as regras correspondentes
AltoCW = max(regra6,regra7,regra8,regra9,regra10);
BaixoCW = max(regra11,regra12,regra13,regra14,regra15);
Centro = max(regra16,regra17,regra18,regra19,regra20);
BaixoCC = max(regra21,regra22,regra23,regra24,regra25);
AltoCC = max(regra21,regra22,regra23,regra24,regra25);
FullCC = max(regra21,regra22,regra23,regra24,regra25);