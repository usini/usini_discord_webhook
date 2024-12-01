/*
  Discord_WebHook.cpp
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

#include "Discord_WebHook.h"

// Get webhook URL
void Discord_Webhook::begin(String channel_id, String token) {
  Discord_Webhook::webhook_url = "https://discord.com/api/webhooks/" + channel_id + "/" + token;
}

// Add WiFi credentials
void Discord_Webhook::addWiFi(const char* ssid, const char* password) {
  #if defined(ESP32) || defined(ESP8266)
    if (Discord_Webhook::debug) {
      Serial.print("[WIFI] Added ssid: ");
      Serial.println(ssid);
    }
    wifi.addAP(ssid, password);
  #elif defined(ARDUINO_ARCH_RP2040) // Experimental For Raspberry Pi Pico W
    WiFi.begin(ssid, password);
  #endif
}

// Connect to WiFi
void Discord_Webhook::connectWiFi() {
  #if defined(ESP32) || defined(ESP8266)
    WiFi.mode(WIFI_STA);
    if (Discord_Webhook::debug) {
      Serial.println("[WiFi] Connecting WiFi");
    }
    while (wifi.run() != WL_CONNECTED) {
      delay(100);
      if (Discord_Webhook::debug) {
        Serial.print(".");
      }
    }
    if (Discord_Webhook::debug) {
      Serial.println("[WiFi] Connected");
    }
  #elif defined(ARDUINO_ARCH_RP2040) // Experimental For Raspberry Pi Pico W
    if (Discord_Webhook::debug) {
      Serial.println("[WiFi] Connecting WiFi");
    }
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      if (Discord_Webhook::debug) {
        Serial.print(".");
      }
    }
    if (Discord_Webhook::debug) {
      Serial.println("[WiFi] Connected");
    }
  #endif
}

// Set TTS variable
void Discord_Webhook::setTTS() { Discord_Webhook::tts = true; }

// Set debug variable to false
void Discord_Webhook::disableDebug() { Discord_Webhook::debug = false; }

bool Discord_Webhook::send(String content)
{
  return Discord_Webhook::sendRequest("{\"content\":\"" + content + "\"");
}

// @vollukas contribution : https://github.com/usini/usini_discord_webhook/issues/4 Thanks!
bool Discord_Webhook::sendFile(uint8_t *fileData, size_t fileLength, const String &filename, const String &contentType)
{
  WiFiClientSecure *client = new WiFiClientSecure; // Create a WiFiClientSecure
  bool ok = false;
  if (client)
  {
    client->setInsecure(); // Disable SSL certificate verification
    HTTPClient https;      // Create HTTPClient
    String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    String bodyStart = "--" + boundary + "\r\n";
    bodyStart += "Content-Disposition: form-data; name=\"file\"; filename=\"" + filename + "\"\r\n";
    bodyStart += "Content-Type: " + contentType + "\r\n\r\n";

    String bodyEnd = "\r\n--" + boundary + "--\r\n";

    if (Discord_Webhook::debug)
    {
      Serial.println("[HTTP] Connecting to Discord...");
      Serial.println("[HTTP] File Upload Start");
    }

    // Begin HTTPS requests
    if (https.begin(*client, Discord_Webhook::webhook_url))
    {
      https.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

      // Send POST request in chunks
      https.collectHeaders(nullptr, 0); // Ensure no automatic content collection
      if (https.POST((uint8_t *)bodyStart.c_str(), bodyStart.length()) == HTTP_CODE_OK)
      {
        // Write binary file data directly
        client->write(fileData, fileLength); 

        // Write the body end
        client->write((uint8_t *)bodyEnd.c_str(), bodyEnd.length());

        if (Discord_Webhook::debug)
        {
          Serial.println("[HTTP] File Upload Complete");
        }

        ok = true;
      }
      else
      {
        if (Discord_Webhook::debug)
        {
          Serial.println("[HTTP] POST Error");
        }
      }
      https.end();
    }
    else
    {
      if (Discord_Webhook::debug)
      {
        Serial.println("[HTTP] Unable to connect");
      }
    }
  }
  else
  {
    if (Discord_Webhook::debug)
    {
      Serial.println("[HTTP] Unable to create client");
    }
  }
  delete client;
  return ok;
}

// @py7hon contribution Thanks! https://github.com/usini/usini_discord_webhook/pull/5
bool Discord_Webhook::sendEmbed(String title, String description, String hexColor)
{
  long colorValue = strtol(hexColor.startsWith("#") ? hexColor.substring(1).c_str() : hexColor.c_str(), nullptr, 16);
  String embedContent = "{\"embeds\":[{\"title\":\"" + title +
                        "\",\"description\":\"" + description +
                        "\",\"color\":" + String(colorValue) + "}]";
  return Discord_Webhook::sendRequest(embedContent);
}

bool Discord_Webhook::sendEmbedImage(String title, String description, String hexColor, String imageUrl)
{
  // Remove '#' from the hex color if it exists and convert to numeric
  long colorValue = strtol(hexColor.startsWith("#") ? hexColor.substring(1).c_str() : hexColor.c_str(), nullptr, 16);

  String embedContent = "{\"embeds\":[{\"title\":\"" + title +
                        "\",\"description\":\"" + description +
                        "\",\"color\":" + String(colorValue) +
                        ",\"image\":{\"url\":\"" + imageUrl + "\"}}]";
  return Discord_Webhook::sendRequest(embedContent);
}

// Send message to Discord, we disable SSL certificate verification for ease of
// use (Warning: this is insecure)
bool Discord_Webhook::sendRequest(String jsonPayload)
{
  String discord_tts = "false";
  if (Discord_Webhook::tts)
  {
    discord_tts = "true";
  }
  WiFiClientSecure *client = new WiFiClientSecure; // Create a WiFiClientSecure
  bool ok = false;
  if (client)
  {
    client->setInsecure(); // Disable SSL certificate verification

    HTTPClient https; // Create HTTPClient
    String payload = jsonPayload + ",\"tts\":" + discord_tts + "}";
    if (Discord_Webhook::debug)
    {
      Serial.println("[HTTP] Connecting to Discord...");
      Serial.println("[HTTP] Payload: " + payload);
      Serial.println("[HTTP] TTS: " + discord_tts);
    }

    // Begin HTTPS requests
    if (https.begin(*client, Discord_Webhook::webhook_url))
    {
      https.addHeader("Content-Type", "application/json"); // Set request as JSON

      // Send POST request
      int httpCode = https.POST(payload);
      if (httpCode > 0)
      { // if HTTP code is return
        if (httpCode == HTTP_CODE_OK ||
            httpCode == HTTP_CODE_MOVED_PERMANENTLY ||
            httpCode == HTTP_CODE_NO_CONTENT)
        {
          // Discord webhook has changed and our request is not correct, so it
          // will not send response, so we end without getting a response
          // https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
          if (Discord_Webhook::debug)
          {
            Serial.println("[HTTP] OK");
          }
          ok = true;
        }
        else
        {
          if (Discord_Webhook::debug)
          {
            // This should mainly return an error if token or id is invalid
            String payload = https.getString();
            Serial.print("[HTTP] ERROR: ");
            Serial.println(payload);
            ok = false;
          }
        }
        https.end();
      }
      else
      {
        if (Discord_Webhook::debug)
        {
          // This will return an error if the server is unreachable
          Serial.printf("[HTTP] ERROR: %s\n",
                        https.errorToString(httpCode).c_str());
          ok = false;
        }
      }
    }
    else
    {
      if (Discord_Webhook::debug)
      {
        // This will return an error if request failed
        Serial.printf("[HTTP] Unable to connect\n");
        ok = false;
      }
    }
  }
  else
  {
    if (Discord_Webhook::debug)
    {
      // This shouldn't happen but anyway it's better to check
      Serial.println("[HTTP] Unable to create client");
      ok = false;
    }
  }
  delete client;
  return ok;
}
