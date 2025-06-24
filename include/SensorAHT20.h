#pragma once
#include <Adafruit_AHTX0.h>

class TempHumiditySensor
{
public:
    bool begin();
    void readData(float &temp, float &humidity);

private:
    Adafruit_AHTX0 aht;
};