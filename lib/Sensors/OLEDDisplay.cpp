#include "OLEDDisplay.h"

OLEDDisplay::OLEDDisplay(uint8_t sda_pin, uint8_t scl_pin)
    : display(U8G2_R0, /*reset=*/U8X8_PIN_NONE, scl_pin, sda_pin) {}

bool OLEDDisplay::begin()
{
    display.begin();
    return true;
}

void OLEDDisplay::showData(float temp, float humidity, float pressure, float light)
{
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);

    display.setCursor(0, 12);
    display.print("Temp: ");
    display.print(temp, 1);
    display.print(" C");

    display.setCursor(0, 28);
    display.print("Humidity: ");
    display.print(humidity, 1);
    display.print(" %");

    display.setCursor(0, 44);
    display.print("Pressure: ");
    display.print(pressure, 1);
    display.print(" hPa");

    display.setCursor(0, 60);
    display.print("Light: ");
    display.print(light, 1);
    display.print(" lx");

    display.sendBuffer();
}