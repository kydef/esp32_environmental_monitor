#include "ens160_manager.h"
#include <ens160.h>

ENS160_Manager::ENS160_Manager()
{
  sensor = new ENS160();
}

ENS160_Manager::~ENS160_Manager()
{
  if (sensor)
  {
    delete sensor;
  }
}

bool ENS160_Manager::init(int8_t address, bool debug_enabled)
{
  debug = debug_enabled;

  if (!sensor)
    return false;

  if (debug)
  {
    sensor->enableDebugging(Serial);
  }

  sensor->begin(&Wire, address);
  ENS160_DEBUG_PRINT("SYS", "Status after begin: 0x%02X\n", sensor->deviceStatus);

  sensor->init();
  ENS160_DEBUG_PRINT("SYS", "Status after begin: 0x%02X\n", sensor->deviceStatus);

  ENS160_DEBUG_PRINT("SYS", "Start STANDARD measure...\n");
  sensor->startStandardMeasure();

  ENS160_DEBUG_PRINT("SYS", "Initialized successfully.\n");
  is_initialized = true;

  return true;
}

ENS160_Sensors_Data ENS160_Manager::read()
{
  ENS160_Sensors_Data data;

  if (!is_initialized || !sensor)
  {
    ENS160_DEBUG_PRINT("ERROR", "Not initialized. Please call init() first.\n");
    return data;
  }

  bool is_update_ok = sensor->update() == RESULT_OK;
  bool has_new_data = sensor->hasNewData();
  ENS160_DEBUG_PRINT("SYS", "Update status: %d\n", is_update_ok);
  ENS160_DEBUG_PRINT("SYS", "Has new data: %d\n", has_new_data);

  if (is_update_ok && has_new_data)
  {
    data.aqi = (uint8_t)sensor->getAirQualityIndex_UBA();
    data.tvoc = sensor->getTvoc();
    data.eco2 = sensor->getEco2();
  }
  else
  {
    ENS160_DEBUG_PRINT("ERROR", "Failed to read data.\n");
    return data;
  }

  ENS160_DEBUG_PRINT("SYS", "AQI: %d\n", data.aqi);
  ENS160_DEBUG_PRINT("SYS", "TVOC: %d\n", data.tvoc);
  ENS160_DEBUG_PRINT("SYS", "ECO2: %d\n", data.eco2);

  return data;
}
