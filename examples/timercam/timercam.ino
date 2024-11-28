#include <Arduino.h>

/*
  Example for M5Timer Camera X, send an image every hour
  Need Timer-CAM Library

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !!DO NOT INSTALL DEPENDENCIES !!! ArduinoHttpClient DO NOT INSTALL DEPENDENCIES !!!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ArduinoHTTPClient is not needed and incompatible with native HTTPClient library

  There seems to be a lot of issues with this library unfortunately thought it seems to work on platformio
  https://github.com/m5stack/TimerCam-arduino/issues/23 (RTCClass error)
  Add #include <cstdint> to RTC8563_Class.h

  Power_Class.h error
  Change L17: #define BAT_ADC_CHANNEL (adc1_channel_t)ADC1_GPIO38_CHANNEL

  Image is sometimes corrupted with FHD resolution
  https://github.com/m5stack/TimerCam-arduino/issues/26

  Platformio settings:
  [env:timer-cam]
  platform = espressif32
  board = m5stack-timer-cam
  framework = arduino
  monitor_speed = 115200
  upload_speed = 1500000
  lib_deps =
      m5stack/Timer-CAM
  lib_ignore =
      ArduinoHttpClient
*/

#include "Discord_WebHook.h"

#include "M5TimerCAM.h"

Discord_Webhook discord; // Create a Discord_Webhook object
// How to get the Webhook URL
// https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks
String DISCORD_WEBHOOK = "https://discord.com/api/webhooks/id/token";

void setup()
{
    TimerCAM.begin(true); // true Enable RTC if not used crash on deep sleep

    if (!TimerCAM.Camera.begin())
    {
        Serial.println("Camera Init Fail");
        return;
    }
    Serial.println("Camera Init Success");

    TimerCAM.Camera.sensor->set_pixformat(TimerCAM.Camera.sensor, PIXFORMAT_JPEG);
    TimerCAM.Camera.sensor->set_framesize(TimerCAM.Camera.sensor, FRAMESIZE_FHD);
    TimerCAM.Camera.sensor->set_vflip(TimerCAM.Camera.sensor, 1);
    TimerCAM.Camera.sensor->set_hmirror(TimerCAM.Camera.sensor, 0);
    // Use Camera tools to setup this : https://docs.m5stack.com/en/quick_start/timer_cam/cameratool
    TimerCAM.Camera.sensor->set_contrast(TimerCAM.Camera.sensor, 2);
    TimerCAM.Camera.sensor->set_saturation(TimerCAM.Camera.sensor, 3);
    TimerCAM.Camera.sensor->set_brightness(TimerCAM.Camera.sensor, -2);

    discord.begin(DISCORD_WEBHOOK);              // Initialize the Discord_Webhook object
    discord.addWiFi("WiFiName", "WiFiPassword"); // Add WiFi credentials (you can add multiples WiFi SSID)
    // discord.setTTS(true); // Add TTS
    discord.connectWiFi(); // Connect to WiFi
}

void loop()
{
    if (TimerCAM.Camera.get())
    {

        Serial.println("making POST request");

        String contentType = "image/jpeg";

        discord.send("Battery Level:" + String(TimerCAM.Power.getBatteryLevel()));
        discord.sendFile(TimerCAM.Camera.fb->buf, TimerCAM.Camera.fb->len, "timelapse.jpg", contentType.c_str());
        TimerCAM.Camera.free();
        WiFi.mode(WIFI_OFF);
        Serial.println("Time to sleep");
        TimerCAM.Power.timerSleep(3600);
    }
}