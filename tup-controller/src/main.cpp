#include <WiFi.h>
#include "wifi/wifi.h"
#include "private.h"
#include "reg/reg.h"
#include "semphr.h"

SemaphoreHandle_t xledRowMutex;

long screen[6][3];

void printBits(long num)
// Can be used for debugging
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
  if (y == 1)
    row = 0b010;
  if (y == 2)
    row = 0b001;
  for (int _ = 0; _ < 18; _++)
  {
    row <<= 1;
    row |= 0b1;
  }

  long rgb = 0;
  if (b == 1)
    rgb |= 0b1;
  rgb <<= 1;
  if (g == 1)
    rgb |= 0b1;
  rgb <<= 1;
  if (r == 1)
    rgb |= 0b1;

  long led = rgb << x * 3;
  xSemaphoreTake(xledRowMutex, portMAX_DELAY);
  screen[x][y] = row ^ led;
  xSemaphoreGive(xledRowMutex);
}

void highlight(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0, unsigned int update_delay = 100)
{
  for (int x = 0; x < 6; x++)
  {
    for (int y = 2; y >= 0; y--)
    {
      setLED(x, y, r, g, b);
      delay(update_delay);
    }
  }
}

void updateLEDstate(String action)
{
  int leg_coords[3] = {0, 0, 0};
  int p = 0;
  highlight(0,0,0,20);
  for (int i = 1; i < action.length(); i++)
  {
    String ch = String(action[i]); // i=0 is an unknown char, hence start from 1
    if (!ch.equals(",") && !ch.equals(":") && !ch.equals("") && !ch.equals(" "))
    {

      leg_coords[p] = ch.toInt();
      p++;
    }

    if (ch.equals(":"))
    {

      p = 0;
      if (leg_coords[2] == 1) // if '1' price change is positive
      {
        setLED(leg_coords[0], leg_coords[1], 0, 1, 0);
      }
      else
      {
        setLED(leg_coords[0], leg_coords[1], 1, 0, 0);
      }

      leg_coords[0] = 0;
      leg_coords[1] = 0;
      leg_coords[2] = 0;
    }
    delay(10);
  }
}

unsigned int t = 0;

void loop()
{

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

      xSemaphoreTake(xledRowMutex, portMAX_DELAY);
      for (int y = 0; y < 3; y++)
      {
        send_data(screen[x][y]);
      }
      xSemaphoreGive(xledRowMutex);
      delayMicroseconds(5);
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
  xTaskCreate(refreshLEDs, "refresh", 10000, NULL, 1, NULL);

  highlight(0, 0, 1, 100);
  if (!wifiSetup(ssid, password))
  {
    highlight(1, 0, 0, 100);
    delay(100);
    highlight(0, 0, 0, 100);
    delay(100);
    highlight(1, 0, 0, 100);
    delay(3000);
    exit(1);
  }
  highlight(0, 1, 0, 100);

  delay(1000);
}
