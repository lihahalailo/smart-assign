#include "OLEDDisplay.h"

OLEDDisplay::OLEDDisplay(uint8_t sda_pin, uint8_t scl_pin)
    : sda(sda_pin), scl(scl_pin)
{
    display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
}

bool OLEDDisplay::begin()
{
    display->begin();
    display->setFont(u8g2_font_6x10_tr);
    display->setContrast(255);
    return true;
}

void OLEDDisplay::showSensorData(float temp, float humidity, float pressure, float light)
{
    display->clearBuffer();

    display->setCursor(0, 10);
    display->print("Room Sensors");
    display->drawHLine(0, 12, 128);

    display->setCursor(0, 25);
    display->printf("T:%.1fC  H:%.1f%%", temp, humidity);

    display->setCursor(0, 40);
    display->printf("P:%.1fhPa", pressure);

    display->setCursor(0, 55);
    display->printf("L:%.1flux", light);

    display->sendBuffer();
}

void OLEDDisplay::showTimeDate(String time, String date)
{
    display->clearBuffer();

    display->setCursor(0, 10);
    display->print("Current Time");
    display->drawHLine(0, 12, 128);

    // 居中显示时间
    int timeWidth = display->getStrWidth(time.c_str());
    display->setCursor((128 - timeWidth) / 2, 35);
    display->print(time);

    // 居中显示日期
    int dateWidth = display->getStrWidth(date.c_str());
    display->setCursor((128 - dateWidth) / 2, 50);
    display->print(date);

    display->sendBuffer();
}

void OLEDDisplay::showWeather(String location, String temp, String humidity, String description)
{
    display->clearBuffer();

    display->setCursor(0, 10);
    display->print("Weather: " + location);
    display->drawHLine(0, 12, 128);

    display->setCursor(0, 25);
    display->printf("Temp: %sC", temp.c_str());

    display->setCursor(0, 40);
    display->printf("Humidity: %s%%", humidity.c_str());

    display->setCursor(0, 55);
    display->print(description);

    display->sendBuffer();
}

void OLEDDisplay::showConnectionInfo(String status, String ip)
{
    display->clearBuffer();

    display->setCursor(0, 10);
    display->print("WiFi Status");
    display->drawHLine(0, 12, 128);

    display->setCursor(0, 25);
    display->print("Status: " + status);

    display->setCursor(0, 40);
    display->print("IP: " + ip);

    display->setCursor(0, 55);
    display->print("Smart Dorm Monitor");

    display->sendBuffer();
}