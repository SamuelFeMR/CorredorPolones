#pragma once

#include <Arduino.h>
#include <QTRSensors.h>

#define NUM_SENSORS 8

class Sensors
{
public:
    void begin();
    void calibrate();

    void update();

    int getError() const;

    const uint16_t* getValues() const;

private:
    QTRSensors qtr;
    uint16_t sensorValues[NUM_SENSORS];
    int error = 0;
};