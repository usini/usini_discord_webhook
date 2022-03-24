/*
  Discord_WebHook.h
  Library for sending messages to Discord via WebHook

  Copyright (c) 2022 µsini
  Author : Rémi Sarrailh
  Version : 1.0.0

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

// Define if it is an ESP32 (code should works on new board ESP32-S2/ESP32-C3)
#if defined(ESP32)
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
//#include "WiFiClientSecureAxTLS.h"
#include <ESP8266HTTPClient.h>
#else
// We still open it as ESP32 for compatibility with later version (not tested)
#warning "Library worked on ESP8266/ESP32 only"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#endif

class Discord_Webhook {
public:
  void begin(String webhook_url);
  void addWiFi(const char* ssid, const char* password);
  void connectWiFi();
  void disableDebug();
  void setTTS();
  bool send(String content);

private:
  #ifdef ESP32
    WiFiMulti wifi;
  #endif
  #ifdef ESP8266
    ESP8266WiFiMulti wifi;
  #endif
  String webhook_url;
  bool tts = false;
  bool debug = true;
};

#endif