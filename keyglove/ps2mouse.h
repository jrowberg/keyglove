// Keyglove controller source code - PS/2 mouse class library
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

#ifndef ps2mouse_h
#define ps2mouse_h

#ifdef LUFA
    #include "ArduinoWrapper.h"
#endif

#ifndef LUFA
    #include "WProgram.h"
#endif

#include "ps2dev.h"

#ifdef round
    #undef round
#endif

#define PS2MOUSE_MODE_RESET 1
#define PS2MOUSE_MODE_STREAM 2
#define PS2MOUSE_MODE_REMOTE 3
#define PS2MOUSE_MODE_WRAP 4

#define PS2MOUSE_RESOLUTION_1 0
#define PS2MOUSE_RESOLUTION_2 1
#define PS2MOUSE_RESOLUTION_4 2
#define PS2MOUSE_RESOLUTION_8 3

// sample rates are simply integers (10, 20, 40, 60, 80, 100, 200)
// scaling is either 1 or 2

class PS2mouse {
    public:
        PS2mouse(PS2dev *m);
        void move(int16_t dx, int16_t dy);
        void initialize();
        void process_command();
    
    private:
        PS2dev *mouse;
        uint8_t mode;
        bool reporting_enabled;
        uint8_t sample_rate;
        uint8_t resolution;
        uint8_t scaling;
        uint8_t buttons[3];
        uint8_t last_sent_byte;
        uint8_t last_mode;
        
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
        
        // send mouse movement data
        void cmd_move(int16_t dx, int16_t dy);
};

#endif /* ps2mouse_h */
