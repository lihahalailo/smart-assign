#include "SensorBH1750.h"

LightSensor::LightSensor(uint8_t addr) : _addr(addr), sensor(addr) {}

bool LightSensor::begin(TwoWire &wire)
{
    return sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, &wire);
}

float LightSensor::readLightLevel()
{
    return sensor.readLightLevel();
}