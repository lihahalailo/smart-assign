#include <Arduino.h>
#include <Wire.h>
#include "SensorAHT20.h"
#include "SensorBMP280.h"
#include "SensorBH1750.h"
#include "OLEDDisplay.h"
#include "WiFiManager.h"
#include "TimeManager.h"
#include "WeatherClient.h"

// ������I2C����
#define SENSOR_SDA 21
#define SENSOR_SCL 22

#define LED_PIN 2
bool ledState = false;
unsigned long lastBlinkTime = 0;
const int BLINK_INTERVAL = 500; // ��˸���(ms)

// OLED I2C����
#define OLED_SDA 18
#define OLED_SCL 19

// ��������I2Cʵ��
//TwoWire I2CSensors = TwoWire(0); // I2C0
//TwoWire I2COLED = TwoWire(1);    // I2C1

// WiFi����
const char *WIFI_SSID = "lhh";
const char *WIFI_PASSWORD = "123456789";

// ����API����
const char *WEATHER_API_KEY = "YOUR_API_KEY"; // �滻Ϊ����OpenWeatherMap API��Կ
const char *WEATHER_CITY = "Beijing";         // �滻Ϊ���ĳ���

// ������ʵ��
TempHumiditySensor aht20;
PressureSensor bmp280;
LightSensor bh1750;

// ��ʾʵ��
OLEDDisplay oled(OLED_SDA, OLED_SCL);

// �������ʵ��
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
TimeManager timeManager;
WeatherClient weatherClient(WEATHER_API_KEY, WEATHER_CITY);

// ��Ļ״̬
unsigned long lastScreenChange = 0;
int currentScreen = 0;
const int SCREEN_DURATION = 5000; // ÿ����Ļ��ʾ5��

void setup()
{
  Serial.begin(115200);

  // ��ʼ��������I2C����
  Wire.begin(OLED_SDA, OLED_SCL);

  // ��ʼ��OLED
  oled.begin();
  oled.showConnectionInfo("Starting...", "0.0.0.0");

  // ��ʼ��LED����
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // ��ʼ״̬�ر�


  // ��ʼ��������I2C����
  Wire.begin(SENSOR_SDA, SENSOR_SCL);

  // ��ʼ��������
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

  // ����WiFi
  oled.showConnectionInfo("Connecting WiFi", WIFI_SSID);
  bool wifiConnected = wifiManager.connect();

  if (wifiConnected)
  {
    Serial.println("WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(wifiManager.getLocalIP());

    for (int i = 0; i < 6; i++)
    { // 6�α仯 = 3����˸
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(200);
    }

    // ��ʼ��ʱ�����
    timeManager.begin();

    // ��ʾ������Ϣ
    oled.showConnectionInfo("Connected", wifiManager.getLocalIP());
    delay(2000);

    // ��ȡ��ʼ��������
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
  // ��ȡ����������
  float temp, humidity, pressure, light;
  aht20.readData(temp, humidity);
  pressure = bmp280.readPressure();
  light = bh1750.readLightLevel();

  if (wifiManager.isConnected())
  {
    if (millis() - lastBlinkTime > BLINK_INTERVAL)
    {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      lastBlinkTime = millis();
    }
  }
  else
  {
    digitalWrite(LED_PIN, LOW); // WiFi�Ͽ�ʱ�ر�LED
  }

  // ��������ʱ��
  timeManager.update();

  // �����������ݣ����������WiFi��
  if (wifiManager.isConnected())
  {
    weatherClient.updateWeather();
  }

  // ÿ5���л���ʾ��Ļ
  if (millis() - lastScreenChange > SCREEN_DURATION)
  {
    currentScreen = (currentScreen + 1) % 4;
    lastScreenChange = millis();
  }

  // ���ݵ�ǰ��Ļ״̬��ʾ��ͬ����
  switch (currentScreen)
  {
  case 0: // ����������
    oled.showSensorData(temp, humidity, pressure, light);
    break;

  case 1: // ʱ�������
    if (wifiManager.isConnected())
    {
      oled.showTimeDate(timeManager.getFormattedTime(), timeManager.getFormattedDate());
    }
    else
    {
      oled.showConnectionInfo("WiFi Disconnected", "No Time Sync");
    }
    break;

  case 2: // ������Ϣ
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

  case 3: // ������Ϣ
    if (wifiManager.isConnected())
    {
      oled.showConnectionInfo("Connected", wifiManager.getLocalIP());
    }
    else
    {
      oled.showConnectionInfo("Disconnected", "Reconnecting...");
      // ������������
      wifiManager.connect();
    }
    break;
  }

  // �������������Ϣ
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
      Serial.print("��C, ");
      Serial.print(weatherClient.getHumidity());
      Serial.print("%, ");
      Serial.println(weatherClient.getWeatherDescription());
    }

    lastPrint = millis();
  }

  delay(500); // ��ѭ���ӳ�
}