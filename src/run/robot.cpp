#include "robot.h"

// BEGIN
void Robot::begin()
{
    Serial.begin(115200);

    // Garantia extra antes de começar
    pararMotores();

    // SENSORES
    sensoresRobot.begin();

    // MOTORES
    configurarMotores();

    // CALIBRACAO
    Serial.println("Iniciando calibracao...");

    giroCalib();

    sensoresRobot.calibrate();

    // Garantia extra depois da calibração
    pararMotores();


    // INICIALIZACAO DO CONTROLE
    erroAnterior = 0;
    primeiroCiclo = true;

    Serial.println("Robot pronto!");
}


// UPDATE
void Robot::update()
{
    // 1. LEITURA DOS SENSORES
    sensoresRobot.update();


    if (!sensoresRobot.linhaDetectada())
    {
        pararMotores();

        Serial.println("LINHA PERDIDA!");

        return;
    }

    // 2. CALCULO DO ERRO
    int erroAtual = sensoresRobot.calculaErro();


    // 3. CALCULO DA VARIACAO DO ERRO
    int varErro = 0;

    if (primeiroCiclo)
    {
        varErro = 0;
        primeiroCiclo = false;
    }
    else
    {
        varErro = erroAtual - erroAnterior;
    }


    // 4. INFERENCIA FUZZY
    valoresFuzzy regras = controladorFuzzy.funcoes(erroAtual, varErro);


    // 5. DEFUZZIFICACAO
    controladorDefuzzy.calcularPWM(regras, PWM_BASE);


    // 6. ATUACAO DOS MOTORES
    motorEsquerdo(controladorDefuzzy.pwmEsq);

    motorDireito(controladorDefuzzy.pwmDir);

    // 7. ATUALIZA ESTADO
    erroAnterior = erroAtual;


    // DEBUG
    Serial.print("Erro: ");
    Serial.print(erroAtual);

    Serial.print(" | dErro: ");
    Serial.print(varErro);

    Serial.print(" | PWM Esq: ");
    Serial.print(controladorDefuzzy.pwmEsq);

    Serial.print(" | PWM Dir: ");
    Serial.println(controladorDefuzzy.pwmDir);
    
}


// CONFIGURACAO DOS MOTORES
void Robot::configurarMotores()
{
    // Direcao motor esquerdo
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    // Direcao motor direito
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    // PWM
    ledcSetup(
        PWM_CHANNEL_A,
        PWM_FREQ,
        PWM_RES
    );

    ledcSetup(
        PWM_CHANNEL_B,
        PWM_FREQ,
        PWM_RES
    );


    ledcAttachPin(
        PWMA,
        PWM_CHANNEL_A
    );

    ledcAttachPin(
        PWMB,
        PWM_CHANNEL_B
    );

    // Sentido inicial
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);

    ledcWrite(
        PWM_CHANNEL_A,
        0
    );

    ledcWrite(
        PWM_CHANNEL_B,
        0
    );
}

// MOTOR ESQUERDO
void Robot::motorEsquerdo(int pwm)
{
    pwm = constrain(
        pwm,
        0,
        255
    );


    // Frente
    digitalWrite(
        AIN1,
        HIGH
    );

    digitalWrite(
        AIN2,
        LOW
    );


    ledcWrite(
        PWM_CHANNEL_A,
        pwm
    );
}

// MOTOR DIREITO
void Robot::motorDireito(int pwm)
{
    pwm = constrain(
        pwm,
        0,
        255
    );


    // Frente
    digitalWrite(
        BIN1,
        HIGH
    );

    digitalWrite(
        BIN2,
        LOW
    );


    ledcWrite(
        PWM_CHANNEL_B,
        pwm
    );
}

// PARAR MOTORES
void Robot::giroCalib()
{
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2,LOW);
    ledcWrite(
        PWM_CHANNEL_A,
        1024
    );

    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2,HIGH);
    ledcWrite(
        PWM_CHANNEL_B,
        1024    
    );
}

// PARAR MOTORES
void Robot::pararMotores()
{
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2,HIGH);
    ledcWrite(
        PWM_CHANNEL_A,
        0
    );

    digitalWrite(BIN1,HIGH);
    digitalWrite(BIN2,HIGH);
    ledcWrite(
        PWM_CHANNEL_B,
        0
    );
}