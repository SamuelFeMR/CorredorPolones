#include "robot.h"


// =================================================
// BEGIN
// =================================================

void Robot::begin()
{
    Serial.begin(115200);

    // SENSORES
    sensoresRobot.begin();

    // MOTORES
    configurarMotores();

    pararMotores();


    // =================================================
    // CALIBRACAO
    // =================================================

    Serial.println("Iniciando calibracao...");

    giroCalib();

    sensoresRobot.calibrate();

    pararMotores();


    // =================================================
    // CONTROLE
    // =================================================

    controladorPID.reset();

    Serial.println("Robot pronto!");
    inicioPista = millis();
    pistaIniciada = true;
}


// =================================================
// UPDATE
// =================================================

void Robot::update()
{
    // =================================================
    // LEITURA DOS SENSORES
    // =================================================

    sensoresRobot.update();

    if (pistaIniciada && millis() - inicioPista >= TEMPO_PISTA_MS)
    {
        pararMotores();

        roboParado = true;
        pistaIniciada = false;

        Serial.println("================================");
        Serial.println("40 SEGUNDOS DE PISTA!");
        Serial.println("ROBO PARADO!");
        Serial.println("================================");

        return;
    }


    if (roboParado)
    {
        pararMotores();
        return;
    }


    // =================================================
    // SENSOR LATERAL DIREITO
    // =================================================

    bool direitaAtual =
        sensoresRobot.direitaDetected();


    if (direitaAtual && !direitaAnterior)
    {
        contadorDeteccoesDireita++;

        Serial.print("DETECCAO DIREITA: ");
        Serial.println(contadorDeteccoesDireita);


        if (contadorDeteccoesDireita ==
            DETECCOES_NECESSARIAS)
        {
            inicioContagemParada = millis();

            contandoParada = true; 

            Serial.println("DETECCOES!");
            Serial.println("INICIANDO CONTAGEM PARA PARADA");
        }
    }


    direitaAnterior = direitaAtual;


    // =================================================
    // CONTAGEM DA DISTANCIA
    // =================================================

    if (contandoParada)
    {
        if (millis() - inicioContagemParada >=
            TEMPO_ATE_PARADA_MS)
        {
            pararMotores();

            contandoParada = false;
            roboParado = true;

            Serial.println("ROBO PARADO!");

            return;
        }
    }


    // =================================================
    // LINHA PERDIDA
    // =================================================

    if (!sensoresRobot.linhaDetectada())
    {
        if (!linhaPerdida)
        {
            linhaPerdida = true;

            inicioLinhaPerdida =
                millis();

            Serial.println(
                "LINHA PERDIDA! Mantendo ultimo PWM..."
            );
        }


        // Mantem o ultimo comando
        if (millis() - inicioLinhaPerdida <
            TEMPO_RECUPERACAO_MS)
        {
            motorEsquerdo(ultimoPwmEsq);
            motorDireito(ultimoPwmDir);

            return;
        }


        // Linha nao recuperada
        pararMotores();

        controladorPID.reset();

        Serial.println(
            "LINHA NAO RECUPERADA! MOTORES PARADOS."
        );

        return;
    }


    // Linha encontrada novamente
    linhaPerdida = false;


    // =================================================
    // CALCULO DO ERRO
    // =================================================

    int erroAtual =
        sensoresRobot.calculaErro();


    // =================================================
    // CONTROLE
    // =================================================

    float correcao = 0.0f;

    int pwmEsq = 0;
    int pwmDir = 0;


    // =================================================
    // FUZZY
    // =================================================

    if (CONTROLE_ATUAL == CONTROLE_FUZZY)
    {
        // O fuzzy precisa da variacao do erro.
        static int erroAnteriorFuzzy = 0;

        int varErro =
            erroAtual - erroAnteriorFuzzy;

        erroAnteriorFuzzy =
            erroAtual;


        valoresFuzzy regras =
            controladorFuzzy.funcoes(
                erroAtual,
                varErro
            );


        controladorDefuzzy.calcularPWM(
            regras,
            PWM_BASE
        );


        pwmEsq =
            controladorDefuzzy.pwmEsq;

        pwmDir =
            controladorDefuzzy.pwmDir;

        correcao =
            controladorDefuzzy.getCorrecao();
    }


    // =================================================
    // PD
    // =================================================

    else if (CONTROLE_ATUAL == CONTROLE_PID)
    {
        correcao =
            controladorPID.calcular(
                erroAtual
            );


        // Correcao positiva:
        //
        // Esquerdo aumenta
        // Direito diminui
        //
        // Correcao negativa:
        //
        // Esquerdo diminui
        // Direito aumenta

        pwmEsq =
            PWM_BASE + correcao;

        pwmDir =
            PWM_BASE - correcao;


        pwmEsq =
            constrain(
                pwmEsq,
                0,
                PWM_MAX
            );


        pwmDir =
            constrain(
                pwmDir,
                0,
                PWM_MAX
            );
    }


    // =================================================
    // ATUACAO
    // =================================================

    motorEsquerdo(pwmEsq);
    motorDireito(pwmDir);


    // Guarda o ultimo PWM
    // para recuperacao da linha

    ultimoPwmEsq = pwmEsq;
    ultimoPwmDir = pwmDir;


    // =================================================
    // DEBUG
    // =================================================

    if (millis() - ultimoDebug >=
        INTERVALO_DEBUG_MS)
    {
        ultimoDebug = millis();


        const uint16_t* valores =
            sensoresRobot.getValues();


        Serial.print("SENSORES: ");


        for (int i = 0; i < NUM_SENSORS; i++)
        {
            Serial.print(valores[i]);


            if (i < NUM_SENSORS - 1)
            {
                Serial.print(" | ");
            }
        }


        Serial.print(" || Erro: ");
        Serial.print(erroAtual);


        Serial.print(" | Correcao: ");
        Serial.print(correcao);


        Serial.print(" | PWM Esq: ");
        Serial.print(pwmEsq);


        Serial.print(" | PWM Dir: ");
        Serial.print(pwmDir);


        // =================================================
        // DEBUG SENSORES LATERAIS
        // =================================================

        Serial.print(" || LATERAIS | ");

        Serial.print("D: ");
        Serial.print(sensoresRobot.getLeituraDireita());

        Serial.print(" [");
        Serial.print(sensoresRobot.getMinDireita());
        Serial.print("-");
        Serial.print(sensoresRobot.getMaxDireita());
        Serial.print("] ");

        Serial.print(
            sensoresRobot.direitaDetected()
                ? "DETECTOU"
                : "NAO"
        );

        Serial.print(" | E: ");
        Serial.print(sensoresRobot.getLeituraEsquerda());

        Serial.print(" [");
        Serial.print(sensoresRobot.getMinEsquerda());
        Serial.print("-");
        Serial.print(sensoresRobot.getMaxEsquerda());
        Serial.print("] ");

        Serial.println(
            sensoresRobot.esquerdaDetected()
                ? "DETECTOU"
                : "NAO"
        );
    }
}


// =================================================
// CONFIGURACAO DOS MOTORES
// =================================================

void Robot::configurarMotores()
{
    // Motor esquerdo
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);


    // Motor direito
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


    // Sentido para frente
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);


    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);


    // Comeca parado
    ledcWrite(
        PWM_CHANNEL_A,
        0
    );


    ledcWrite(
        PWM_CHANNEL_B,
        0
    );
}


// =================================================
// MOTOR ESQUERDO
// =================================================

void Robot::motorEsquerdo(int pwm)
{
    pwm =
        constrain(
            pwm,
            0,
            PWM_MAX
        );


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


// =================================================
// MOTOR DIREITO
// =================================================

void Robot::motorDireito(int pwm)
{
    pwm =
        constrain(
            pwm,
            0,
            PWM_MAX
        );


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


// =================================================
// GIRO DE CALIBRACAO
// =================================================

void Robot::giroCalib()
{
    digitalWrite(
        AIN1,
        LOW
    );

    digitalWrite(
        AIN2,
        LOW
    );

    ledcWrite(
        PWM_CHANNEL_A,
        180
    );


    digitalWrite(
        BIN1,
        LOW
    );

    digitalWrite(
        BIN2,
        LOW
    );

    ledcWrite(
        PWM_CHANNEL_B,
        180
    );
}


// =================================================
// PARAR MOTORES
// =================================================

void Robot::pararMotores()
{
    digitalWrite(
        AIN1,
        HIGH
    );

    digitalWrite(
        AIN2,
        HIGH
    );

    ledcWrite(
        PWM_CHANNEL_A,
        0
    );


    digitalWrite(
        BIN1,
        HIGH
    );

    digitalWrite(
        BIN2,
        HIGH
    );

    ledcWrite(
        PWM_CHANNEL_B,
        0
    );
}