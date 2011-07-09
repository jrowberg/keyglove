// Keyglove controller source code - PS/2 keyboard class library
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

#ifndef ps2keyboard_h
#define ps2keyboard_h

#ifdef USE_LUFA
    #include "ArduinoWrapper.h"
#endif

#ifndef USE_LUFA
    #include "WProgram.h"
#endif

#include "ps2dev.h"

#ifdef round
    #undef round
#endif

// NUMERIC SCANCODE ARRAY INDEXES
#define PKEY_A 0
#define PKEY_B 1
#define PKEY_C 2
#define PKEY_D 3
#define PKEY_E 4
#define PKEY_F 5
#define PKEY_G 6
#define PKEY_H 7
#define PKEY_I 8
#define PKEY_J 9
#define PKEY_K 10
#define PKEY_L 11
#define PKEY_M 12
#define PKEY_N 13
#define PKEY_O 14
#define PKEY_P 15
#define PKEY_Q 16
#define PKEY_R 17
#define PKEY_S 18
#define PKEY_T 19
#define PKEY_U 20
#define PKEY_V 21
#define PKEY_W 22
#define PKEY_X 23
#define PKEY_Y 24
#define PKEY_Z 25
#define PKEY_0 26
#define PKEY_1 27
#define PKEY_2 28
#define PKEY_3 29
#define PKEY_4 30
#define PKEY_5 31
#define PKEY_6 32
#define PKEY_7 33
#define PKEY_8 34
#define PKEY_9 35
#define PKEY_BACKTICK 36
#define PKEY_MINUS 37
#define PKEY_EQUALS 38
#define PKEY_BACKSLASH 39
#define PKEY_BKSP 40
#define PKEY_SPACE 41
#define PKEY_TAB 42
#define PKEY_CAPS 43
#define PKEY_LSHIFT 44
#define PKEY_LCTRL 45
#define PKEY_LGUI 46
#define PKEY_LALT 47
#define PKEY_RSHIFT 48
#define PKEY_RCTRL 49
#define PKEY_RGUI 50
#define PKEY_RALT 51
#define PKEY_APPS 52
#define PKEY_ENTER 53
#define PKEY_ESC 54
#define PKEY_F1 55
#define PKEY_F2 56
#define PKEY_F3 57
#define PKEY_F4 58
#define PKEY_F5 59
#define PKEY_F6 60
#define PKEY_F7 61
#define PKEY_F8 62
#define PKEY_F9 63
#define PKEY_F10 64
#define PKEY_F11 65
#define PKEY_F12 66
#define PKEY_PRTSC 67
#define PKEY_SCRLOCK 68
#define PKEY_PAUSE 69
#define PKEY_LBRACKET 70
#define PKEY_INS 71
#define PKEY_HOME 72
#define PKEY_PGUP 73
#define PKEY_DEL 74
#define PKEY_END 75
#define PKEY_PGDN 76
#define PKEY_UP 77
#define PKEY_DOWN 78
#define PKEY_LEFT 79
#define PKEY_RIGHT 80
#define PKEY_NUMLOCK 81
#define PKEY_PAD_SLASH 82
#define PKEY_PAD_ASTR 83
#define PKEY_PAD_MINUS 84
#define PKEY_PAD_PLUS 85
#define PKEY_PAD_ENTER 86
#define PKEY_PAD_PERIOD 87
#define PKEY_PAD_0 88
#define PKEY_PAD_1 89
#define PKEY_PAD_2 90
#define PKEY_PAD_3 91
#define PKEY_PAD_4 92
#define PKEY_PAD_5 93
#define PKEY_PAD_6 94
#define PKEY_PAD_7 95
#define PKEY_PAD_8 96
#define PKEY_PAD_9 97
#define PKEY_RBRACKET 98
#define PKEY_SEMICOLON 99
#define PKEY_APOS 100
#define PKEY_COMMA 101
#define PKEY_PERIOD 102
#define PKEY_SLASH 103

#define PKEY_ACPI_POWER 104
#define PKEY_ACPI_SLEEP 105
#define PKEY_ACPI_WAKE 106

#define PKEY_WIN_NEXTTRK 107
#define PKEY_WIN_PREVTRK 108
#define PKEY_WIN_STOP 109
#define PKEY_WIN_PLAY 110
#define PKEY_WIN_MUTE 111
#define PKEY_WIN_VOLUP 112
#define PKEY_WIN_VOLDN 113
#define PKEY_WIN_MEDIA 114
#define PKEY_WIN_EMAIL 115
#define PKEY_WIN_CALC 116
#define PKEY_WIN_COMPUTER 117

#define PKEY_WWW_SEARCH 118
#define PKEY_WWW_HOME 119
#define PKEY_WWW_BACK 120
#define PKEY_WWW_FORWARD 121
#define PKEY_WWW_STOP 122
#define PKEY_WWW_REFRESH 123
#define PKEY_WWW_FAVORITES 124


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

class PS2keyboard {
    public:
        PS2keyboard(PS2dev *k);
        void keydown(int keycode);
        void keyup(int keycode);
        void keypress(int keycode);
        void initialize();
        void process_command();
    
    private:
        PS2dev *keyboard;
        boolean enabled;
        int typematic_delay;
        float typematic_rate;
        int scancode_set;
        int keys_mode;
        boolean led_numlock;
        boolean led_capslock;
        boolean led_scrolllock;
        unsigned char buffer[16];
        unsigned char last_sent_byte;
        int buf_length;
        
        // get/set keyboard typematic delay in ms (250, 500, 750, 1000)
        int get_typematic_delay();
        void set_typematic_delay(int d);
        
        // get/set keyboard typematic rate
        int get_typematic_rate();
        void set_typematic_rate(float r);
        
        // get/set keyboard scan code set
        int get_scancode_set();
        void set_scancode_set(int s);
        
        // get/set all keys to all states
        int get_allkeys_mode();
        void set_allkeys_mode(int mode);
        
        boolean get_led_numlock();
        void set_led_numlock(boolean state);

        boolean get_led_capslock();
        void set_led_capslock(boolean state);

        boolean get_led_scrolllock();
        void set_led_scrolllock(boolean state);
        
        void run_bat();
        void set_defaults();
        
        // send PS/2 "acknowledge" code
        void cmd_ack();
        
        // send PS/2 "resend" code
        void cmd_resend();
        
        // send PS/2 "success" code
        void cmd_success();
        
        // send PS/2 "error" code
        void cmd_error();
        
        // send PS/2 "echo" code
        void cmd_echo();
        
        // send key scancodes (make or break)
        void cmd_keypress(int keycode, boolean make);
};

#endif /* ps2keyboard_h */
