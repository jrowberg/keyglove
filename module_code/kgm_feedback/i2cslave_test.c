#include <avr/io.h>
#include <avr/interrupt.h>
#include "usiTwiSlave.h"

int main(void) {
    sei(); // enable interupts

    DDRA |= 0b00000001;
    PORTA &= 0b11111110;

    // raw 8-bit I2C write address = 0xAE
    // raw 8-bit I2C read address = 0xAF
    const uint8_t slaveAddress = 0x57;
    usiTwiSlaveInit(slaveAddress);
    for(;;) {
        if (usiTwiDataInReceiveBuffer()) {
            if (usiTwiReceiveByte() != 0) {
                PORTA |= 0b00000001;
            } else {
                PORTA &= 0b11111110;
            }
        }
    }
}
