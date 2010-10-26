// Keyglove controller source code - PS/2 keyboard class library
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2010 Jeff Rowberg

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

#include "WProgram.h"
#include "ps2dev.h"
#include "ps2keyboard.h"
#ifdef round
    #undef round
#endif

// info gleaned from http://www.computer-engineering.org/ps2keyboard/scancodes2.html
unsigned char scancodes[125][3] = {
    { 0x1C, 0 },        // KEY_A 0
    { 0x32, 0 },        // KEY_B 1
    { 0x21, 0 },        // KEY_C 2
    { 0x23, 0 },        // KEY_D 3
    { 0x24, 0 },        // KEY_E 4
    { 0x2B, 0 },        // KEY_F 5
    { 0x34, 0 },        // KEY_G 6
    { 0x33, 0 },        // KEY_H 7
    { 0x43, 0 },        // KEY_I 8
    { 0x3B, 0 },        // KEY_J 9
    { 0x42, 0 },        // KEY_K 10
    { 0x4B, 0 },        // KEY_L 11
    { 0x3A, 0 },        // KEY_M 12
    { 0x31, 0 },        // KEY_N 13
    { 0x44, 0 },        // KEY_O 14
    { 0x4D, 0 },        // KEY_P 15
    { 0x15, 0 },        // KEY_Q 16
    { 0x2D, 0 },        // KEY_R 17
    { 0x1B, 0 },        // KEY_S 18
    { 0x2C, 0 },        // KEY_T 19
    { 0x3C, 0 },        // KEY_U 20
    { 0x2A, 0 },        // KEY_V 21
    { 0x1D, 0 },        // KEY_W 22
    { 0x22, 0 },        // KEY_X 23
    { 0x35, 0 },        // KEY_Y 24
    { 0x1A, 0 },        // KEY_Z 25
    { 0x45, 0 },        // KEY_0 26
    { 0x16, 0 },        // KEY_1 27
    { 0x1E, 0 },        // KEY_2 28
    { 0x26, 0 },        // KEY_3 29
    { 0x25, 0 },        // KEY_4 30
    { 0x2E, 0 },        // KEY_5 31
    { 0x36, 0 },        // KEY_6 32
    { 0x3D, 0 },        // KEY_7 33
    { 0x3E, 0 },        // KEY_8 34
    { 0x46, 0 },        // KEY_9 35
    { 0x0E, 0 },        // KEY_BACKTICK 36
    { 0x4E, 0 },        // KEY_MINUS 37
    { 0x55, 0 },        // KEY_EQUALS 38
    { 0x5D, 0 },        // KEY_BACKSLASH 39
    { 0x66, 0 },        // KEY_BKSP 40
    { 0x29, 0 },        // KEY_SPACE 41
    { 0x0D, 0 },        // KEY_TAB 42
    { 0x58, 0 },        // KEY_CAPS 43
    { 0x12, 0 },        // KEY_LSHIFT 44
    { 0x14, 0 },        // KEY_LCTRL 45
    { 0xE0, 0x1F, 0 },  // KEY_LGUI 46
    { 0x11, 0 },        // KEY_LALT 47
    { 0x59, 0 },        // KEY_RSHIFT 48
    { 0xE0, 0x14, 0 },  // KEY_RCTRL 49
    { 0xE0, 0x27, 0 },  // KEY_RGUI 50
    { 0xE0, 0x11, 0 },  // KEY_RALT 51
    { 0xE0, 0x2F, 0 },  // KEY_APPS 52
    { 0x5A, 0 },        // KEY_ENTER 53
    { 0x76, 0 },        // KEY_ESC 54
    { 0x05, 0 },        // KEY_F1 55
    { 0x06, 0 },        // KEY_F2 56
    { 0x04, 0 },        // KEY_F3 57
    { 0x0C, 0 },        // KEY_F4 58
    { 0x03, 0 },        // KEY_F5 59
    { 0x0B, 0 },        // KEY_F6 60
    { 0x83, 0 },        // KEY_F7 61
    { 0x0A, 0 },        // KEY_F8 62
    { 0x01, 0 },        // KEY_F9 63
    { 0x09, 0 },        // KEY_F10 64
    { 0x78, 0 },        // KEY_F11 65
    { 0x07, 0 },        // KEY_F12 66
    { 0 },              // KEY_PRTSC 67 (SPECIAL)
    { 0x7E, 0 },        // KEY_SCRLOCK 68
    { 0 },              // KEY_PAUSE 69 (SPECIAL)
    { 0x54, 0 },        // KEY_LBRACKET 70
    { 0xE0, 0x70, 0 },  // KEY_INS 71
    { 0xE0, 0x6C, 0 },  // KEY_HOME 72
    { 0xE0, 0x7D, 0 },  // KEY_PGUP 73
    { 0xE0, 0x71, 0 },  // KEY_DEL 74
    { 0xE0, 0x69, 0 },  // KEY_END 75
    { 0xE0, 0x7A, 0 },  // KEY_PGDN 76
    { 0xE0, 0x75, 0 },  // KEY_UP 77
    { 0xE0, 0x6B, 0 },  // KEY_DOWN 78
    { 0xE0, 0x72, 0 },  // KEY_LEFT 79
    { 0xE0, 0x74, 0 },  // KEY_RIGHT 80
    { 0x77, 0 },        // KEY_NUMLOCK 81
    { 0xE0, 0x4A, 0 },  // KEY_PAD_SLASH 82
    { 0x7C, 0 },        // KEY_PAD_ASTR 83
    { 0x7B, 0 },        // KEY_PAD_MINUS 84
    { 0x79, 0 },        // KEY_PAD_PLUS 85
    { 0xE0, 0x5A, 0 },  // KEY_PAD_ENTER 86
    { 0x71, 0 },        // KEY_PAD_PERIOD 87
    { 0x70, 0 },        // KEY_PAD_0 88
    { 0x69, 0 },        // KEY_PAD_1 89
    { 0x72, 0 },        // KEY_PAD_2 90
    { 0x7A, 0 },        // KEY_PAD_3 91
    { 0x6B, 0 },        // KEY_PAD_4 92
    { 0x73, 0 },        // KEY_PAD_5 93
    { 0x74, 0 },        // KEY_PAD_6 94
    { 0x6C, 0 },        // KEY_PAD_7 95
    { 0x75, 0 },        // KEY_PAD_8 96
    { 0x7D, 0 },        // KEY_PAD_9 97
    { 0x5B, 0 },        // KEY_RBRACKET 98
    { 0x4C, 0 },        // KEY_SEMICOLON 99
    { 0x52, 0 },        // KEY_APOS 100
    { 0x41, 0 },        // KEY_COMMA 101
    { 0x49, 0 },        // KEY_PERIOD 102
    { 0x4A, 0 },        // KEY_SLASH 103

    { 0xE0, 0x37, 0 },  // KEY_ACPI_POWER 104
    { 0xE0, 0x3F, 0 },  // KEY_ACPI_SLEEP 105
    { 0xE0, 0x5E, 0 },  // KEY_ACPI_WAKE 106

    { 0xE0, 0x4D, 0 },  // KEY_WIN_NEXTTRK 107
    { 0xE0, 0x15, 0 },  // KEY_WIN_PREVTRK 108
    { 0xE0, 0x3B, 0 },  // KEY_WIN_STOP 109
    { 0xE0, 0x34, 0 },  // KEY_WIN_PLAY 110
    { 0xE0, 0x23, 0 },  // KEY_WIN_MUTE 111
    { 0xE0, 0x32, 0 },  // KEY_WIN_VOLUP 112
    { 0xE0, 0x21, 0 },  // KEY_WIN_VOLDN 113
    { 0xE0, 0x50, 0 },  // KEY_WIN_MEDIA 114
    { 0xE0, 0x48, 0 },  // KEY_WIN_EMAIL 115
    { 0xE0, 0x2B, 0 },  // KEY_WIN_CALC 116
    { 0xE0, 0x40, 0 },  // KEY_WIN_COMPUTER 117

    { 0xE0, 0x10, 0 },  // KEY_WWW_SEARCH 118
    { 0xE0, 0x3A, 0 },  // KEY_WWW_HOME 119
    { 0xE0, 0x38, 0 },  // KEY_WWW_BACK 120
    { 0xE0, 0x30, 0 },  // KEY_WWW_FORWARD 121
    { 0xE0, 0x28, 0 },  // KEY_WWW_STOP 122
    { 0xE0, 0x20, 0 },  // KEY_WWW_REFRESH 123
    { 0xE0, 0x18, 0 }   // KEY_WWW_FAVORITES 124
};

unsigned char scancode_prtsc[5] = { 0xE0, 0x12, 0xE0, 0x7C, 0 };
unsigned char scancode_pause[9] = { 0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77, 0 };

PS2keyboard::PS2keyboard(PS2dev *k) {
    Serial.println("Creating PS/2 keyboard device wrapper");
    keyboard = k;
    Serial.print("PS2 keyboard created successfully\n");
}

void PS2keyboard::keydown(int keycode) { cmd_keypress(keycode, true); }
void PS2keyboard::keyup(int keycode) { cmd_keypress(keycode, false); }
void PS2keyboard::keypress(int keycode) {
    cmd_keypress(keycode, true);
    delay(20);
    cmd_keypress(keycode, false);
}

// get/set keyboard typematic delay in ms (250, 500, 750, 1000)
int PS2keyboard::get_typematic_delay() { return typematic_delay; }
void PS2keyboard::set_typematic_delay(int d) {
    if (d < 250 || d > 1000) d = 500;
    typematic_delay = d;
}

// get/set keyboard typematic rate
int PS2keyboard::get_typematic_rate() { return typematic_rate; }
void PS2keyboard::set_typematic_rate(float r) {
    typematic_rate = r;
}

// get/set keyboard scan code set
int PS2keyboard::get_scancode_set() { return scancode_set; }
void PS2keyboard::set_scancode_set(int s) {
    if (s < 1 || s > 3) s = 2;
    scancode_set = s;
}

// get/set all keys to all states
int PS2keyboard::get_allkeys_mode() { return keys_mode; }
void PS2keyboard::set_allkeys_mode(int mode) {
    keys_mode = mode;
}

boolean PS2keyboard::get_led_numlock() { return led_numlock; }
void PS2keyboard::set_led_numlock(boolean state) { led_numlock = state; }

boolean PS2keyboard::get_led_capslock() { return led_capslock; }
void PS2keyboard::set_led_capslock(boolean state) { led_capslock = state; }

boolean PS2keyboard::get_led_scrolllock() { return led_scrolllock; }
void PS2keyboard::set_led_scrolllock(boolean state) { led_scrolllock = state; }

void PS2keyboard::initialize() {
    Serial.print("Sending PS/2 keyboard initialization codes...\n");
    while (keyboard -> write(0xAA) != 0);
    delay(10);
    run_bat();
    Serial.print("PS/2 keyboard initialization complete\n");
}

void PS2keyboard::run_bat() {
    Serial.print("Running Basic Assurance Test (BAT)...\n");
    set_led_numlock(true);
    set_led_capslock(true);
    set_led_scrolllock(true);
    set_defaults();
    enabled = true;
    delay(520);
    Serial.print("BAT completed, sending success code...\n");
    cmd_success();
    Serial.print("Finished\n");
}

void PS2keyboard::set_defaults() {
    Serial.print("Setting PS/2 keyboard defaults...\n");
    set_typematic_delay(500);
    set_typematic_rate(10.9);
    set_scancode_set(2);
    set_allkeys_mode(0);
    buf_length = 0;
    last_sent_byte = 0;
    Serial.print("PS/2 keyboard defaults set\n");
}

// send PS2 "acknowledge" code
void PS2keyboard::cmd_ack() { while (keyboard -> write(0xFA) != 0); last_sent_byte = 0xFA; }

// send PS2 "resend" code
void PS2keyboard::cmd_resend() { while (keyboard -> write(0xFE) != 0); last_sent_byte = 0xFE; }

// send PS2 "success" code
void PS2keyboard::cmd_success() { while (keyboard -> write(0xAA) != 0); last_sent_byte = 0xAA; }

// send PS2 "error" code
void PS2keyboard::cmd_error() { while (keyboard -> write(0xFC) != 0); last_sent_byte = 0xFC; }

// send PS2 "echo" code
void PS2keyboard::cmd_echo() { while (keyboard -> write(0xEE) != 0); last_sent_byte = 0xEE; }

// send key scancodes (make or break)
int PS2keyboard::cmd_keypress(int keycode, boolean make) {
    unsigned char *bytes;
    if (keycode == KEY_PRTSC) {
        bytes = scancode_prtsc;
    } else if (keycode == KEY_PAUSE) {
        bytes = scancode_pause;
    } else {
        bytes = scancodes[keycode];
    }
    
    // determine make/break mode
    if (make) {
        Serial.print("\nDEBUG: make scancode index ");
        Serial.print(keycode);
        Serial.print("\n");
        for (int k = 0; bytes[k] > 0; k++) {
            Serial.print(bytes[k], HEX);
            while (keyboard -> write(bytes[k]) != 0);
            last_sent_byte = bytes[k];
        }
    } else {
        if (bytes[0] != 0xE1) {
            Serial.print("\nDEBUG: break scancode index ");
            Serial.print(keycode);
            Serial.print("\n");
            for (int k = 0; bytes[k] > 0; k++) {
                if (bytes[k] == 0xE0) {
                    Serial.print(0xE0, HEX);
                    while (keyboard -> write(bytes[k]) != 0);
                }
                Serial.print(0xF0, HEX);
                while (keyboard -> write(0xF0) != 0);
                Serial.print(bytes[k], HEX);
                while (keyboard -> write(bytes[k]) != 0);
                last_sent_byte = bytes[k];
            }
        }
    }
}

// process command from host
void PS2keyboard::process_command() {
    unsigned char command = 0, b = 0;
    while (keyboard -> read(&command));
    Serial.print("Received keyboard host command 0x");
    Serial.println(command, HEX);
    switch (command) {
        case 0xED: // set/reset LEDs
            cmd_ack();
            keyboard -> read(&b);
            set_led_scrolllock(bitRead(b, 0) == 1);
            set_led_numlock(bitRead(b, 1) == 1);
            set_led_capslock(bitRead(b, 2) == 1);
            break;
        case 0xEE: // echo
            while (keyboard -> write(0xEE) != 0);
            last_sent_byte = 0xEE;
            break;
        case 0xF0: // set scan code set
            cmd_ack();
            keyboard -> read(&b);
            if (b == 0) {
                while (keyboard -> write(scancode_set));
            } else {
                set_scancode_set(b);
            }
            break;
        case 0xF2: // read ID
            cmd_ack();
            break;
        case 0xF3: // set typematic rate/delay
            unsigned char td, tr_a, tr_b;
            cmd_ack();
            keyboard -> read(&b);
            td = b >> 5;
            tr_a = (b & 0x00011000) >> 3;
            tr_b = (b & 0x00000111);
            if (td == 3) set_typematic_delay(1000);
            else if (td == 2) set_typematic_delay(750);
            else if (td == 1) set_typematic_delay(500);
            else set_typematic_delay(250);
            set_typematic_delay((pow(2, tr_a) * (8 + tr_b)) / 240);
            break;
        case 0xF4: // enable
            cmd_ack();
            enabled = true;
            buf_length = 0;
            break;
        case 0xF5: // disable and set defaults (not full BAT)
            cmd_ack();
            set_defaults();
            enabled = false;
            break;
        case 0xF6: // set defaults and reset w/full test
            cmd_ack();
            run_bat();
            break;
        case 0xF7: // SC MODE 3 ONLY: set all keys type typematic
            cmd_ack();
            break;
        case 0xF8: // SC MODE 3 ONLY: set all keys type make/break
            cmd_ack();
            break;
        case 0xF9: // SC MODE 3 ONLY: set all keys type make
            cmd_ack();
            break;
        case 0xFA: // SC MODE 3 ONLY: set all keys type make/break/typematic
            cmd_ack();
            break;
        case 0xFB: // SC MODE 3 ONLY: set key type typematic
            cmd_ack();
            break;
        case 0xFC: // SC MODE 3 ONLY: set key type make/break
            cmd_ack();
            break;
        case 0xFD: // SC MODE 3 ONLY: set key type make
            cmd_ack();
            break;
        case 0xFE: // resend
            while (keyboard -> write(last_sent_byte) != 0);
            break;
        case 0xFF: // reset w/full test
            cmd_ack();
            run_bat();
            break;
        default: // unknown, hmm
            //cmd_ack();
            cmd_resend();
    }
}

