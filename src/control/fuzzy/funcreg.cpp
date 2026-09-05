#include "funcreg.h"
#include "func.h"

valoresFuzzy fuzzy::funcoes(float erro, float varerro)
{
    valoresFuzzy saida;

    /*
        ================================
        FUNÇÕES DE PERTINÊNCIA - ERRO
        ================================

        Universo aproximado:
        -3500 ---------------------- +3500

        Mais sobreposição entre os conjuntos
        para deixar a correção progressiva.
    */

    EGRN = trapmf(erro, -3500, -3500, -1000, -650);
    EPQN = trimf(erro, -1000, -500, -100);
    EZER = trimf(erro, -250, 0, 250);
    EPQP = trimf(erro, 100, 500, 1000);
    EGRP = trapmf(erro, 650, 1000, 3500, 3500);


    /*
        ================================
        VARIAÇÃO DO ERRO
        ================================

        Reduzi bastante a sensibilidade.

        Antes:
            Grande = aproximadamente ±200

        Agora:
            Pequeno  ≈ ±60 até ±300
            Grande   > ±300

        Isso evita que pequenas oscilações dos
        sensores provoquem mudanças violentas.
    */

    VGRN = trapmf(varerro, -1200, -1200, -300, -180);
    VPQN = trimf(varerro, -300, -180, -60);
    VZER = trimf(varerro, -100, 0, 100);
    VPQP = trimf(varerro, 60, 180, 300);
    VGRP = trapmf(varerro, 180, 300, 1200, 1200);


    /*
        ================================
        REGRAS
        ================================
    */

    float regra1 = min(EGRN,VGRN); // VCD
    float regra2 = min(EGRN,VPQN); // VCD
    float regra3 = min(EGRN,VZER); // VMD
    float regra4 = min(EGRN,VPQP); // VPD
    float regra5 = min(EGRN,VGRP); // VPD

    float regra6 = min(EPQN,VGRN); // VCD
    float regra7 = min(EPQN,VPQN); // VMD
    float regra8 = min(EPQN,VZER); // VPD
    float regra9 = min(EPQN,VPQP); // CENTRAL
    float regra10 = min(EPQN,VGRP); // VPD

    float regra11 = min(EZER,VGRN); // VMD
    float regra12 = min(EZER,VPQN); // VPD
    float regra13 = min(EZER,VZER); // CENTRAL
    float regra14 = min(EZER,VPQP); // VPE
    float regra15 = min(EZER,VGRP); // VME

    float regra16 = min(EPQP,VGRN); // VPE
    float regra17 = min(EPQP,VPQN); // CENTRAL
    float regra18 = min(EPQP,VZER); // VPE
    float regra19 = min(EPQP,VPQP); // VME
    float regra20 = min(EPQP,VGRP); // VCE

    float regra21 = min(EGRP,VGRN); // VPD
    float regra22 = min(EGRP,VPQN); // VPE
    float regra23 = min(EGRP,VZER); // VME
    float regra24 = min(EGRP,VPQP); // VCE
    float regra25 = min(EGRP,VGRP); // VCE


    /*
        ================================
        AGREGAÇÃO DAS REGRAS
        ================================
    */

    saida.VCD =
        max(regra1,
        max(regra2,
        regra6));

    saida.VMD =
        max(regra3,
        max(regra7,
        regra11));

    saida.VPD =
        max(regra4,
        max(regra5,
        max(regra8,
        max(regra10,
        max(regra12,
        regra21)))));

    saida.CEN =
        max(regra9,
        max(regra13,
        regra17));

    saida.VPE =
        max(regra14,
        max(regra16,
        max(regra18,
        regra22)));

    saida.VME =
        max(regra15,
        max(regra19,
        regra23));

    saida.VCE =
        max(regra20,
        max(regra24,
        regra25)));


    return saida;
}
