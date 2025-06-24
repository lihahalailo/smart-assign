#pragma once
#include <BH1750.h>

// 定义 BH1750 的默认 I2C 地址
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