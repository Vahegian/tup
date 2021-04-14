#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"
#include "reg/reg.h"

void setup()
{
  Serial.begin(9600);
  delay(10);

  bool connected = wifiSetup(ssid, password);
  if (!connected)
  {
    exit(1);
  }

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
}


int16_t l = 0;
void loop()
{
  String action = wifiGet(host, port, "/tup");
  int led = 0;
  for (int i = 0; i < action.length(); i++)
  {
    if (i % 9 == 0 && i != 0)
    {
      led++;
    }
    Serial.print(action[i]);
  }
  Serial.println("\nMSG END __________");

  send_data(l++);
  // l++;
  // delay(1);
  
  if (l%5000==0){
    Serial.println(l);
  }
}
