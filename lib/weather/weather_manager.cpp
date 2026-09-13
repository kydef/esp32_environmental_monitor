#include <WiFi.h>
#include <ArduinoJson.h>
#include "weather_manager.h"

const char *weatherUrl = "http://api.open-meteo.com/v1/forecast?latitude=49.55&longitude=25.59&daily=weather_code,temperature_2m_max,temperature_2m_min,precipitation_probability_max&timezone=auto&forecast_days=5";
const int MAX_ATTEMPTS = 3;

bool Weather_Manager::connect(const char *ssid, const char *password)
{
  if (WiFi.status() == WL_CONNECTED)
    return true;

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout > 0)
  {
    delay(500);
    WEATHER_DEBUG_PRINT("SYS", "Connecting to Wi-Fi...\n");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    WEATHER_DEBUG_PRINT("ERROR", "Failed to connect to Wi-Fi!");
    return false;
  }

  WEATHER_DEBUG_PRINT("SYS", "Wi-Fi connected!");
  return true;
};

void Weather_Manager::disconnect()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

Weather_Data_Collection Weather_Manager::fetchWeather(const char *ssid, const char *password)
{
  Weather_Data_Collection collection = {};
  collection.valid = false;

  if (!connect(ssid, password))
  {
    WEATHER_DEBUG_PRINT("ERROR", "Wi-Fi not connected. Skipping weather fetch.");
    return collection;
  }

  for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++)
  {
    WEATHER_DEBUG_PRINT("[SYS]", "Fetching weather (Attempt %d/%d)...\n", attempt, MAX_ATTEMPTS);

    WiFiClient client;
    HTTPClient http;

    http.setTimeout(5000);
    http.begin(client, weatherUrl);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error)
      {
        JsonObject daily = doc["daily"];
        JsonArray dates = daily["time"];
        JsonArray maxTemps = daily["temperature_2m_max"];
        JsonArray minTemps = daily["temperature_2m_min"];
        JsonArray rainProb = daily["precipitation_probability_max"];
        JsonArray codes = daily["weather_code"];

        size_t count = min(dates.size(), (size_t)5);

        if (count > 0)
        {
          for (size_t i = 0; i < count; i++)
          {
            collection.days[i].tMax = maxTemps[i].as<float>();
            collection.days[i].tMin = minTemps[i].as<float>();
            collection.days[i].rain = rainProb[i].as<uint8_t>();
            collection.days[i].code = codes[i].as<uint16_t>();

            const char *raw_date = dates[i].as<const char *>();
            if (raw_date)
            {
              strncpy(collection.days[i].date, raw_date, sizeof(collection.days[i].date) - 1);
              collection.days[i].date[sizeof(collection.days[i].date) - 1] = '\0';
            }
          }

          collection.valid = true;
          WEATHER_DEBUG_PRINT("[SYS]", "Weather data fetched successfully.");
          http.end();
          break;
        }
        else
        {
          WEATHER_DEBUG_PRINT("[ERROR]", "'daily.time' array is empty in JSON!");
        }
      }
      else
      {
        WEATHER_DEBUG_PRINT("[ERROR]", "JSON parse failed!");
      }
    }
    else
    {
      WEATHER_DEBUG_PRINT("[ERROR]", "HTTP GET failed, code: %d\n", httpCode);
    }

    http.end();

    if (attempt < MAX_ATTEMPTS)
    {
      WEATHER_DEBUG_PRINT("[SYS]", "Waiting 2 seconds before retry...");
      delay(2000);
    }
  }

  disconnect();
  return collection;
}
