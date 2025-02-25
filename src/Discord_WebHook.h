/*
  Discord_WebHook.h
  Library for sending messages to Discord via WebHook

  Copyright (c) 2024 µsini
  Author : Rémi Sarrailh
  Version : 2.0.0

  The MIT License (MIT)
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef Discord_WebHook_h
#define Discord_WebHook_h

#include <Arduino.h>

// Define platform-specific dependencies
#if defined(ESP32)
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#elif defined(ARDUINO_ARCH_RP2040) // Experimental For Raspberry Pi Pico W
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#else
#error "This library supports ESP32, ESP8266, and Raspberry Pi Pico W only."
#endif

class Discord_Webhook {
public:
  void begin(String channel_id, String token);
  void addWiFi(const char* ssid, const char* password);
  void connectWiFi();
  void disableDebug();
  void setTTS();
  bool send(String content);
  bool sendEmbed(String title, String description, String hexColor);
  bool sendEmbedImage(String title, String description, String imageUrl, String hexColor);
  bool sendFile(uint8_t* imageData, size_t imageLength, const String& filename, const String& contentType = "image/jpeg");

private:
  bool sendRequest(String jsonPayload);

#if defined(ESP32) || defined(ESP8266)
  #ifdef ESP32
    WiFiMulti wifi;
  #endif
  #ifdef ESP8266
    ESP8266WiFiMulti wifi;
  #endif
#elif defined(ARDUINO_ARCH_RP2040)
  // No multi-AP feature yet for RP2040
#endif

  String webhook_url;
  bool tts = false;
  bool debug = true;
};

#endif
