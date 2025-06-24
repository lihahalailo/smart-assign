#include "SensorBMP280.h"
#include <Adafruit_BMP280.h>

bool PressureSensor::begin()
{
    return bmp.begin();
}

float PressureSensor::readPressure()
{
    return bmp.readPressure() / 100.0; // ×ª»»Îª hPa
}