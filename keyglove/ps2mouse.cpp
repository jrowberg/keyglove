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

#ifndef USE_LUFA
    #include "WProgram.h"
#endif

#include "ps2dev.h"
#include "ps2mouse.h"
#ifdef round
    #undef round
#endif

PS2mouse::PS2mouse(PS2dev *m) {
    mouse = m;
}

void PS2mouse::move(int dx, int dy) {
    cmd_move(dx, dy);
}

void PS2mouse::initialize() {
    Serial.print("Starting PS/2 mouse initialization...\n");
    run_bat();
    Serial.print("PS/2 mouse initialization complete\n");
}

void PS2mouse::run_bat() {
    Serial.print("Running mouse Basic Assurance Test (BAT)...\n");
    set_defaults();
    delay(520);
    Serial.print("Mouse BAT completed, sending success code...\n");
    cmd_success();
    while (mouse -> write(0x00) != 0); // send device ID so host knows it's a mouse
    Serial.print("Finished\n");
}

void PS2mouse::set_defaults() {
    Serial.print("Setting PS/2 mouse defaults...\n");
    mode = PS2MOUSE_MODE_RESET;
    sample_rate = 100;
    resolution = PS2MOUSE_RESOLUTION_4;
    scaling = 1;
    reporting_enabled = false;
    buttons[0] = 0;
    buttons[1] = 0;
    buttons[2] = 0;
    last_sent_byte = 0;
    Serial.print("PS/2 mouse defaults set\n");
}

// send PS2 "acknowledge" code
void PS2mouse::cmd_ack() {
    Serial.println("MOUSE COMMAND: acknowledge (0xFA)");
    while (mouse -> write(0xFA) != 0);
    last_sent_byte = 0xFA;
}

// send PS2 "resend" code
void PS2mouse::cmd_resend() {
    Serial.println("MOUSE COMMAND: resend (0xFE)");
    while (mouse -> write(0xFE) != 0);
    last_sent_byte = 0xFE;
}

// send PS2 "success" code
void PS2mouse::cmd_success() {
    Serial.println("MOUSE COMMAND: success (0xAA)");
    while (mouse -> write(0xAA) != 0);
    last_sent_byte = 0xAA;
}

// send PS2 "error" code
void PS2mouse::cmd_error() {
    Serial.println("MOUSE COMMAND: error (0xFC)");
    while (mouse -> write(0xFC) != 0);
    last_sent_byte = 0xFC;
}

// send mouse movement code
void PS2mouse::cmd_move(int dx, int dy) {
    char overflowx = 0;
    char overflowy = 0;
    char data[3];
    
    if (dx > 255) {
        overflowx = 1;
        dx = 255;
    } else if (dx < -255) {
        overflowx = 1;
        dx = -255;
    }  
    if (dy > 255) {
        overflowy = 1;
        dy = 255;
    } else if (dy < -255) {
        overflowy = 1;
        dy = -255;
    }
    
    // mouse control/button byte
    data[0] =
        ((overflowy & 1) << 7) |             // bit 7
        ((overflowx & 1) << 6) |             // bit 6
        ((((dy & 0x100) >> 8) & 1) << 5) |   // bit 5
        ((((dx & 0x100) >> 8) & 1) << 4) |   // bit 4
        ((1) << 3) |                         // bit 3
        ((buttons[1] & 1) << 2) |            // bit 2
        ((buttons[2] & 1) << 1) |            // bit 1
        ((buttons[0] & 1) << 0) ;            // bit 0
      
    // mouse coordinate movement bytes
    data[1] = dx & 0xff;
    data[2] = dy & 0xff;
    
    while (mouse -> write(data[0]));
    while (mouse -> write(data[1]));
    while (mouse -> write(data[2]));
}

// process command from host
void PS2mouse::process_command() {
    unsigned char command = 0, b = 0;
    while (mouse -> read(&command));
    Serial.print("Received mouse host command 0x");
    Serial.println(command, HEX);
    switch (command) {
        case 0xE6: // set scaling 1:1
            Serial.println("M-HOST COMMAND: set scaling 1:1");
            cmd_ack();
            break;
        case 0xE7: // set scaling 2:1
            Serial.println("M-HOST COMMAND: set scaling 2:1");
            cmd_ack();
            break;
        case 0xE8: // set resolution
            Serial.println("M-HOST COMMAND: set resolution");
            cmd_ack();
            mouse -> read(&b);
            resolution = b;
            cmd_ack();
            break;
        case 0xE9: // status request
            Serial.println("M-HOST COMMAND: status request");
            cmd_ack();
            break;
        case 0xEA: // set stream mode
            Serial.println("M-HOST COMMAND: set stream mode");
            cmd_ack();
            last_mode = mode;
            mode = PS2MOUSE_MODE_STREAM;
            break;
        case 0xEB: // read data
            Serial.println("M-HOST COMMAND: read data");
            cmd_ack();
            cmd_move(1, 1);
            break;
        case 0xEC: // reset wrap mode
            Serial.println("M-HOST COMMAND: reset wrap mode");
            cmd_ack();
            mode = last_mode;
            break;
        case 0xEE: // set wrap mode
            Serial.println("M-HOST COMMAND: set wrap mode");
            cmd_ack();
            last_mode = mode;
            mode = PS2MOUSE_MODE_WRAP;
            break;
        case 0xF0: // set remote mode
            Serial.println("M-HOST COMMAND: set remote mode");
            cmd_ack();
            last_mode = mode;
            mode = PS2MOUSE_MODE_REMOTE;
            break;
        case 0xF2: // get device ID
            Serial.println("M-HOST COMMAND: get device ID");
            cmd_ack();
            while (mouse -> write(0x00));
            break;
        case 0xF3: // set sample rate
            Serial.println("M-HOST COMMAND: set sample rate");
            cmd_ack();
            mouse -> read(&b);
            sample_rate = b;
            cmd_ack();
            break;
        case 0xF4: // enable data reporting
            Serial.println("M-HOST COMMAND: enable data reporting");
            cmd_ack();
            reporting_enabled = true;
            break;
        case 0xF5: // disable
            Serial.println("M-HOST COMMAND: disable data reporting");
            cmd_ack();
            reporting_enabled = false;
            break;
        case 0xF6: // set defaults and reset
            Serial.println("M-HOST COMMAND: set defaults and reset");
            cmd_ack();
            initialize();
            break;
        case 0xFE: // resend
            Serial.println("M-HOST COMMAND: resend");
            while (mouse -> write(last_sent_byte) != 0);
            break;
        case 0xFF: // reset
            Serial.println("M-HOST COMMAND: reset");
            cmd_ack();
            break;
        default: // unknown, hmm
            Serial.println("UNKNOWN M-HOST COMMAND");
            cmd_error();
            break;
    }
}

