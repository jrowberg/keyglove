// Keyglove controller source code - General feedback support declarations
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
 * @file support_feedback.h
 * @brief General feedback support declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file a conditional way to include specific types of functional support
 * files for a general category based on Keyglove configuration settings,
 * without needing to have that conditional logic in every implementation file
 * that might need to include the functional support in question. This header
 * deals specifically with feedback support.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_FEEDBACK_H_
#define _SUPPORT_FEEDBACK_H_

#if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
    #include "support_feedback_blink.h"
#endif
#if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
    #include "support_feedback_piezo.h"
#endif
#if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
    #include "support_feedback_vibrate.h"
#endif
#if (KG_FEEDBACK & KG_FEEDBACK_RGB)
    #include "support_feedback_rgb.h"
#endif

int16_t mod(int16_t x, int16_t m);
uint8_t square_wave(int32_t x, int32_t period, uint8_t duty);
int16_t triangle_wave(int32_t x, int32_t period, int32_t amplitude);

#endif // _SUPPORT_FEEDBACK_H_
