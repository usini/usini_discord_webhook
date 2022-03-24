/*
  Send Hello World to Discord using WebHook
*/

#include <Discord_WebHook.h>

Discord_Webhook discord; // Create a Discord_Webhook object
// How to get the Webhook URL
// https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
String DISCORD_WEBHOOK = "https://discord.com/api/webhooks/id/token";

void setup() {
  Serial.begin(115200);
  discord.begin(DISCORD_WEBHOOK); // Initialize the Discord_Webhook object
  discord.addWiFi("WiFiName","WiFiPassword"); // Add WiFi credentials (you can add multiples WiFi SSID)
  discord.connectWiFi(); // Connect to WiFi
  discord.send("Hello World"); // Send Hello World to Discord
}

void loop() {

}