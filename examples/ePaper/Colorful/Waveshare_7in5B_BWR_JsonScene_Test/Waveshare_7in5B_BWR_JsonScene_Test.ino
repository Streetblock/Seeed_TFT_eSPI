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
    {
      "type": "rect",
      "x": 0,
      "y": 0,
      "w": 800,
      "h": 480,
      "stroke": "black"
    },
    {
      "type": "rect",
      "x": 4,
      "y": 4,
      "w": 792,
      "h": 472,
      "stroke": "red"
    },
    {
      "type": "rect",
      "x": 16,
      "y": 16,
      "w": 768,
      "h": 88,
      "fill": "black"
    },
    {
      "type": "text",
      "value": "Waveshare 7.5in BWR",
      "x": 32,
      "y": 34,
      "font": 4,
      "size": 1,
      "color": "red",
      "bg": "black",
      "datum": "TL"
    },
    {
      "type": "text",
      "value": "JSON runtime scene renderer",
      "x": 32,
      "y": 74,
      "font": 2,
      "size": 1,
      "color": "white",
      "bg": "black",
      "datum": "TL"
    },
    {
      "type": "rect",
      "x": 24,
      "y": 128,
      "w": 250,
      "h": 320,
      "fill": "red"
    },
    {
      "type": "rect",
      "x": 274,
      "y": 128,
      "w": 250,
      "h": 320,
      "fill": "white",
      "stroke": "black"
    },
    {
      "type": "rect",
      "x": 524,
      "y": 128,
      "w": 252,
      "h": 320,
      "fill": "black"
    },
    {
      "type": "qrcode",
      "value": "https://github.com/olikraus",
      "x": 341,
      "y": 146,
      "size": 4,
      "version": 3,
      "color": "black",
      "bg": "white"
    },
    {
      "type": "text",
      "value": "RED",
      "x": 120,
      "y": 278,
      "font": 4,
      "size": 1,
      "color": "white",
      "bg": "red",
      "datum": "TL"
    },
    {
      "type": "text",
      "value": "WHITE",
      "x": 358,
      "y": 278,
      "font": 4,
      "size": 1,
      "color": "black",
      "bg": "white",
      "datum": "TL"
    },
    {
      "type": "text",
      "value": "BLACK",
      "x": 612,
      "y": 278,
      "font": 4,
      "size": 1,
      "color": "red",
      "bg": "black",
      "datum": "TL"
    },
    {
      "type": "hline",
      "x": 24,
      "y": 448,
      "w": 752,
      "color": "red"
    },
    {
      "type": "vline",
      "x": 775,
      "y": 128,
      "h": 320,
      "color": "red"
    },
    {
      "type": "roundRect",
      "x": 560,
      "y": 146,
      "w": 180,
      "h": 70,
      "r": 12,
      "stroke": "white"
    },
    {
      "type": "ellipse",
      "x": 650,
      "y": 196,
      "rx": 46,
      "ry": 20,
      "stroke": "white"
    },
    {
      "type": "number",
      "value": 75,
      "x": 620,
      "y": 304,
      "font": 4,
      "size": 1,
      "color": "white",
      "bg": "black"
    },
    {
      "type": "char",
      "value": "B",
      "x": 700,
      "y": 304,
      "font": 4,
      "size": 1,
      "color": "red",
      "bg": "black"
    },
    {
      "type": "pixel",
      "x": 780,
      "y": 460,
      "color": "black"
    },
    {
      "type": "line",
      "x1": 24,
      "y1": 462,
      "x2": 776,
      "y2": 462,
      "color": "black"
    },
    {
      "type": "text",
      "value": "Types: pixel,hline,vline,rect,roundRect,circle,ellipse,line,triangle,text,number,char,qrcode",
      "x": 32,
      "y": 456,
      "font": 1,
      "size": 1,
      "color": "black",
      "bg": "white",
      "datum": "BL"
    }
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
