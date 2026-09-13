#pragma once
#include <Arduino.h>

class ENS160;

struct ENS160_Sensors_Data
{
  uint8_t aqi = 0;
  uint16_t tvoc = 0;
  uint16_t eco2 = 0;
};

class ENS160_Manager
{
private:
  bool debug = true;
  bool is_initialized = false;
  ENS160 *sensor = nullptr;

public:
  ENS160_Manager();
  ~ENS160_Manager();
  bool init(int8_t address, bool debug_enabled = true);
  ENS160_Sensors_Data read();
};

#define ENS160_DEBUG_PRINT(level, fmt, ...)                      \
  do                                                             \
  {                                                              \
    if (debug)                                                   \
    {                                                            \
      Serial.printf("[%s] [ENS160] " fmt, level, ##__VA_ARGS__); \
    }                                                            \
  } while (0)
