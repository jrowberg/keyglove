// Keyglove controller source code - pin definition file
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

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

/* ===============================================
 * ARCHITECTURE PIN ALIASES
=============================================== */

#if (defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__))
     // 32-pin ATMegaxxx MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_AT90USB162__))
     // 32-pin AT90USB MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__))
     // 44-pin ATMEGAxxUx MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
     // 100-pin ATMegaxxx0 MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) || \
     defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || \
     defined(__AVR_ATmega32U6__))
     // 64-pin AT90USB MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif



#if KG_BOARD == KG_BOARD_ARDUINO_MEGA2560
    #define KSP_A 16
    #define KSP_B 17
    #define KSP_C 22
    #define KSP_D 23
    #define KSP_E 24
    #define KSP_F 25
    #define KSP_G 26
    #define KSP_H 27
    #define KSP_I 28
    #define KSP_J 29
    #define KSP_K 30
    #define KSP_L 31
    #define KSP_M 32
    #define KSP_N 33
    #define KSP_O 34
    #define KSP_P 35
    #define KSP_Q 36
    #define KSP_R 37
    #define KSP_S 38
    #define KSP_T 39
    #define KSP_U 40
    #define KSP_V 41
    #define KSP_W 42
    #define KSP_X 43
    #define KSP_Y 44
    #define KSP_Z 45
    #define KSP_1 46
    #define KSP_2 47
    #define KSP_3 48
    #define KSP_4 49
    #define KSP_5 50
    #define KSP_6 51
    #define KSP_7 52
    #define KSP_8 53

    #ifdef ENABLE_RX400    
        #define KSP_5 2
        #define KSP_6 3
        #define KSP_7 4
        #define KSP_8 5
    #endif /* ENABLE_RX400 */


    #define KG_INTERRUPT_PIN_ACCEL 1    // E4 (internal/tiny, right)
    #define KG_INTERRUPT_NUM_ACCEL 4    // Teensy++ interrupt #4

    #define KG_INTERRUPT_PIN_GYRO 1     // E5 (internal/tiny, left)
    #define KG_INTERRUPT_NUM_GYRO 5     // Teensy++ interrupt #5

    #define KG_INTERRUPT_PIN_FUSION 1   // E4 (internal/tiny, right)
    #define KG_INTERRUPT_NUM_FUSION 4   // Teensy++ interrupt #4

    #define KG_PIN_BLINK 13 // Arduino Mega LED is pin 6

    #define KG_PIN_PIEZO 12
    #define KG_PIN_VIBRATE 11

    #define KG_PIN_RGB_RED 10
    #define KG_PIN_RGB_GREEN 9
    #define KG_PIN_RGB_BLUE 8



    #define BLINK_LED_PIN 13 // Arduino LED pin is 13
    #define VIBRATE_PIN 10
    #define SOUND_PIN 9

    #define RGB_RED_PIN 11
    #define RGB_GREEN_PIN 12
    #define RGB_BLUE_PIN 13

    #define KB_PIN_CLOCK    19
    #define KB_PIN_DATA     18
    #define KB_INT1         2
    #define KB_INT2         3

    #define MOUSE_PIN_CLOCK 3
    #define MOUSE_PIN_DATA  2
    #define MOUSE_INT1      0
    #define MOUSE_INT2      1
#endif /* USE_ARDUINO */

#if KG_BOARD == KG_BOARD_TEENSYPP2
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
    
    For the sake of the Keyglove Kit board, the sensor pin connections should be arranged
    in a clockwise manner as Thumb, Index, Middle, Ring, Little, with each individual
    finger's sensors in increasing alphabetical order: (Y,Z,a*,8,9,0), (A,B,C,M,N,O,4),
    and so on. Start with pin 26 (B6)
    */

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
    #define KSP_F   31  // PA3
    #define KSP_P   30  // PA2
    #define KSP_Q   29  // PA1
    #define KSP_R   28  // PA0
    #define KSP_5   32  // PA4

    #define KSP_G   33  // PA5
    #define KSP_H   34  // PA6
    #define KSP_I   35  // PA7
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

    #define KG_INTERRUPT_PIN_ACCEL  36  // PE4 (internal/tiny, right)
    #define KG_INTERRUPT_NUM_ACCEL  4   // Teensy++ interrupt #4
    // KKYX-9CL3X2-ZXZ2

    #define KG_INTERRUPT_PIN_GYRO   37  // PE5 (internal/tiny, left)
    #define KG_INTERRUPT_NUM_GYRO   5   // Teensy++ interrupt #5

    // FUSION and ACCEL are never both used at the same time
    #define KG_INTERRUPT_PIN_FUSION 36  // PE4 (internal/tiny, right)
    #define KG_INTERRUPT_NUM_FUSION 4   // Teensy++ interrupt #4

    #define KG_PIN_BLINK            6   // PD6

    #define KG_PIN_BT_LINK              19  // PE7
    #define KG_INTERRUPT_NUM_BT_LINK    7   // Teensy++ interrupt #7
    #define KG_PIN_BT_DTR               18  // PE6

    // ITEMS BELOW THIS CANNOT BE USED SIMULTANEOUSLY WITH ALL ABOVE FEATURES

    // direct I/O feedback pins (kills 3 thumb sensors and Bluetooth mode control)
    #define KG_PIN_PIEZO        19  // PE7
    #define KG_PIN_VIBRATE      18  // PE6
    #define KG_PIN_RGB_RED      21  // PB1
    #define KG_PIN_RGB_GREEN    22  // PB2
    #define KG_PIN_RGB_BLUE     24  // PB4

    // PS/2 clock/data pins for keyboard (kills Bluetooth mode control, direct I/O piezo/vibe)
    #define KG_PIN_KB_CLOCK     19  // PE7
    #define KG_PIN_KB_DATA      18  // PE6

    // PS/2 clock/data pins for mouse (kills Bluetooth UART connection)
    #define KG_PIN_MOUSE_CLOCK  3   // PD3
    #define KG_PIN_MOUSE_DATA   2   // PD2
#endif /* KG_BOARD == KG_BOARD_TEENSYPP2 */

#define KG_TOTAL_SENSORS 37
#define KG_BASE_COMBINATIONS 60

uint8_t pins[KG_TOTAL_SENSORS] = {
    KSP_A, KSP_B, KSP_C, KSP_D, KSP_E,
    KSP_F, KSP_G, KSP_H, KSP_I, KSP_J,
    KSP_K, KSP_L, KSP_M, KSP_N, KSP_O,
    KSP_P, KSP_Q, KSP_R, KSP_S, KSP_T,
    KSP_U, KSP_V, KSP_W, KSP_X, KSP_Y,
    KSP_Z, KSP_1, KSP_2, KSP_3, KSP_4,
    KSP_5, KSP_6, KSP_7, KSP_8, KSP_9,
    KSP_0, KSP_AA };
    
uint8_t combinations[KG_BASE_COMBINATIONS][2] = {
    { KSP_Z, KSP_7 /* Z7 */ },
    { KSP_Y, KSP_7 /* Y7 */ },
    { KSP_J, KSP_8 /* J8 */ },
    { KSP_J, KSP_Y /* JY */ },
    { KSP_K, KSP_Y /* KY */ },
    { KSP_L, KSP_Y /* LY */ },
    { KSP_H, KSP_Y /* HY */ },
    { KSP_V, KSP_Y /* VY */ },
    { KSP_W, KSP_Y /* WY */ },
    { KSP_X, KSP_Y /* XY */ },
    { KSP_Z, KSP_6 /* Z6 */ },
    { KSP_Y, KSP_6 /* Y6 */ },
    { KSP_G, KSP_8 /* G8 */ },
    { KSP_G, KSP_Y /* GY */ },
    { KSP_I, KSP_Y /* IY */ },
    { KSP_S, KSP_Y /* SY */ },
    { KSP_T, KSP_Y /* TY */ },
    { KSP_U, KSP_Y /* UY */ },
    { KSP_G, KSP_7 /* G7 */ },
    { KSP_Z, KSP_5 /* Z5 */ },
    { KSP_Y, KSP_5 /* Y5 */ },
    { KSP_D, KSP_8 /* D8 */ },
    { KSP_D, KSP_Y /* DY */ },
    { KSP_E, KSP_Y /* EY */ },
    { KSP_F, KSP_Y /* FY */ },
    { KSP_P, KSP_Z /* PZ */ },
    { KSP_P, KSP_Y /* PY */ },
    { KSP_Q, KSP_Z /* QZ */ },
    { KSP_Q, KSP_Y /* QY */ },
    { KSP_R, KSP_Z /* RZ */ },
    { KSP_R, KSP_Y /* RY */ },
    { KSP_D, KSP_4 /* D4 */ },
    { KSP_D, KSP_6 /* D6 */ },
    { KSP_D, KSP_7 /* D7 */ },
    { KSP_D, KSP_M /* DM */ },
    { KSP_Z, KSP_4 /* Z4 */ },
    { KSP_Y, KSP_4 /* Y4 */ },
    { KSP_A, KSP_8 /* A8 */ },
    { KSP_M, KSP_Y /* MY */ },
    { KSP_A, KSP_Y /* AY */ },
    { KSP_B, KSP_Y /* BY */ },
    { KSP_C, KSP_Y /* CY */ },
    { KSP_M, KSP_Z /* MZ */ },
    { KSP_N, KSP_Z /* NZ */ },
    { KSP_N, KSP_Y /* NY */ },
    { KSP_O, KSP_Z /* OZ */ },
    { KSP_O, KSP_Y /* OY */ },
    { KSP_A, KSP_3 /* A3 */ },
    { KSP_A, KSP_2 /* A2 */ },
    { KSP_A, KSP_1 /* A1 */ },
    { KSP_D, KSP_3 /* D3 */ },
    { KSP_D, KSP_2 /* D2 */ },
    { KSP_D, KSP_1 /* D1 */ },
    { KSP_G, KSP_3 /* G3 */ },
    { KSP_G, KSP_2 /* G2 */ },
    { KSP_G, KSP_1 /* G1 */ },
    { KSP_J, KSP_3 /* J3 */ },
    { KSP_J, KSP_2 /* J2 */ },
    { KSP_J, KSP_1 /* J1 */ },
    { KSP_Y, KSP_1 /* Y1 */ }
};