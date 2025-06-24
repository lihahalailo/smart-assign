#include "WeatherClient.h"

WeatherClient::WeatherClient(const char *apiKey, const char *city)
    : _apiKey(apiKey), _city(city) {}

bool WeatherClient::updateWeather()
{
    // 每10分钟更新一次天气
    if (millis() - lastUpdate < 600000)
        return false;

    if (WiFi.status() != WL_CONNECTED)
    {
        _error = "WiFi not connected";
        return false;
    }

    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=";
    url += _city;
    url += "&appid=";
    url += _apiKey;
    url += "&units=metric";

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http.getString();

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        _temperature = String(doc["main"]["temp"].as<float>(), 1);
        _humidity = String(doc["main"]["humidity"].as<int>());
        _description = doc["weather"][0]["description"].as<String>();
        _location = doc["name"].as<String>();
        _error = "";

        lastUpdate = millis();
        http.end();
        return true;
    }
    else
    {
        _error = "HTTP error: " + String(httpCode);
        http.end();
        return false;
    }
}

String WeatherClient::getTemperature() { return _temperature; }
String WeatherClient::getHumidity() { return _humidity; }
String WeatherClient::getWeatherDescription() { return _description; }
String WeatherClient::getLocation() { return _location; }
String WeatherClient::getError() { return _error; }