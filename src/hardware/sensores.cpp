#include "sensores.h"

volatile int32_t pulsosEsq = 0;
volatile int32_t pulsosDir = 0;

volatile int32_t pulsosDistanciaEsq = 0;
volatile int32_t pulsosDistanciaDir = 0;

float distanciaInicial = 0.0f;

void IRAM_ATTR encoderEsqISR()
{
    // C1 sofreu uma borda de subida.
    //
    // C2 determina o sentido.
    if (digitalRead(esqC2))
    {
        pulsosEsq++;
        pulsosDistanciaEsq++;
    }
    else
    {
        pulsosEsq--;
        pulsosDistanciaEsq--;
    }
}


void IRAM_ATTR encoderDirISR()
{
    // C1 sofreu uma borda de subida.
    //
    // C2 determina o sentido.
    if (digitalRead(dirC2))
    {
        pulsosDir++;
        pulsosDistanciaDir++;
    }
    else
    {
        pulsosDir--;
        pulsosDistanciaDir--;
    }
}

void sensores::begin()
{
    // -------------------------------
    // SENSORES LATERAIS
    // -------------------------------

    pinMode(SensorDireita, INPUT);
    pinMode(SensorEsquerda, INPUT);
    
    // -------------------------------
    // ENCODERS
    // -------------------------------

    pinMode(esqC1, INPUT);
    pinMode(esqC2, INPUT);

    pinMode(dirC1, INPUT);
    pinMode(dirC2, INPUT);


    // -------------------------------
    // INTERRUPÇÃO ESQUERDA
    // -------------------------------

    attachInterrupt(
        digitalPinToInterrupt(esqC1),
        encoderEsqISR,
        RISING
    );

    // -------------------------------
    // INTERRUPÇÃO DIREITA
    // -------------------------------

    attachInterrupt(
        digitalPinToInterrupt(dirC1),
        encoderDirISR,
        RISING
    );


    qtr.setTypeAnalog();

    qtr.setSensorPins(
        (const uint8_t[]){39, 34, 35, 32, 33, 25, 26, 27},
         
        NUM_SENSORS
    );
}

void sensores::calibrate()
{

    Serial.println();
    Serial.println("==============================");
    Serial.println("ROBO GIRANDO PARA CALIBRACAO...");
    Serial.println("==============================");

    const unsigned long TEMPO_CALIBRACAO = 3000;
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

// ATUALIZA VELOCIDADE
void sensores::atualizarVelocidade()
{
    static unsigned long ultimoCalculo = 0;

    unsigned long agora = millis();

    unsigned long intervalo =
        agora - ultimoCalculo;


    // Ainda não passou tempo suficiente
    if (intervalo < INTERVALO_VELOCIDADE_MS)
    {
        return;
    }


    ultimoCalculo = agora;

    // COPIA ATÔMICA DOS CONTADORES
    int32_t pulsosEsqAtual;
    int32_t pulsosDirAtual;

    noInterrupts();

    pulsosEsqAtual = pulsosEsq;
    pulsosDirAtual = pulsosDir;

    pulsosEsq = 0;
    pulsosDir = 0;

    interrupts();

    // CONVERSÃO PARA RPM
    float intervaloSegundos =
        intervalo / 1000.0f;


    float voltasEsq =
        pulsosEsqAtual /
        ENCODER_PULSOS_POR_VOLTA;


    float voltasDir =
        pulsosDirAtual /
        ENCODER_PULSOS_POR_VOLTA;


    velEsq =
        (voltasEsq / intervaloSegundos) * 60.0f;


    velDir =
        (voltasDir / intervaloSegundos) * 60.0f;
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

    // VELOCIDADE DOS MOTORES
    atualizarVelocidade();
}

// DETECÇÃO DOS SENSORES LATERAIS
bool sensores::direitaDetected() const
{
    return detectDireita;
}

bool sensores::esquerdaDetected() const
{
    return detectEsquerda;
}

// ERRO
int sensores::calculaErro() const
{
    return erro;
}

// VALORES DOS SENSORES
const uint16_t* sensores::getValues() const
{
    return sensorValues;
}

// LINHA DETECTADA
bool sensores::linhaDetectada() const
{
    const uint16_t LIMIAR_LINHA = 800;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensorValues[i] < LIMIAR_LINHA)
        {
            return true;
        }
    }

    return false;
}

// VELOCIDADE ESQUERDA
float sensores::getVelocidadeEsq() const
{
    return velEsq;
}

// VELOCIDADE DIREITA
float sensores::getVelocidadeDir() const
{
    return velDir;
}

float sensores::getDistanciaEsqMm() const
{
    int32_t pulsos;

    noInterrupts();
    pulsos = pulsosDistanciaEsq;
    interrupts();

    float circunferencia = PI * DIAMETRO_RODA_MM;

    return (pulsos / ENCODER_PULSOS_POR_VOLTA) * circunferencia;
}

float sensores::getDistanciaDirMm() const
{
    int32_t pulsos;

    noInterrupts();
    pulsos = pulsosDistanciaDir;
    interrupts();

    float circunferencia = PI * DIAMETRO_RODA_MM;

    return (pulsos / ENCODER_PULSOS_POR_VOLTA) * circunferencia;
}

void sensores::zerarDistancia()
{
    noInterrupts();

    pulsosDistanciaEsq = 0;
    pulsosDistanciaDir = 0;

    interrupts();
}