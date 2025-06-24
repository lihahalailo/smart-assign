#pragma once
#include <BH1750.h>

class LightSensor
{
public:
    LightSensor(uint8_t addr = 0x23); // 0x23 �� BH1750 ��Ĭ�ϵ�ַ
    bool begin();
    float readLightLevel();

private:
    BH1750 sensor;
    uint8_t _addr;
};