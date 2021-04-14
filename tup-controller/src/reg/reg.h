#ifndef REG_H

#include <Arduino.h>

#define DATA_PIN 21  //DS
#define LATCH_PIN 22 //ST_CP
#define CLOCK_PIN 23 //SH_CP

#define DEFAULT_REG_DELAY 1 //delay for shift register delay and latch millisec.
#define TOTAL_NUM_BITS 16 //Number of bits to shift

void clock_signal();
void latch_enable();
void send_data(unsigned int data_out);
#endif