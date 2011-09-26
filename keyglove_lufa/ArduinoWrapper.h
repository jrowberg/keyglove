// Keyglove controller source code - Wrapper functions header file to use Arduino-style code in LUFA environment
// 7/15/2010 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _ARDUINOWRAPPER_H_
#define _ARDUINOWRAPPER_H_

#include <math.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

#ifdef true
    #undef true
    #undef false
#endif

#include "wiring/wiring.h"

#ifndef HEX
    #define DEC 10
    #define HEX 16
    #define OCT 8
    #define BIN 2
    #define BYTE 0
#endif

class USBVirtualSerial {

    char numStr[32];

    public:
        void begin(uint32_t baud);

        void print(const char *str);
        void println(const char *str);

        void print(byte val);
        void print(int val);
        void print(unsigned int val);
        void print(long int val);
        void print(long unsigned int val);
        void print(double val);

        void print(byte val, byte base);
        void print(int val, byte base);
        void print(unsigned int val, byte base);
        void print(long int val, byte base);
        void print(long unsigned int val, byte base);
        void print(double val, byte base);

        void println(byte val);
        void println(int val);
        void println(unsigned int val);
        void println(long int val);
        void println(long unsigned int val);
        void println(double val);

        void println(byte val, byte base);
        void println(int val, byte base);
        void println(unsigned int val, byte base);
        void println(long int val, byte base);
        void println(long unsigned int val, byte base);
        void println(double val, byte base);
        
        uint8_t read();
        uint8_t available();

};

class HardwareSerial {

    public:
        void begin(uint32_t baud);

        void print(const char *str);
        void print(byte val);
        void print(int val);
        void print(long unsigned int val);
        void print(long double val);

        void print(const char *str, int format);
        void print(byte val, int format);
        void print(int val, int format);
        void print(long unsigned int val, int format);
        void print(long double val, int format);

        void println(const char *str);
        void println(byte val);
        void println(int val);
        void println(long unsigned int val);
        void println(long double val);

        void println(const char *str, int format);
        void println(byte val, int format);
        void println(int val, int format);
        void println(long unsigned int val, int format);
        void println(long double val, int format);

        uint8_t read();
        uint8_t available();

};

class USBMouse {

    private:
        uint8_t report[8];

    public:
        void move(uint8_t x, uint8_t y);
        void move(uint8_t x, uint8_t y, uint8_t z);
        void set_buttons(uint8_t b1, uint8_t b2, uint8_t b3);
        void scroll(uint8_t z);

};

class USBKeyboard {

    private:
        uint8_t report[8];

    public:
        void set_modifier(uint8_t modifier);
        void set_key1(uint8_t code);
        void set_key2(uint8_t code);
        void set_key3(uint8_t code);
        void set_key4(uint8_t code);
        void set_key5(uint8_t code);
        void set_key6(uint8_t code);
        void send_now();

};

class WireI2C {
    
    public:
        void begin();
        void beginTransmission(int);
        void endTransmission();
        void send(byte);
        uint8_t receive();
        void requestFrom(int, int);
        bool available();

};

// Keyglove-specific implementations of these, not using Wiring's
int digitalRead(uint8_t pin);
void digitalWrite(uint8_t pin, uint8_t level);
void pinMode(uint8_t pin, uint8_t mode);

void tone(uint8_t _pin, uint16_t frequency, uint32_t duration);
void noTone(uint8_t _pin);

extern USBVirtualSerial Serial;
extern USBMouse Mouse;
extern USBKeyboard Keyboard;
extern WireI2C Wire;

#endif /* _ARDUINO_WRAPPER_H_ */