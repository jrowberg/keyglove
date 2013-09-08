// Keyglove controller source code - Special hardware setup file
// 7/17/2011 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SUPPORT_FEEDBACK_RGB_H_
#define _SUPPORT_FEEDBACK_RGB_H_

#define KG_RGB_OFF 0
#define KG_RGB_LONGBLINK 1
#define KG_RGB_LONGPULSE 2
#define KG_RGB_SHORTBLINK 3
#define KG_RGB_SHORTPULSE 4
#define KG_RGB_SOLID 5

uint8_t rgbBlinkRed;
uint8_t rgbBlinkGreen;
uint8_t rgbBlinkBlue;

void set_rgb_mode(uint8_t r, uint8_t g, uint8_t b) {
    rgbBlinkRed = r;
    rgbBlinkGreen = g;
    rgbBlinkBlue = b;
    // logic: r=0 -> off, r=5 -> on, else no immediate change
    set_rgb_logic(
        r == 0 ? 0 : (r == 5 ? 1 : 255),
        g == 0 ? 0 : (g == 5 ? 1 : 255),
        b == 0 ? 0 : (b == 5 ? 1 : 255)
    );
    #if (KG_HOSTIF > 0)
        txPacketLength = create_packet(txPacket, KG_PACKET_CLASS_FEEDBACK, KG_PACKET_FEEDBACK_RGB);
        send_keyglove_packet(txPacket, txPacketLength, true);
    #endif
}

void setup_feedback_rgb() {
    // SELF-TEST
    //set_rgb_logic(1, 1, 1); // turn everything on and wait 1/20 sec
    //delay(50);
    //set_rgb_logic(0, 0, 0); // turn everything off again
}

void update_feedback_rgb() {
    if      (rgbBlinkRed   == 1 && keygloveTick % 50 == 0) set_rgb_logic(keygloveTick % 100 >= 50 ? 0 : 1, 255, 255);
    else if (rgbBlinkRed   == 2 && keygloveTick % 25 == 0) set_rgb_logic(keygloveTick % 100 >= 25 ? 0 : 1, 255, 255);
    else if (rgbBlinkRed   == 3 && keygloveTick % 10 == 0) set_rgb_logic(keygloveTick %  20 >= 10 ? 0 : 1, 255, 255);
    else if (rgbBlinkRed   == 4 && keygloveTick %  5 == 0) set_rgb_logic(keygloveTick %  20 >=  5 ? 0 : 1, 255, 255);
    if      (rgbBlinkGreen == 1 && keygloveTick % 50 == 0) set_rgb_logic(255, keygloveTick % 100 >= 50 ? 0 : 1, 255);
    else if (rgbBlinkGreen == 2 && keygloveTick % 25 == 0) set_rgb_logic(255, keygloveTick % 100 >= 25 ? 0 : 1, 255);
    else if (rgbBlinkGreen == 3 && keygloveTick % 10 == 0) set_rgb_logic(255, keygloveTick %  20 >= 10 ? 0 : 1, 255);
    else if (rgbBlinkGreen == 4 && keygloveTick %  5 == 0) set_rgb_logic(255, keygloveTick %  20 >=  5 ? 0 : 1, 255);
    if      (rgbBlinkBlue  == 1 && keygloveTick % 50 == 0) set_rgb_logic(255, 255, keygloveTick % 100 >= 50 ? 0 : 1);
    else if (rgbBlinkBlue  == 2 && keygloveTick % 25 == 0) set_rgb_logic(255, 255, keygloveTick % 100 >= 25 ? 0 : 1);
    else if (rgbBlinkBlue  == 3 && keygloveTick % 10 == 0) set_rgb_logic(255, 255, keygloveTick %  20 >= 10 ? 0 : 1);
    else if (rgbBlinkBlue  == 4 && keygloveTick %  5 == 0) set_rgb_logic(255, 255, keygloveTick %  20 >=  5 ? 0 : 1);
}

#endif // _SUPPORT_FEEDBACK_RGB_H_


