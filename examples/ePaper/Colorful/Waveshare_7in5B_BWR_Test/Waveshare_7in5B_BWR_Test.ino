#include "driver.h"
#include "TFT_eSPI.h"

#ifdef EPAPER_ENABLE
EPaper epaper;
#endif

static const int SCREEN_W = 800;
static const int SCREEN_H = 480;

void setup()
{
#ifdef EPAPER_ENABLE
  epaper.begin();
  epaper.setRotation(0);

  // One clear is enough for a normal full refresh test.
  epaper.fillScreen(TFT_WHITE);

  // Full-size frame to verify 800x480 coverage.
  epaper.drawRect(0, 0, SCREEN_W, SCREEN_H, TFT_BLACK);
  epaper.drawRect(4, 4, SCREEN_W - 8, SCREEN_H - 8, TFT_RED);

  // Top banner.
  epaper.fillRect(16, 16, SCREEN_W - 32, 88, TFT_BLACK);
  epaper.setTextDatum(TL_DATUM);
  epaper.setTextFont(4);
  epaper.setTextColor(TFT_RED, TFT_BLACK);
  epaper.drawString("Waveshare 7.5in BWR", 32, 34);
  epaper.setTextFont(2);
  epaper.setTextColor(TFT_WHITE, TFT_BLACK);
  epaper.drawString("Resolution test: 800x480 full area", 32, 74);

  // Content area using all three colors.
  epaper.fillRect(24, 128, 250, 320, TFT_RED);
  epaper.fillRect(274, 128, 250, 320, TFT_WHITE);
  epaper.fillRect(524, 128, 252, 320, TFT_BLACK);

  epaper.setTextFont(4);
  epaper.setTextColor(TFT_WHITE, TFT_RED);
  epaper.drawString("RED", 120, 278);
  epaper.setTextColor(TFT_BLACK, TFT_WHITE);
  epaper.drawString("WHITE", 358, 278);
  epaper.setTextColor(TFT_RED, TFT_BLACK);
  epaper.drawString("BLACK", 612, 278);

  epaper.update();
#endif
}

void loop()
{
}
