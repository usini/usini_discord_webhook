/*
  Send Hello World to Discord using WebHook
  This example disable serial debug messages
  discord.send returns true if the message was sent successfully
*/


#include <Discord_WebHook.h>

Discord_Webhook discord;
// How to get the Webhook URL
// https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
String DISCORD_WEBHOOK = "https://discord.com/api/webhooks/id/token";

void setup() {
  Serial.begin(115200);
  discord.begin(DISCORD_WEBHOOK); // Initialize the Discord_Webhook object
  discord.disableDebug(); // Disable debug (no serial message will be send)
  discord.addWiFi("WiFiName","WiFiPassword"); // Add WiFi credentials (you can add multiples WiFi SSID)
  discord.connectWiFi(); // Connect to WiFi
  bool message_sent = discord.send("Hello World"); // Send message
  if(message_sent) {
    Serial.println("Message sent");
  } else {
    Serial.println("I AM ERROR");
  }
}

void loop() {

}