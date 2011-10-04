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
int16_t mousex, mousey, mousez, scrolly;  // absolute coordinates
int16_t mousex0, mousey0, mousez0, scrolly0;               // last-iteration absolute coordinates
int16_t mousedx, mousedy, mousedz, scrolldy;               // relative movement
// (note z is for 3D movement, scrolly is for scrolling)

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
    mousex = mousey = mousez = 0;
    mousex0 = mousey0 = mousez0 = 0;
    scrolly = scrolly0 = 0;
}

void update_mouse() {
    moveMouse = false;
    switch (opt_mouse_mode) {
        case MOUSE_MODE_TILT_VELOCITY:
            if (aset) {
                mousex0 = mousex;
                mousey0 = mousey;
                mousex -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                mousey -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                moveMouse = mousex0 != mousex || mousey0 != mousey;
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
                mousex -= (pow(2, abs(axv)) * opt_mouse_scale_mode2[0]) * (float)(ax - axBase);
                mousey -= (pow(2, abs(ayv)) * opt_mouse_scale_mode2[1]) * (float)(ay - ayBase);
            }
            axBase = ax;
            ayBase = ay;
            azBase = az;
            aset = true;
            break;*/
        case MOUSE_MODE_TILT_POSITION:
            mousex0 = mousex;
            mousey0 = mousey;
            mousex += ((gx < 0) ? -pow(-gx/100.0, 1.3)*3 : pow(gx/100.0, 1.3)*3) + ((gz < 0) ? -pow(-gz/100.0, 1.3)*3 : pow(gz/100.0, 1.3)*3);
            mousey += (gy < 0) ? -pow(-gy/100.0, 1.3)*3 : pow(gy/100.0, 1.3)*3;
            moveMouse = mousex0 != mousex || mousey0 != mousey;
            break;
        case MOUSE_MODE_MOVEMENT_POSITION:
            if (aset) {
                mousex0 = mousex;
                mousey0 = mousey;
                mousex -= (float)(ax - axBase) / opt_mouse_scale_mode3[0];
                mousey -= (float)((az - 256) - azBase) / opt_mouse_scale_mode3[1];
                moveMouse = mousex0 != mousex || mousey0 != mousey;
            } else {
                axBase = ax;
                ayBase = ay;
                azBase = az - 256;
                aset = true;
            }
            break;
        case MOUSE_MODE_3D:
            mousex0 = mousex;
            mousey0 = mousey;
            mousez0 = mousez;
            mousex -= axv / opt_mouse_scale_mode4[0];
            mousey -= azv / opt_mouse_scale_mode4[1];
            mousez -= ayv / opt_mouse_scale_mode4[2];
            moveMouse = mousex0 != mousex || mousey0 != mousey || mousez0 != mousez;
            break;
    }
    switch (opt_scroll_mode) {
        case SCROLL_MODE_TILT_VELOCITY: // gyro
            if (aset) {
                mousex0 = mousex;
                mousey0 = mousey;
                mousex -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                mousey -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                scrollMouse = true;
            } else {
                axBase = ax;
                ayBase = ay;
                azBase = az;
                aset = true;
            }
            break;
        case SCROLL_MODE_TILT_POSITION: // gyro
            scrolly0 = scrolly;
            scrolly -= (gx < 0) ? -sqrt(-gx / 30) : sqrt(gx / 30);
            scrollMouse = true;
            break;
        case SCROLL_MODE_MOVEMENT_POSITION: // accel
            if (aset) {
                mousex0 = mousex;
                mousey0 = mousey;
                mousex -= (float)(ax - axBase) / opt_mouse_scale_mode3[0];
                mousey -= (float)((az - 256) - azBase) / opt_mouse_scale_mode3[1];
                scrollMouse = true;
            } else {
                axBase = ax;
                ayBase = ay;
                azBase = az - 256;
                aset = true;
            }
            break;
    }

    //if (BOUND_MOUSEX > 0 && abs(mousex) > BOUND_MOUSEX) mousex = mousex < 0 ? -BOUND_MOUSEX : BOUND_MOUSEX;
    //if (BOUND_MOUSEY > 0 && abs(mousey) > BOUND_MOUSEY) mousey = mousey < 0 ? -BOUND_MOUSEY : BOUND_MOUSEY;

    // get relative movement amounts
    mousedx = mousex - mousex0;
    mousedy = mousey - mousey0;
    mousedz = mousez - mousez0;

    if (opt_mouse_invert_x == 1) mousedx = -mousedx;
    if (opt_mouse_invert_y == 1) mousedy = -mousedy;
    if (opt_mouse_invert_z == 1) mousedz = -mousedz;
    if (moveMouse && opt_mouse_mode > 0 && (mousedx != 0 || mousedy != 0 || mousedz != 0)) {
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
        DEBUG_PRN_MOUSE(mousex); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mousey); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mousez); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mousedx); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRN_MOUSE(mousedy); DEBUG_PRN_MOUSE(" ");
        DEBUG_PRNL_MOUSE(mousedz);
    }
    if (scrollMouse && opt_scroll_mode > 0 && scrolldy != 0) {
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
            Serial.print(scrolly); Serial.print(" ");
            Serial.println(scrolldy);
        #endif /* SERIAL_DEBUG_MOUSE */
    }
}

#endif // _SETUP_MOUSE_H_