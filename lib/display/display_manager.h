#pragma once
#include <GxEPD2_BW.h>
#include <weather_manager.h>

struct Display_Config
{
  int16_t cs_pin;
  int16_t dc_pin;
  int16_t rst_pin;
  int16_t busy_pin;
};

struct Display_Env_Data
{
  float temperature = 0.0f;
  float humidity = 0.0f;
  float pressure = 0.0f;
  uint8_t aqi = 0;
  uint16_t tvoc = 0;
  uint16_t eco2 = 0;
};

typedef GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> DisplayType;

class Display_Manager
{
private:
  DisplayType *display = nullptr;
  bool debug = true;
  bool is_initialized = false;

  const char *get_aqi_status_text(uint8_t aqi);
  void draw_ui(const Display_Env_Data &data, const Weather_Data_Collection &weather);

public:
  void init(Display_Config config, bool is_initial_boot = true);
  void render(const Display_Env_Data &env_data, const Weather_Data_Collection &weather);
  void hibernate();
  void clear();
};

#define DISPLAY_DEBUG_PRINT(level, fmt, ...)                      \
  do                                                              \
  {                                                               \
    if (debug)                                                    \
    {                                                             \
      Serial.printf("[%s] [DISPLAY] " fmt, level, ##__VA_ARGS__); \
    }                                                             \
  } while (0)
