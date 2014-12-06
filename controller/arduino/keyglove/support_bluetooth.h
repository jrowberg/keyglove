// Keyglove controller source code - General Bluetooth support declaration file
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
 * @file support_bluetooth.h
 * @brief General Bluetooth support declaration file
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file a conditional way to include specific types of functional support
 * files for a general category based on Keyglove configuration settings,
 * without needing to have that conditional logic in every implementation file
 * that might need to include the functional support in question. This header
 * deals specifically with Bluetooth functionality.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_BLUETOOTH_H_
#define _SUPPORT_BLUETOOTH_H_

#if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
    #include "support_bluetooth2_iwrap.h"
#endif

#endif // _SUPPORT_BLUETOOTH_H_
