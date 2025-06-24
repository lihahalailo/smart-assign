#include "SensorBMP280.h"

bool PressureSensor::begin()
{
    return bmp.begin();
}

float PressureSensor::readPressure()
{
    return bmp.readPressure() / 100.0; // Convert to hPa
}