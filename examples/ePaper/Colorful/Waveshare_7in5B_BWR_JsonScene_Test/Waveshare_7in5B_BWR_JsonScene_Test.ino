#include "driver.h"
#include "TFT_eSPI.h"
#include "JsonSceneRenderer.h"

#ifdef EPAPER_ENABLE
EPaper epaper;
JsonSceneRenderer renderer(epaper);
#endif

static const char* kSceneJson = R"JSON(
{
  "clear": "white",
  "objects": [
    { "type": "rect", "x": 0, "y": 0, "w": 800, "h": 480, "stroke": "black" },
    { "type": "rect", "x": 4, "y": 4, "w": 792, "h": 472, "stroke": "red" },

    { "type": "rect", "x": 16, "y": 16, "w": 768, "h": 88, "fill": "black" },
    { "type": "text", "value": "Waveshare 7.5in BWR", "x": 32, "y": 34, "font": 4, "size": 1, "color": "red", "bg": "black", "datum": "TL" },
    { "type": "text", "value": "JSON runtime scene renderer", "x": 32, "y": 74, "font": 2, "size": 1, "color": "white", "bg": "black", "datum": "TL" },

    { "type": "rect", "x": 24, "y": 128, "w": 250, "h": 320, "fill": "red" },
    { "type": "rect", "x": 274, "y": 128, "w": 250, "h": 320, "fill": "white", "stroke": "black" },
    { "type": "rect", "x": 524, "y": 128, "w": 252, "h": 320, "fill": "black" },

    { "type": "text", "value": "RED", "x": 120, "y": 278, "font": 4, "size": 1, "color": "white", "bg": "red", "datum": "TL" },
    { "type": "text", "value": "WHITE", "x": 358, "y": 278, "font": 4, "size": 1, "color": "black", "bg": "white", "datum": "TL" },
    { "type": "text", "value": "BLACK", "x": 612, "y": 278, "font": 4, "size": 1, "color": "red", "bg": "black", "datum": "TL" },

    { "type": "line", "x1": 24, "y1": 448, "x2": 776, "y2": 448, "color": "red" },
    { "type": "text", "value": "Shapes: rect, circle, line, triangle, text", "x": 32, "y": 456, "font": 2, "size": 1, "color": "black", "bg": "white", "datum": "BL" }
  ]
}
)JSON";

void setup()
{
#ifdef EPAPER_ENABLE
  epaper.begin();
  epaper.setRotation(0);

  if (!renderer.render(kSceneJson))
  {
    epaper.fillScreen(TFT_WHITE);
    epaper.setTextColor(TFT_RED, TFT_WHITE);
    epaper.setTextFont(2);
    epaper.setTextDatum(TL_DATUM);
    epaper.drawString("JSON render error:", 16, 16);
    epaper.setTextColor(TFT_BLACK, TFT_WHITE);
    epaper.drawString(renderer.lastError() ? renderer.lastError() : "unknown", 16, 40);
  }

  epaper.update();
#endif
}

void loop()
{
}
