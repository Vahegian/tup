#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"
#include "reg/reg.h"

const int num_of_leds = 18;

long leds[num_of_leds] = {};

char comma = ',';
char colon = ':';

long load_anim[] = {
  0b010100100100100100101,
  0b010100100100100101101,
  0b010100100100101101101,
  0b010100100101101101101,
  0b010100101101101101101,
  0b010101101101101101101
};

long getLedActionBinary(long data[4])
{
  long n = 0;
  for (int r = 0; r < 3; r++) //preparing the row flip the corresponding bit to 1 if the row is selected
  {
    n <<= 1;
    if (r - data[1] == 0)
      n = n | 1;
  }

  for (int i = 0; i < num_of_leds; i++)
  {
    n <<= 1;
    int index = ((data[0] * 3) + data[2]); // find the index of the LED (3 bits per LED) on X-axis
    if (i == 17 - index)
      continue; // flip all to 1 except the index
    n = n | 1;
  }

  return n;
}

void playLoadingAnimation(int t_ms_delay, int pwm_delay_micro_s){
  long leds_anim = 0b000000000000000000000;
    for (int y=0; y>=0; y--){
      for (int x=0; x<6; x++){
        for (int c=0; c<3; c++){
          long rgb[] = {x,y,c};
          leds_anim |= 0b111101101101101101101;
          leds_anim ^= getLedActionBinary(rgb);
          
          if (y==2)leds_anim |= 0b001000000000000000000;
          else if (y==1)leds_anim |= 0b010000000000000000000;
          else if (y==0)leds_anim |= 0b100000000000000000000;
          
          for (int d=0; d<(t_ms_delay*1000)/pwm_delay_micro_s; d++){ //150 ms delay
            send_data(leds_anim);
            send_data(0b000111111111111111111);
            delayMicroseconds(pwm_delay_micro_s);
          }
        }
      }
    }
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
      // Serial.println(leds[leds_index], BIN);
      chosen_led_action_index = 0;
      leds_index++;
    }
  }
}


unsigned int t = 0;

void loop()
{
  if (t == 20 )
  {
    // for (int a=0; a<5; a++){
    //   send_data(load_anim[a]);
    //   delay(500-(a*100));
    // }

    playLoadingAnimation(200, 300);
    String action = wifiGet(host, port, "/tup");
    for (int i=0; i<num_of_leds; i++){
      leds[i]=0b000111111111111111111;
    }
    updateLEDstate(action);

    for (int n = 0; n < 18; n++)
    {
      send_data(leds[n]);
      delayMicroseconds(5);
    }
    
    t = 0;
  }

  t++;
  delay(1000);
}


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
  // playLoadingAnimation(200, 300);
  send_data((0b001000101000000000000));
  if (!wifiSetup(ssid, password))
  {
    exit(1);
  }
  send_data((0b100011101101101101011));
  delay(5000);
  send_data((0b000111111111111111111));
}
