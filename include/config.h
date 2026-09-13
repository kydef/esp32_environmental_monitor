// General
#define TIME_TO_SLEEP 600
#define S_TO_uS_FACTOR 1000000ULL

// I2C
#define I2C_SDA_PIN 6
#define I2C_SCL_PIN 7

// BME280
#define BMP280_I2C_ADDRESS 0x76

// ENS160
#define ENS160_I2C_ADDRESS 0x53

// WeAct 2.9" e-paper display
#define EPD_CS 3   // D1 (GPIO3)
#define EPD_DC 4   // D2 (GPIO4)
#define EPD_RST 5  // D3 (GPIO5)
#define EPD_BUSY 2 // D0 (GPIO2)

// WiFi
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// DEBUG
#define SERIAL_SPEED 115200
#define DEBUG 1
#if DEBUG
#define DEBUG_PRINT(level, fmt, ...) Serial.printf("[%s] " fmt, level, ##__VA_ARGS__);
#else
#define DEBUG_PRINT(level, fmt, ...)
#endif
