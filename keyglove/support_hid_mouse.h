// Keyglove controller source code - HID mouse helper function declarations
// 2014-11-07 by Jeff Rowberg <jeff@rowberg.net>

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
 * @brief HID mouse helper function declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
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

void setup_hid_mouse();
void update_hid_mouse();

void mouse_on(uint8_t mode);
void mouse_off(uint8_t mode);
void mouse_down(uint8_t button);
void mouse_up(uint8_t button);
void mouse_click(uint8_t button);

#endif // _SUPPORT_HID_MOUSE_H_