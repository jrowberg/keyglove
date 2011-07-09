// Keyglove controller source code - Bluegiga iWRAP interface class library
// 4/13/2011 by Jeff Rowberg <jeff@rowberg.net>

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

#ifndef iwrap_h
#define iwrap_h

#ifdef USE_LUFA
    #include "ArduinoWrapper.h"
#endif

#ifndef USE_LUFA
    #include "WProgram.h"
#endif

#ifdef round
    #undef round
#endif

class iWRAP {
    public:
        iWRAP(HardwareSerial *u);
        void parse(int ch);
        void set_modifier(uint8_t c);
        void set_key1(uint8_t c);
        void set_key2(uint8_t c);
        void set_key3(uint8_t c);
        void set_key4(uint8_t c);
        void set_key5(uint8_t c);
        void set_key6(uint8_t c);
        void send_now();
        void set_buttons(uint8_t left, uint8_t middle, uint8_t right);
        void move(uint8_t x, uint8_t y);
        void move(uint8_t x, uint8_t y, uint8_t z);
        void scroll(uint8_t s);
    
    private:
        uint8_t mouse_buttons;
        uint8_t keyboard_keys[6];
        uint8_t keyboard_modifier_keys;
        
        HardwareSerial *uart;
};

#endif /* iwrap_h */
