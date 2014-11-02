// Keyglove controller source code - HID mouse helper functions
// 7/4/2014 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

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

/**
 * @file support_hid_mouse.h
 * @brief HID mouse helper functions
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file provides supporting code for translating motion into mouse movement
 * data, and for sending this HID mouse data to a connected wired or wireless
 * host. This is only relevant if you have enabled normal HID support over USB
 * or Bluetooth.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_HID_MOUSE_H_
#define _SUPPORT_HID_MOUSE_H_

#define MOUSE_LEFT                      1   ///< Left mouse button bitmask
#define MOUSE_RIGHT                     2   ///< Right mouse button bitmask
#define MOUSE_MIDDLE                    4   ///< Middle mouse button bitmask

#define MOUSE_ACTION_MOVE               1   ///< Mouse cursor movement action
#define MOUSE_ACTION_SCROLL             2   ///< Scrolling movement action

/**
 * @brief List of possible values for cursor movement mode
 */
typedef enum {
    MOUSE_MODE_OFF = 0,             ///< (0) Mouse movement disabled
    MOUSE_MODE_TILT_VELOCITY,       ///< (1) Mouse movement based on tilt velocity (rotational)
    MOUSE_MODE_TILT_POSITION,       ///< (2) Mouse movement based on tilt position (rotational)
    MOUSE_MODE_MOVEMENT_POSITION,   ///< (3) Mouse movement based on actual movement (2D linear)
    MOUSE_MODE_3D,                  ///< (4) Mouse movement based on actual movement (3D linear)
    MOUSE_MODE_MAX
} mouse_movement_mode_t;

/**
 * @brief List of possible values for scroll movement mode
 */
typedef enum {
    SCROLL_MODE_OFF = 0,            ///< (0) Scrolling disabled
    SCROLL_MODE_TILT_VELOCITY,      ///< (1) Scrolling based on tilt velocity (rotational)
    SCROLL_MODE_TILT_POSITION,      ///< (2) Scrolling based on tilt position (rotational)
    SCROLL_MODE_MOVEMENT_POSITION,  ///< (3) Scrolling based on actual movement (2D linear)
    SCROLL_MODE_MAX
} scroll_movement_mode_t;

uint8_t hidMouseDown = 0;   ///< Mouse buttons currently down

int8_t hidMouseDX;          ///< Mouse cursor X axis movement delta
int8_t hidMouseDY;          ///< Mouse cursor Y axis movement delta
int8_t hidMouseDZ;          ///< Mouse cursor Z axis movement delta (3D-only)
int8_t hidScrollDX;         ///< Horizontal scroll movement delta
int8_t hidScrollDY;         ///< Vertical scroll movement delta

uint8_t opt_hid_mouse_invert_x = 0;                 ///< OPTION: Invert mouse x movements
uint8_t opt_hid_mouse_invert_y = 1;                 ///< OPTION: Invert mouse y movements
uint8_t opt_hid_mouse_invert_z = 0;                 ///< OPTION: Invert mouse z movements

mouse_movement_mode_t opt_hid_mouse_mode = MOUSE_MODE_OFF;      ///< OPTION: Mouse cursor movement mode
scroll_movement_mode_t opt_hid_scroll_mode = SCROLL_MODE_OFF;   ///< OPTION: Scroll movement mode

float opt_hid_mouse_scale_mode1[] = { 1, 1 };       ///< OPTION: Speed scale [x,y] for mode 1 (tilt-velocity)
float opt_hid_mouse_scale_mode2[] = { 1, 1 };       ///< OPTION: Speed scale [x,y] for mode 2 (tilt-position)
float opt_hid_mouse_scale_mode3[] = { 1, 1 };       ///< OPTION: Speed scale [x,y] for mode 3 (movement-position)
float opt_hid_mouse_scale_mode4[] = { 1, 1, 1 };    ///< OPTION: Speed scale [x,y,z] for mode 4 (3D)

/**
 * @brief Initialize HID mouse behavior
 */
void setup_hid_mouse() {
    // zero all relative movements
    hidMouseDX = hidMouseDY = hidMouseDZ = 0;
    hidScrollDX = hidScrollDY = 0;
}

/**
 * @brief Update mouse movements if necessary, called at 100Hz from loop()
 */
void update_hid_mouse() {
    #if KG_MOTION > 0
        switch (opt_hid_mouse_mode) {
            case MOUSE_MODE_TILT_VELOCITY:
                hidMouseDX += ((gv.y < 0) ? -pow(-(float)gv.y/30, 1.3)*3 : pow((float)gv.y/30, 1.3)*3) - ((gv.z < 0) ? -pow(-(float)gv.z/30, 1.3)*3 : pow((float)gv.z/30, 1.3)*3);
                hidMouseDY += (gv.x < 0) ? -pow(-(float)gv.x/30, 1.3)*3 : pow((float)gv.x/30, 1.3)*3;
                break;
            case MOUSE_MODE_TILT_POSITION:
                hidMouseDX = ((gv.y < 0) ? -pow(-(float)gv.y/30, 1.3)*3 : pow((float)gv.y/30, 1.3)*3) - ((gv.z < 0) ? -pow(-(float)gv.z/30, 1.3)*3 : pow((float)gv.z/30, 1.3)*3);
                hidMouseDY = (gv.x < 0) ? -pow(-(float)gv.x/30, 1.3)*3 : pow((float)gv.x/30, 1.3)*3;
                break;
            case MOUSE_MODE_MOVEMENT_POSITION:
                #if (KG_FUSION > 0)
                    hidMouseDX = apFrame.x * opt_hid_mouse_scale_mode4[0];
                    hidMouseDY = apFrame.y * opt_hid_mouse_scale_mode4[1];
                #else
                    hidMouseDX = aa.x * opt_hid_mouse_scale_mode4[0];
                    hidMouseDY = aa.y * opt_hid_mouse_scale_mode4[1];
                #endif
                break;
            case MOUSE_MODE_3D:
                #if (KG_FUSION > 0)
                    hidMouseDX = apFrame.x * opt_hid_mouse_scale_mode4[0];
                    hidMouseDY = apFrame.y * opt_hid_mouse_scale_mode4[1];
                    hidMouseDZ = apFrame.z * opt_hid_mouse_scale_mode4[2];
                #else
                    hidMouseDX = aa.x * opt_hid_mouse_scale_mode4[0];
                    hidMouseDY = aa.y * opt_hid_mouse_scale_mode4[1];
                    hidMouseDZ = aa.z * opt_hid_mouse_scale_mode4[2];
                #endif
                break;
        }
        switch (opt_hid_scroll_mode) {
            case SCROLL_MODE_TILT_VELOCITY: // gyro
                break;
            case SCROLL_MODE_TILT_POSITION: // gyro
                hidScrollDY -= ((float)gv.y < 0) ? -sqrt(-(float)gv.y / 30) : sqrt((float)gv.y / 30);
                break;
            case SCROLL_MODE_MOVEMENT_POSITION: // accel
                break;
        }
    #endif

    if (opt_hid_mouse_mode > 0 && (hidMouseDX != 0 || hidMouseDY != 0 || hidMouseDZ != 0)) {
        if (opt_hid_mouse_invert_x == 1) hidMouseDX = -hidMouseDX;
        if (opt_hid_mouse_invert_y == 1) hidMouseDY = -hidMouseDY;
        if (opt_hid_mouse_invert_z == 1) hidMouseDZ = -hidMouseDZ;
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            if (hidMouseDZ > 0) Mouse.move(hidMouseDX, hidMouseDY, hidMouseDZ);
            else Mouse.move(hidMouseDX, hidMouseDY);
        #endif /* KG_HOSTIF_USB_HID */
        #if KG_HOSTIF & KG_HOSTIF_BT2_HID
            if (hidMouseDZ > 0) BTMouse.move(hidMouseDX, hidMouseDY, hidMouseDZ, 0);
            else BTMouse.move(hidMouseDX, hidMouseDY);
        #endif /* KG_HOSTIF_BT2_HID */
        #if KG_HOSTIF & KG_HOSTIF_R400_HID
            if (hidMouseDZ > 0) RX400.move(hidMouseDX, hidMouseDY, hidMouseDZ);
            else RX400.move(hidMouseDX, hidMouseDY);
        #endif /* KG_HOSTIF_R400_HID */
        DEBUG_HID_MOUSE(Serial.print("mouse "));
        DEBUG_HID_MOUSE(Serial.print(hidMouseDX)); DEBUG_HID_MOUSE(Serial.print(" "));
        DEBUG_HID_MOUSE(Serial.print(hidMouseDY)); DEBUG_HID_MOUSE(Serial.print(" "));
        DEBUG_HID_MOUSE(Serial.println(hidMouseDZ));
    }
    if (opt_hid_scroll_mode > 0 && hidScrollDY != 0) {
        if (opt_hid_mouse_invert_y == 1) hidMouseDY = -hidMouseDY;
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            Mouse.scroll(hidScrollDY);
        #endif /* KG_HOSTIF_USB_HID */
        #if KG_HOSTIF & KG_HOSTIF_BT2_HID
            BTMouse.scroll(hidScrollDY, 0);
        #endif /* KG_HOSTIF_BT2_HID */
        #if KG_HOSTIF & KG_HOSTIF_R400_HID
            RX400.scroll(hidScrollDY);
        #endif /* KG_HOSTIF_R400_HID */
        DEBUG_HID_MOUSE(Serial.print("scroll "));
        DEBUG_HID_MOUSE(Serial.println(hidScrollDY));
    }
}

/**
 * @brief Turn on mouse for cursor movement or scrolling
 * @param[in] mode Which operational mode to turn on
 * @see MOUSE_ACTION_MOVE
 * @see MOUSE_ACTION_SCROLL
 */
void mouse_on(uint8_t mode) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseon "));
    DEBUG_TOUCHSET(Serial.println(mode));
    #if KG_HID & KG_HID_MOUSE
        if (mode == MOUSE_ACTION_MOVE) {
            opt_hid_mouse_mode = MOUSE_MODE_TILT_POSITION;
        } else if (mode == MOUSE_ACTION_SCROLL) {
            opt_hid_scroll_mode = SCROLL_MODE_TILT_POSITION;
        }
        #ifdef ENABLE_ACCELEROMETER
            if (!activeAccelerometer) enable_motion_accelerometer();
        #endif
        #ifdef ENABLE_GYROSCOPE
            if (!activeGyroscope) enable_motion_gyroscope();
        #endif
        #ifdef ENABLE_ACCELGYRO
            if (!activeAccelGyro) enable_motion_accelgyro();
        #endif
    #endif
}

/**
 * @brief Turn off mouse to end cursor movement or scrolling
 * @param[in] mode Which operational mode to turn off
 * @see MOUSE_ACTION_MOVE
 * @see MOUSE_ACTION_SCROLL
 */
void mouse_off(uint8_t mode) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseoff "));
    DEBUG_TOUCHSET(Serial.println(mode));
    #if KG_HID & KG_HID_MOUSE
        if (mode == MOUSE_ACTION_MOVE) {
            opt_hid_mouse_mode = MOUSE_MODE_OFF;
        } else if (mode == MOUSE_ACTION_SCROLL) {
            opt_hid_scroll_mode = SCROLL_MODE_OFF;
        }
        #ifdef ENABLE_ACCELEROMETER
            if (activeAccelerometer) disable_motion_accelerometer();
        #endif
        #ifdef ENABLE_GYROSCOPE
            if (activeGyroscope) disable_motion_gyroscope();
        #endif
        #ifdef ENABLE_ACCELGYRO
            if (activeAccelGyro) disable_motion_accelgyro();
        #endif
    #endif
}

/**
 * @brief Send button-press (down) HID report
 * @param[in] button Mouse button bitmask
 * @see MOUSE_LEFT
 * @see MOUSE_RIGHT
 * @see MOUSE_MIDDLE
 */
void mouse_down(uint8_t button) {
    DEBUG_TOUCHSET(Serial.print("touchset hidMouseDown "));
    DEBUG_TOUCHSET(Serial.println(button));
    #if KG_HID & KG_HID_MOUSE
        hidMouseDown = hidMouseDown | button;
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            if (interfaceUSBHIDReady) {
                Mouse.set_buttons((hidMouseDown & 1) > 0 ? 1 : 0, (hidMouseDown & 2) > 0 ? 1 : 0, (hidMouseDown & 4) > 0 ? 1 : 0);
            }
        #endif /* ENABLE_USB */
    #endif
}

/**
 * @brief Send button-release (up) HID report
 * @param[in] button Mouse button bitmask
 * @see MOUSE_LEFT
 * @see MOUSE_RIGHT
 * @see MOUSE_MIDDLE
 */
void mouse_up(uint8_t button) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseup "));
    DEBUG_TOUCHSET(Serial.println(button));
    #if KG_HID & KG_HID_MOUSE
        if ((hidMouseDown & button) > 0) {
            hidMouseDown -= button;
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                if (interfaceUSBHIDReady) {
                    Mouse.set_buttons((hidMouseDown & 1) > 0 ? 1 : 0, (hidMouseDown & 2) > 0 ? 1 : 0, (hidMouseDown & 4) > 0 ? 1 : 0);
                }
            #endif /* ENABLE_USB */
        }
    #endif
}

/**
 * @brief Send button-click (down-up) HID reports
 * @param[in] button Mouse button bitmask
 * @see MOUSE_LEFT
 * @see MOUSE_RIGHT
 * @see MOUSE_MIDDLE
 */
void mouse_click(uint8_t button) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseclick "));
    DEBUG_TOUCHSET(Serial.println(button));
    mouse_down(button);
    delay(5);
    mouse_up(button);
}

#endif // _SUPPORT_HID_MOUSE_H_