#pragma once
#include <Adafruit_BMP280.h>

class PressureSensor
{
public:
    bool begin();
    float readPressure();

private:
    Adafruit_BMP280 bmp;
};