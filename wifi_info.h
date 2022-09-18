#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

const char *ssid = "Router666";
const char *password = "home@2647";

IPAddress ipaddr = NULL;

void wifi_connect() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  Serial.println("WiFi connecting...");
  while (!WiFi.isConnected()) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
  ipaddr = WiFi.localIP();
}

void wifi_monitor() { // if wifi reconnects and gets a new IP, then reset the module, otherwise the clients would not reconnect.
  if(WiFi.status() == WL_CONNECTED) {
    if(WiFi.localIP() != ipaddr) {
      Serial.println("IP address changed");
      Serial.println(WiFi.localIP().toString().c_str());
      Serial.println(ipaddr);
      Serial.println("Resetting NODEMCU");
      ESP.restart(); //ESP.reset();
    }   
  }

 
}

#endif /* WIFI_INFO_H_ */
