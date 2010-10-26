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

#ifndef ps2keyboard_h
#define ps2keyboard_h

#include "WProgram.h"
#include "ps2dev.h"
#undef round

// NUMERIC SCANCODE ARRAY INDEXES
#define KEY_A 0
#define KEY_B 1
#define KEY_C 2
#define KEY_D 3
#define KEY_E 4
#define KEY_F 5
#define KEY_G 6
#define KEY_H 7
#define KEY_I 8
#define KEY_J 9
#define KEY_K 10
#define KEY_L 11
#define KEY_M 12
#define KEY_N 13
#define KEY_O 14
#define KEY_P 15
#define KEY_Q 16
#define KEY_R 17
#define KEY_S 18
#define KEY_T 19
#define KEY_U 20
#define KEY_V 21
#define KEY_W 22
#define KEY_X 23
#define KEY_Y 24
#define KEY_Z 25
#define KEY_0 26
#define KEY_1 27
#define KEY_2 28
#define KEY_3 29
#define KEY_4 30
#define KEY_5 31
#define KEY_6 32
#define KEY_7 33
#define KEY_8 34
#define KEY_9 35
#define KEY_BACKTICK 36
#define KEY_MINUS 37
#define KEY_EQUALS 38
#define KEY_BACKSLASH 39
#define KEY_BKSP 40
#define KEY_SPACE 41
#define KEY_TAB 42
#define KEY_CAPS 43
#define KEY_LSHIFT 44
#define KEY_LCTRL 45
#define KEY_LGUI 46
#define KEY_LALT 47
#define KEY_RSHIFT 48
#define KEY_RCTRL 49
#define KEY_RGUI 50
#define KEY_RALT 51
#define KEY_APPS 52
#define KEY_ENTER 53
#define KEY_ESC 54
#define KEY_F1 55
#define KEY_F2 56
#define KEY_F3 57
#define KEY_F4 58
#define KEY_F5 59
#define KEY_F6 60
#define KEY_F7 61
#define KEY_F8 62
#define KEY_F9 63
#define KEY_F10 64
#define KEY_F11 65
#define KEY_F12 66
#define KEY_PRTSC 67
#define KEY_SCRLOCK 68
#define KEY_PAUSE 69
#define KEY_LBRACKET 70
#define KEY_INS 71
#define KEY_HOME 72
#define KEY_PGUP 73
#define KEY_DEL 74
#define KEY_END 75
#define KEY_PGDN 76
#define KEY_UP 77
#define KEY_DOWN 78
#define KEY_LEFT 79
#define KEY_RIGHT 80
#define KEY_NUMLOCK 81
#define KEY_PAD_SLASH 82
#define KEY_PAD_ASTR 83
#define KEY_PAD_MINUS 84
#define KEY_PAD_PLUS 85
#define KEY_PAD_ENTER 86
#define KEY_PAD_PERIOD 87
#define KEY_PAD_0 88
#define KEY_PAD_1 89
#define KEY_PAD_2 90
#define KEY_PAD_3 91
#define KEY_PAD_4 92
#define KEY_PAD_5 93
#define KEY_PAD_6 94
#define KEY_PAD_7 95
#define KEY_PAD_8 96
#define KEY_PAD_9 97
#define KEY_RBRACKET 98
#define KEY_SEMICOLON 99
#define KEY_APOS 100
#define KEY_COMMA 101
#define KEY_PERIOD 102
#define KEY_SLASH 103

#define KEY_ACPI_POWER 104
#define KEY_ACPI_SLEEP 105
#define KEY_ACPI_WAKE 106

#define KEY_WIN_NEXTTRK 107
#define KEY_WIN_PREVTRK 108
#define KEY_WIN_STOP 109
#define KEY_WIN_PLAY 110
#define KEY_WIN_MUTE 111
#define KEY_WIN_VOLUP 112
#define KEY_WIN_VOLDN 113
#define KEY_WIN_MEDIA 114
#define KEY_WIN_EMAIL 115
#define KEY_WIN_CALC 116
#define KEY_WIN_COMPUTER 117

#define KEY_WWW_SEARCH 118
#define KEY_WWW_HOME 119
#define KEY_WWW_BACK 120
#define KEY_WWW_FORWARD 121
#define KEY_WWW_STOP 122
#define KEY_WWW_REFRESH 123
#define KEY_WWW_FAVORITES 124

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
        int cmd_keypress(int keycode, boolean make);
};

#endif /* ps2keyboard_h */
