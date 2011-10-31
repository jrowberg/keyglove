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



#ifndef _SETUP_MOUSE_H_
#define _SETUP_MOUSE_H_

#ifndef NO_USB_SUPPORT
#endif

#define MOUSE_LEFT 1
#define MOUSE_MIDDLE 2
#define MOUSE_RIGHT 4

#define MOUSE_ACTION_MOVE 0
#define MOUSE_ACTION_SCROLL 1

#define MOUSE_MODE_TILT_VELOCITY 1
#define MOUSE_MODE_TILT_POSITION 2
#define MOUSE_MODE_MOVEMENT_POSITION 3
#define MOUSE_MODE_3D 4

#define SCROLL_MODE_TILT_VELOCITY 1
#define SCROLL_MODE_TILT_POSITION 2
#define SCROLL_MODE_MOVEMENT_POSITION 3

// mouse measurements
int16_t mousedx, mousedy, mousedz, scrolldy;    // relative movement
// (note z is for 3D movement, scrolly is for scrolling)

uint8_t opt_mouse_invert_x = 1;                 // invert mouse x movements
uint8_t opt_mouse_invert_y = 1;                 // invert mouse y movements
uint8_t opt_mouse_invert_z = 0;                 // invert mouse z movements

uint8_t opt_mouse_mode = 0;                     // which mouse move mode (0=disable, 1/2/3/4 as defined)
uint8_t opt_scroll_mode = 0;                    // which mouse scroll mode (0=disable, 1/2/3 as defined), uses mouse Y axis values

float opt_mouse_scale_mode1[] = { 1, 1 };       // speed scale [x,y] for mode 1 (tilt-velocity)
float opt_mouse_scale_mode2[] = { 1, 1 };       // speed scale [x,y] for mode 2 (tilt-position)
float opt_mouse_scale_mode3[] = { 1, 1 };       // speed scale [x,y] for mode 3 (movement-position)
float opt_mouse_scale_mode4[] = { 1, 1, 1 };    // speed scale [x,y,z] for mode 4 (3D)

void setup_mouse() {
    mousedx = mousedy = mousedz = 0;
    scrolldy = 0;
}

void update_mouse() {
    switch (opt_mouse_mode) {
        case MOUSE_MODE_TILT_VELOCITY:
            break;
        case MOUSE_MODE_TILT_POSITION:
            mousedx = ((gxv < 0) ? -pow(-(float)gxv/3, 1.3)*3 : pow((float)gxv/3, 1.3)*3) + ((gzv < 0) ? -pow(-(float)gzv/3, 1.3)*3 : pow((float)gzv/3, 1.3)*3);
            mousedy = (gyv < 0) ? -pow(-(float)gyv/3, 1.3)*3 : pow((float)gyv/3, 1.3)*3;
            break;
        case MOUSE_MODE_MOVEMENT_POSITION:
            break;
        case MOUSE_MODE_3D:
            mousedx = axa * opt_mouse_scale_mode4[0];
            mousedy = aya * opt_mouse_scale_mode4[1];
            mousedz = aza * opt_mouse_scale_mode4[2];
            break;
    }
    switch (opt_scroll_mode) {
        case SCROLL_MODE_TILT_VELOCITY: // gyro
            break;
        case SCROLL_MODE_TILT_POSITION: // gyro
            scrolldy -= ((float)gyv < 0) ? -sqrt(-(float)gyv / 30) : sqrt((float)gyv / 30);
            break;
        case SCROLL_MODE_MOVEMENT_POSITION: // accel
            break;
    }

    if (opt_mouse_mode > 0 && (mousedx != 0 || mousedy != 0 || mousedz != 0)) {
        if (opt_mouse_invert_x == 1) mousedx = -mousedx;
        if (opt_mouse_invert_y == 1) mousedy = -mousedy;
        if (opt_mouse_invert_z == 1) mousedz = -mousedz;
        #ifdef ENABLE_USB
            if (mousedz > 0) Mouse.move(mousedx, mousedy, mousedz);
            else Mouse.move(mousedx, mousedy);
        #endif /* ENABLE_USB */
        #ifdef ENABLE_BLUETOOTH
            if (mousedz > 0) bluetooth.move(mousedx, mousedy, mousedz);
            else bluetooth.move(mousedx, mousedy);
        #endif /* ENABLE_BLUETOOTH */
        #ifdef ENABLE_RX400
            if (mousedz > 0) RX400.move(mousedx, mousedy, mousedz);
            else RX400.move(mousedx, mousedy);
        #endif /* ENABLE_RX400 */
        DEBUG_PRN_MOUSE("mouse ");
        DEBUG_PRN_MOUSE(mousedx); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mousedy); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRNL_MOUSE(mousedz);
    }
    if (opt_scroll_mode > 0 && scrolldy != 0) {
        if (opt_mouse_invert_y == 1) mousedy = -mousedy;
        #ifdef ENABLE_USB
            Mouse.scroll(scrolldy);
        #endif /* ENABLE_USB */
        #ifdef ENABLE_BLUETOOTH
            bluetooth.scroll(scrolldy);
        #endif /* ENABLE_BLUETOOTH */
        #ifdef ENABLE_RX400
            RX400.scroll(scrolldy);
        #endif /* ENABLE_RX400 */
        #ifdef SERIAL_DEBUG_MOUSE
            Serial.print("scroll ");
            Serial.println(scrolldy);
        #endif /* SERIAL_DEBUG_MOUSE */
    }
}

#endif // _SETUP_MOUSE_H_