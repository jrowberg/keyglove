// Keyglove controller source code - Host MCU definitions specific to Teensy++ v2.0 / Touch:37
// 7/4/2014 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

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

/**
 * @file support_board_teensypp2_t37.h
 * @brief Host MCU definitions specific to Teensy++ v2.0 / Touch:37
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file defines all of the hardware-specific parts of the firmware that are
 * unique to the Teensy++ v2.0 platform. Any other hardware-specific code should
 * be added here and abstracted in such a way that other files can use those new
 * features without knowing what is actually happening behind the scenes (here).
 *
 * Normally it is not necessary to edit this file.
 *

Teensy++ 2.0 Pin arrangement:

                     __|||||__
                GND |   USB   | VCC
        3        27 |         | 26        Y
              SCL 0 |         | 25        Z
              SDA 1 |         | 24        a*
              RXD 2 |         | 23        8
              TXD 3 |         | 22        9
        2         4 |  37.36  | 21        0
        1         5 |         | 20        A
              LED 6 |         | 19 INT7
        7         7 |         | 18 INT6
        X         8 |         | GND
        W         9 |         | AREF
        V        10 |         | 38        B
        L    5   11 | 32 . 28 | 39  R     C
        K    G   12 | 33 . 29 | 40  Q     M
        J    H   13 | 34 . 30 | 41  P     N
        6    I   14 | 35 . 31 | 42  F     O
        U        15 |         | 43        4
        T        16 |         | 44        D
        S        17 |_________| 45        E

                  36=INT4, 37=INT5

This is a grand total of 46 usable I/O pins.

- We use SCL (0) and SDA (1) for I2C communication, leaving 44 usable pins.
- We use RXD (2) and TXD (3) for Bluetooth UART communication, leaving 42 usable pins.
- We use INT4 (36) and INT5 (37) for accel and gyro interrupts, leaving 40 usable pins.
- We use INT7 (19) for Bluetooth Link interrupts, leaving 39 usable pins.
- We use INT6 (18) for DTR control, leaving 38 usable pins.
- We use LED (6) for BLINK feedback, leaving 37 usable pins.
- ...and we have a total of 37 sensors. Yay!

Pin Change interrupts (not currently used, could help with sleep modes):

- Y, 26, PB6
- Z, 25, PB5
- a*, 24, PB4
- 8, 23, PB3
- 9, 22, PB2
- 0, 21, PB1
- 1, 5, PD5
- 2, 4, PD4
- 3, 27, PB7

For the sake of the Keyglove Kit board, the sensor pin connections should be arranged
in a clockwise manner as Thumb, Index, Middle, Ring, Little, with each individual
finger's sensors in increasing alphabetical order: (Y,Z,a*,8,9,0), (A,B,C,M,N,O,4),
and so on. Start with pin 26 (B6)

 */

#ifndef _SUPPORT_BOARD_TEENSYPP2_T37_H_
#define _SUPPORT_BOARD_TEENSYPP2_T37_H_

// ======================== BEGIN PIN DEFINITIONS ========================

//#define KEYGLOVE_KIT_BUG_PORTA_REVERSED     ///< Software fix for internal Port A pin position bug in Eagle package

#define USBSerial Serial                    ///< USB serial interface name
#define BT2Serial Serial1                   ///< Bluetooth module UART interface name

#define KG_HOSTIF_USB_SERIAL_BAUD 115200    ///< USB serial interface baud rate
#define KG_HOSTIF_BT2_SERIAL_BAUD 125000    ///< Bluetooth module UART interface baud rate

#define KG_INTERRUPT_PIN_MPU6050_HAND 36    ///< PE4 (internal/tiny, right)
#define KG_INTERRUPT_NUM_MPU6050_HAND 4     ///< Teensy++ interrupt #4

#define KG_PIN_BLINK                6       ///< PD6

// ======================== END PIN DEFINITIONS ========================

// sensor count and base combination count
#define KG_TOTAL_SENSORS 37                 ///< Total sensor count with most efficient pin configuration
#define KG_BASE_COMBINATIONS 60             ///< Number of physically reasonable 1-to-1 touch combinations
#define KG_BASE_COMBINATION_BYTES 8         ///< Number of bytes required to store all touch status bits

// NOTE: KG_BASE_COMBINATIONS seems like it would be very high, but there are
// physical and practical limitations that make this number much smaller

#define KGT_DM(test) (test[0] & 0x01)
#define KGT_AY(test) (test[0] & 0x02)
#define KGT_BY(test) (test[0] & 0x04)
#define KGT_CY(test) (test[0] & 0x08)
#define KGT_DY(test) (test[0] & 0x10)
#define KGT_EY(test) (test[0] & 0x20)
#define KGT_FY(test) (test[0] & 0x40)
#define KGT_GY(test) (test[0] & 0x80)
#define KGT_HY(test) (test[1] & 0x01)
#define KGT_IY(test) (test[1] & 0x02)
#define KGT_JY(test) (test[1] & 0x04)
#define KGT_KY(test) (test[1] & 0x08)
#define KGT_LY(test) (test[1] & 0x10)
#define KGT_MY(test) (test[1] & 0x20)
#define KGT_NY(test) (test[1] & 0x40)
#define KGT_OY(test) (test[1] & 0x80)
#define KGT_PY(test) (test[2] & 0x01)
#define KGT_QY(test) (test[2] & 0x02)
#define KGT_RY(test) (test[2] & 0x04)
#define KGT_SY(test) (test[2] & 0x08)
#define KGT_TY(test) (test[2] & 0x10)
#define KGT_UY(test) (test[2] & 0x20)
#define KGT_VY(test) (test[2] & 0x40)
#define KGT_WY(test) (test[2] & 0x80)
#define KGT_XY(test) (test[3] & 0x01)
#define KGT_MZ(test) (test[3] & 0x02)
#define KGT_NZ(test) (test[3] & 0x04)
#define KGT_OZ(test) (test[3] & 0x08)
#define KGT_PZ(test) (test[3] & 0x10)
#define KGT_QZ(test) (test[3] & 0x20)
#define KGT_RZ(test) (test[3] & 0x40)
#define KGT_A1(test) (test[3] & 0x80)
#define KGT_D1(test) (test[4] & 0x01)
#define KGT_G1(test) (test[4] & 0x02)
#define KGT_J1(test) (test[4] & 0x04)
#define KGT_Y1(test) (test[4] & 0x08)
#define KGT_A2(test) (test[4] & 0x10)
#define KGT_D2(test) (test[4] & 0x20)
#define KGT_G2(test) (test[4] & 0x40)
#define KGT_J2(test) (test[4] & 0x80)
#define KGT_A3(test) (test[5] & 0x01)
#define KGT_D3(test) (test[5] & 0x02)
#define KGT_G3(test) (test[5] & 0x04)
#define KGT_J3(test) (test[5] & 0x08)
#define KGT_D4(test) (test[5] & 0x10)
#define KGT_Y4(test) (test[5] & 0x20)
#define KGT_Z4(test) (test[5] & 0x40)
#define KGT_Y5(test) (test[5] & 0x80)
#define KGT_Z5(test) (test[6] & 0x01)
#define KGT_D6(test) (test[6] & 0x02)
#define KGT_Y6(test) (test[6] & 0x04)
#define KGT_Z6(test) (test[6] & 0x08)
#define KGT_D7(test) (test[6] & 0x10)
#define KGT_G7(test) (test[6] & 0x20)
#define KGT_Y7(test) (test[6] & 0x40)
#define KGT_Z7(test) (test[6] & 0x80)
#define KGT_A8(test) (test[7] & 0x01)
#define KGT_D8(test) (test[7] & 0x02)
#define KGT_G8(test) (test[7] & 0x04)
#define KGT_J8(test) (test[7] & 0x08)

#define KGT_ADY (KGT_AY && KGT_DY)
#define KGT_AJY (KGT_AY && KGT_JY)
#define KGT_DGY (KGT_DY && KGT_GY)
#define KGT_GJY (KGT_GY && KGT_JY)

#define CLR(x, y) (x &= (~(1 << y)))    ///< Bit-clearing macro for port/pin combination
#define SET(x, y) (x |= (1 << y))       ///< Bit-setting macro for port/pin combination
#define _BV(bit) (1 << (bit))           ///< Bit-value calculation macro for lazy people

uint8_t _pina;                          ///< Container for reading Port A logic state
uint8_t _pinb;                          ///< Container for reading Port B logic state
uint8_t _pinc;                          ///< Container for reading Port C logic state
uint8_t _pind;                          ///< Container for reading Port D logic state
uint8_t _pine;                          ///< Container for reading Port E logic state
uint8_t _pinf;                          ///< Container for reading Port F logic state

bool interfaceUSBSerialReady = false;   ///< Status indicator for USB serial interface
uint8_t interfaceUSBSerialMode = 0;     ///< USB serial communication mode setting @see KG_INTERFACE_MODE_NONE, @see KG_INTERFACE_MODE_OUTGOING_PACKET, @see KG_INTERFACE_MODE_INCOMING_PACKET
bool interfaceUSBRawHIDReady = false;   ///< Status indicator for USB raw HID interface
uint8_t interfaceUSBRawHIDMode = 0;     ///< USB raw HID communication mode setting @see KG_INTERFACE_MODE_NONE, @see KG_INTERFACE_MODE_OUTGOING_PACKET, @see KG_INTERFACE_MODE_INCOMING_PACKET
bool interfaceUSBHIDReady = false;      ///< Status indicator for USB HID interface

/**
 * @brief Hardware timer comparator interrupt for tracking 100Hz ticks
 */
ISR(TIMER1_COMPA_vect) {
    keyglove100Hz = 1;
}

/**
 * @brief Initialize Teensy++ v2.0 board hardware/registers (37-sensor arrangement)
 *
 * This function sets the required hardware timer for a 100Hz tick interrupt,
 * initializes touch sensor pins to be pulled high, and starts the necessary
 * USB serial and/or hardware UART interfaces for host and Bluetooth control.
 *
 * @see setup()
 */
void setup_board_teensypp2_t37() {
    // setup internal 100Hz "tick" interrupt
    // thanks to http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1212098919 (and 'bens')
    // also, lots of timer info here and here:
    //    http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=50106
    //    http://www.avrbeginners.net/architecture/timers/timers.html
    // also, section 15.10 of the AT90USB128x datasheet:
    //    http://www.atmel.com/Images/doc7593.pdf

    // set up Timer1
    //   WGM13 = 0  \
    //   WGM12 = 1   | --> CTC mode, TOP=OCR1A
    //   WGM11 = 0   |
    //   WGM10 = 0  /
    //   CS12 = 1   \
    //   CS11 = 0    | --> clk/256 prescaler
    //   CS10 = 0   /

    TCCR1A = 0x00;  // TCCR1A: COM1A1=0, COM1A0=0, COM1B1=0, COM1B0=0, COM1C1=0, COM1C0=0, WGM11=0, WGM10=0
    TCCR1B = 0x0A;  // TCCR1B: ICNC1=0, ICES1=0, -, WGM13=1, WGM12=1, CS12=1, CS11=0, CS10=0
    OCR1A = 0x2710; // 10ms interval @ 8MHz
    //OCR1A = 0x0500; // speed up ALL THE THINGS!!
    TIMSK1 |= (1 << OCIE1A); // enable TIMER1 output compare match interrupt

    // setup touch sensors and make sure we enable internal pullup resistors
    DDRA  &= 0x00;
    PORTA |= 0xFF; // 0,1,2,3,4,5,6,7
    DDRB  &= 0x00;
    PORTB |= 0xFF; // 0,1,2,3,4,5,6,7
    DDRC  &= 0x00;
    PORTC |= 0xFF; // 0,1,2,3,4,5,6,7
    DDRD  &= 0x4F;
    PORTD |= 0xB0; // 4,5,7
    DDRE  &= 0xFC;
    PORTE |= 0x03; // 0,1
    DDRF  &= 0x00;
    PORTF |= 0xFF; // 0,1,2,3,4,5,6,7

    /*
    Pin Change interrupts for thumb points:
    - Y, 26, PB6
    - Z, 25, PB5
    - a*, 24, PB4
    - 8, 23, PB3
    - 9, 22, PB2
    - 0, 21, PB1
    */

    //PCICR = 0x01; // PCIE0=1, pin change interrupt 0 enabled
    //PCMSK0 = 0x7E; // enable pin change interupts on PB1 - PB6

    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        // start USB serial interface
        USBSerial.begin(KG_HOSTIF_USB_SERIAL_BAUD);
        interfaceUSBSerialReady = true;
        interfaceUSBSerialMode = KG_APIMODE_USB_SERIAL;
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        interfaceUSBRawHIDReady = true;
        interfaceUSBRawHIDMode = KG_APIMODE_USB_RAWHID;
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_HID
        interfaceUSBHIDReady = true;
    #endif

    #if KG_HOSTIF & (KG_HOSTIF_BT2_SERIAL | KG_HOSTIF_BT2_RAWHID | KG_HOSTIF_BT2_HID | KG_HOSTIF_BT2_IAP)
        // start BT2 serial interface if any BT2 interfaces are used
        BT2Serial.begin(KG_HOSTIF_BT2_SERIAL_BAUD);
    #endif
}

/**
 * @brief Get status of all touch sensors via direct port read/write operations
 *
 * This is a very fast AT90USB128x-specific pin polling routine. It's so fast
 * that I actually had to add manual delays after setting each pin low in order
 * for the logic to take effect when reading the connected pin states on the
 * following line of code.
 *
 * @see loop()
 * @see update_touch()
 */
void update_board_touch(uint8_t *touches) {
    // check on M combinations (PF2)
    SET(DDRF, 2);       // set to OUTPUT
    CLR(PORTF, 2);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinf = PINF;
    CLR(DDRF, 2);       // set to INPUT
    SET(PORTF, 2);      // pull HIGH
    if (!(_pinf & (1 << 6))) touches[0] |= 0x01;    // D (PF6)
//     { KSP_D, KSP_M /* 34 DM */ },

    // check on Y combinations (PB6)
    SET(DDRB, 6);       // set to OUTPUT
    CLR(PORTB, 6);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pind = PIND; _pine = PINE; _pinf = PINF;
    CLR(DDRB, 6);       // set to INPUT
    SET(PORTB, 6);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[0] |= 0x02;    // A (PB0)
    if (!(_pinf & (1 << 0))) touches[0] |= 0x04;    // B (PF0)
    if (!(_pinf & (1 << 1))) touches[0] |= 0x08;    // C (PF1)
    if (!(_pinf & (1 << 6))) touches[0] |= 0x10;    // D (PF6)
    if (!(_pinf & (1 << 7))) touches[0] |= 0x20;    // E (PF7)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 7))) touches[0] |= 0x40;    // F (PA7 !PA3)
        if (!(_pina & (1 << 1))) touches[0] |= 0x80;    // G (PA1 !PA5)
        if (!(_pina & (1 << 2))) touches[1] |= 0x01;    // H (PA2 !PA6)
        if (!(_pina & (1 << 3))) touches[1] |= 0x02;    // I (PA3 !PA7)
    #else
        if (!(_pina & (1 << 3))) touches[0] |= 0x40;    // F (PA3)
        if (!(_pina & (1 << 5))) touches[0] |= 0x80;    // G (PA5)
        if (!(_pina & (1 << 6))) touches[1] |= 0x01;    // H (PA6)
        if (!(_pina & (1 << 7))) touches[1] |= 0x02;    // I (PA7)
    #endif
    if (!(_pinc & (1 << 3))) touches[1] |= 0x04;    // J (PC3)
    if (!(_pinc & (1 << 2))) touches[1] |= 0x08;    // K (PC2)
    if (!(_pinc & (1 << 1))) touches[1] |= 0x10;    // L (PC1)
    if (!(_pinf & (1 << 2))) touches[1] |= 0x20;    // M (PF2)
    if (!(_pinf & (1 << 3))) touches[1] |= 0x40;    // N (PF3)
    if (!(_pinf & (1 << 4))) touches[1] |= 0x80;    // O (PF4)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 6))) touches[2] |= 0x01;    // P (PA6 !PA2)
        if (!(_pina & (1 << 5))) touches[2] |= 0x02;    // Q (PA5 !PA1)
        if (!(_pina & (1 << 4))) touches[2] |= 0x04;    // R (PA4 !PA0)
    #else
        if (!(_pina & (1 << 2))) touches[2] |= 0x01;    // P (PA2)
        if (!(_pina & (1 << 1))) touches[2] |= 0x02;    // Q (PA1)
        if (!(_pina & (1 << 0))) touches[2] |= 0x04;    // R (PA0)
    #endif
    if (!(_pinc & (1 << 7))) touches[2] |= 0x08;    // S (PC7)
    if (!(_pinc & (1 << 6))) touches[2] |= 0x10;    // T (PC6)
    if (!(_pinc & (1 << 5))) touches[2] |= 0x20;    // U (PC5)
    if (!(_pinc & (1 << 0))) touches[2] |= 0x40;    // V (PC0)
    if (!(_pine & (1 << 1))) touches[2] |= 0x80;    // W (PE1)
    if (!(_pine & (1 << 0))) touches[3] |= 0x01;    // X (PE0)
//     { KSP_A, KSP_Y /* 39 AY */ },
//     { KSP_B, KSP_Y /* 40 BY */ },
//     { KSP_C, KSP_Y /* 41 CY */ },
//     { KSP_D, KSP_Y /* 22 DY */ },
//     { KSP_E, KSP_Y /* 23 EY */ },
//     { KSP_F, KSP_Y /* 24 FY */ },
//     { KSP_G, KSP_Y /* 13 GY */ },
//     { KSP_H, KSP_Y /* 6 HY */ },
//     { KSP_I, KSP_Y /* 14 IY */ },
//     { KSP_J, KSP_Y /* 3 JY */ },
//     { KSP_K, KSP_Y /* 4 KY */ },
//     { KSP_L, KSP_Y /* 5 LY */ },
//     { KSP_M, KSP_Y /* 38 MY */ },
//     { KSP_N, KSP_Y /* 44 NY */ },
//     { KSP_O, KSP_Y /* 46 OY */ },
//     { KSP_P, KSP_Y /* 26 PY */ },
//     { KSP_Q, KSP_Y /* 28 QY */ },
//     { KSP_R, KSP_Y /* 30 RY */ },
//     { KSP_S, KSP_Y /* 15 SY */ },
//     { KSP_T, KSP_Y /* 16 TY */ },
//     { KSP_U, KSP_Y /* 17 UY */ },
//     { KSP_V, KSP_Y /* 7 VY */ },
//     { KSP_W, KSP_Y /* 8 WY */ },
//     { KSP_X, KSP_Y /* 9 XY */ },

    // check on Z combinations (PB5)
    SET(DDRB, 5);       // set to OUTPUT
    CLR(PORTB, 5);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinf = PINF;
    CLR(DDRB, 5);       // set to INPUT
    SET(PORTB, 5);      // pull HIGH
    if (!(_pinf & (1 << 2))) touches[3] |= 0x02;    // M (PF2)
    if (!(_pinf & (1 << 3))) touches[3] |= 0x04;    // N (PF3)
    if (!(_pinf & (1 << 4))) touches[3] |= 0x08;    // O (PF4)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 6))) touches[3] |= 0x10;    // P (PA6 !PA2)
        if (!(_pina & (1 << 5))) touches[3] |= 0x20;    // Q (PA5 !PA1)
        if (!(_pina & (1 << 4))) touches[3] |= 0x40;    // R (PA4 !PA0)
    #else
        if (!(_pina & (1 << 2))) touches[3] |= 0x10;    // P (PA2)
        if (!(_pina & (1 << 1))) touches[3] |= 0x20;    // Q (PA1)
        if (!(_pina & (1 << 0))) touches[3] |= 0x40;    // R (PA0)
    #endif
//     { KSP_M, KSP_Z /* 42 MZ */ },
//     { KSP_N, KSP_Z /* 43 NZ */ },
//     { KSP_O, KSP_Z /* 45 OZ */ },
//     { KSP_P, KSP_Z /* 25 PZ */ },
//     { KSP_Q, KSP_Z /* 27 QZ */ },
//     { KSP_R, KSP_Z /* 29 RZ */ },

    // check on 1 combinations (PD5)
    SET(DDRD, 5);       // set to OUTPUT
    CLR(PORTD, 5);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRD, 5);       // set to INPUT
    SET(PORTD, 5);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[3] |= 0x80;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[4] |= 0x01;    // D (PF6)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 1))) touches[4] |= 0x02;    // G (PA1 !PA5)
    #else
        if (!(_pina & (1 << 5))) touches[4] |= 0x02;    // G (PA5)
    #endif
    if (!(_pinc & (1 << 3))) touches[4] |= 0x04;    // J (PC3)
    if (!(_pinb & (1 << 6))) touches[4] |= 0x08;    // Y (PB6)
//     { KSP_A, KSP_1 /* 49 A1 */ },
//     { KSP_D, KSP_1 /* 52 D1 */ },
//     { KSP_G, KSP_1 /* 55 G1 */ },
//     { KSP_J, KSP_1 /* 58 J1 */ },
//     { KSP_Y, KSP_1 /* 59 Y1 */ }

    // check on 2 combinations (PD4)
    SET(DDRD, 4);       // set to OUTPUT
    CLR(PORTD, 4);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRD, 4);       // set to INPUT
    SET(PORTD, 4);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[4] |= 0x10;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[4] |= 0x20;    // D (PF6)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 1))) touches[4] |= 0x40;    // G (PA1 !PA5)
    #else
        if (!(_pina & (1 << 5))) touches[4] |= 0x40;    // G (PA5)
    #endif
    if (!(_pinc & (1 << 3))) touches[4] |= 0x80;    // J (PC3)
//     { KSP_A, KSP_2 /* 48 A2 */ },
//     { KSP_D, KSP_2 /* 51 D2 */ },
//     { KSP_G, KSP_2 /* 57 G2 */ },
//     { KSP_J, KSP_2 /* 54 J2 */ },

    // check on 3 combinations (PB7)
    SET(DDRB, 7);       // set to OUTPUT
    CLR(PORTB, 7);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRB, 7);       // set to INPUT
    SET(PORTB, 7);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[5] |= 0x01;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[5] |= 0x02;    // D (PF6)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 1))) touches[5] |= 0x04;    // G (PA1 !PA5)
    #else
        if (!(_pina & (1 << 5))) touches[5] |= 0x04;    // G (PA5)
    #endif
    if (!(_pinc & (1 << 3))) touches[5] |= 0x08;    // J (PC3)
//     { KSP_A, KSP_3 /* 47 A3 */ },
//     { KSP_D, KSP_3 /* 50 D3 */ },
//     { KSP_G, KSP_3 /* 56 G3 */ },
//     { KSP_J, KSP_3 /* 53 J3 */ },

    // check on 4 combinations (PF5)
    SET(DDRF, 5);       // set to OUTPUT
    CLR(PORTF, 5);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinb = PINB; _pinf = PINF;
    CLR(DDRF, 5);       // set to INPUT
    SET(PORTF, 5);      // pull HIGH
    if (!(_pinf & (1 << 6))) touches[5] |= 0x10;    // D (PF6)
    if (!(_pinb & (1 << 6))) touches[5] |= 0x20;    // Y (PB6)
    if (!(_pinb & (1 << 5))) touches[5] |= 0x40;    // Z (PB5)
//     { KSP_D, KSP_4 /* 31 D4 */ },
//     { KSP_Y, KSP_4 /* 36 Y4 */ },
//     { KSP_Z, KSP_4 /* 35 Z4 */ },

    // check on 5 combinations (PA4)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        SET(DDRA, 0);       // set to OUTPUT
        CLR(PORTA, 0);      // set to LOW
    #else
        SET(DDRA, 4);       // set to OUTPUT
        CLR(PORTA, 4);      // set to LOW
    #endif
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinb = PINB;
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        CLR(DDRA, 0);       // set to OUTPUT
        SET(PORTA, 0);      // set to LOW
    #else
        CLR(DDRA, 4);       // set to INPUT
        SET(PORTA, 4);      // pull HIGH
    #endif
    if (!(_pinb & (1 << 6))) touches[5] |= 0x80;    // Y (PB6)
    if (!(_pinb & (1 << 5))) touches[6] |= 0x01;    // Z (PB5)
//     { KSP_Y, KSP_5 /* 20 Y5 */ },
//     { KSP_Z, KSP_5 /* 19 Z5 */ },

    // check on 6 combinations (PC4)
    SET(DDRC, 4);       // set to OUTPUT
    CLR(PORTC, 4);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinb = PINB; _pinf = PINF;
    CLR(DDRC, 4);       // set to INPUT
    SET(PORTC, 4);      // pull HIGH
    if (!(_pinf & (1 << 6))) touches[6] |= 0x02;    // D (PF6)
    if (!(_pinb & (1 << 6))) touches[6] |= 0x04;    // Y (PB6)
    if (!(_pinb & (1 << 5))) touches[6] |= 0x08;    // Z (PB5)
//     { KSP_D, KSP_6 /* 32 D6 */ },
//     { KSP_Y, KSP_6 /* 11 Y6 */ },
//     { KSP_Z, KSP_6 /* 10 Z6 */ },

    // check on 7 combinations (PD7)
    SET(DDRD, 7);       // set to OUTPUT
    CLR(PORTD, 7);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinf = PINF;
    CLR(DDRD, 7);       // set to INPUT
    SET(PORTD, 7);      // pull HIGH
    if (!(_pinf & (1 << 6))) touches[6] |= 0x10;    // D (PF6)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 1))) touches[6] |= 0x20;    // G (PA1 !PA5)
    #else
        if (!(_pina & (1 << 5))) touches[6] |= 0x20;    // G (PA5)
    #endif
    if (!(_pinb & (1 << 6))) touches[6] |= 0x40;    // Y (PB6)
    if (!(_pinb & (1 << 5))) touches[6] |= 0x80;    // Z (PB5)
//     { KSP_D, KSP_7 /* 33 D7 */ },
//     { KSP_G, KSP_7 /* 18 G7 */ },
//     { KSP_Y, KSP_7 /* 1 Y7 */ },
//     { KSP_Z, KSP_7 /* 0 Z7 */ },

    // check on 8 combinations (PB3)
    SET(DDRB, 3);       // set to OUTPUT
    CLR(PORTB, 3);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRB, 3);       // set to INPUT
    SET(PORTB, 3);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[7] |= 0x01;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[7] |= 0x02;    // D (PF6)
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        if (!(_pina & (1 << 1))) touches[7] |= 0x04;    // G (PA1 !PA5)
    #else
        if (!(_pina & (1 << 5))) touches[7] |= 0x04;    // G (PA5)
    #endif
    if (!(_pinc & (1 << 3))) touches[7] |= 0x08;    // J (PC3)
//     { KSP_A, KSP_8 /* 37 A8 */ },
//     { KSP_D, KSP_8 /* 21 D8 */ },
//     { KSP_G, KSP_8 /* 12 G8 */ },
//     { KSP_J, KSP_8 /* 2 J8 */ },
}

#endif // _SUPPORT_BOARD_TEENSYPP2_T37_H_