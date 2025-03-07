#include <Discord_WebHook.h>

// Replace with your Discord webhook credentials
String channel_id = "123";
String token = "123";

// Update interval (in milliseconds)
#define UPDATE_INTERVAL 300000 // 5 minutes

Discord_Webhook discord;

void setup() {
  Serial.begin(115200);
  
  // Initialize Discord webhook
  discord.begin(channel_id, token);
  discord.addWiFi("WiFiName","WiFiPassword"); 
  discord.connectWiFi();
  
  // Disable debug messages after successful connection
  discord.disableDebug();
}

void sendSystemStatus() {
  // Get system metrics
  float temperature = getTemperature(); // See implementation below
  int rssi = WiFi.RSSI();
  uint32_t uptime = millis() / 1000;
  String ipAddress = WiFi.localIP().toString();
  uint32_t freeHeap = ESP.getFreeHeap();
  
  // Format uptime
  int hours = uptime / 3600;
  int minutes = (uptime % 3600) / 60;
  int seconds = uptime % 60;
  String uptimeStr = String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
  
  // Create JSON fields array
  String fields = "["
    "{\"name\":\"Status\",\"value\":\"✅ Online\",\"inline\":true},"
    "{\"name\":\"Uptime\",\"value\":\"" + uptimeStr + "\",\"inline\":true},"
    "{\"name\":\"WiFi Signal\",\"value\":\"" + String(rssi) + " dBm\",\"inline\":true},"
    "{\"name\":\"IP Address\",\"value\":\"" + ipAddress + "\",\"inline\":true},"
    "{\"name\":\"Free Memory\",\"value\":\"" + String(freeHeap) + " bytes\",\"inline\":true},"
    "{\"name\":\"Temperature\",\"value\":\"" + String(temperature, 1) + " °C\",\"inline\":true}"
  "]";
  
  // Send advanced embed
  discord.sendEmbedAdvanced(
    "System Status Report", 
    "Current device metrics",
    "#00FF00", // Green color
    fields,
    "https://placehold.co/600x200/000000/FFFFFF?text=System+Status"
  );
}

float getTemperature() {
  #if defined(ESP32)
    // ESP32 temperature sensor
    static bool sensorInitialized = false;
    if (!sensorInitialized) {
      temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
      temp_sensor_set_config(temp_sensor);
      temp_sensor_start();
      sensorInitialized = true;
    }
    return temp_sensor_read_celsius();
  #elif defined(ESP8266)
    // ESP8266 doesn't have built-in temp sensor
    // Return mock value or connect external sensor
    return 25.0 + random(-2, 3); 
  #endif
}

void loop() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    lastUpdate = millis();
    sendSystemStatus();
  }
  
  // Maintain WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    discord.connectWiFi();
  }
}