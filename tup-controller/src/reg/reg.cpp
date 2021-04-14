#include "reg.h"

void clock_signal()
{
    digitalWrite(CLOCK_PIN, HIGH);
    delayMicroseconds(DEFAULT_REG_DELAY);
    digitalWrite(CLOCK_PIN, LOW);
    delayMicroseconds(DEFAULT_REG_DELAY);
}
void latch_enable()
{
    digitalWrite(LATCH_PIN, HIGH);
    delayMicroseconds(DEFAULT_REG_DELAY);
    digitalWrite(LATCH_PIN, LOW);
}

void send_data(unsigned int data_out)
{
    int i;
    for (i = 0; i < TOTAL_NUM_BITS; i++)
    {
        if ((data_out >> i) & (0x01))
        {
            digitalWrite(DATA_PIN, HIGH);
        }
        else
        {
            digitalWrite(DATA_PIN, LOW);
        }

        clock_signal();
    }
    latch_enable();
}