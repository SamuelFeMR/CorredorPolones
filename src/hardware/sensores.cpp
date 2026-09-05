#include "sensores.h"

void sensores::begin()
{
    pinMode(SensorDireita, INPUT);
    pinMode(SensorEsquerda, INPUT);

    qtr.setTypeAnalog();

    qtr.setSensorPins(
        (const uint8_t[]){39, 34, 35, 32, 33, 25, 26, 27},
         
        NUM_SENSORS
    );

    attachInterrupt(
        digitalPinToInterrupt(PINO_ENCODER_ESQ),
        encoderEsqISR,
        RISING
    );

    attachInterrupt(
        digitalPinToInterrupt(PINO_ENCODER_DIR),
        encoderDirISR,
        RISING
    );
}

void sensores::calibrate()
{

    Serial.println();
    Serial.println("==============================");
    Serial.println(" INICIANDO CALIBRACAO");
    Serial.println(" NAO MOVA O ROBO!");
    Serial.println("==============================");

    const unsigned long TEMPO_CALIBRACAO = 10000;
    const unsigned long inicio = millis();

    while (millis() - inicio < TEMPO_CALIBRACAO)
    {
        // Calibração dos 8 sensores QTR
        qtr.calibrate();

        // -------------------------------
        // SENSOR LATERAL DIREITO
        // -------------------------------

        int direita = analogRead(SensorDireita);

        thersholdDirMin = min(
            thersholdDirMin,
            direita
        );

        thersholdDirMax = max(
            thersholdDirMax,
            direita
        );


        // -------------------------------
        // SENSOR LATERAL ESQUERDO
        // -------------------------------

        int esquerda = analogRead(SensorEsquerda);

        thersholdEsqMin = min(
            thersholdEsqMin,
            esquerda
        );

        thersholdEsqMax = max(
            thersholdEsqMax,
            esquerda
        );

        delay(5);
    }

    Serial.println();
    Serial.println("==============================");
    Serial.println(" CALIBRACAO CONCLUIDA!");
    Serial.println(" ROBO PRONTO!");
    Serial.println("==============================");
}


void sensores::update()
{
    uint16_t position = qtr.readLineWhite(sensorValues);

    erro = (int)position - 3500;

    // SENSOR LATERAL DIREITO
    int leituraDireita = analogRead(SensorDireita);

    leituraDireita = constrain(
        leituraDireita,
        thersholdDirMin,
        thersholdDirMax
    );

    if (thersholdDirMax > thersholdDirMin)
    {
        float normalizado =
            (leituraDireita - thersholdDirMin) * 1000.0f /
            (thersholdDirMax - thersholdDirMin);

        detectDireita = normalizado < 500.0f;
    }
    else
    {
        detectDireita = false;
    }

    // SENSOR LATERAL ESQUERDO
    int leituraEsquerda = analogRead(SensorEsquerda);

    leituraEsquerda = constrain(
        leituraEsquerda,
        thersholdEsqMin,
        thersholdEsqMax
    );

    if (thersholdEsqMax > thersholdEsqMin)
    {
        float normalizado =
            (leituraEsquerda - thersholdEsqMin) * 1000.0f /
            (thersholdEsqMax - thersholdEsqMin);

        detectEsquerda = normalizado < 500.0f;
    }
    else
    {
        detectEsquerda = false;
    }
}

bool sensores::direitaDetected() const
{
    return detectDireita;
}

bool sensores::esquerdaDetected() const
{
    return detectEsquerda;
}

int sensores::calculaErro() const
{
    return erro;
}

const uint16_t* sensores::getValues() const
{
    return sensorValues;
}

bool sensores::linhaDetectada() const
{
    const uint16_t LIMIAR_LINHA = 700;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensorValues[i] < LIMIAR_LINHA)
        {
            return true;
        }
    }

    return false;
}

void IRAM_ATTR encoderEsqISR()
{
    pulsosEsq++;
}

void IRAM_ATTR encoderDirISR()
{
    pulsosDir++;
}

float sensores::getVelocidadeEsq()
{
    ...
}

float sensores::getVelocidadeDir()
{
    ...
}