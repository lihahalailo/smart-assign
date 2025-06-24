#include <Arduino.h>
#include <Wire.h>
#include "SensorAHT20.h"
#include "SensorBMP280.h"
#include "SensorBH1750.h"
#include "OLEDDisplay.h"

// ������I2C����
#define SENSOR_SDA 21
#define SENSOR_SCL 22

// OLED I2C����
#define OLED_SDA 16
#define OLED_SCL 17

TempHumiditySensor aht20;
PressureSensor bmp280;
LightSensor bh1750;
OLEDDisplay oled(OLED_SDA, OLED_SCL);

void setup()
{
  Serial.begin(115200);

  // ��ʼ��������I2C����
  Wire.begin(SENSOR_SDA, SENSOR_SCL);

  // ��ʼ��������
  if (!aht20.begin())
    Serial.println("AHT20 not found!");
  if (!bmp280.begin())
    Serial.println("BMP280 not found!");
  if (!bh1750.begin())
    Serial.println("BH1750 not found!");

  // ��ʼ��OLED
  oled.begin();
  oled.showData(0, 0, 0, 0); // ��ʼ��ʾ
}

void loop()
{
  float temp, humidity, pressure, light;

  // ��ȡ����������
  aht20.readData(temp, humidity);
  pressure = bmp280.readPressure();
  light = bh1750.readLightLevel();

  // �������
  Serial.printf("Temp: %.1fC | Hum: %.1f%% | Pres: %.1fhPa | Light: %.1flux\n",
                temp, humidity, pressure, light);

  // OLED��ʾ
  oled.showData(temp, humidity, pressure, light);

  delay(2000); // 2�����һ��
}