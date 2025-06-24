#include "TimeManager.h"

// ʹ���й���׼ʱ�� (UTC+8)
TimeManager::TimeManager()
    : timeClient(ntpUDP, "ntp.ntsc.ac.cn", 8 * 3600, 60000) {}

void TimeManager::begin()
{
    timeClient.begin();
}

void TimeManager::update()
{
    if (millis() - lastUpdate > 5000)
    { // ÿ5�����һ��
        timeClient.update();
        lastUpdate = millis();
    }
}

String TimeManager::getFormattedTime()
{
    return timeClient.getFormattedTime();
}

String TimeManager::getFormattedDate()
{
    time_t rawtime = timeClient.getEpochTime();
    struct tm *ti;
    ti = localtime(&rawtime);

    char buffer[20];
    sprintf(buffer, "%04d-%02d-%02d",
            ti->tm_year + 1900,
            ti->tm_mon + 1,
            ti->tm_mday);
    return String(buffer);
}