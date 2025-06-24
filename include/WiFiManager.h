#pragma once
#include <WiFi.h>

class WiFiManager
{
public:
    WiFiManager(const char *ssid, const char *password);
    bool connect(int timeout = 10000);
    bool isConnected();
    void disconnect();
    String getLocalIP();

private:
    const char *_ssid;
    const char *_password;
};