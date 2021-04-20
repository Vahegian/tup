#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"
#include "reg/reg.h"

// // int bits [21] = {4,2,1};
// uint32_t leds18_and_rows = (0b000111111111111111111);
// uint32_t leds18_start = (0b000100000000000000000);
// // uint8_t led_rows_3 = (0b000);
// uint8_t led_rows_start = (0b100);

void setup()
{

  Serial.begin(9600);

  delay(500);

  // Serial.println("S--");
  // Serial.println(leds18_and_rows, BIN);
  // Serial.println((leds18_and_rows&led_rows_start), BIN);
  // Serial.println((leds18_and_rows&leds18_start), BIN);
  // Serial.println("E--");

  // delay(10000);

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

// int16_t l = 0;

long t = 0;
char c = 0b1000;
int d = 0;
bool dir = true;


const int num_of_leds = 18;
long leds[num_of_leds] = {};
int leds_index = 0;

long getLedActionBinary(long data[4])
{
  for(int k=0; k<3; k++){
    Serial.print(data[k]);
    Serial.print(" ( ");
    Serial.print(data[k]-'0');
    Serial.print(" ) ");
    Serial.print(" - ");
  }
  // Serial.print(data[0]-'0');
  // Serial.print(data[1]-'0');
  // Serial.print(data[2]-'0');
  Serial.println("");
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

char comma = ',';
char colon = ':';

void updateLEDstate(String action)
{
  if (action.length()>num_of_leds*4) return;

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

int coords[3] = {0, 0, 0};

void loop()
{
  if (t % 5000 == 0)
  {
    send_data((0b000111111111111111111));
    String action = wifiGet(host, port, "/tup");

    // for (int i = 0; i < action.length(); i++)
    // {
    //   Serial.print(action[i]);
    //   if (action[i] == *",")
    //   {
    //     Serial.println(": Found Comma");
    //   }
    // }
    leds_index = 0;
    for (int i=0; i<num_of_leds; i++){
      leds[i]=0b000111111111111111111;
    }
    updateLEDstate(action);
    // Serial.println(action);
    // Serial.println("\nMSG END __________");
    // Serial.println(sizeof(leds18_and_rows));
    // c >>= 1;
    // Serial.println(c, BIN);
    // Serial.println(0b1000 & 1, BIN);
    // Serial.println("\n------\n");

    t = 0;
  }

  // send_data((0b001110101110101110110), false);
  // send_data((0b010111111111111111101), false);
  // send_data((0b100111111111111111110), false);
  // if (t%2==0){
  // }else{
  //   // send_data((0b101000000000011110111));
  //   // delayMicroseconds(15000);
  // }
  // send_data(getLedActionBinary(coords));
  // send_data((0b000111111111111111111));
  // delayMicroseconds(d);
  for (int n = 0; n < 18; n++)
  {
    send_data(leds[n]);
  }
  t++;
  // if (dir){
  //   d+=10;
  // }else
  // {
  //   d-=10;
  // }

  // if (d>=5000){
  //   dir = false;
  // }else if (d<=0){
  //   dir = true;
  // }

  // l++;
  // delay(1);

  // if (l%5000==0){
  // Serial.println(l);
  // }
}
