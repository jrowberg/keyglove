// Keyglove controller source code - Special hardware setup file
// 7/17/2011 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SUPPORT_BOARD_TEENSYPP2_H_
#define _SUPPORT_BOARD_TEENSYPP2_H_

/*
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
We use SCL (0) and SDA (1) for I2C communication, leaving 44 usable pins.
We use RXD (2) and TXD (2) for Bluetooth UART communication, leaving 42 usable pins.
We use INT4 (36) and INT5 (37) for accel and gyro interrupts, leaving 40 usable pins.
We use INT7 (19) for Bluetooth Link interrupts, leaving 39 usable pins.
We use INT6 (18) for DTR control, leaving 38 usable pins.
We use LED for BLINK feedback, leaving 37 usable pins.
...and we have a total of 37 sensors. Yay!

Pin Change interrupts:
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



// ======================== BEGIN PIN DEFINITIONS ========================

#define KEYGLOVE_KIT_BUG_PORTA_REVERSED

/*
#if (KG_HAND == KG_HAND_RIGHT)
    #define KSP_Y   26  // PB6
    #define KSP_Z   25  // PB5
    #define KSP_AA  24  // PB4
    #define KSP_8   23  // PB3
    #define KSP_9   22  // PB2
    #define KSP_0   21  // PB1

    #define KSP_A   20  // PB0
    #define KSP_B   38  // PF0
    #define KSP_C   39  // PF1
    #define KSP_M   40  // PF2
    #define KSP_N   41  // PF3
    #define KSP_O   42  // PF4
    #define KSP_4   43  // PF5

    #define KSP_D   44  // PF6
    #define KSP_E   45  // PF7

    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        #define KSP_F   35  // PA3
        #define KSP_P   34  // PA2
        #define KSP_Q   33  // PA1
        #define KSP_R   32  // PA0
        #define KSP_5   28  // PA4
    
        #define KSP_G   29  // PA5
        #define KSP_H   30  // PA6
        #define KSP_I   31  // PA7
    #else
        #define KSP_F   31  // PA3
        #define KSP_P   30  // PA2
        #define KSP_Q   29  // PA1
        #define KSP_R   28  // PA0
        #define KSP_5   32  // PA4

        #define KSP_G   33  // PA5
        #define KSP_H   34  // PA6
        #define KSP_I   35  // PA7
    #endif

    #define KSP_S   17  // PC7
    #define KSP_T   16  // PC6
    #define KSP_U   15  // PC5
    #define KSP_6   14  // PC4
    
    #define KSP_J   13  // PC3
    #define KSP_K   12  // PC2
    #define KSP_L   11  // PC1
    #define KSP_V   10  // PC0
    #define KSP_W   9   // PE1
    #define KSP_X   8   // PE0
    #define KSP_7   7   // PD7

    #define KSP_1   5   // PD5
    #define KSP_2   4   // PD4
    #define KSP_3   27  // PB7
#else  /* !KG_HAND_RIGHT (KG_HAND_LEFT) */
/*    #define KSP_X   26  // PB6
    #define KSP_W   25  // PB5
    #define KSP_V   24  // PB4
    #define KSP_L   23  // PB3
    #define KSP_K   22  // PB2
    #define KSP_J   21  // PB1

    #define KSP_6   20  // PB0
    #define KSP_U   38  // PF0
    #define KSP_T   39  // PF1
    #define KSP_S   40  // PF2
    #define KSP_I   41  // PF3
    #define KSP_H   42  // PF4
    #define KSP_G   43  // PF5

    #define KSP_5   44  // PF6
    #define KSP_R   45  // PF7

    #define KEYGLOVE_KIT_BUG_PORTA_REVERSED
    #ifdef KEYGLOVE_KIT_BUG_PORTA_REVERSED
        // d'oh! Teensy++ part in Eagle PA0-7 pins were backwards
        // PA0 = PA4, PA1 = PA5, PA2 = PA6, PA3 = PA7, PA4 = PA0, PA5 = PA1, PA6 = PA2, PA7 = PA3
        #define KSP_Q   35  // PA3
        #define KSP_P   34  // PA2
        #define KSP_F   33  // PA1
        #define KSP_E   32  // PA0
        #define KSP_D   28  // PA4

        #define KSP_4   29  // PA5
        #define KSP_O   30  // PA6
        #define KSP_N   31  // PA7
    #else
        #define KSP_Q   31  // PA3
        #define KSP_P   30  // PA2
        #define KSP_F   29  // PA1
        #define KSP_E   28  // PA0
        #define KSP_D   32  // PA4

        #define KSP_4   33  // PA5
        #define KSP_O   34  // PA6
        #define KSP_N   35  // PA7
    #endif

    #define KSP_M   17  // PC7
    #define KSP_C   16  // PC6
    #define KSP_B   15  // PC5
    #define KSP_A   14  // PC4

    #define KSP_0   13  // PC3
    #define KSP_9   12  // PC2
    #define KSP_8   11  // PC1
    #define KSP_AA  10  // PC0
    #define KSP_Z   9   // PE1
    #define KSP_Y   8   // PE0
    #define KSP_7   7   // PD7

    #define KSP_1   5   // PD5
    #define KSP_2   4   // PD4
    #define KSP_3   27  // PB7
#endif /* KG_HAND_LEFT */
/*
#define KBC_Z7 0
#define KBC_Y7 1
#define KBC_J8 2
#define KBC_JY 3
#define KBC_KY 4
#define KBC_LY 5
#define KBC_HY 6
#define KBC_VY 7
#define KBC_WY 8
#define KBC_XY 9
#define KBC_Z6 10
#define KBC_Y6 11
#define KBC_G8 12
#define KBC_GY 13
#define KBC_IY 14
#define KBC_SY 15
#define KBC_TY 16
#define KBC_UY 17
#define KBC_G7 18
#define KBC_Z5 19
#define KBC_Y5 20
#define KBC_D8 21
#define KBC_DY 22
#define KBC_EY 23
#define KBC_FY 24
#define KBC_PZ 25
#define KBC_PY 26
#define KBC_QZ 27
#define KBC_QY 28
#define KBC_RZ 29
#define KBC_RY 30
#define KBC_D4 31
#define KBC_D6 32
#define KBC_D7 33
#define KBC_DM 34
#define KBC_Z4 35
#define KBC_Y4 36
#define KBC_A8 37
#define KBC_MY 38
#define KBC_AY 39
#define KBC_BY 40
#define KBC_CY 41
#define KBC_MZ 42
#define KBC_NZ 43
#define KBC_NY 44
#define KBC_OZ 45
#define KBC_OY 46
#define KBC_A3 47
#define KBC_A2 48
#define KBC_A1 49
#define KBC_D3 50
#define KBC_D2 51
#define KBC_D1 52
#define KBC_G3 53
#define KBC_G2 54
#define KBC_G1 55
#define KBC_J3 56
#define KBC_J2 57
#define KBC_J1 58
#define KBC_Y1 59
#define KBC_AAA 60
#define KBC_DAA 61
#define KBC_GAA 62
#define KBC_JAA 63
#define KBC_AA4 64
#define KBC_AA5 65
#define KBC_AA6 66
#define KBC_AA7 67
#define KBC_A9 68
#define KBC_D9 69
#define KBC_G9 70
#define KBC_J9 71
#define KBC_A0 72
#define KBC_D0 73
#define KBC_G0 74
#define KBC_J0 75
*/



#define USBSerial Serial
#define BT2Serial Serial1

#define KG_HOSTIF_USB_SERIAL_BAUD 115200
#define KG_HOSTIF_USB_SERIAL_BAUD 125000

#define KG_INTERFACE_MODE_NONE                  0x00
#define KG_INTERFACE_MODE_OUTGOING_PACKET       0x01
#define KG_INTERFACE_MODE_INCOMING_PACKET       0x02
//#define KG_INTERFACE_MODE_OUTGOING_INFO         0x04



#define KG_INTERRUPT_PIN_ACCEL  36  // PE4 (internal/tiny, right)
#define KG_INTERRUPT_NUM_ACCEL  4   // Teensy++ interrupt #4

#define KG_INTERRUPT_PIN_GYRO   37  // PE5 (internal/tiny, left)
#define KG_INTERRUPT_NUM_GYRO   5   // Teensy++ interrupt #5

// FUSION and ACCEL are never both used at the same time
#define KG_INTERRUPT_PIN_FUSION 36  // PE4 (internal/tiny, right)
#define KG_INTERRUPT_NUM_FUSION 4   // Teensy++ interrupt #4

#define KG_PIN_BLINK            6   // PD6

#define KG_PIN_BT_RTS           19  // PE7
#define KG_INTERRUPT_NUM_BT_RTS 7   // Teensy++ interrupt #7
#define KG_PIN_BT_CTS           18  // PE6

// ITEMS BELOW THIS CANNOT BE USED SIMULTANEOUSLY WITH ALL ABOVE FEATURES

// direct I/O feedback pins (kills 3 thumb sensors and Bluetooth flow control)
#define KG_PIN_PIEZO        19  // PE7
#define KG_PIN_VIBRATE      18  // PE6
#define KG_PIN_RGB_RED      21  // PB1
#define KG_PIN_RGB_GREEN    22  // PB2
#define KG_PIN_RGB_BLUE     24  // PB4

// PS/2 clock/data pins for keyboard (kills direct I/O piezo/vibe or Bluetooth flow control)
#define KG_PIN_KB_CLOCK     19  // PE7
#define KG_PIN_KB_DATA      18  // PE6

// PS/2 clock/data pins for mouse (kills Bluetooth UART connection)
#define KG_PIN_MOUSE_CLOCK  3   // PD3
#define KG_PIN_MOUSE_DATA   2   // PD2

// ======================== END PIN DEFINITIONS ========================

// sensor count and base combination count
#define KG_TOTAL_SENSORS 37
#define KG_BASE_COMBINATIONS 60
#define KG_BASE_COMBINATION_BYTES 8

// NOTE: KG_BASE_COMBINATIONS seems like it would be very high, but there are
// physical and practical limitations that make this number much smaller

#define KS_DM   ((touches[0] & 0x01) != 0)
#define KS_AY   ((touches[0] & 0x02) != 0)
#define KS_BY   ((touches[0] & 0x04) != 0)
#define KS_CY   ((touches[0] & 0x08) != 0)
#define KS_DY   ((touches[0] & 0x10) != 0)
#define KS_EY   ((touches[0] & 0x20) != 0)
#define KS_FY   ((touches[0] & 0x40) != 0)
#define KS_GY   ((touches[0] & 0x80) != 0)
#define KS_HY   ((touches[1] & 0x01) != 0)
#define KS_IY   ((touches[1] & 0x02) != 0)
#define KS_JY   ((touches[1] & 0x04) != 0)
#define KS_KY   ((touches[1] & 0x08) != 0)
#define KS_LY   ((touches[1] & 0x10) != 0)
#define KS_MY   ((touches[1] & 0x20) != 0)
#define KS_NY   ((touches[1] & 0x40) != 0)
#define KS_OY   ((touches[1] & 0x80) != 0)
#define KS_PY   ((touches[2] & 0x01) != 0)
#define KS_QY   ((touches[2] & 0x02) != 0)
#define KS_RY   ((touches[2] & 0x04) != 0)
#define KS_SY   ((touches[2] & 0x08) != 0)
#define KS_TY   ((touches[2] & 0x10) != 0)
#define KS_UY   ((touches[2] & 0x20) != 0)
#define KS_VY   ((touches[2] & 0x40) != 0)
#define KS_WY   ((touches[2] & 0x80) != 0)
#define KS_XY   ((touches[3] & 0x01) != 0)
#define KS_MZ   ((touches[3] & 0x02) != 0)
#define KS_NZ   ((touches[3] & 0x04) != 0)
#define KS_OZ   ((touches[3] & 0x08) != 0)
#define KS_PZ   ((touches[3] & 0x10) != 0)
#define KS_QZ   ((touches[3] & 0x20) != 0)
#define KS_RZ   ((touches[3] & 0x40) != 0)
#define KS_A1   ((touches[3] & 0x80) != 0)
#define KS_D1   ((touches[4] & 0x01) != 0)
#define KS_G1   ((touches[4] & 0x02) != 0)
#define KS_J1   ((touches[4] & 0x04) != 0)
#define KS_Y1   ((touches[4] & 0x08) != 0)
#define KS_A2   ((touches[4] & 0x10) != 0)
#define KS_D2   ((touches[4] & 0x20) != 0)
#define KS_G2   ((touches[4] & 0x40) != 0)
#define KS_J2   ((touches[4] & 0x80) != 0)
#define KS_A3   ((touches[5] & 0x01) != 0)
#define KS_D3   ((touches[5] & 0x02) != 0)
#define KS_G3   ((touches[5] & 0x04) != 0)
#define KS_J3   ((touches[5] & 0x08) != 0)
#define KS_D4   ((touches[5] & 0x10) != 0)
#define KS_Y4   ((touches[5] & 0x20) != 0)
#define KS_Z4   ((touches[5] & 0x40) != 0)
#define KS_Y5   ((touches[5] & 0x80) != 0)
#define KS_Z5   ((touches[6] & 0x01) != 0)
#define KS_D6   ((touches[6] & 0x02) != 0)
#define KS_Y6   ((touches[6] & 0x04) != 0)
#define KS_Z6   ((touches[6] & 0x08) != 0)
#define KS_D7   ((touches[6] & 0x10) != 0)
#define KS_G7   ((touches[6] & 0x20) != 0)
#define KS_Y7   ((touches[6] & 0x40) != 0)
#define KS_Z7   ((touches[6] & 0x80) != 0)
#define KS_A8   ((touches[7] & 0x01) != 0)
#define KS_D8   ((touches[7] & 0x02) != 0)
#define KS_G8   ((touches[7] & 0x04) != 0)
#define KS_J8   ((touches[7] & 0x08) != 0)

#define KS_ADY  (KS_AY && KS_DY)
#define KS_AJY  (KS_AY && KS_JY)
#define KS_DGY  (KS_DY && KS_GY)
#define KS_GJY  (KS_GY && KS_JY)

#define CLR(x, y) (x &= (~(1 << y)))
#define SET(x, y) (x |= (1 << y))
#define _BV(bit) (1 << (bit))
uint8_t _pina, _pinb, _pinc, _pind, _pine, _pinf;

bool interfaceUSBSerialReady = false;
uint8_t interfaceUSBSerialMode = 0;
bool interfaceUSBRawHIDReady = false;
uint8_t interfaceUSBRawHIDMode = 0;
bool interfaceUSBHIDReady = false;
uint8_t interfaceUSBHIDMode = 0;

ISR(TIMER1_COMPA_vect) {
    keyglove100Hz = 1;
}

void setup_board_teensypp2() {
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
        interfaceUSBSerialMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        interfaceUSBRawHIDReady = true;
        interfaceUSBRawHIDMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;
    #endif

    #if KG_HOSTIF & (KG_HOSTIF_BT2_SERIAL | KG_HOSTIF_BT2_RAWHID | KG_HOSTIF_BT2_HID | KG_HOSTIF_BT2_IAP)
        // start BT2 serial interface if any BT2 interfaces are used
        BT2Serial.begin(KG_HOSTIF_BT2_SERIAL_BAUD);
    #endif
}

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

#endif // _SUPPORT_BOARD_TEENSYPP2_H_