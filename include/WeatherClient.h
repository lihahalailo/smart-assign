#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class WeatherClient
{
public:
    WeatherClient(const char *apiKey, const char *city);
    bool updateWeather();
    String getTemperature();
    String getHumidity();
    String getWeatherDescription();
    String getLocation();
    String getError();

private:
    const char *_apiKey;
    const char *_city;
    String _temperature = "N/A";
    String _humidity = "N/A";
    String _description = "N/A";
    String _location = "N/A";
    String _error = "";
    unsigned long lastUpdate = 0;
};