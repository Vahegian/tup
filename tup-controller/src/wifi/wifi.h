#ifndef WIFI_H

#include <WString.h>

bool wifiSetup(const char *ssid, const char *password);
String wifiGet(const char *host, int port, String url);

#endif