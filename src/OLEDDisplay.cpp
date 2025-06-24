#include "OLEDDisplay.h"
#include <Wire.h>
#include <U8g2lib.h>

OLEDDisplay::OLEDDisplay(uint8_t sda_pin, uint8_t scl_pin)
    : sda(sda_pin), scl(scl_pin)
{
    // 延迟创建display对象
    display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, /*reset=*/U8X8_PIN_NONE);
}

bool OLEDDisplay::begin()
{
    // 在begin时设置I2C引脚
    display->setI2CAddress(0x3C * 2); // OLED地址通常是0x3C
    display->begin();
    return true;
}

void OLEDDisplay::showData(float temp, float humidity, float pressure, float light)
{
    display->clearBuffer();
    display->setFont(u8g2_font_ncenB08_tr);

    display->setCursor(0, 12);
    display->print("Temp: ");
    display->print(temp, 1);
    display->print(" C");

    display->setCursor(0, 28);
    display->print("Humidity: ");
    display->print(humidity, 1);
    display->print(" %");

    display->setCursor(0, 44);
    display->print("Pressure: ");
    display->print(pressure, 1);
    display->print(" hPa");

    display->setCursor(0, 60);
    display->print("Light: ");
    display->print(light, 1);
    display->print(" lx");

    display->sendBuffer();
}