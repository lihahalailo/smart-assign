#include "SensorBH1750.h"

LightSensor::LightSensor(uint8_t addr) : _addr(addr), sensor(addr) {}

bool LightSensor::begin()
{
    // ʹ����ȷ�� begin() ����ǩ��
    return sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, _addr);
}

float LightSensor::readLightLevel()
{
    return sensor.readLightLevel();
}