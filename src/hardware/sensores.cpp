#include "sensores.h"

void begin()sensores::
{
    pinMode(2, OUTPUT);

    qtr.setTypeAnalog();

    qtr.setSensorPins(
        (const uint8_t[]){39, 34, 35, 32, 33, 25, 26, 27},
        NUM_SENSORS
    );
}

void sensores::calibrate()
{
    digitalWrite(2, HIGH);

    for (int i = 0; i < 400; i++)
    {
        qtr.calibrate();
        delay(5);
    }

    digitalWrite(2, LOW);
}

void sensores::update()
{
    uint16_t position = qtr.readLineBlack(sensorValues);

    error = (int)position - 3500;
}

int sensores::calculaErro() const
{
    return error;
}

const uint16_t* sensores::getValues() const
{
    return sensorValues;
}