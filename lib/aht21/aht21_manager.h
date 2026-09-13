#pragma once
#include <Adafruit_AHTX0.h>

struct AHT21_Sensors_Data
{
  float temperature = 0.0f;
  float humidity = 0.0f;
};

class AHT21_Manager
{
private:
  bool debug = true;
  bool is_initialized = false;
  Adafruit_AHTX0 sensor;

public:
  bool init(bool debug = true);
  AHT21_Sensors_Data read();
};

#define AHT21_DEBUG_PRINT(level, fmt, ...)                      \
  do                                                            \
  {                                                             \
    if (debug)                                                  \
    {                                                           \
      Serial.printf("[%s] [AHT21] " fmt, level, ##__VA_ARGS__); \
    }                                                           \
  } while (0)
