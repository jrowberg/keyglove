// Keyglove controller source code - touchset definition file
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

#define KGI_ -1 // signifies UNUSED

// DEFAULT KEYGLOVE TOUCHSET
int touchset[] = {
    // KEY_A 0
    KGI_GY,

    // KEY_B 1
    KGI_WY,

    // KEY_C 2
    KGI_MY,

    // KEY_D 3
    KGI_FY,

    // KEY_E 4
    KGI_AY,

    // KEY_F 5
    KGI_SY,

    // KEY_G 6
    KGI_QY,

    // KEY_H 7
    KGI_CY,

    // KEY_I 8
    KGI_BY,

    // KEY_J 9
    KGI_Y4,

    // KEY_K 10
    KGI_RY,

    // KEY_L 11
    KGI_IY,

    // KEY_M 12
    KGI_PY,

    // KEY_N 13
    KGI_EY,

    // KEY_O 14
    KGI_JY,

    // KEY_P 15
    KGI_TY,

    // KEY_Q 16
    KGI_XY,

    // KEY_R 17
    KGI_KY,

    // KEY_S 18
    KGI_HY,

    // KEY_T 19
    KGI_DY,

    // KEY_U 20
    KGI_LY,

    // KEY_V 21
    KGI_OY,

    // KEY_W 22
    KGI_NY,

    // KEY_X 23
    KGI_UY,

    // KEY_Y 24
    KGI_VY,

    // KEY_Z 25
    KGI_Z4,

    // KEY_0 26
    KGI_DJ2,

    // KEY_1 27
    KGI_A2,

    // KEY_2 28
    KGI_D2,

    // KEY_3 29
    KGI_AD2,

    // KEY_4 30
    KGI_G2,

    // KEY_5 31
    KGI_AG2,

    // KEY_6 32
    KGI_DG2,

    // KEY_7 33
    KGI_ADG2,

    // KEY_8 34
    KGI_J2,

    // KEY_9 35
    KGI_AJ2,

    // KEY_BACKTICK 36
    KGI_,

    // KEY_MINUS 37
    KGI_,

    // KEY_EQUALS 38
    KGI_,

    // KEY_BACKSLASH 39
    KGI_,

    // KEY_BKSP 40
    KGI_GJY,

    // KEY_SPACE 41
    KGI_ADY,

    // KEY_TAB 42
    KGI_,

    // KEY_CAPS 43
    KGI_,

    // KEY_LSHIFT 44
    KGI_,

    // KEY_LCTRL 45
    KGI_,

    // KEY_LGUI 46
    KGI_,

    // KEY_LALT 47
    KGI_,

    // KEY_RSHIFT 48
    KGI_,

    // KEY_RCTRL 49
    KGI_,

    // KEY_RGUI 50
    KGI_,

    // KEY_RALT 51
    KGI_,

    // KEY_APPS 52
    KGI_,

    // KEY_ENTER 53
    KGI_,

    // KEY_ESC 54
    KGI_,

    // KEY_F1 55
    KGI_,

    // KEY_F2 56
    KGI_,

    // KEY_F3 57
    KGI_,

    // KEY_F4 58
    KGI_,

    // KEY_F5 59
    KGI_,

    // KEY_F6 60
    KGI_,

    // KEY_F7 61
    KGI_,

    // KEY_F8 62
    KGI_,

    // KEY_F9 63
    KGI_,

    // KEY_F10 64
    KGI_,

    // KEY_F11 65
    KGI_,

    // KEY_F12 66
    KGI_,

    // KEY_PRTSC 67
    KGI_,

    // KEY_SCRLOCK 68
    KGI_,

    // KEY_PAUSE 69
    KGI_,

    // KEY_LBRACKET 70
    KGI_,

    // KEY_INS 71
    KGI_,

    // KEY_HOME 72
    KGI_,

    // KEY_PGUP 73
    KGI_,

    // KEY_DEL 74
    KGI_,

    // KEY_END 75
    KGI_,

    // KEY_PGDN 76
    KGI_,

    // KEY_UP 77
    KGI_,

    // KEY_DOWN 78
    KGI_,

    // KEY_LEFT 79
    KGI_,

    // KEY_RIGHT 80
    KGI_,

    // KEY_NUMLOCK 81
    KGI_,

    // KEY_PAD_SLASH 82
    KGI_,

    // KEY_PAD_ASTR 83
    KGI_,

    // KEY_PAD_MINUS 84
    KGI_,

    // KEY_PAD_PLUS 85
    KGI_,

    // KEY_PAD_ENTER 86
    KGI_,

    // KEY_PAD_PERIOD 87
    KGI_,

    // KEY_PAD_0 88
    KGI_,

    // KEY_PAD_1 89
    KGI_,

    // KEY_PAD_2 90
    KGI_,

    // KEY_PAD_3 91
    KGI_,

    // KEY_PAD_4 92
    KGI_,

    // KEY_PAD_5 93
    KGI_,

    // KEY_PAD_6 94
    KGI_,

    // KEY_PAD_7 95
    KGI_,

    // KEY_PAD_8 96
    KGI_,

    // KEY_PAD_9 97
    KGI_,

    // KEY_RBRACKET 98
    KGI_,

    // KEY_SEMICOLON 99
    KGI_,

    // KEY_APOS 100
    KGI_,

    // KEY_COMMA 101
    KGI_,

    // KEY_PERIOD 102
    KGI_,

    // KEY_SLASH 103
    KGI_,



    // KEY_ACPI_POWER 104
    KGI_,

    // KEY_ACPI_SLEEP 105
    KGI_,

    // KEY_ACPI_WAKE 106
    KGI_,



    // KEY_WIN_NEXTTRK 107
    KGI_,

    // KEY_WIN_PREVTRK 108
    KGI_,

    // KEY_WIN_STOP 109
    KGI_,

    // KEY_WIN_PLAY 110
    KGI_,

    // KEY_WIN_MUTE 111
    KGI_,

    // KEY_WIN_VOLUP 112
    KGI_,

    // KEY_WIN_VOLDN 113
    KGI_,

    // KEY_WIN_MEDIA 114
    KGI_,

    // KEY_WIN_EMAIL 115
    KGI_,

    // KEY_WIN_CALC 116
    KGI_,

    // KEY_WIN_COMPUTER 117
    KGI_,




    // KEY_WWW_SEARCH 118
    KGI_,

    // KEY_WWW_HOME 119
    KGI_,

    // KEY_WWW_BACK 120
    KGI_,

    // KEY_WWW_FORWARD 121
    KGI_,

    // KEY_WWW_STOP 122
    KGI_,

    // KEY_WWW_REFRESH 123
    KGI_,

    // KEY_WWW_FAVORITES 124
    KGI_
};
