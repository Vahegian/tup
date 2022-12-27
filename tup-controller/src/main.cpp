#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"
#include "reg/reg.h"
#include "semphr.h"

SemaphoreHandle_t xledRowMutex;

// long ledRow[3] = {
//     0b100111111111111111111,
//     0b010111111111111111111,
//     0b001111111111111111111,
// };

long screen[6][3];

char comma = ',';
char colon = ':';

void printBits(long num)
{
  for (int bit = 0; bit < (sizeof(num) * 8); bit++)
  {
    Serial.print(num & 0x01);
    num = num >> 1;
  }
  Serial.println("");
}

void setLED(unsigned int x, unsigned int y, int r = 0, int g = 0, int b = 0)
{
  if (y > 2)
    return;
  if (x > 5)
    return;
  long row = 0b100;
  if(y==1) row = 0b010;
  if(y==2) row = 0b001;
  for (int _=0; _<18; _++){
    row <<=1;
    row |= 0b1;
  }
  // printBits(row);

  long rgb = 0;
  if (b == 1)
    rgb |= 0b1;
  rgb <<= 1;
  if (g == 1)
    rgb |= 0b1;
  rgb <<= 1;
  if (r == 1)
    rgb |= 0b1;

  // printBits(rgb);
  long led = rgb << x * 3; // todo x: 0, y:1 is always blue 
  // printBits(led);
  // led = ~led;
  xSemaphoreTake(xledRowMutex, portMAX_DELAY);
  screen[x][y] = row ^ led;
  xSemaphoreGive(xledRowMutex);
  // printBits(row);
  // printBits(screen[x][y]);
  // Serial.println("______________");
}

void updateLEDstate(String action)
{
  int perLed[3] = {0, 0, 0};
  int p = 0;
  // Serial.println(action);
  for (int i = 0; i < action.length(); i++)
  {
    if (action[i] != comma)
    {
      perLed[p] = String(action[i]).toInt();
      p++;
    }

    if (action[i] == colon)
    {

      p = 0;
      if (perLed[2] == 1)
      {
        setLED(perLed[0], perLed[1], 0, 1, 0);
      }
      else
      {
        setLED(perLed[0], perLed[1], 1, 0, 0);
      }
      perLed[0] = 0;
      perLed[1] = 0;
      perLed[2] = 0;
    }
    delay(10);
  }
}

unsigned int t = 0;

void loop()
{
  for (int x = 0; x < 6; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      setLED(x, y, 0, 0, 1);
      delay(30);
    }
  }

  String action = wifiGet(host, port, "/tup");
  updateLEDstate(action);

  delay(30000);
}

void refreshLEDs(void *parameter)
{
  while (true)
  {
    for (int x = 0; x < 6; x++)
    {
      send_data((0b000111111111111111111));
                //  0b001011111111111111111
      // delayMicroseconds(1);

      xSemaphoreTake(xledRowMutex, portMAX_DELAY);
      for (int y = 0; y < 3; y++){
        send_data(screen[x][y]);
      }
      xSemaphoreGive(xledRowMutex);
      delayMicroseconds(5);
      // delay(100);
    }
  }
}

void setup()
{

  xledRowMutex = xSemaphoreCreateMutex();
  Serial.begin(9600);

  delay(500);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  // playLoadingAnimation(200, 300);
  // send_data((0b001011101101101101011));
  xTaskCreate(refreshLEDs, "refresh", 10000, NULL, 1, NULL);

  setLED(0, 0, 1, 0, 0);
  setLED(5, 0, 1, 0, 0);
  setLED(0, 2, 1, 0, 0);
  setLED(5, 2, 1, 0, 0);
  if (!wifiSetup(ssid, password))
  {
    exit(1);
  }
  // send_data((0b100011101101101101011));
  setLED(0, 0, 0, 0, 1);
  setLED(5, 0, 0, 0, 1);
  setLED(0, 2, 0, 0, 1);
  setLED(5, 2, 0, 0, 1);
  // send_data((0b001100111111111111111));
             
  delay(2000);
  
  // send_data((0b000111111111111111111));
}
