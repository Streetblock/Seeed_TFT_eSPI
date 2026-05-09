#pragma once

#include "TFT_eSPI.h"

#include <ArduinoJson.h>
#define JSON_SCENE_RENDERER_HAS_ARDUINOJSON 1

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
  void drawQRCode(JsonObjectConst obj);
};
