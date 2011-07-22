#include "ps2dev.h"
#include "debug.h"

//since for the device side we are going to be in charge of the clock,
//the two defines below are how long each _phase_ of the clock cycle is
#define CLKFULL 40

// we make changes in the middle of a phase, this how long from the
// start of phase to the when we drive the data line
#define CLKHALF 20

/*
 * the clock and data pins can be wired directly to the clk and data pins
 * of the PS2 connector.  No external parts are needed.
 */
PS2dev::PS2dev(uint8_t clockPin, uint8_t dataPin) {
    _ps2clk = clockPin;
    _ps2data = dataPin;
    gohi(_ps2clk);
    gohi(_ps2data);
}

/*
 * according to some code I saw, these functions will
 * correctly set the clock and data pins for
 * various conditions.  It's done this way so you don't need
 * pullup resistors.
 */
void PS2dev::gohi(uint8_t pin) {
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
}

void PS2dev::golo(uint8_t pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

int8_t PS2dev::write(uint8_t data) {
    uint8_t i;
    uint8_t parity = 1;

    DEBUG_PRN_PS2("PS2dev debug: sending ");
    DEBUG_PRNLF_PS2(data, HEX);
    
    if (digitalRead(_ps2clk) == LOW) return -1;
    if (digitalRead(_ps2data) == LOW) return -2;

    golo(_ps2data);
    delayMicroseconds(CLKHALF);
    
    // device sends on falling clock
    
    // start bit
    golo(_ps2clk);
    delayMicroseconds(CLKFULL);
    gohi(_ps2clk);
    delayMicroseconds(CLKHALF);

    // data bits
    for (i = 0; i < 8; i++) {
        if (data & 0x01) gohi(_ps2data);
        else golo(_ps2data);
    
        delayMicroseconds(CLKHALF);
        golo(_ps2clk);	
        delayMicroseconds(CLKFULL);
        gohi(_ps2clk);
        delayMicroseconds(CLKHALF);
    
        parity = parity ^ (data & 0x01);
        data = data >> 1;
    }

    // parity bit
    if (parity) gohi(_ps2data);
    else golo(_ps2data);
    
    delayMicroseconds(CLKHALF);
    golo(_ps2clk);	
    delayMicroseconds(CLKFULL);
    gohi(_ps2clk);
    delayMicroseconds(CLKHALF);

    // stop bit
    gohi(_ps2data);
    delayMicroseconds(CLKHALF);
    golo(_ps2clk);	
    delayMicroseconds(CLKFULL);
    gohi(_ps2clk);
    delayMicroseconds(CLKHALF);

    delayMicroseconds(50);
    
    return 0;
}

uint8_t PS2dev::read() {
    uint8_t data = 0x00;
    uint8_t i;
    uint8_t bit = 0x01;
    uint8_t parity = 1;
  
    // wait for data line to go low
    while (digitalRead(_ps2data) == HIGH);
    
    // wait for clock line to go high
    while (digitalRead(_ps2clk) == LOW);

    delayMicroseconds(CLKHALF);
    golo(_ps2clk);
    delayMicroseconds(CLKFULL);
    gohi(_ps2clk);
    delayMicroseconds(CLKHALF);

    for (i = 0; i < 8; i++) {
        if (digitalRead(_ps2data) == HIGH) data |= bit;
        bit = bit << 1;
        
        delayMicroseconds(CLKHALF);
        golo(_ps2clk);	
        delayMicroseconds(CLKFULL);
        gohi(_ps2clk);
        delayMicroseconds(CLKHALF);
        
        parity = parity ^ (data & 0x01);
    }

    // we do the delay at the end of the loop, so at this point we have
    // already done the delay for the parity bit
  
    // stop bit
    delayMicroseconds(CLKHALF);
    golo(_ps2clk);	
    delayMicroseconds(CLKFULL);
    gohi(_ps2clk);
    delayMicroseconds(CLKHALF);

    delayMicroseconds(CLKHALF);
    golo(_ps2data);
    golo(_ps2clk);	
    delayMicroseconds(CLKFULL);
    gohi(_ps2clk);
    delayMicroseconds(CLKHALF);
    gohi(_ps2data);

    return data;
}