/*
  Send BME280 sensor value to Discord using WebHook
  You need Adafruit_BME280 library
*/


#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Discord_WebHook.h>

Discord_Webhook discord;
// How to get the Webhook URL
// https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
String DISCORD_WEBHOOK = "https://discord.com/api/webhooks/id/token";

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  if(bme.begin(0x76)){ // Sensor address can be 0x76 or 0x77
    Serial.println("BME280 init success");
  } else {
    Serial.println("BME280 init failed");
  }

  discord.begin(DISCORD_WEBHOOK); // Initialize the Discord_Webhook object
  discord.addWiFi("WiFiName","WiFiPassword"); // Add WiFi credentials (you can add multiples WiFi SSID)
  discord.connectWiFi(); // Connect to WiFi
}

void loop() {
  // Send BME280 sensor value to Discord using WebHook every minute
  discord.send("Temperature: " + String(bme.readTemperature()) +
              "°C - Humidity: " + String(bme.readHumidity()) +
              "% - Pressure: " + String(bme.readPressure() / 100.0F) +
              " hPa");
  delay(60000);
}