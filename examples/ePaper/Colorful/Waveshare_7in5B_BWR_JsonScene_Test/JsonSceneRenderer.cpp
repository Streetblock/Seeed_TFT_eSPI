#include "JsonSceneRenderer.h"

#include <string.h>
#include "qrcode.h" // HINZUGEFÜGT: Arduino QR-Code Bibliothek (von Richard Moore)

JsonSceneRenderer::JsonSceneRenderer(EPaper& display)
    : display_(display), lastError_(nullptr)
{
}

const char* JsonSceneRenderer::lastError() const
{
  return lastError_;
}

uint16_t JsonSceneRenderer::parseColor(JsonVariantConst value, uint16_t fallback) const
{
  if (value.is<uint32_t>())
  {
    return static_cast<uint16_t>(value.as<uint32_t>());
  }

  if (!value.is<const char*>())
  {
    return fallback;
  }

  const char* name = value.as<const char*>();
  if (!name) return fallback;

  if (strcmp(name, "black") == 0) return TFT_BLACK;
  if (strcmp(name, "white") == 0) return TFT_WHITE;
  if (strcmp(name, "red") == 0) return TFT_RED;
  if (strcmp(name, "yellow") == 0) return TFT_YELLOW;
  if (strcmp(name, "green") == 0) return TFT_GREEN;
  if (strcmp(name, "blue") == 0) return TFT_BLUE;
  if (strcmp(name, "orange") == 0) return TFT_ORANGE;

  return fallback;
}

uint8_t JsonSceneRenderer::parseDatum(const char* datum, uint8_t fallback) const
{
  if (!datum) return fallback;

  if (strcmp(datum, "TL") == 0) return TL_DATUM;
  if (strcmp(datum, "TC") == 0) return TC_DATUM;
  if (strcmp(datum, "TR") == 0) return TR_DATUM;
  if (strcmp(datum, "ML") == 0) return ML_DATUM;
  if (strcmp(datum, "MC") == 0) return MC_DATUM;
  if (strcmp(datum, "MR") == 0) return MR_DATUM;
  if (strcmp(datum, "BL") == 0) return BL_DATUM;
  if (strcmp(datum, "BC") == 0) return BC_DATUM;
  if (strcmp(datum, "BR") == 0) return BR_DATUM;
  if (strcmp(datum, "L_BASELINE") == 0) return L_BASELINE;
  if (strcmp(datum, "C_BASELINE") == 0) return C_BASELINE;
  if (strcmp(datum, "R_BASELINE") == 0) return R_BASELINE;

  return fallback;
}

void JsonSceneRenderer::drawPixel(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  display_.drawPixel(x, y, color);
}

void JsonSceneRenderer::drawHLine(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int w = obj["w"] | 0;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  display_.drawFastHLine(x, y, w, color);
}

void JsonSceneRenderer::drawVLine(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int h = obj["h"] | 0;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  display_.drawFastVLine(x, y, h, color);
}

void JsonSceneRenderer::drawRect(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int w = obj["w"] | 0;
  int h = obj["h"] | 0;

  if (obj["fill"].is<JsonVariantConst>())
  {
    display_.fillRect(x, y, w, h, parseColor(obj["fill"], TFT_WHITE));
  }

  if (obj["stroke"].is<JsonVariantConst>())
  {
    display_.drawRect(x, y, w, h, parseColor(obj["stroke"], TFT_BLACK));
  }
}

void JsonSceneRenderer::drawRoundRect(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int w = obj["w"] | 0;
  int h = obj["h"] | 0;
  int r = obj["r"] | 8;

  if (obj["fill"].is<JsonVariantConst>())
  {
    display_.fillRoundRect(x, y, w, h, r, parseColor(obj["fill"], TFT_WHITE));
  }

  if (obj["stroke"].is<JsonVariantConst>())
  {
    display_.drawRoundRect(x, y, w, h, r, parseColor(obj["stroke"], TFT_BLACK));
  }
}

void JsonSceneRenderer::drawCircle(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int r = obj["r"] | 0;

  if (obj["fill"].is<JsonVariantConst>())
  {
    display_.fillCircle(x, y, r, parseColor(obj["fill"], TFT_WHITE));
  }

  if (obj["stroke"].is<JsonVariantConst>())
  {
    display_.drawCircle(x, y, r, parseColor(obj["stroke"], TFT_BLACK));
  }
}

void JsonSceneRenderer::drawEllipse(JsonObjectConst obj)
{
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int rx = obj["rx"] | 0;
  int ry = obj["ry"] | 0;

  if (obj["fill"].is<JsonVariantConst>())
  {
    display_.fillEllipse(x, y, rx, ry, parseColor(obj["fill"], TFT_WHITE));
  }

  if (obj["stroke"].is<JsonVariantConst>())
  {
    display_.drawEllipse(x, y, rx, ry, parseColor(obj["stroke"], TFT_BLACK));
  }
}

void JsonSceneRenderer::drawLine(JsonObjectConst obj)
{
  int x1 = obj["x1"] | 0;
  int y1 = obj["y1"] | 0;
  int x2 = obj["x2"] | 0;
  int y2 = obj["y2"] | 0;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  display_.drawLine(x1, y1, x2, y2, color);
}

void JsonSceneRenderer::drawTriangle(JsonObjectConst obj)
{
  int x1 = obj["x1"] | 0;
  int y1 = obj["y1"] | 0;
  int x2 = obj["x2"] | 0;
  int y2 = obj["y2"] | 0;
  int x3 = obj["x3"] | 0;
  int y3 = obj["y3"] | 0;

  if (obj["fill"].is<JsonVariantConst>())
  {
    display_.fillTriangle(x1, y1, x2, y2, x3, y3, parseColor(obj["fill"], TFT_WHITE));
  }

  if (obj["stroke"].is<JsonVariantConst>())
  {
    display_.drawTriangle(x1, y1, x2, y2, x3, y3, parseColor(obj["stroke"], TFT_BLACK));
  }
}

void JsonSceneRenderer::drawText(JsonObjectConst obj)
{
  const char* value = obj["value"] | "";
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  uint8_t font = obj["font"] | 2;
  uint8_t size = obj["size"] | 1;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  uint16_t bg = parseColor(obj["bg"], TFT_WHITE);
  const char* datum = obj["datum"] | "TL";

  display_.setTextFont(font);
  display_.setTextSize(size);
  display_.setTextColor(color, bg);
  display_.setTextDatum(parseDatum(datum, TL_DATUM));
  display_.drawString(value, x, y);
}

void JsonSceneRenderer::drawNumber(JsonObjectConst obj)
{
  long value = obj["value"] | 0;
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  uint8_t font = obj["font"] | 2;
  uint8_t size = obj["size"] | 1;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  uint16_t bg = parseColor(obj["bg"], TFT_WHITE);

  display_.setTextFont(font);
  display_.setTextSize(size);
  display_.setTextColor(color, bg);
  display_.drawNumber(value, x, y, font);
}

void JsonSceneRenderer::drawChar(JsonObjectConst obj)
{
  const char* value = obj["value"] | "";
  char ch = (value && value[0]) ? value[0] : '?';
  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  uint8_t font = obj["font"] | 2;
  uint8_t size = obj["size"] | 1;
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  uint16_t bg = parseColor(obj["bg"], TFT_WHITE);

  display_.setTextFont(font);
  display_.setTextSize(size);
  display_.setTextColor(color, bg);
  display_.drawChar(ch, x, y, font);
}

void JsonSceneRenderer::drawQRCode(JsonObjectConst obj)
{
  const char* value = obj["value"] | "";
  if (strlen(value) == 0) return;

  int x = obj["x"] | 0;
  int y = obj["y"] | 0;
  int size = obj["size"] | 3; // Skalierung: 3 Pixel pro QR-Modul ist meist gut lesbar
  uint8_t version = obj["version"] | 3; // QR Version (bestimmt die Datenmenge)
  uint16_t color = parseColor(obj["color"], TFT_BLACK);
  uint16_t bg = parseColor(obj["bg"], TFT_WHITE);

  // QR Code Instanz erstellen
  QRCode qrcode;
  
  // Dynamisch den nötigen RAM reservieren (qrcode_getBufferSize ist ein Makro)
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  
  // Text generieren. Die '0' steht für ECC_LOW (niedrigste Fehlerkorrektur = meiste Daten)
  qrcode_initText(&qrcode, qrcodeData, version, 0, value);

  // Zuerst eine Hintergrund-Box zeichnen (QR Codes brauchen einen weißen Rand, die "Quiet Zone")
  // Für das Padding addieren wir 2 "Module" pro Seite
  int padding = size * 2; 
  int totalSize = (qrcode.size * size) + (padding * 2);
  
  // Hintergrund zeichnen (etwas nach links und oben verschoben für das Padding)
  display_.fillRect(x - padding, y - padding, totalSize, totalSize, bg);

  // Jetzt die eigentliche Matrix durchgehen und zeichnen
  for (uint8_t qry = 0; qry < qrcode.size; qry++) {
    for (uint8_t qrx = 0; qrx < qrcode.size; qrx++) {
      // getModule gibt true (1) zurück, wenn der Punkt "schwarz" ist
      if (qrcode_getModule(&qrcode, qrx, qry)) {
        display_.fillRect(x + (qrx * size), y + (qry * size), size, size, color);
      }
    }
  }
}

bool JsonSceneRenderer::render(const char* json)
{
#if !JSON_SCENE_RENDERER_HAS_ARDUINOJSON
  (void)json;
  lastError_ = "ArduinoJson not found. Please install ArduinoJson library.";
  return false;
#else
  lastError_ = nullptr;

  DynamicJsonDocument doc(12288);
  DeserializationError err = deserializeJson(doc, json);
  if (err)
  {
    lastError_ = err.c_str();
    return false;
  }

  JsonObjectConst root = doc.as<JsonObjectConst>();

  if (root["clear"].is<JsonVariantConst>())
  {
    display_.fillScreen(parseColor(root["clear"], TFT_WHITE));
  }

  JsonArrayConst objects = root["objects"].as<JsonArrayConst>();
  for (JsonObjectConst obj : objects)
  {
    const char* type = obj["type"] | "";

    if (strcmp(type, "pixel") == 0) drawPixel(obj);
    else if (strcmp(type, "hline") == 0) drawHLine(obj);
    else if (strcmp(type, "vline") == 0) drawVLine(obj);
    else if (strcmp(type, "rect") == 0) drawRect(obj);
    else if (strcmp(type, "roundRect") == 0) drawRoundRect(obj);
    else if (strcmp(type, "circle") == 0) drawCircle(obj);
    else if (strcmp(type, "ellipse") == 0) drawEllipse(obj);
    else if (strcmp(type, "line") == 0) drawLine(obj);
    else if (strcmp(type, "triangle") == 0) drawTriangle(obj);
    else if (strcmp(type, "text") == 0) drawText(obj);
    else if (strcmp(type, "number") == 0) drawNumber(obj);
    else if (strcmp(type, "char") == 0) drawChar(obj);
    else if (strcmp(type, "qrcode") == 0) drawQRCode(obj);
  }

  return true;
#endif
}
