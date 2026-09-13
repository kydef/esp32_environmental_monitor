#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

struct Weather_Data
{
  char date[11];
  float tMax = 0;
  float tMin = 0;
  int rain = 0;
  int code = 0;
};

struct Weather_Data_Collection
{
  Weather_Data days[5];
  bool valid;
};

class Weather_Manager
{
private:
  bool debug = true;
  bool connect(const char *ssid, const char *password);
  void disconnect();

public:
  Weather_Manager() = default;
  Weather_Data_Collection fetchWeather(const char *ssid, const char *password);
};

#define WEATHER_DEBUG_PRINT(level, fmt, ...)                      \
  do                                                              \
  {                                                               \
    if (debug)                                                    \
    {                                                             \
      Serial.printf("[%s] [WEATHER] " fmt, level, ##__VA_ARGS__); \
    }                                                             \
  } while (0)
