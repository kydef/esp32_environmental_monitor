# ESP32-C3 Environmental Monitor

Battery-powered environmental monitor based on **Seeed Studio XIAO ESP32-C3**.

### Hardware

* BMP280 — pressure
* AHT21 — temperature / humidity
* ENS160 — AQI / TVOC / eCO₂
* WeAct 2.9" e-paper display

### Configuration

* I²C: SDA `GPIO6`, SCL `GPIO7`
* BMP280: `0x76`
* ENS160: `0x53`
* Sleep interval: `600 s` (10 min)
* Serial: `115200`

### Weather

Uses [Open-Meteo](https://open-meteo.com/) for a 5-day forecast.

### Operation

1. Initialize sensors and display
2. Read environmental data
3. Fetch weather forecast
4. Update e-paper display
5. Enter deep sleep for 10 minutes
