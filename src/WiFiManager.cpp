#include "WiFiManager.h"

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : _ssid(ssid), _password(password) {}

bool WiFiManager::connect(int timeout)
{
    WiFi.begin(_ssid, _password);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout)
    {
        delay(500);
    }

    return isConnected();
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::disconnect()
{
    WiFi.disconnect();
}

String WiFiManager::getLocalIP()
{
    return WiFi.localIP().toString();
}