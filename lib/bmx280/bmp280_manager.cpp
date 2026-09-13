#include "bmp280_manager.h"

bool BMP280_Manager::init(int8_t address, bool debug_enabled)
{
  debug = debug_enabled;

  if (!sensor.begin(address))
  {
    BMP280_DEBUG_PRINT("ERROR", "Sensor not found. Please check wiring or I2C address.\n");
    return false;
  }

  sensor.setSampling(Adafruit_BMP280::MODE_NORMAL,
                     Adafruit_BMP280::SAMPLING_X2,
                     Adafruit_BMP280::SAMPLING_X16,
                     Adafruit_BMP280::FILTER_X16,
                     Adafruit_BMP280::STANDBY_MS_500);

  BMP280_DEBUG_PRINT("SYS", "Initialized successfully\n");
  is_initialized = true;

  return true;
}

BMP280_Sensors_Data BMP280_Manager::read()
{
  BMP280_Sensors_Data data;

  if (!is_initialized)
  {
    BMP280_DEBUG_PRINT("ERROR", "Not initialized. Please call init() first.\n");
    return data;
  }

  data.temperature = sensor.readTemperature();
  data.pressure = sensor.readPressure() / 100.0F;

  BMP280_DEBUG_PRINT("SYS", "Temperature: %f\n", data.temperature);
  BMP280_DEBUG_PRINT("SYS", "Pressure: %f\n", data.pressure);

  return data;
}
