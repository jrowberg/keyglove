// Keyglove controller source code - General feedback support implementations
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
 * @file support_feedback.cpp
 * @brief General feedback support implementations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file provides the structural framework for feedback implementations in
 * the overall Keyglove architecture.
 *
 * Normally it is not necessary to edit this file.
 */

#include "keyglove.h"
#include "support_feedback.h"

// for people who don't want to waste lots of hours: "%" != "mod" ...

/**
 * @brief Modulo function that correctly handles negative values
 * @param[in] x Input value
 * @param[in] m Modulo constant
 * @return Calculated result on range [0, m-1]
 */
int16_t mod(int16_t x, int16_t m) {
	return ((x) < 0 ? (((x) % m) + m) % m : (x) % m);
}

/**
 * @brief Simple 1/0 square wave generator function
 * @param[in] x Input value
 * @param[in] period Period of full 1/0 cycle
 * @param[in] duty Duty cycle between 0 - 100 (%)
 * @return Output level (1=high, 0=low)
 */
uint8_t square_wave(int32_t x, int32_t period, uint8_t duty) {
	return (mod(x, period) < (period * duty / 100)) ? 1 : 0;
}

/**
 * @brief Simple triangle wave generator function
 * @param[in] x Input value
 * @param[in] period Period of full cycle
 * @param[in] amplitude Full amplitude (100 will give range = [-50, +50])
 * @return Output value
 *
 * This wave is aligned to exhibit the following pattern:
 * - (0,         0)
 * - (period/4,  amplitude/2)
 * - (period/2,  0)
 * - (3period/4, -amplitude/2)
 * - (period,    0)
 *
 * Or in graphscii:
 *
 *    |  A
 *    | / \
 * ___|/___\_____ (period, 0)
 *    |     \   /
 *    |      \ /
 *    |       V
 *
 * It can be shifted in either the X or Y dimension if necessary, but this form
 * makes it easily useful for a number of different LED fading routines.
 */
int16_t triangle_wave(int32_t x, int32_t period, int32_t amplitude) {
    return abs((amplitude * mod(x - (period >> 2), period) / (period >> 1)) - amplitude) - (amplitude >> 1);
}
