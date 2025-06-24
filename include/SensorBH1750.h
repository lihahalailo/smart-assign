#pragma once
#include <BH1750.h>

// ���� BH1750 ��Ĭ�� I2C ��ַ
#define BH1750_DEFAULT_ADDR 0x23

class LightSensor
{
public:
    LightSensor(uint8_t addr = BH1750_DEFAULT_ADDR);
    bool begin(TwoWire &wire = Wire);
    float readLightLevel();

private:
    BH1750 sensor;
    uint8_t _addr;
};