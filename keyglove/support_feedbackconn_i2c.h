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



#ifndef _SUPPORT_FEEDBACKCONN_I2C_H_
#define _SUPPORT_FEEDBACKCONN_I2C_H_

#ifdef NO_I2C_SUPPORT
    #error Selected board has no I2C support. I2C devices cannot be enabled under these circumstances.
#endif

#define KG_FEEDBACK_MODULE_ADDRESS 0x57

void set_rgb_logic(uint8_t r, uint8_t g, uint8_t b) {
    if (r != 255) I2Cdev::writeByte(KG_FEEDBACK_MODULE_ADDRESS, 0x00, r ? 255 : 0);
    if (g != 255) I2Cdev::writeByte(KG_FEEDBACK_MODULE_ADDRESS, 0x01, g ? 255 : 0);
    if (b != 255) I2Cdev::writeByte(KG_FEEDBACK_MODULE_ADDRESS, 0x02, b ? 255 : 0);
}

void set_vibrate_logic(uint8_t vibrate) {
    I2Cdev::writeByte(KG_FEEDBACK_MODULE_ADDRESS, 0x03, vibrate ? 255 : 0);
}

void set_piezo_tone(uint16_t frequency) {
    if (frequency == 0) {
        I2Cdev::writeByte(KG_FEEDBACK_MODULE_ADDRESS, 0x04, 0);
    } else {
        uint8_t buf[3];
        buf[0] = 0xFF;
        buf[1] = frequency >> 8;
        buf[2] = frequency;
        I2Cdev::writeBytes(KG_FEEDBACK_MODULE_ADDRESS, 0x04, 3, buf);
    }
}

void setup_feedbackconn() {
    // set all feedback elements to off/silent
    uint8_t defaults[5] = { 0, 0, 0, 0, 0 };
    I2Cdev::writeBytes(KG_FEEDBACK_MODULE_ADDRESS, 0x00, 5, defaults);
}

//void update_feedbackconn() {
//}

#endif // _SUPPORT_FEEDBACKCONN_I2C_H_