/*Regras definidas para o erro e para a variacao do erro:
EGRN = Trapezoidal      VGRN = Trapezoidal
EPQN = Triangular       VPQN = Triangular
EZER = Triangular       VZER = Triangular
EPQP = Triangular       VPQP = Triangular
EGRP = Trapezoidal      VGRP = Trapezoidal
*/
#include <funcreg.h>
#include <func.h>

saidaFuzzy fuzzy::funcoes(float erro, float varerro)
{
    saidaFuzzy saida;
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


//Regras (25 regras)   ERRO DEPOIS VARIAÇÃO

float regra1 = min(EGRN,VGRN); //VCD
float regra2 = min(EGRN,VPQN); //VCD
float regra3 = min(EGRN,VZER); //VMD
float regra4 = min(EGRN,VPQP); //VPD
float regra5 = min(EGRN,VGRP); //VPD
float regra6 = min(EPQN,VGRN); //VCD
float regra7 = min(EPQN,VPQN); //VMD
float regra8 = min(EPQN,VZER); //VPD
float regra9 = min(EPQN,VPQP); //CENTRAL
float regra10 = min(EPQN,VGRP); //VPD
float regra11 = min(EZER,VGRN); //VMD
float regra12 = min(EZER,VPQN); //VPD
float regra13 = min(EZER,VZER); //CENTRAL
float regra14 = min(EZER,VPQP); //VPE
float regra15 = min(EZER,VGRP); //VME
float regra16 = min(EPQP,VGRN); //VPE
float regra17 = min(EPQP,VPQN); //CENTRAL
float regra18 = min(EPQP,VZER); //VPE
float regra19 = min(EPQP,VPQP); //VME
float regra20 = min(EPQP,VGRP); //VCE
float regra21 = min(EGRP,VGRN); //VPD
float regra22 = min(EGRP,VPQN); //VPE
float regra23 = min(EGRP,VZER); //VME
float regra24 = min(EGRP,VPQP); //VCE
float regra25 = min(EGRP,VGRP); //VCE

    VCD = 0.0;
    VMD = 0.0;
    VPD = 0.0;
    CEN = 0.0;
    VPE = 0.0;
    VME = 0.0;
    VCE = 0.0;
/*
VCD = max(regra6,max(regra2,regra1));  //Fazer max do max e corrigir as regras correspondentes
VMD = max(regra1,max(regra2,max(regra3,max(regra4,regra5))));
VPD = max(regra1,max(regra2,max(regra3,max(regra4,regra5))));
CEN = max(regra1,max(regra2,max(regra3,max(regra4,regra5))));
VPE = max(regra1,max(regra2,max(regra3,max(regra4,regra5))));
VME = max(regra1,max(regra2,max(regra3,max(regra4,regra5))));
VCE = max(regra1,max(regra2,max(regra3,max(regra4,regra5))));
*/

saida.VCD = max(regra1, max(regra2, regra6));  //1,2,6
saida.VMD = max(regra3, max(regra7, regra11));  //3,7,11
saida.VPD = max(regra4, max(regra5, max(regra8, max(regra10, max(regra12, regra21)))));  //4,5,8,10,12,21
saida.CEN = max(regra9, max(regra13, regra17));  //9,13,17
saida.VPE = max(regra14, max(regra16, max(regra18, regra22)));  //14,16,18,22
saida.VME = max(regra15, max(regra19, regra23));  //15,19,23
saida.VCE = max(regra20, max(regra24, regra25));  //20,24,25
return saida;
}