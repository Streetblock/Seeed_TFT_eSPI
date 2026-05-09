#pragma once

#include "TFT_eSPI.h"

#if __has_include(<ArduinoJson.h>)
#include <ArduinoJson.h>
#define JSON_SCENE_RENDERER_HAS_ARDUINOJSON 1
#else
#define JSON_SCENE_RENDERER_HAS_ARDUINOJSON 0
#endif

class JsonSceneRenderer
{
public:
  explicit JsonSceneRenderer(EPaper& display);

  // Renders a scene from a JSON string. Returns true on success.
  bool render(const char* json);

  // Last parse/render error for diagnostics.
  const char* lastError() const;

private:
  EPaper& display_;
  const char* lastError_;

#if JSON_SCENE_RENDERER_HAS_ARDUINOJSON
  uint16_t parseColor(JsonVariantConst value, uint16_t fallback) const;
  uint8_t parseDatum(const char* datum, uint8_t fallback) const;
  void drawPixel(JsonObjectConst obj);
  void drawHLine(JsonObjectConst obj);
  void drawVLine(JsonObjectConst obj);
  void drawRect(JsonObjectConst obj);
  void drawRoundRect(JsonObjectConst obj);
  void drawCircle(JsonObjectConst obj);
  void drawEllipse(JsonObjectConst obj);
  void drawLine(JsonObjectConst obj);
  void drawTriangle(JsonObjectConst obj);
  void drawText(JsonObjectConst obj);
  void drawNumber(JsonObjectConst obj);
  void drawChar(JsonObjectConst obj);
#endif
};
