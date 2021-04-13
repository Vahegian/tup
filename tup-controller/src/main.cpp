/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"

void setup()
{
    Serial.begin(9600);
    delay(10);

    bool connected = setupWifi(ssid, password);
    if (!connected){
      exit(1);
    }
}

void loop()
{

  Serial.println(wifiGet(host, "/tup"));

}
