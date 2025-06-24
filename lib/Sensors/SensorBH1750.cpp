#include "SensorBH1750.h"

LightSensor::LightSensor(uint8_t addr) : sensor(addr) {}

bool LightSensor::begin()
{
    return sensor.begin() == BH1750_OK;
}

float LightSensor::readLightLevel()
{
    return sensor.getAmbientLight();
}