#ifndef WIFI_H

#include <WString.h>

bool setupWifi(const char* ssid, const char* password);
String wifiGet(char* host, String url);

#endif