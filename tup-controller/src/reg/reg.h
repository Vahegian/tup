#ifndef REG_H

#include <Arduino.h>

#define DATA_PIN 5  //DS
#define LATCH_PIN 18 //ST_CP
#define CLOCK_PIN 19 //SH_CP

#define DEFAULT_REG_DELAY 0 //delay for shift register delay and latch microsec.
#define TOTAL_NUM_BITS 21 //Number of bits to shift

void clock_signal();
void latch_enable();
void send_data(unsigned long);
#endif