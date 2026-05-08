#include "driver.h"
#include "TFT_eSPI.h"

#ifdef EPAPER_ENABLE
EPaper epaper;
#endif

void setup()
{
#ifdef EPAPER_ENABLE
epaper.begin();
epaper.setRotation(0);

// 1) hartes Weiß-Clear (2x hilft oft bei BWR)
epaper.fillScreen(TFT_WHITE);
epaper.update();
delay(1500);
epaper.fillScreen(TFT_WHITE);
epaper.update();
delay(1500);

// 2) dann erst dein eigentliches Bild
epaper.fillScreen(TFT_WHITE);
epaper.fillRect(24, 24, 220, 120, TFT_BLACK);
epaper.fillCircle(380, 84, 56, TFT_RED);
epaper.update();

#endif
}

void loop()
{
}
