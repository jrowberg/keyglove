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
uint16_t mx, my, mz, sy;                   // absolute coordinates
uint16_t mx0, my0, mz0, sy0;               // last-iteration absolute coordinates
uint16_t mdx, mdy, mdz, sdy;               // relative movement
// (note z is for 3D movement, sy is for scrolling)

uint8_t moveMouse;                    // whether any mouse movement should possibly occur
uint8_t scrollMouse;                  // whether any mouse scrolling should possibly occur

uint8_t opt_mouse_invert_x = 1;                   // invert mouse x movements
uint8_t opt_mouse_invert_y = 1;                   // invert mouse y movements
uint8_t opt_mouse_invert_z = 0;                   // invert mouse z movements

uint8_t opt_mouse_mode = 0;                       // which mouse move mode (0=disable, 1/2/3/4 as defined)
uint8_t opt_scroll_mode = 0;                      // which mouse scroll mode (0=disable, 1/2/3 as defined), uses mouse Y axis values

float opt_mouse_scale_mode1[] = { 1, 1 };         // speed scale [x,y] for mode 1 (tilt-velocity)
float opt_mouse_scale_mode2[] = { 1, 1 };         // speed scale [x,y] for mode 2 (tilt-position)
float opt_mouse_scale_mode3[] = { 1, 1 };         // speed scale [x,y] for mode 3 (movement-position)
float opt_mouse_scale_mode4[] = { 1, 1, 1 };      // speed scale [x,y,z] for mode 4 (3D)

void setup_mouse() {
    mx = my = mz = mx0 = my0 = mz0 = 0;
    sy = sy0 = 0;
}

void update_mouse() {
    switch (opt_mouse_mode) {
        case MOUSE_MODE_TILT_VELOCITY:
            if (aset) {
                mx0 = mx;
                my0 = my;
                mx -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                my -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                moveMouse = true;
            } else {
                axBase = ax;
                ayBase = ay;
                azBase = az;
                aset = true;
            }
            break;
        /*case MOUSE_MODE_TILT_POSITION:
            if (aset) {
                // progressive scaling
                mx -= (pow(2, abs(axv)) * opt_mouse_scale_mode2[0]) * (float)(ax - axBase);
                my -= (pow(2, abs(ayv)) * opt_mouse_scale_mode2[1]) * (float)(ay - ayBase);
            }
            axBase = ax;
            ayBase = ay;
            azBase = az;
            aset = true;
            break;*/
        case MOUSE_MODE_TILT_POSITION:
            mx0 = mx;
            my0 = my;
            mx += (gx < 0) ? -sqrt(-gx) : sqrt(gx);
            my += (gy < 0) ? -sqrt(-gy) : sqrt(gy);
            moveMouse = true;
            break;
        case MOUSE_MODE_MOVEMENT_POSITION:
            if (aset) {
                mx0 = mx;
                my0 = my;
                mx -= (float)(x - xBase) / opt_mouse_scale_mode3[0];
                my -= (float)((z - 256) - zBase) / opt_mouse_scale_mode3[1];
                moveMouse = true;
            } else {
                xBase = x;
                yBase = y;
                zBase = z - 256;
                aset = true;
            }
            break;
        case MOUSE_MODE_3D:
            mx0 = mx;
            my0 = my;
            mz0 = mz;
            mx -= xv / opt_mouse_scale_mode4[0];
            my -= zv / opt_mouse_scale_mode4[1];
            mz -= yv / opt_mouse_scale_mode4[2];
            moveMouse = true;
            break;
    }
    switch (opt_scroll_mode) {
        case SCROLL_MODE_TILT_VELOCITY: // gyro
            if (aset) {
                mx0 = mx;
                my0 = my;
                mx -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                my -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                scrollMouse = true;
            } else {
                axBase = ax;
                ayBase = ay;
                azBase = az;
                aset = true;
            }
            break;
        case SCROLL_MODE_TILT_POSITION: // gyro
            sy0 = sy;
            sy -= (gx < 0) ? -sqrt(-gx / 30) : sqrt(gx / 30);
            scrollMouse = true;
            break;
        case SCROLL_MODE_MOVEMENT_POSITION: // accel
            if (aset) {
                mx0 = mx;
                my0 = my;
                mx -= (float)(x - xBase) / opt_mouse_scale_mode3[0];
                my -= (float)((z - 256) - zBase) / opt_mouse_scale_mode3[1];
                scrollMouse = true;
            } else {
                xBase = x;
                yBase = y;
                zBase = z - 256;
                aset = true;
            }
            break;
    }

    //if (BOUND_MOUSEX > 0 && abs(mx) > BOUND_MOUSEX) mx = mx < 0 ? -BOUND_MOUSEX : BOUND_MOUSEX;
    //if (BOUND_MOUSEY > 0 && abs(my) > BOUND_MOUSEY) my = my < 0 ? -BOUND_MOUSEY : BOUND_MOUSEY;
    
    // get relative movement amounts
    mdx = mx - mx0;
    mdy = my - my0;
    mdz = mz - mz0;
    
    if (opt_mouse_invert_x == 1) mdx = -mdx;
    if (opt_mouse_invert_y == 1) mdy = -mdy;
    if (opt_mouse_invert_z == 1) mdz = -mdz;
    if (moveMouse && opt_mouse_mode > 0 && (mdx != 0 || mdy != 0 || mdz != 0)) {
        #ifdef ENABLE_USB
            if (mdz > 0) Mouse.move(mdx, mdy, mdz);
            else Mouse.move(mdx, mdy);
        #endif /* ENABLE_USB */
        #ifdef ENABLE_BLUETOOTH
            if (mdz > 0) bluetooth.move(mdx, mdy, mdz);
            else bluetooth.move(mdx, mdy);
        #endif /* ENABLE_BLUETOOTH */
        #ifdef ENABLE_RX400
            if (mdz > 0) RX400.move(mdx, mdy, mdz);
            else RX400.move(mdx, mdy);
        #endif /* ENABLE_RX400 */
        DEBUG_PRN_MOUSE("mouse ");
        DEBUG_PRN_MOUSE(mx); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(my); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mdx); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mdy); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mdz); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRNL_MOUSE(mdz);
    }
    if (scrollMouse && opt_scroll_mode > 0 && sdy != 0) {
        #ifdef ENABLE_USB
            Mouse.scroll(sdy);
        #endif /* ENABLE_USB */
        #ifdef ENABLE_BLUETOOTH
            bluetooth.scroll(sdy);
        #endif /* ENABLE_BLUETOOTH */
        #ifdef ENABLE_RX400
            RX400.scroll(sdy);
        #endif /* ENABLE_RX400 */
        #ifdef SERIAL_DEBUG_MOUSE
            Serial.print("scroll ");
            Serial.print(sy); Serial.print(" ");
            Serial.println(sdy);
        #endif /* SERIAL_DEBUG_MOUSE */
    }
}

#endif // _SETUP_MOUSE_H_