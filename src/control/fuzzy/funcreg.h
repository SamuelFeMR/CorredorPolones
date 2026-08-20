#pragma once

#include <Arduino.h>


struct saidaFuzzy
    {
    float VCD = 0;
    float VMD  = 0;
    float VPD = 0;
    float CEN  = 0;
    float VPE = 0;
    float VME  = 0;
    float VCE = 0;
    };

class fuzzy
{
public:
    saidaFuzzy funcoes(float erro, float varerro0);
    
private:
      // Funções de pertinência do erro
    float EGRN = 0.0;
    float EPQN = 0.0;
    float EZER = 0.0;
    float EPQP = 0.0;
    float EGRP = 0.0;

    // Funções de pertinência da variação do erro
    float VGRN = 0.0;
    float VPQN = 0.0;
    float VZER = 0.0;
    float VPQP = 0.0;
    float VGRP = 0.0;
/*
float EGRN, EPQN, EZER, EPQP, EGRP;
float VGRN, VPQN, VZER, VPQP, VGRP;*/
};