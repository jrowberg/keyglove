// Keyglove controller source code - Host MCU definitions specific to Teensy++ v2.0 / Touch:19
// 8/4/2014 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file support_board_teensypp2_t19.h
 * @brief Host MCU definitions specific to Teensy++ v2.0 / Touch:19
 * @author Jeff Rowberg
 * @date 2014-08-04
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
        1        PB7 27 |         | 26 PB6        Y
            SCL  PD0  0 |         | 25 PB5        8
            SDA  PD1  1 |         | 24 PB4    SPK
            RXD  PD2  2 |         | 23 PB3    VIB
            TXD  PD3  3 |         | 22 PB2    CS2 (STAT2)
            RTS  PD4  4 |  37.36  | 21 PB1    CS1 (STAT1)
        7        PD5  5 |         | 20 PB0    CS0 (/PG)
            LED  PD6  6 |         | 19 PE7    CTS (INT7)
        L        PD7  7 |         | 18 PE6    MPU (INT6)
        K        PE0  8 |         | GND
        J        PE1  9 |         | AREF
        6        PC0 10 |         | 38 PF0    BAT (VBDIV)
        I        PC1 11 | 32 . 28 | 39 PF1        A
        H        PC2 12 | 33 . 29 | 40 PF2        B
        G        PC3 13 | 34 . 30 | 41 PF3        C
            RED  PC4 14 | 35 . 31 | 42 PF4        4
            GRN  PC5 15 |         | 43 PF5        D
            BLU  PC6 16 |         | 44 PF6        E
        5        PC7 17 |_________| 45 PF7        F

                      36=INT4, 37=INT5

This is a grand total of 46 usable I/O pins.

- We use SCL (0) and SDA (1) for I2C communication, leaving 44 usable pins.
- We use RXD (2) and TXD (3) and RTS (4) for Bluetooth UART communication, leaving 41 usable pins.
- We use INT7 (19) for CTS interrupts (WT12 RTS), leaving 40 usable pins
- We use INT6 (18) for accel/gyro interrupt, leaving 39 usable pins
- We use LED (6) for BLINK feedback, leaving 38 usable pins.
- We use SPK (24) for piezo buzzer feedback, leaving 37 usable pins.
- We use VIB (23) for vibration motor feedback, leaving 36 usable pins.
- We use REG/GRN/BLU (14/15/16) for RGB feedback, leaving 33 usable pins.
- We use CS1/CS2/CS3 (22/21/20) for charge state indicators, leaving 30 usable pins.
- We use BAT (38) for battery level measurement (ADC), leaving 29 usable pins.
- ...and we have a total of 19 sensors. There's some wiggle room here (10 extra pins).

Pin Change interrupts (not currently used, could help with sleep modes):

- 1,   27, PB7
- Y,   26, PB6
- 8,   25, PB5
- SPK, 24, PB4
- VIB, 23, PB3
- CS1, 22, PB2
- CS2, 21, PB1
- CS3, 20, PB0
- 7,   5,  PD5
- RTS, 4,  PD4

For the sake of the Keyglove Kit board, the sensor pin connections should be arranged
in a clockwise manner as Thumb, Index, Middle, Ring, Little, with each individual
finger's sensors in increasing alphabetical order: (Y,Z,a*,8,9,0), (A,B,C,M,N,O,4),
and so on. Start with pin 26 (B6)

 */

#ifndef _SUPPORT_BOARD_TEENSYPP2_T19_H_
#define _SUPPORT_BOARD_TEENSYPP2_T19_H_

// ======================== BEGIN PIN DEFINITIONS ========================

#define USBSerial Serial                    ///< USB serial interface name
#define BT2Serial Serial1                   ///< Bluetooth module UART interface name

#define KG_HOSTIF_USB_SERIAL_BAUD 115200    ///< USB serial interface baud rate
#define KG_HOSTIF_BT2_SERIAL_BAUD 125000    ///< Bluetooth module UART interface baud rate

#define KG_INTERRUPT_PIN_MPU6050_HAND  18   ///< PE6
#define KG_INTERRUPT_NUM_MPU6050_HAND  6    ///< Teensy++ interrupt #6

#define KG_PIN_BT2_CTS              19      ///< PE7
#define KG_INTERRUPT_NUM_BT2_CTS    7       ///< Teensy++ interrupt #7
#define KG_PIN_BT2_RTS              4       ///< PD4

#define KG_PIN_BLINK                6       ///< PD6
#define KG_PIN_PIEZO                24      ///< PB4
#define KG_PIN_VIBRATE              23      ///< PB3
#define KG_PIN_RGB_RED              14      ///< PC4
#define KG_PIN_RGB_GREEN            15      ///< PC5
#define KG_PIN_RGB_BLUE             16      ///< PC6

// ======================== END PIN DEFINITIONS ========================

// sensor count and base combination count
#define KG_TOTAL_SENSORS 19                 ///< Total sensor count with most efficient pin configuration
#define KG_BASE_COMBINATIONS 25             ///< Number of physically reasonable 1-to-1 touch combinations
#define KG_BASE_COMBINATION_BYTES 4         ///< Number of bytes required to store all touch status bits

// NOTE: KG_BASE_COMBINATIONS seems like it would be very high, but there are
// physical and practical limitations that make this number much smaller

#define KGT_AY(test) (test[0] & 0x01)
#define KGT_BY(test) (test[0] & 0x02)
#define KGT_CY(test) (test[0] & 0x04)
#define KGT_DY(test) (test[0] & 0x08)
#define KGT_EY(test) (test[0] & 0x10)
#define KGT_FY(test) (test[0] & 0x20)
#define KGT_GY(test) (test[0] & 0x40)
#define KGT_HY(test) (test[0] & 0x80)
#define KGT_IY(test) (test[1] & 0x01)
#define KGT_JY(test) (test[1] & 0x02)
#define KGT_KY(test) (test[1] & 0x04)
#define KGT_LY(test) (test[1] & 0x08)
#define KGT_A1(test) (test[1] & 0x10)
#define KGT_D1(test) (test[1] & 0x20)
#define KGT_G1(test) (test[1] & 0x40)
#define KGT_J1(test) (test[1] & 0x80)
#define KGT_Y4(test) (test[2] & 0x01)
#define KGT_Y5(test) (test[2] & 0x02)
#define KGT_Y6(test) (test[2] & 0x04)
#define KGT_Y7(test) (test[2] & 0x08)
#define KGT_A8(test) (test[2] & 0x10)
#define KGT_D8(test) (test[2] & 0x20)
#define KGT_G8(test) (test[2] & 0x40)
#define KGT_J8(test) (test[2] & 0x80)
#define KGT_Y1(test) (test[3] & 0x01)

#define KGT_ADY (KGT_AY && KGT_DY)
#define KGT_AJY (KGT_AY && KGT_JY)
#define KGT_DGY (KGT_DY && KGT_GY)
#define KGT_GJY (KGT_GY && KGT_JY)

#define CLR(x, y) (x &= (~(1 << y)))    ///< Bit-clearing macro for port/pin combination
#define SET(x, y) (x |= (1 << y))       ///< Bit-setting macro for port/pin combination
#define _BV(bit) (1 << (bit))           ///< Bit-value calculation macro for lazy people

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

volatile uint8_t keygloveBatteryStatus0;    ///< Variable for comparing new vs. old battery status

/**
 * @brief Hardware timer comparator interrupt for tracking 100Hz ticks
 */
ISR(TIMER1_COMPA_vect) {
    keyglove100Hz = 1;
}

ISR(PCINT0_vect) {
    keygloveBatteryStatus0 = (~PINB) & 0x07;
    if (keygloveBatteryStatus0 != (keygloveBatteryStatus & 0x07)) {
        keygloveBatteryStatus &= (0xF8 | keygloveBatteryStatus0);
        keygloveBatteryInterrupt = 1;
    }
}

/**
 * @brief Initialize Teensy++ v2.0 board hardware/registers (19-sensor arrangement)
 *
 * This function sets the required hardware timer for a 100Hz tick interrupt,
 * initializes touch sensor pins to be pulled high, and starts the necessary
 * USB serial and/or hardware UART interfaces for host and Bluetooth control.
 *
 * @see setup()
 */
void setup_board_teensypp2_t19() {
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

    // set up pin change interrupts on relevant Port B pins
    PCICR = 0x01;   // PCICR: PCIE0=1 (enable pin change interrupts)
    PCMSK0 = 0x07;  // PCMSK0: PCINT0/1/2=1 (enable PB0/PB1/PB2 pins)

    /*
                         __|||||__
                    GND |   USB   | VCC
        1        PB7 27 |         | 26 PB6        Y
            SCL  PD0  0 |         | 25 PB5        8
            SDA  PD1  1 |         | 24 PB4    SPK
            RXD  PD2  2 |         | 23 PB3    VIB
            TXD  PD3  3 |         | 22 PB2    CS2 (STAT2)
            RTS  PD4  4 |  37.36  | 21 PB1    CS1 (STAT1)
        7        PD5  5 |         | 20 PB0    CS0 (/PG)
            LED  PD6  6 |         | 19 PE7    CTS (INT7)
        L        PD7  7 |         | 18 PE6    MPU (INT6)
        K        PE0  8 |         | GND
        J        PE1  9 |         | AREF
        6        PC0 10 |         | 38 PF0    BAT (VBDIV)
        I        PC1 11 | 32 . 28 | 39 PF1        A
        H        PC2 12 | 33 . 29 | 40 PF2        B
        G        PC3 13 | 34 . 30 | 41 PF3        C
            RED  PC4 14 | 35 . 31 | 42 PF4        4
            GRN  PC5 15 |         | 43 PF5        D
            BLU  PC6 16 |         | 44 PF6        E
        5        PC7 17 |_________| 45 PF7        F
    */

    // setup touch/battery sensors and make sure we enable internal pullup resistors EXCEPT on PF0 (BAT)
    DDRB  &=  (0x18);   // 7,6,5, , ,2,1,0
    PORTB |= ~(0x18);
    DDRC  &=  (0x70);   // 7, , , ,3,2,1,0
    PORTC |= ~(0x70);
    DDRD  &=  (0x5F);   // 7, ,5, , , , , 
    PORTD |= ~(0x5F);
    DDRE  &=  (0xFC);   //  , , , , , ,1,0
    PORTE |= ~(0xFC);
    DDRF  &=  (0x00);   // 0,1,2,3,4,5,6,7
    PORTF  = ~(0x01);

    // set UART RTS to be output/low (asserted, i.e. "ready to receive")
    DDRD |= 0x10;
    PORTD &= ~(0x10);

    /*
    uint8_t b = PINB;
    int16_t bat = analogRead(0);
    Serial.println(b & 0x07);
    Serial.println(bat);
    // 4.2v in with USB supply, ~880 reading with 47k/100k voltage divider
    // http://www.raltron.com/cust/tools/voltage_divider.asp
    */

    /*
    Relevant pin change interrupts for touch sensors:
    - 1, 27, PB7 (palm)
    - Y, 26, PB6 (thumbtip)
    - 8, 25, PB5 (thumbnail)
    */

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
    /*
                         __|||||__
                    GND |   USB   | VCC
        1        PB7 27 |         | 26 PB6        Y
            SCL  PD0  0 |         | 25 PB5        8
            SDA  PD1  1 |         | 24 PB4    SPK
            RXD  PD2  2 |         | 23 PB3    VIB
            TXD  PD3  3 |         | 22 PB2    CS2 (STAT2)
            RTS  PD4  4 |  37.36  | 21 PB1    CS1 (STAT1)
        7        PD5  5 |         | 20 PB0    CS0 (/PG)
            LED  PD6  6 |         | 19 PE7    CTS (INT7)
        L        PD7  7 |         | 18 PE6    MPU (INT6)
        K        PE0  8 |         | GND
        J        PE1  9 |         | AREF
        6        PC0 10 |         | 38 PF0    BAT (VBDIV)
        I        PC1 11 | 32 . 28 | 39 PF1        A
        H        PC2 12 | 33 . 29 | 40 PF2        B
        G        PC3 13 | 34 . 30 | 41 PF3        C
            RED  PC4 14 | 35 . 31 | 42 PF4        4
            GRN  PC5 15 |         | 43 PF5        D
            BLU  PC6 16 |         | 44 PF6        E
        5        PC7 17 |_________| 45 PF7        F
    */

    // check on Y combinations (PB6)
    SET(DDRB, 6);       // set to OUTPUT
    CLR(PORTB, 6);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinb = PINB; _pinc = PINC; _pind = PIND; _pine = PINE; _pinf = PINF;
    CLR(DDRB, 6);       // set to INPUT
    SET(PORTB, 6);      // pull HIGH
    if (!(_pinf & (1 << 1))) touches[0] |= 0x01;    // A (PF1)
    if (!(_pinf & (1 << 2))) touches[0] |= 0x02;    // B (PF2)
    if (!(_pinf & (1 << 3))) touches[0] |= 0x04;    // C (PF3)
    if (!(_pinf & (1 << 5))) touches[0] |= 0x08;    // D (PF5)
    if (!(_pinf & (1 << 6))) touches[0] |= 0x10;    // E (PF6)
    if (!(_pinf & (1 << 7))) touches[0] |= 0x20;    // F (PF7)
    if (!(_pinc & (1 << 3))) touches[0] |= 0x40;    // G (PC3)
    if (!(_pinc & (1 << 2))) touches[0] |= 0x80;    // H (PC2)
    if (!(_pinc & (1 << 1))) touches[1] |= 0x01;    // I (PC1)
    if (!(_pine & (1 << 1))) touches[1] |= 0x02;    // J (PE1)
    if (!(_pine & (1 << 0))) touches[1] |= 0x04;    // K (PE0)
    if (!(_pind & (1 << 7))) touches[1] |= 0x08;    // L (PD7)
    if (!(_pinf & (1 << 4))) touches[1] |= 0x10;    // 4 (PF4)
    if (!(_pinc & (1 << 7))) touches[1] |= 0x20;    // 5 (PC7)
    if (!(_pinc & (1 << 0))) touches[1] |= 0x40;    // 6 (PC0)
    if (!(_pind & (1 << 5))) touches[1] |= 0x80;    // 7 (PD5)
    if (!(_pinb & (1 << 7))) touches[2] |= 0x01;    // 1 (PB7)

    // check on 1 combinations (PB7)
    SET(DDRB, 7);       // set to OUTPUT
    CLR(PORTB, 7);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinc = PINC; _pine = PINE; _pinf = PINF;
    CLR(DDRB, 7);       // set to INPUT
    SET(PORTB, 7);      // pull HIGH
    if (!(_pinf & (1 << 1))) touches[2] |= 0x02;    // A (PF1)
    if (!(_pinf & (1 << 5))) touches[2] |= 0x04;    // D (PF5)
    if (!(_pinc & (1 << 3))) touches[2] |= 0x08;    // G (PC3)
    if (!(_pine & (1 << 1))) touches[2] |= 0x10;    // J (PE1)

    // check on 8 combinations (PB5)
    SET(DDRB, 5);       // set to OUTPUT
    CLR(PORTB, 5);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinc = PINC; _pine = PINE; _pinf = PINF;
    CLR(DDRB, 5);       // set to INPUT
    SET(PORTB, 5);      // pull HIGH
    if (!(_pinf & (1 << 1))) touches[2] |= 0x20;    // A (PF1)
    if (!(_pinf & (1 << 5))) touches[2] |= 0x40;    // D (PF5)
    if (!(_pinc & (1 << 3))) touches[2] |= 0x80;    // G (PC3)
    if (!(_pine & (1 << 1))) touches[3] |= 0x01;    // J (PE1)
}

#endif // _SUPPORT_BOARD_TEENSYPP2_T19_H_