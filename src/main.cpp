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
LightSensor bh1750; // �͵�ƽʹ��Ĭ�ϵ�ַ 0x23������ʹ��0x5c
OLEDDisplay oled(OLED_SDA, OLED_SCL);

// �����ڶ���I2Cʵ������OLED
TwoWire I2COLED = TwoWire(1);

void setup()
{
  Serial.begin(115200);

  // ��ʼ��������I2C���� (Wire0)
  Wire.begin(SENSOR_SDA, SENSOR_SCL);

  // ��ʼ��OLED I2C���� (Wire1)
  I2COLED.begin(OLED_SDA, OLED_SCL);

  // ��ʼ��������
  if (!aht20.begin())
    Serial.println("AHT20 not found!");
  if (!bmp280.begin())
    Serial.println("BMP280 not found!");
  if (!bh1750.begin(Wire))
    Serial.println("BH1750 not found!"); // ����Wireʵ��

  // ��ʼ��OLED
  oled.begin();
  oled.showData(0, 0, 0, 0); // ��ʼ��ʾ

  Serial.println("All sensors initialized");
  //********************************************* */
  // ��⵱ǰ�������豸��I2C��ַ
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