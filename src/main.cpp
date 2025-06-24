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
LightSensor bh1750; // 低电平使用默认地址 0x23，拉高使用0x5c
OLEDDisplay oled(OLED_SDA, OLED_SCL);

// 创建第二个I2C实例用于OLED
TwoWire I2COLED = TwoWire(1);

void setup()
{
  Serial.begin(115200);

  // 初始化传感器I2C总线 (Wire0)
  Wire.begin(SENSOR_SDA, SENSOR_SCL);

  // 初始化OLED I2C总线 (Wire1)
  I2COLED.begin(OLED_SDA, OLED_SCL);

  // 初始化传感器
  if (!aht20.begin())
    Serial.println("AHT20 not found!");
  if (!bmp280.begin())
    Serial.println("BMP280 not found!");
  if (!bh1750.begin(Wire))
    Serial.println("BH1750 not found!"); // 传入Wire实例

  // 初始化OLED
  oled.begin();
  oled.showData(0, 0, 0, 0); // 初始显示

  Serial.println("All sensors initialized");
  //********************************************* */
  // 检测当前已连接设备的I2C地址
  Serial.println("Scanning I2C bus...");
  byte error, address;
  int nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.printf("I2C device found at address 0x%02X\n", address);
      nDevices++;
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  /************************************************* */
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