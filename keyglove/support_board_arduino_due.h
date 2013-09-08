// Keyglove controller source code - Special hardware setup file
// 8/27/2013 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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



#ifndef _SUPPORT_BOARD_ARDUINO_DUE_H_
#define _SUPPORT_BOARD_ARDUINO_DUE_H_

#define BTSerial Serial2 // UART1: TXD1/RXD1/CTS1/RTS1
                             
#define US1_CR * (volatile uint32_t *) (0x4009C000) /* USART Control Register address for USART1 */
#define US1_MR * (volatile uint32_t *) (0x4009C004) /* USART Mode Register address for USART1 */

#define KG_HOSTIF_SERIAL_BAUD 115200

/* ===============================================
 * 100 HZ INTERRUPT VECTOR
=============================================== */

volatile uint8_t timer1Overflow = 0;
ISR(TIMER1_OVF_vect) {
    timer1Overflow++;
}

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

void setup_board() {
    // enable RTS/CTS handshaking (flow control) on UART1 (Serial2 in Arduino)
    // uses digital pins 23 and 24
    //US1_MR |= 0x02;
}

void update_board() {
}

void setup_board_touch() {
    // make sure we enable internal pullup resistors
    /*DDRA  &= 0x00;
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
    PORTF |= 0xFF; // 0,1,2,3,4,5,6,7*/
}

void update_board_touch(uint8_t *touches) {
/*    // check on M combinations (PF2)
    SET(DDRF, 2);       // set to OUTPUT
    CLR(PORTF, 2);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinf = PINF;
    CLR(DDRF, 2);       // set to INPUT
    SET(PORTF, 2);      // pull HIGH
    if (!(_pinf & (1 << 6))) touches[0] |= 0x01;    // D (PF6)

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
    if (!(_pina & (1 << 3))) touches[0] |= 0x40;    // F (PA3)
    if (!(_pina & (1 << 5))) touches[0] |= 0x80;    // G (PA5)
    if (!(_pina & (1 << 6))) touches[1] |= 0x01;    // H (PA6)
    if (!(_pina & (1 << 7))) touches[1] |= 0x02;    // I (PA7)
    if (!(_pinc & (1 << 3))) touches[1] |= 0x04;    // J (PC3)
    if (!(_pinc & (1 << 2))) touches[1] |= 0x08;    // K (PC2)
    if (!(_pinc & (1 << 1))) touches[1] |= 0x10;    // L (PC1)
    if (!(_pinf & (1 << 2))) touches[1] |= 0x20;    // M (PF2)
    if (!(_pinf & (1 << 3))) touches[1] |= 0x40;    // N (PF3)
    if (!(_pinf & (1 << 4))) touches[1] |= 0x80;    // O (PF4)
    if (!(_pina & (1 << 2))) touches[2] |= 0x01;    // P (PA2)
    if (!(_pina & (1 << 1))) touches[2] |= 0x02;    // Q (PA1)
    if (!(_pina & (1 << 0))) touches[2] |= 0x04;    // R (PA0)
    if (!(_pinc & (1 << 7))) touches[2] |= 0x08;    // S (PC7)
    if (!(_pinc & (1 << 6))) touches[2] |= 0x10;    // T (PC6)
    if (!(_pinc & (1 << 5))) touches[2] |= 0x20;    // U (PC5)
    if (!(_pinc & (1 << 0))) touches[2] |= 0x40;    // V (PC0)
    if (!(_pine & (1 << 1))) touches[2] |= 0x80;    // W (PE1)
    if (!(_pine & (1 << 0))) touches[3] |= 0x01;    // X (PE0)

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
    if (!(_pina & (1 << 2))) touches[3] |= 0x10;    // P (PA2)
    if (!(_pina & (1 << 1))) touches[3] |= 0x20;    // Q (PA1)
    if (!(_pina & (1 << 0))) touches[3] |= 0x40;    // R (PA0)

    // check on 1 combinations (PD5)
    SET(DDRD, 5);       // set to OUTPUT
    CLR(PORTD, 5);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRD, 5);       // set to INPUT
    SET(PORTD, 5);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[3] |= 0x80;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[4] |= 0x01;    // D (PF6)
    if (!(_pina & (1 << 5))) touches[4] |= 0x02;    // G (PA5)
    if (!(_pinc & (1 << 3))) touches[4] |= 0x04;    // J (PC3)
    if (!(_pinb & (1 << 6))) touches[4] |= 0x08;    // Y (PB6)

    // check on 2 combinations (PD4)
    SET(DDRD, 4);       // set to OUTPUT
    CLR(PORTD, 4);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRD, 4);       // set to INPUT
    SET(PORTD, 4);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[4] |= 0x10;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[4] |= 0x20;    // D (PF6)
    if (!(_pina & (1 << 5))) touches[4] |= 0x40;    // G (PA5)
    if (!(_pinc & (1 << 3))) touches[4] |= 0x80;    // J (PC3)

    // check on 3 combinations (PB7)
    SET(DDRB, 7);       // set to OUTPUT
    CLR(PORTB, 7);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRB, 7);       // set to INPUT
    SET(PORTB, 7);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[5] |= 0x01;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[5] |= 0x02;    // D (PF6)
    if (!(_pina & (1 << 5))) touches[5] |= 0x04;    // G (PA5)
    if (!(_pinc & (1 << 3))) touches[5] |= 0x08;    // J (PC3)

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

    // check on 5 combinations (PA4)
    SET(DDRA, 4);       // set to OUTPUT
    CLR(PORTA, 4);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pinb = PINB;
    CLR(DDRA, 4);       // set to INPUT
    SET(PORTA, 4);      // pull HIGH
    if (!(_pinb & (1 << 6))) touches[5] |= 0x80;    // Y (PB6)
    if (!(_pinb & (1 << 5))) touches[6] |= 0x01;    // Z (PB5)

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

    // check on 7 combinations (PD7)
    SET(DDRD, 7);       // set to OUTPUT
    CLR(PORTD, 7);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinf = PINF;
    CLR(DDRD, 7);       // set to INPUT
    SET(PORTD, 7);      // pull HIGH
    if (!(_pinf & (1 << 6))) touches[6] |= 0x10;    // D (PF6)
    if (!(_pina & (1 << 5))) touches[6] |= 0x20;    // G (PA5)
    if (!(_pinb & (1 << 6))) touches[6] |= 0x40;    // Y (PB6)
    if (!(_pinb & (1 << 5))) touches[6] |= 0x80;    // Z (PB5)

    // check on 8 combinations (PB3)
    SET(DDRB, 3);       // set to OUTPUT
    CLR(PORTB, 3);      // set to LOW
    delayMicroseconds(3); // give the poor receiving pins a chance to change state
    _pina = PINA; _pinb = PINB; _pinc = PINC; _pinf = PINF;
    CLR(DDRB, 3);       // set to INPUT
    SET(PORTB, 3);      // pull HIGH
    if (!(_pinb & (1 << 0))) touches[7] |= 0x01;    // A (PB0)
    if (!(_pinf & (1 << 6))) touches[7] |= 0x02;    // D (PF6)
    if (!(_pina & (1 << 5))) touches[7] |= 0x04;    // G (PA5)
    if (!(_pinc & (1 << 3))) touches[7] |= 0x08;    // J (PC3)
    */
}

#endif // _SUPPORT_BOARD_ARDUINO_DUE_H_