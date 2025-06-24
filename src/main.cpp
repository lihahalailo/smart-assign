#include <Arduino.h>
#include <Wire.h>
#include "SensorAHT20.h"
#include "SensorBMP280.h"
#include "SensorBH1750.h"
#include "OLEDDisplay.h"
#include "WiFiManager.h"
#include "TimeManager.h"
#include "WeatherClient.h"

// 传感器I2C配置
#define SENSOR_SDA 21
#define SENSOR_SCL 22

// OLED I2C配置
#define OLED_SDA 16
#define OLED_SCL 17

// WiFi配置
const char *WIFI_SSID = "lhh";
const char *WIFI_PASSWORD = "123456789";

// 天气API配置
const char *WEATHER_API_KEY = "YOUR_API_KEY"; // 替换为您的OpenWeatherMap API密钥
const char *WEATHER_CITY = "Beijing";         // 替换为您的城市

// 传感器实例
TempHumiditySensor aht20;
PressureSensor bmp280;
LightSensor bh1750;

// 显示实例
OLEDDisplay oled(OLED_SDA, OLED_SCL);

// 网络服务实例
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
TimeManager timeManager;
WeatherClient weatherClient(WEATHER_API_KEY, WEATHER_CITY);

// 屏幕状态
unsigned long lastScreenChange = 0;
int currentScreen = 0;
const int SCREEN_DURATION = 5000; // 每个屏幕显示5秒

void setup()
{
  Serial.begin(115200);

  // 初始化OLED
  oled.begin();
  oled.showConnectionInfo("Starting...", "0.0.0.0");

  // 初始化传感器I2C总线
  Wire.begin(SENSOR_SDA, SENSOR_SCL);

  // 初始化传感器
  if (!aht20.begin())
  {
    Serial.println("AHT20 not found!");
    oled.showConnectionInfo("Sensor Error", "AHT20 failed");
    delay(2000);
  }

  if (!bmp280.begin())
  {
    Serial.println("BMP280 not found!");
    oled.showConnectionInfo("Sensor Error", "BMP280 failed");
    delay(2000);
  }

  if (!bh1750.begin())
  {
    Serial.println("BH1750 not found!");
    oled.showConnectionInfo("Sensor Error", "BH1750 failed");
    delay(2000);
  }

  // 连接WiFi
  oled.showConnectionInfo("Connecting WiFi", WIFI_SSID);
  bool wifiConnected = wifiManager.connect();

  if (wifiConnected)
  {
    Serial.println("WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(wifiManager.getLocalIP());

    // 初始化时间服务
    timeManager.begin();

    // 显示连接信息
    oled.showConnectionInfo("Connected", wifiManager.getLocalIP());
    delay(2000);

    // 获取初始天气数据
    weatherClient.updateWeather();
  }
  else
  {
    Serial.println("WiFi connection failed!");
    oled.showConnectionInfo("WiFi Failed", "Check Credentials");
    delay(2000);
  }

  Serial.println("System initialized");
}

void loop()
{
  // 读取传感器数据
  float temp, humidity, pressure, light;
  aht20.readData(temp, humidity);
  pressure = bmp280.readPressure();
  light = bh1750.readLightLevel();

  // 更新网络时间
  timeManager.update();

  // 更新天气数据（如果连接了WiFi）
  if (wifiManager.isConnected())
  {
    weatherClient.updateWeather();
  }

  // 每5秒切换显示屏幕
  if (millis() - lastScreenChange > SCREEN_DURATION)
  {
    currentScreen = (currentScreen + 1) % 4;
    lastScreenChange = millis();
  }

  // 根据当前屏幕状态显示不同内容
  switch (currentScreen)
  {
  case 0: // 传感器数据
    oled.showSensorData(temp, humidity, pressure, light);
    break;

  case 1: // 时间和日期
    if (wifiManager.isConnected())
    {
      oled.showTimeDate(timeManager.getFormattedTime(), timeManager.getFormattedDate());
    }
    else
    {
      oled.showConnectionInfo("WiFi Disconnected", "No Time Sync");
    }
    break;

  case 2: // 天气信息
    if (wifiManager.isConnected())
    {
      oled.showWeather(
          weatherClient.getLocation(),
          weatherClient.getTemperature(),
          weatherClient.getHumidity(),
          weatherClient.getWeatherDescription());
    }
    else
    {
      oled.showConnectionInfo("WiFi Disconnected", "No Weather Data");
    }
    break;

  case 3: // 连接信息
    if (wifiManager.isConnected())
    {
      oled.showConnectionInfo("Connected", wifiManager.getLocalIP());
    }
    else
    {
      oled.showConnectionInfo("Disconnected", "Reconnecting...");
      // 尝试重新连接
      wifiManager.connect();
    }
    break;
  }

  // 串口输出调试信息
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000)
  {
    Serial.printf("Temp: %.1fC | Hum: %.1f%% | Pres: %.1fhPa | Light: %.1flux\n",
                  temp, humidity, pressure, light);

    if (wifiManager.isConnected())
    {
      Serial.print("Time: ");
      Serial.print(timeManager.getFormattedTime());
      Serial.print(" | Date: ");
      Serial.println(timeManager.getFormattedDate());

      Serial.print("Weather: ");
      Serial.print(weatherClient.getLocation());
      Serial.print(" - ");
      Serial.print(weatherClient.getTemperature());
      Serial.print("°C, ");
      Serial.print(weatherClient.getHumidity());
      Serial.print("%, ");
      Serial.println(weatherClient.getWeatherDescription());
    }

    lastPrint = millis();
  }

  delay(500); // 主循环延迟
}