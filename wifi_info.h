#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

/*
 * wifi_info.h
 * From: https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266/blob/master/examples/Example02_Switch/wifi_info.h
 */

const char *ssid = "latte";
const char *password = "****";

void wifi_connect() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  Serial.println("WiFi connecting...");
  while (!WiFi.isConnected()) {
    delay(50);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}

#endif /* WIFI_INFO_H_ */
