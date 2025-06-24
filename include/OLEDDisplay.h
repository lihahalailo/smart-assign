#pragma once
#include <U8g2lib.h>

class OLEDDisplay
{
public:
    OLEDDisplay(uint8_t sda_pin, uint8_t scl_pin);
    bool begin();
    void showSensorData(float temp, float humidity, float pressure, float light);
    void showTimeDate(String time, String date);
    void showWeather(String location, String temp, String humidity, String description);
    void showConnectionInfo(String status, String ip);

private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C *display;
    uint8_t sda;
    uint8_t scl;
    unsigned long lastScreenChange = 0;
    int currentScreen = 0;
};