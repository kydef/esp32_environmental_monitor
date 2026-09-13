#pragma once
#include <Adafruit_BMP280.h>

struct BMP280_Sensors_Data
{
  float temperature = 0.0f;
  float pressure = 0.0f;
};

class BMP280_Manager
{
private:
  bool debug = true;
  bool is_initialized = false;
  Adafruit_BMP280 sensor;

public:
  bool init(int8_t address, bool debug = true);
  BMP280_Sensors_Data read();
};

#define BMP280_DEBUG_PRINT(level, fmt, ...)                      \
  do                                                             \
  {                                                              \
    if (debug)                                                   \
    {                                                            \
      Serial.printf("[%s] [BMP280] " fmt, level, ##__VA_ARGS__); \
    }                                                            \
  } while (0)
