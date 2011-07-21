/*

  Keyglove controller source code - LUFA-powered main entry point header file
  7/15/2011 by Jeff Rowberg <jeff@rowberg.net>

*/

/*
  LUFA Library code:
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Copyright 2010  Denver Gingerich (denver [at] ossguy [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef _KEYGLOVE_H_
#define _KEYGLOVE_H_

    /* Includes: */
        #include <avr/io.h>
        #include <avr/wdt.h>
        #include <avr/power.h>
        #include <avr/interrupt.h>
        #include <stdbool.h>
        #include <string.h>

        #include "Descriptors.h"

        // yay c++! bleh
        // http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=308100
        #undef FDEV_SETUP_STREAM
        #define FDEV_SETUP_STREAM(p, g, f) { 0, 0, f, 0, 0, p, g, 0 }

        #include <LUFA/Version.h>
        #include <LUFA/Drivers/USB/USB.h>
        #include <LUFA/Drivers/Board/LEDs.h>
        #include <LUFA/Drivers/Board/Buttons.h>
        #include <LUFA/Drivers/Peripheral/Serial.h>
        #include <LUFA/Drivers/Peripheral/TWI.h>
        #include <LUFA/Drivers/Misc/RingBuffer.h>

    /* Macros: */
        /** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
        #define LEDMASK_USB_NOTREADY        (LEDS_LED1 | LEDS_LED2)

        /** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
        #define LEDMASK_USB_ENUMERATING     (LEDS_LED2 | LEDS_LED3)

        /** LED mask for the library LED driver, to indicate that the USB interface is ready. */
        #define LEDMASK_USB_READY           LEDS_LED2

        /** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
        #define LEDMASK_USB_ERROR           LEDS_LED1

        /** CPU prescaler values */
        #define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
        #define CPU_16MHz       0x00
        #define CPU_8MHz        0x01
        #define CPU_4MHz        0x02
        #define CPU_2MHz        0x03
        #define CPU_1MHz        0x04
        #define CPU_500kHz      0x05
        #define CPU_250kHz      0x06
        #define CPU_125kHz      0x07
        #define CPU_62kHz       0x08

    /* Type Defines: */

    /* Function Prototypes: */
        void EVENT_USB_Device_Connect(void);
        void EVENT_USB_Device_Disconnect(void);
        void EVENT_USB_Device_ConfigurationChanged(void);
        void EVENT_USB_Device_ControlRequest(void);
        void EVENT_USB_Device_StartOfFrame(void);

        void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);

        void SetupHardware(void);
        void CDC_Task(void);
        void HID_Task(void);
        void ProcessHIDReport(uint8_t* DataArray);

        /** Buffer to hold the HID reports to and from the host. */
        extern uint8_t HIDReportInData[GENERIC_REPORT_IN_SIZE];
        extern uint8_t HIDReportOutData[GENERIC_REPORT_OUT_SIZE];

        /** Circular buffer to hold data from the host before it is processed on the AVR. */
        extern RingBuffer_t HostRXSerial_Buffer;

        /** Underlying data buffer for \ref HostRXSerial_Buffer, where the stored bytes are located. */
        extern uint8_t      HostRXSerial_Buffer_Data[256];

        /** Circular buffer to hold data from the AVR before it is sent to the host. */
        extern RingBuffer_t HostTXSerial_Buffer;

        /** Underlying data buffer for \ref HostTXSerial_Buffer, where the stored bytes are located. */
        extern uint8_t      HostTXSerial_Buffer_Data[256];

#endif // _KEYGLOVE_H_

