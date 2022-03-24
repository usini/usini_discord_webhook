/*
  Send a message with variables to Discord using WebHook
*/

#include <Discord_WebHook.h>

Discord_Webhook discord;
// How to get the Webhook URL
// https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
String DISCORD_WEBHOOK = "https://discord.com/api/webhooks/id/token";

void setup() {
  int variable_int = 69; // Int variable (can be float, long etc...)
  String variable_string = "SUS"; // String variable

  Serial.begin(115200);
  discord.begin(DISCORD_WEBHOOK); // Initialize the Discord_Webhook object
  discord.addWiFi("WiFiName","WiFiPassword"); // Add WiFi credentials (you can add multiples WiFi SSID)
  discord.connectWiFi(); // Connect to WiFi
  discord.send("When the variable " + String(variable_int) + " is " + variable_string); // Send message
}

void loop() {
}