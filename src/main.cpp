#include <Arduino.h>
#include <Wire.h>
#include "SensorAHT20.h"
#include "SensorBMP280.h"
#include "SensorBH1750.h"
#include "OLEDDisplay.h"

// 传感器I2C配置
#define SENSOR_SDA 21
#define SENSOR_SCL 22

// OLED I2C配置
#define OLED_SDA 16
#define OLED_SCL 17

TempHumiditySensor aht20;
PressureSensor bmp280;
LightSensor bh1750;
OLEDDisplay oled(OLED_SDA, OLED_SCL);

void setup()
{
  Serial.begin(115200);

  // 初始化传感器I2C总线
  Wire.begin(SENSOR_SDA, SENSOR_SCL);

  // 初始化传感器
  if (!aht20.begin())
    Serial.println("AHT20 not found!");
  if (!bmp280.begin())
    Serial.println("BMP280 not found!");
  if (!bh1750.begin())
    Serial.println("BH1750 not found!");

  // 初始化OLED
  oled.begin();
  oled.showData(0, 0, 0, 0); // 初始显示
}

void loop()
{
  float temp, humidity, pressure, light;

  // 读取传感器数据
  aht20.readData(temp, humidity);
  pressure = bmp280.readPressure();
  light = bh1750.readLightLevel();

  // 串口输出
  Serial.printf("Temp: %.1fC | Hum: %.1f%% | Pres: %.1fhPa | Light: %.1flux\n",
                temp, humidity, pressure, light);

  // OLED显示
  oled.showData(temp, humidity, pressure, light);

  delay(2000); // 2秒更新一次
}