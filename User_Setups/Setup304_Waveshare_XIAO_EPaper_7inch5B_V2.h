#include <Wire.h>

#define USER_SETUP_ID 403

// Waveshare 7.5" e-Paper HAT (B) V2/V3, 800x480, B/W/R
#define WS7IN5B_V2_DRIVER

#define EPAPER_ENABLE
#define USE_BWRY_EPAPER

#define TFT_WIDTH 800
#define TFT_HEIGHT 480

#define EPD_WIDTH TFT_WIDTH
#define EPD_HEIGHT TFT_HEIGHT

// #define EPD_HORIZONTAL_MIRROR

#ifdef ENABLE_EPAPER_BOARD_PIN_SETUPS
#include "EPaper_Board_Pins_Setups.h"
#else
#define TFT_SCLK D8
#define TFT_MISO D9
#define TFT_MOSI D10
#define TFT_CS D1   // Chip select control pin
#define TFT_DC D3   // Data/Command control pin
#define TFT_BUSY D2
#define TFT_RST D0  // Reset pin
#endif

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

#ifdef CONFIG_IDF_TARGET_ESP32S3
#define USE_HSPI_PORT
#endif
#include "XIAO_SPI_Frequency.h"
