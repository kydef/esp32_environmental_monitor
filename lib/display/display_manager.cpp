#include "display_manager.h"

void Display_Manager::init(Display_Config config, bool is_initial_boot)
{
  display = new DisplayType(GxEPD2_290_BS(config.cs_pin, config.dc_pin, config.rst_pin, config.busy_pin));
  display->init(115200, true, 50, false);
  display->setRotation(1);

  is_initialized = true;
  DISPLAY_DEBUG_PRINT("SYS", "Initialized successfully.\n");
}

void Display_Manager::clear()
{
  if (!is_initialized)
  {
    DISPLAY_DEBUG_PRINT("ERROR", "Not initialized. Please call init() first.\n");
    return;
  }

  display->setFullWindow();
  display->firstPage();
  do
  {
    display->fillScreen(GxEPD_WHITE);
  } while (display->nextPage());
}

void Display_Manager::hibernate()
{
  if (!is_initialized)
  {
    DISPLAY_DEBUG_PRINT("ERROR", "Not initialized. Please call init() first.\n");
    return;
  }

  display->hibernate();
}

const char *Display_Manager::get_aqi_status_text(uint8_t aqi)
{
  switch (aqi)
  {
  case 1:
    return "EXCELLENT";
  case 2:
    return "GOOD";
  case 3:
    return "MODERATE";
  case 4:
    return "POOR";
  case 5:
    return "UNHEALTHY";
  default:
    return "WARM-UP";
  }
}

void Display_Manager::draw_ui(const Display_Env_Data &input, const Weather_Data_Collection &weather)
{
  display->fillScreen(GxEPD_WHITE);

  display->setTextColor(GxEPD_BLACK);
  display->setTextSize(1);
  display->setCursor(6, 6);
  display->print("INDOOR");

  display->setTextSize(3);
  display->setCursor(6, 20);
  display->printf("%.1f C", input.temperature);

  display->setTextSize(1);
  display->setCursor(6, 54);
  display->printf("H:%.0f%%  P:%.0fhPa", input.humidity, input.pressure);

  display->drawFastVLine(144, 4, 66, GxEPD_BLACK);

  display->fillRoundRect(148, 4, 144, 66, 4, GxEPD_BLACK);
  display->setTextColor(GxEPD_WHITE);

  display->setTextSize(2);
  display->setCursor(154, 10);
  display->print(get_aqi_status_text(input.aqi));

  display->setTextSize(1);
  display->setCursor(154, 34);
  display->printf("AQI: %d", input.aqi);

  display->setCursor(154, 45);
  display->printf("eCO2: %d ppm", input.eco2);

  display->setCursor(154, 56);
  display->printf("TVOC: %d ppb", input.tvoc);

  display->drawFastHLine(0, 74, 296, GxEPD_BLACK);

  const uint8_t col_w = 59;

  for (uint8_t i = 0; i < 5; i++)
  {
    uint16_t x_start = i * col_w;

    if (i > 0)
    {
      display->drawFastVLine(x_start, 75, 53, GxEPD_BLACK);
    }

    if (weather.valid)
    {
      char rawDate[11];
      strncpy(rawDate, weather.days[i].date, sizeof(rawDate));

      char shortDate[6] = "--.--";
      if (strlen(rawDate) >= 10)
      {
        snprintf(shortDate, sizeof(shortDate), "%.2s.%.2s", rawDate + 8, rawDate + 5);
      }

      display->fillRoundRect(x_start + 4, 78, 51, 13, 2, GxEPD_BLACK);
      display->setTextColor(GxEPD_WHITE);
      display->setTextSize(1);
      display->setCursor(x_start + 14, 81);
      display->print(shortDate);

      display->setTextColor(GxEPD_BLACK);
      display->setCursor(x_start + 11, 96);
      display->printf("%.0f/%.0f", weather.days[i].tMin, weather.days[i].tMax);

      display->setCursor(x_start + 16, 110);
      display->printf("%d%%", weather.days[i].rain);
    }
    else
    {
      display->fillRoundRect(x_start + 4, 78, 51, 13, 2, GxEPD_BLACK);
      display->setTextColor(GxEPD_WHITE);
      display->setTextSize(1);
      display->setCursor(x_start + 20, 81);
      display->print("--");

      display->setTextColor(GxEPD_BLACK);
      display->setCursor(x_start + 20, 98);
      display->print("N/A");
    }
  }
}

void Display_Manager::render(const Display_Env_Data &env_data, const Weather_Data_Collection &weather)
{
  if (!is_initialized)
  {
    DISPLAY_DEBUG_PRINT("ERROR", "Not initialized. Please call init() first.\n");
    return;
  }

  display->setPartialWindow(0, 0, display->width(), display->height());

  display->firstPage();
  do
  {
    draw_ui(env_data, weather);
  } while (display->nextPage());

  display->hibernate();
}
