// Keyglove controller source code - Wrapper functions to use Arduino-style code in LUFA environment
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

#include "ArduinoWrapper.h"

extern "C" {
    #include "Keyglove.h"
}

void USBVirtualSerial::begin(uint32_t baud) { /* no baud setting for virtual serial */ }

void USBVirtualSerial::print(const char *str) {
    for (uint16_t i = 0; i < strlen(str); i++) RingBuffer_Insert(&HostTXSerial_Buffer, str[i]);
    CDC_Task();
}
void USBVirtualSerial::println(const char *str) {
    for (uint16_t i = 0; i < strlen(str); i++) RingBuffer_Insert(&HostTXSerial_Buffer, str[i]);
    RingBuffer_Insert(&HostTXSerial_Buffer, '\r');
    RingBuffer_Insert(&HostTXSerial_Buffer, '\n');
    CDC_Task();
}

void USBVirtualSerial::write(uint8_t val) { print(val, BYTE); }
void USBVirtualSerial::print(uint8_t val) { print(val, BYTE); }
void USBVirtualSerial::print(int16_t val) { print(val, BYTE); }
void USBVirtualSerial::print(uint16_t val) { print(val, BYTE); }
void USBVirtualSerial::print(int32_t val) { print(val, BYTE); }
void USBVirtualSerial::print(uint32_t val) { print(val, BYTE); }
void USBVirtualSerial::print(double val) { print(val, BYTE); }

void USBVirtualSerial::print(uint8_t val, uint8_t format) {
    RingBuffer_Insert(&HostTXSerial_Buffer, val);
    CDC_Task();
}
void USBVirtualSerial::print(int16_t val, uint8_t format) { print(itoa(val, numStr, DEC)); }
void USBVirtualSerial::print(uint16_t val, uint8_t format) { print(itoa(val, numStr, DEC)); }
void USBVirtualSerial::print(int32_t val, uint8_t format) { print(ltoa(val, numStr, DEC)); }
void USBVirtualSerial::print(uint32_t val, uint8_t format) { print(ultoa(val, numStr, DEC)); }
void USBVirtualSerial::print(double val, uint8_t format) { print(ultoa(val, numStr, DEC)); }

void USBVirtualSerial::println(uint8_t val) { println(val, BYTE); }
void USBVirtualSerial::println(int16_t val) { println(val, BYTE); }
void USBVirtualSerial::println(uint16_t val) { println(val, BYTE); }
void USBVirtualSerial::println(int32_t val) { println(val, BYTE); }
void USBVirtualSerial::println(uint32_t val) { println(val, BYTE); }
void USBVirtualSerial::println(double val) { print(val, BYTE); }

void USBVirtualSerial::println(uint8_t val, uint8_t format) {
    RingBuffer_Insert(&HostTXSerial_Buffer, val);
    RingBuffer_Insert(&HostTXSerial_Buffer, '\r');
    RingBuffer_Insert(&HostTXSerial_Buffer, '\n');
    CDC_Task();
}
void USBVirtualSerial::println(int16_t val, uint8_t format) { println(itoa(val, numStr, DEC)); }
void USBVirtualSerial::println(uint16_t val, uint8_t format) { println(itoa(val, numStr, DEC)); }
void USBVirtualSerial::println(int32_t val, uint8_t format) { println(ltoa(val, numStr, DEC)); }
void USBVirtualSerial::println(uint32_t val, uint8_t format) { println(ultoa(val, numStr, DEC)); }
void USBVirtualSerial::println(double val, uint8_t format) { println(ultoa(val, numStr, DEC)); }

uint8_t USBVirtualSerial::read() { return 0; }
uint8_t USBVirtualSerial::available() { return 0; }

void HardwareSerial::begin(uint32_t baud) { /*Serial_Init(baud, false);*/ }
// bool Serial_IsCharReceived();

void HardwareSerial::write(uint8_t val) { print(val, BYTE); }
void HardwareSerial::print(const char *str) { }
void HardwareSerial::print(uint8_t val) { }
void HardwareSerial::print(uint16_t val) { }
void HardwareSerial::print(uint32_t val) { }
void HardwareSerial::print(long double val) { }

void HardwareSerial::print(const char *str, int16_t format) { }
void HardwareSerial::print(uint8_t val, int16_t format) { }
void HardwareSerial::print(uint16_t val, int16_t format) { }
void HardwareSerial::print(uint32_t val, int16_t format) { }
void HardwareSerial::print(long double val, int16_t format) { }

void HardwareSerial::println(const char *str) { }
void HardwareSerial::println(uint8_t val) { }
void HardwareSerial::println(uint16_t val) { }
void HardwareSerial::println(uint32_t val) { }
void HardwareSerial::println(long double val) { }

void HardwareSerial::println(const char *str, int16_t format) { }
void HardwareSerial::println(uint8_t val, int16_t format) { }
void HardwareSerial::println(uint16_t val, int16_t format) { }
void HardwareSerial::println(uint32_t val, int16_t format) { }
void HardwareSerial::println(long double val, int16_t format) { }

uint8_t HardwareSerial::read() { return 0; }
uint8_t HardwareSerial::available() { return 0; }

void USBMouse::move(uint8_t x, uint8_t y) {
    move(x, y, 0);
}
void USBMouse::move(uint8_t x, uint8_t y, uint8_t z) {
    HIDReportInData[0] = HID_REPORTID_MouseReport;
    report[1] = x;
    report[2] = y;
    report[3] = z;
    memcpy(HIDReportInData + 1, report, 8);
    HID_Task();
}
void USBMouse::set_buttons(uint8_t b1, uint8_t b2, uint8_t b3) {
    HIDReportInData[0] = HID_REPORTID_MouseReport;
    report[0] = (b1 << 2) + (b2 << 1) + b3;
    memcpy(HIDReportInData + 1, report, 8);
    HID_Task();
}
void USBMouse::scroll(uint8_t z) {
    HIDReportInData[0] = HID_REPORTID_MouseReport;
    report[1] = 0;
    report[2] = 0;
    report[3] = z;
    memcpy(HIDReportInData + 1, report, 8);
    HID_Task();
}

void USBKeyboard::set_modifier(uint8_t modifier) {
    report[0] = modifier;
}
void USBKeyboard::set_key1(uint8_t code) {
    report[2] = code;
}
void USBKeyboard::set_key2(uint8_t code) {
    report[3] = code;
}
void USBKeyboard::set_key3(uint8_t code) {
    report[4] = code;
}
void USBKeyboard::set_key4(uint8_t code) {
    report[5] = code;
}
void USBKeyboard::set_key5(uint8_t code) {
    report[6] = code;
}
void USBKeyboard::set_key6(uint8_t code) {
    report[7] = code;
}
void USBKeyboard::send_now() {
    HIDReportInData[0] = HID_REPORTID_KeyboardReport;
    memcpy(HIDReportInData + 1, report, 8);
    HID_Task();
}

int16_t digitalRead(uint8_t pin) {
    if (pin >= 0 && pin < 8) {
        return PIND & (1 << pin);
    } else if (pin >= 8 && pin < 10) {
        return PINE & (1 << (pin - 8));
    } else if (pin >= 10 && pin < 18) {
        return PINC & (1 << (pin - 10));
    } else if (pin >= 18 && pin < 20) {
        return PINE & (1 << (pin - 18 + 6));
    } else if (pin >= 20 && pin < 28) {
        return PINB & (1 << (pin - 20));
    } else if (pin >= 28 && pin < 36) {
        return PINA & (1 << (pin - 28));
    } else if (pin >= 36 && pin < 38) {
        return PINE & (1 << (pin - 36 + 4));
    } else if (pin >= 38 && pin < 46) {
        return PINF & (1 << (pin - 38));
    }
    return 0;
}

void digitalWrite(uint8_t pin, uint8_t level) {
    if (level == LOW) {
        if (pin >= 0 && pin < 8) {
            PORTD &= ~(1 << pin);
        } else if (pin >= 8 && pin < 10) {
            PORTE &= ~(1 << (pin - 8));
        } else if (pin >= 10 && pin < 18) {
            PORTC &= ~(1 << (pin - 10));
        } else if (pin >= 18 && pin < 20) {
            PORTE &= ~(1 << (pin - 18 + 6));
        } else if (pin >= 20 && pin < 28) {
            PORTB &= ~(1 << (pin - 20));
        } else if (pin >= 28 && pin < 36) {
            PORTA &= ~(1 << (pin - 28));
        } else if (pin >= 36 && pin < 38) {
            PORTE &= ~(1 << (pin - 36 + 4));
        } else if (pin >= 38 && pin < 46) {
            PORTF &= ~(1 << (pin - 38));
        }
    } else {
        if (pin >= 0 && pin < 8) {
            PORTD |= (1 << pin);
        } else if (pin >= 8 && pin < 10) {
            PORTE |= (1 << (pin - 8));
        } else if (pin >= 10 && pin < 18) {
            PORTC |= (1 << (pin - 10));
        } else if (pin >= 18 && pin < 20) {
            PORTE |= (1 << (pin - 18 + 6));
        } else if (pin >= 20 && pin < 28) {
            PORTB |= (1 << (pin - 20));
        } else if (pin >= 28 && pin < 36) {
            PORTA |= (1 << (pin - 28));
        } else if (pin >= 36 && pin < 38) {
            PORTE |= (1 << (pin - 36 + 4));
        } else if (pin >= 38 && pin < 46) {
            PORTF |= (1 << (pin - 38));
        }
    }
}

void pinMode(uint8_t pin, uint8_t mode) {
    if (mode == INPUT) {
        if (pin >= 0 && pin < 8) {
            DDRD &= ~(1 << pin);
        } else if (pin >= 8 && pin < 10) {
            DDRE &= ~(1 << (pin - 8));
        } else if (pin >= 10 && pin < 18) {
            DDRC &= ~(1 << (pin - 10));
        } else if (pin >= 18 && pin < 20) {
            DDRE &= ~(1 << (pin - 18 + 6));
        } else if (pin >= 20 && pin < 28) {
            DDRB &= ~(1 << (pin - 20));
        } else if (pin >= 28 && pin < 36) {
            DDRA &= ~(1 << (pin - 28));
        } else if (pin >= 36 && pin < 38) {
            DDRE &= ~(1 << (pin - 36 + 4));
        } else if (pin >= 38 && pin < 46) {
            DDRF &= ~(1 << (pin - 38));
        }
    } else {
        if (pin >= 0 && pin < 8) {
            DDRD |= (1 << pin);
        } else if (pin >= 8 && pin < 10) {
            DDRE |= (1 << (pin - 8));
        } else if (pin >= 10 && pin < 18) {
            DDRC |= (1 << (pin - 10));
        } else if (pin >= 18 && pin < 20) {
            DDRE |= (1 << (pin - 18 + 6));
        } else if (pin >= 20 && pin < 28) {
            DDRB |= (1 << (pin - 20));
        } else if (pin >= 28 && pin < 36) {
            DDRA |= (1 << (pin - 28));
        } else if (pin >= 36 && pin < 38) {
            DDRE |= (1 << (pin - 36 + 4));
        } else if (pin >= 38 && pin < 46) {
            DDRF |= (1 << (pin - 38));
        }
    }
}

/*
// reference: 440hz = 2273 usec/cycle, 1136 usec/half-cycle
#define DELAY_OFFSET 11

void tone(uint8_t _pin, uint16_t frequency, uint32_t duration) {
    uint32_t us = (500000 / frequency) - DELAY_OFFSET;
    uint32_t rep = (duration * 500L) / (us + DELAY_OFFSET);
    for (int16_t i = 0; i < rep; i++) {
        digitalWrite(_pin, HIGH);
        delayMicroseconds(us);
        digitalWrite(_pin, LOW);
        delayMicroseconds(us);
    }
}

void noTone(uint8_t _pin) {
}
*/

USBVirtualSerial Serial;
USBMouse Mouse;
USBKeyboard Keyboard;