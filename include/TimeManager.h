#pragma once
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class TimeManager
{
public:
    TimeManager();
    void begin();
    void update();
    String getFormattedTime();
    String getFormattedDate();

private:
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    unsigned long lastUpdate = 0;
};