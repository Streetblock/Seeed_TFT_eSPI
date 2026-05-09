#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Deine Display-Bibliotheken
#include "driver.h"
#include "TFT_eSPI.h"
#include "JsonSceneRenderer.h"

// --- Hier laden wir die geheimen Daten rein! ---
#include "secrets.h"

#ifdef EPAPER_ENABLE
EPaper epaper;
JsonSceneRenderer renderer(epaper);
#endif


void setup() {
    Serial.begin(115200);

#ifdef EPAPER_ENABLE
    epaper.begin();
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setTextColor(TFT_BLACK, TFT_WHITE);
    epaper.setTextFont(2);
    epaper.drawString("Verbinde mit WLAN...", 16, 16);
    epaper.update();
#endif

    // 1. WLAN verbinden
    Serial.println("\nInitialisiere WLAN...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(ssid, password);
    Serial.print("Verbinde mit WLAN");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nVerbunden!");
        
        // Status auf Display updaten
#ifdef EPAPER_ENABLE
        epaper.fillScreen(TFT_WHITE);
        epaper.drawString("Lade JSON Layout...", 16, 16);
        epaper.update();
#endif

        // 2. JSON Layout herunterladen und rendern
        fetchAndRenderLayout();
        
    } else {
        Serial.println("\nFehler: WLAN-Verbindung fehlgeschlagen!");
#ifdef EPAPER_ENABLE
        epaper.fillScreen(TFT_WHITE);
        epaper.setTextColor(TFT_RED, TFT_WHITE);
        epaper.drawString("WLAN Fehler!", 16, 16);
        epaper.update();
#endif
        delay(5000);
        ESP.restart();
    }
}

void loop() {
    // Hier kannst du später einen Timer einbauen, 
    // falls sich das Layout alle X Minuten aktualisieren soll.
}

void fetchAndRenderLayout() {
    Serial.println("Lade JSON von URL...");
    
    WiFiClientSecure client;
    client.setInsecure(); // Zertifikatsprüfung überspringen
    client.setHandshakeTimeout(20000); // WICHTIG FÜR ESP32-S3: Zeit für den Handshake geben

    HTTPClient http;
    http.begin(client, json_url);
    http.setTimeout(15000); // Zeit für den eigentlichen Download

    int httpResponseCode = http.GET();
    String payload = "";

    if (httpResponseCode > 0) {
        Serial.printf("HTTP Statuscode: %d\n", httpResponseCode);
        payload = http.getString(); // Das JSON als Text speichern
        Serial.println("Download erfolgreich.");
    } else {
        Serial.printf("HTTP Fehlercode: %d\n", httpResponseCode);
    }
    
    http.end(); // Verbindung schließen

    // 3. Wenn wir Text bekommen haben, schicken wir ihn an den Renderer
    if (payload.length() > 0) {
        Serial.println("Rendere Scene...");
        
#ifdef EPAPER_ENABLE
        // payload.c_str() wandelt den String in das Format um, das der Renderer erwartet
        if (!renderer.render(payload.c_str())) {
            // Falls das JSON fehlerhaft formatiert ist (z.B. Komma vergessen)
            epaper.fillScreen(TFT_WHITE);
            epaper.setTextColor(TFT_RED, TFT_WHITE);
            epaper.setTextFont(2);
            epaper.drawString("JSON Render Error:", 16, 16);
            epaper.setTextColor(TFT_BLACK, TFT_WHITE);
            epaper.drawString(renderer.lastError() ? renderer.lastError() : "unknown", 16, 40);
        }
        
        epaper.update();
        Serial.println("Display aktualisiert!");
#endif
    } else {
        // Falls kein Payload kam (z.B. wegen SSL Fehler -1)
#ifdef EPAPER_ENABLE
        epaper.fillScreen(TFT_WHITE);
        epaper.setTextColor(TFT_RED, TFT_WHITE);
        epaper.drawString("Download fehlgeschlagen!", 16, 16);
        epaper.update();
#endif
    }
}
