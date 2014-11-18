// Keyglove controller source code - General touch support declarations
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
 * @file support_touch.h
 * @brief General touch support declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _SUPPORT_TOUCH_H_
#define _SUPPORT_TOUCH_H_

void touch_set_mode(uint8_t mode);

extern uint8_t touchMode;
extern uint8_t touchTick;
extern uint8_t touchOn;

extern uint32_t touchTime;

extern uint8_t touches_now[KG_BASE_COMBINATION_BYTES];
extern uint8_t touches_verify[KG_BASE_COMBINATION_BYTES];
extern uint8_t touches_active[KG_BASE_COMBINATION_BYTES];

void setup_touch();
void update_touch();
uint8_t touch_check_mode(uint8_t mode, uint8_t pos);
void touch_set_mode(uint8_t mode);
void touch_push_mode(uint8_t mode);
void touch_pop_mode();
void touch_toggle_mode(uint8_t mode);

#endif // _SUPPORT_TOUCH_H_