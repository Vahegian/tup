#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"
#include "reg/reg.h"

long t = 0;

const int num_of_leds = 18;

long leds[num_of_leds] = {};

char comma = ',';
char colon = ':';

long load_anim[] = {
  0b010111111111111111100,
  0b010111111111111100100,
  0b010111111111100100100,
  0b010111111100100100100,
  0b010111100100100100100,
  0b010100100100100100100
};

void setup()
{

  Serial.begin(9600);

  delay(500);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  send_data((0b111000000000000000000));
  if (!wifiSetup(ssid, password))
  {
    exit(1);
  }
  send_data((0b000111111111111111111));
}

long getLedActionBinary(long data[4])
{
  long n = 0;
  for (int r = 0; r < 3; r++)
  {
    n <<= 1;
    if (r - data[1] == 0)
      n = n | 1;
  }

  for (int i = 0; i < num_of_leds; i++)
  {
    n <<= 1;
    int index = ((data[0] * 3) + data[2]);
    if (i == 17 - index)
      continue;
    n = n | 1;
  }

  return n;
}

void updateLEDstate(String action)
{
  if (action.length()-1>num_of_leds*6) return;
  int leds_index = 0;
  long chosen_led_action[4];
  int chosen_led_action_index = 0;
  for (int i = 0; i < action.length(); i++)
  {
    
    if (action[i] == comma)
    {
      chosen_led_action[chosen_led_action_index] = action.substring(i - 1, i).toInt();
      chosen_led_action_index++;
    }
    else if (action[i] == colon)
    {
      chosen_led_action[chosen_led_action_index] = action.substring(i - 1, i).toInt();
      leds[leds_index] = getLedActionBinary(chosen_led_action);
      Serial.println(leds[leds_index], BIN);
      chosen_led_action_index = 0;
      leds_index++;
    }
  }
}

void loop()
{
  if (t % 50000 == 0)
  {
    for (int a=0; a<5; a++){
      send_data(load_anim[a]);
      delay(60);
    }
    String action = wifiGet(host, port, "/tup");
    send_data(load_anim[5]);
    for (int i=0; i<num_of_leds; i++){
      leds[i]=0b000111111111111111111;
    }
    updateLEDstate(action);

    t = 0;
  }

  for (int n = 0; n < 18; n++)
  {
    send_data(leds[n]);
    delayMicroseconds(5);
  }
  t++;
}
