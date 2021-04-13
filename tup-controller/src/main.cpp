#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"

void setup()
{
    Serial.begin(9600);
    delay(10);

    bool connected = wifiSetup(ssid, password);
    if (!connected){
      exit(1);
    }
}

void loop()
{

  Serial.println(wifiGet(host, port, "/tup"));

}
