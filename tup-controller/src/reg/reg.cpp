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

void send_data(unsigned long data_out)
{
    int i;
    // Serial.println("F- ");
    for (i = 0; i < TOTAL_NUM_BITS; i++)
    {   
        // Serial.print(data_out);
        // Serial.print(data_out >> i);
        if ((data_out >> i) & (0x01))
        {
            digitalWrite(DATA_PIN, HIGH);
            // Serial.print(1);
        }
        else
        {
            digitalWrite(DATA_PIN, LOW);
            // Serial.print(0);
        }

        clock_signal();
    }
    // Serial.println(" <-");
    latch_enable();
}