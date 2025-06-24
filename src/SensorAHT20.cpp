#include "SensorAHT20.h"
#include <Adafruit_AHTX0.h>

bool TempHumiditySensor::begin()
{
    return aht.begin();
}

void TempHumiditySensor::readData(float &temp, float &humidity)
{
    sensors_event_t hum, t;
    aht.getEvent(&hum, &t);
    temp = t.temperature;
    humidity = hum.relative_humidity;
}