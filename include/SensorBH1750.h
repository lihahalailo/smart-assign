#pragma once
#include <BH1750FVI.h>

class LightSensor
{
public:
    LightSensor(uint8_t addr = BH1750_DEFAULT_I2CADDR);
    bool begin();
    float readLightLevel();

private:
    BH1750FVI sensor;
};