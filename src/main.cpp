#include <Arduino.h>
#include <Wire.h>
#include <config.h>
#include <bmp280_manager.h>
#include <aht21_manager.h>
#include <ens160_manager.h>
#include <display_manager.h>

bool initialized = false;
BMP280_Manager bmp280_manager;
AHT21_Manager aht21_manager;
ENS160_Manager ens160_manager;
Weather_Manager weather_manager;
Display_Manager display_manager;

void init()
{
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(100000);
  delay(1000);

  bool bmp280_initialized = bmp280_manager.init(BMP280_I2C_ADDRESS, DEBUG);
  delay(1000);

  bool aht21_initialized = aht21_manager.init(DEBUG);
  delay(1000);

  bool ens160_initialized = ens160_manager.init(ENS160_I2C_ADDRESS, DEBUG);
  delay(1000);

  if (bmp280_initialized && aht21_initialized && ens160_initialized)
  {
    initialized = true;
    DEBUG_PRINT("SYS", "All sensors initialized successfully.\n");
  }
  else
  {
    DEBUG_PRINT("ERROR", "One or more sensors failed to initialize.\n");
  }

  Display_Config display_config;
  display_config.cs_pin = EPD_CS;
  display_config.dc_pin = EPD_DC;
  display_config.rst_pin = EPD_RST;
  display_config.busy_pin = EPD_BUSY;
  display_manager.init(display_config);
  delay(1000);
}

Display_Env_Data read_sensors() {
  Display_Env_Data data;

  if (!initialized)
  {
    DEBUG_PRINT("ERROR", "Sensors not initialized. Please check the setup.\n");
    delay(5000);
    return data;
  }

  BMP280_Sensors_Data bmp280_data = bmp280_manager.read();
  delay(100);

  AHT21_Sensors_Data aht21_data = aht21_manager.read();
  delay(100);

  ENS160_Sensors_Data ens160_data = ens160_manager.read();

  data.temperature = bmp280_data.temperature;
  data.humidity = aht21_data.humidity;
  data.pressure = bmp280_data.pressure;
  data.aqi = ens160_data.aqi;
  data.tvoc = ens160_data.tvoc;
  data.eco2 = ens160_data.eco2;

  return data;
}

void setup()
{
  init();

  Display_Env_Data env_data = read_sensors();

  Weather_Data_Collection weather_data = weather_manager.fetchWeather(WIFI_SSID, WIFI_PASSWORD);

  display_manager.render(env_data, weather_data);

  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * S_TO_uS_FACTOR);
  DEBUG_PRINT("SYS", "Entering Deep Sleep...");
  if (DEBUG) delay(3000); 
  esp_deep_sleep_start();
}

void loop()
{
}
