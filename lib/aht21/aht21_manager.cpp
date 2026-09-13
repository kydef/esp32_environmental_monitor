#include "aht21_manager.h"

bool AHT21_Manager::init(bool debug_enabled)
{
  debug = debug_enabled;

  if (!sensor.begin())
  {
    AHT21_DEBUG_PRINT("ERROR", "Sensor not found. Please check wiring or I2C address.\n");
    return false;
  }

  AHT21_DEBUG_PRINT("SYS", "Initialized successfully\n");
  is_initialized = true;

  return true;
}

AHT21_Sensors_Data AHT21_Manager::read()
{
  AHT21_Sensors_Data data;

  if (!is_initialized)
  {
    AHT21_DEBUG_PRINT("ERROR", "Not initialized. Please call init() first.\n");
    return data;
  }

  sensors_event_t humidity, temp;
  if (sensor.getEvent(&humidity, &temp))
  {
    data.humidity = humidity.relative_humidity;
    data.temperature = temp.temperature;
  }
  else
  {
    AHT21_DEBUG_PRINT("ERROR", "Failed to read data from sensor.\n");
    return data;
  }

  AHT21_DEBUG_PRINT("SYS", "Temperature: %f\n", data.temperature);
  AHT21_DEBUG_PRINT("SYS", "Humidity: %f\n", data.humidity);

  return data;
}
