/*

  Keyglove controller source code - LUFA-powered main entry point
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

/** \file
 *
 *  Main source file for the Keyglove LUFA/AVR controller code.
 */

extern "C" {
    #include "Keyglove.h"

    /** Buffer to hold the HID reports to and from the host. */
    // Remember that these are labeled from the viewpoint of the
    // host device, so that IN is coming from the device to the host,
    // and OUT is going to the device from the host. It's backwards
    // from how you might imagine it.

    uint8_t HIDReportInData[GENERIC_REPORT_IN_SIZE];
    uint8_t HIDReportOutData[GENERIC_REPORT_OUT_SIZE];

    /** Contains the current baud rate and other settings of the virtual serial port. While USB virtual serial does not use
     *  the physical USART and thus does not use these settings, they must still be retained and returned to the host
     *  upon request or the host will assume the device is non-functional.
     *
     *  These values are set by the host via a class-specific request, however they are not required to be used accurately.
     *  It is possible to completely ignore these value or use other settings as the host is completely unaware of the physical
     *  serial link characteristics and instead sends and receives data in endpoint streams.
     */
    CDC_LineEncoding_t LineEncodingData = {
                                               BaudRateBPS: 0,
                                               CharFormat:  CDC_LINEENCODING_OneStopBit,
                                               ParityType:  CDC_PARITY_None,
                                               DataBits:    8                            };
    
    /** Circular buffer to hold data from the host before it is processed on the AVR. */
    RingBuffer_t HostRXSerial_Buffer;
    
    /** Underlying data buffer for \ref HostRXSerial_Buffer, where the stored bytes are located. */
    uint8_t      HostRXSerial_Buffer_Data[256];
    
    /** Circular buffer to hold data from the AVR before it is sent to the host. */
    RingBuffer_t HostTXSerial_Buffer;
    
    /** Underlying data buffer for \ref HostTXSerial_Buffer, where the stored bytes are located. */
    uint8_t      HostTXSerial_Buffer_Data[256];

    bool CDCTaskRun = false;
    bool HIDTaskRun = false;
}

// include Arduino release code
#include "../keyglove/keyglove.pde"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    RingBuffer_InitBuffer(&HostRXSerial_Buffer, HostRXSerial_Buffer_Data, sizeof(HostRXSerial_Buffer_Data));
    RingBuffer_InitBuffer(&HostTXSerial_Buffer, HostTXSerial_Buffer_Data, sizeof(HostTXSerial_Buffer_Data));

    SetupHardware();
    //LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
    //sei(); // called by init()
    init();
    setup();

    for (;;)
    {
        /*if (tickCounter % 10000 == 1000)
        {
            // virtual serial demo
            // (sends 'Tick [n]' where [n] is between 0 and 9 sequentially, looping back after 9)
            RingBuffer_Insert(&HostTXSerial_Buffer, 'T');
            RingBuffer_Insert(&HostTXSerial_Buffer, 'i');
            RingBuffer_Insert(&HostTXSerial_Buffer, 'c');
            RingBuffer_Insert(&HostTXSerial_Buffer, 'k');
            RingBuffer_Insert(&HostTXSerial_Buffer, ' ');
            RingBuffer_Insert(&HostTXSerial_Buffer, 48 + (((tickCounter - 1000) / 10000) % 10));
            RingBuffer_Insert(&HostTXSerial_Buffer, 10); // \r
            RingBuffer_Insert(&HostTXSerial_Buffer, 13); // \n
        }
        {
            // joystick demo start
            // (moves left axis down/right, right axis up/left, and presses a button)
            HIDReportInData[0] = HID_REPORTID_JoystickReport;
            HIDReportInData[1] = 1; // buttons 1
            HIDReportInData[2] = 0; // buttons 2
            HIDReportInData[3] = 5; // left x axis
            HIDReportInData[4] = 5; // left y axis
            HIDReportInData[5] = -5; // right x axis
            HIDReportInData[6] = -5; // right y exis
            HIDReportInData[7] = 0; // ] - unused
            HIDReportInData[8] = 0; // ]
        }
        else if (tickCounter % 10000 == 8000)
        {
            // joystick demo end
            // (releases previous joystick actions)
            HIDReportInData[0] = HID_REPORTID_JoystickReport;
            HIDReportInData[1] = 0; // buttons 1
            HIDReportInData[2] = 0; // buttons 2
            HIDReportInData[3] = 0; // left x axis
            HIDReportInData[4] = 0; // left y axis
            HIDReportInData[5] = 0; // right x axis
            HIDReportInData[6] = 0; // right y exis
            HIDReportInData[7] = 0; // ] - unused
            HIDReportInData[8] = 0; // ]
        }*/

        loop();
        if (!CDCTaskRun) CDC_Task(); else CDCTaskRun = false;
        if (!HIDTaskRun) HID_Task(); else HIDTaskRun = false;
        USB_USBTask();
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
    /* disable JTAG to allow corresponding pins to be used */
    // thank you http://code.google.com/p/micropendous/wiki/Micropendous4
    #if ((defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
            defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
            defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
            defined(__AVR_ATmega32U6__)))
        // note the JTD bit must be written twice within 4 clock cycles to disable JTAG
        // you must also set the IVSEL bit at the same time, which requires IVCE to be set first
        // port pull-up resistors are enabled - PUD(Pull Up Disable) = 0
        MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD);
        MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);
    #endif

    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();



	//cli();
	//CLKPR = 0x80;
	//CLKPR = CPU_PRESCALER;
	// timer 0, fast pwm mode
	TCCR0A = (1<<WGM01) | (1<<WGM00);
	TCCR0B = (1<<CS01) | (1<<CS00);		// div 64 prescaler
//	sbi(TIMSK0, TOIE0);
	// timer 1, 8 bit phase correct pwm
	TCCR1A = (1<<WGM10);
	TCCR1B = (1<<CS11);			// div 8 prescaler
#if defined(__AVR_ATmega32U4__)
	// timer 3, 8 bit phase correct pwm
	TCCR3A = (1<<WGM30);
	TCCR3B = (1<<CS31);			// div 8 prescaler
	// timer 4, 8 bit phase correct pwm
	TCCR4A = (1<<PWM4A);
	TCCR4B = (1<<CS42);			// div 8 prescaler
	TCCR4C = (1<<PWM4D);
	TCCR4D = (1<<WGM40);			// phase correct pwm
	TCCR4E = 0;
	// ADC
//	ADCSRA = (1<<ADEN) | (ADC_PRESCALER + ADC_PRESCALE_ADJUST);
//	ADCSRB = DEFAULT_ADCSRB;
	DIDR0 = 0;
	DIDR2 = 0;
#endif
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
	// timer 2, 8 bit phase correct pwm
	TCCR2A = (1<<WGM20);
	TCCR2B = (1<<CS21);			// div 8 prescaler
	// timer 3, 8 bit phase correct pwm
	TCCR3A = (1<<WGM30);
	TCCR3B = (1<<CS31);			// div 8 prescaler
	// ADC
//	ADCSRA = (1<<ADEN) | (ADC_PRESCALER + ADC_PRESCALE_ADJUST);
//	ADCSRB = DEFAULT_ADCSRB;
	DIDR0 = 0;
#endif

    /* Disable clock division */
    //clock_prescale_set(clock_div_1);
        CPU_PRESCALE(CPU_8MHz); // 3.3v AVR shouldn't run faster than 8MHz

    /* Hardware Initialization */
    LEDs_Init();
    USB_Init();
	sei();
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
    /* Indicate USB enumerating */
    LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs and stops the USB management task.
 */
void EVENT_USB_Device_Disconnect(void)
{
    /* Indicate USB not ready */
    LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host sets the current configuration
 *  of the USB device after enumeration, and configures the keyboard and mouse device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    //* Setup CDC Data Endpoints */
    ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
                                                CDC_NOTIFICATION_EPSIZE, ENDPOINT_BANK_SINGLE);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_IN,
                                                CDC_TXRX_EPSIZE, ENDPOINT_BANK_SINGLE);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT,
                                                CDC_TXRX_EPSIZE, ENDPOINT_BANK_SINGLE);

    /* Reset line encoding baud rate so that the host knows to send new values */
    LineEncodingData.BaudRateBPS = 0;

    /* Setup HID Report Endpoints */
    ConfigSuccess &= Endpoint_ConfigureEndpoint(GENERIC_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
                                                GENERIC_EPSIZE, ENDPOINT_BANK_SINGLE);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(GENERIC_OUT_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_OUT,
                                                GENERIC_EPSIZE, ENDPOINT_BANK_SINGLE);

    /* Indicate endpoint configuration success or failure */
    LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
    switch (USB_ControlRequest.bRequest)
    {
        case HID_REQ_GetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                //CreateHIDReport(HIDReportInData);

                Endpoint_ClearSETUP();

                /* Write the report data to the control endpoint */
                Endpoint_Write_Control_Stream_LE(&HIDReportInData, sizeof(HIDReportInData));

                /* Clean out report data after sending */
                memset(&HIDReportInData, 0, sizeof(HIDReportInData));

                /* Finalize the stream transfer to send the last packet */
                Endpoint_ClearOUT();
            }

            break;
        case HID_REQ_SetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                /* Read the report data from the control endpoint */
                Endpoint_Read_Control_Stream_LE(&HIDReportOutData, sizeof(HIDReportOutData));
                Endpoint_ClearIN();

                ProcessHIDReport(HIDReportOutData);
            }

            break;
        case CDC_REQ_GetLineEncoding:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                /* Write the line coding data to the control endpoint */
                Endpoint_Write_Control_Stream_LE(&LineEncodingData, sizeof(CDC_LineEncoding_t));
                Endpoint_ClearOUT();
            }

            break;
        case CDC_REQ_SetLineEncoding:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                /* Read the line coding data in from the host into the global struct */
                Endpoint_Read_Control_Stream_LE(&LineEncodingData, sizeof(CDC_LineEncoding_t));
                Endpoint_ClearIN();
            }

            break;
        case CDC_REQ_SetControlLineState:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                Endpoint_ClearStatusStage();

                /* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
                         lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
                         CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
                */
            }

            break;
    }
}

/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
    /* Device must be connected and configured for the task to run */
    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    uint16_t BufferCount;

    /* Send data packet if anything is waiting */
    BufferCount = RingBuffer_GetCount(&HostTXSerial_Buffer);
    if (BufferCount && LineEncodingData.BaudRateBPS)
    {
        /* Select the Serial TX Endpoint */
        Endpoint_SelectEndpoint(CDC_TX_EPNUM);

        /* Write the byte(s) to the Endpoint */
        for (int i = 0; i < CDC_TXRX_EPSIZE && BufferCount; i++, BufferCount--)
        {
            UEDATX = RingBuffer_Remove(&HostTXSerial_Buffer); // write one byte
        }

        /* Remember if the packet to send completely fills the endpoint */
        bool IsFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);

        /* Finalize the stream transfer to send the last packet */
        Endpoint_ClearIN();

        /* If the last packet filled the endpoint, send an empty packet to release the buffer on
         * the receiver (otherwise all data will be cached until a non-full packet is received) */
        if (IsFull)
        {
            /* Wait until the endpoint is ready for another packet */
            Endpoint_WaitUntilReady();

            /* Send an empty packet to ensure that the host does not buffer data sent to it */
            Endpoint_ClearIN();
        }
    }

    /* Select the Serial RX Endpoint */
    Endpoint_SelectEndpoint(CDC_RX_EPNUM);

    /* Store any received data from the host */
    if (Endpoint_IsOUTReceived())
    {
        BufferCount = Endpoint_BytesInEndpoint();
        for (uint8_t i = 0; i < BufferCount; i++)
        {
            uint8_t b = UEDATX;
            RingBuffer_Insert(&HostRXSerial_Buffer, b); // read byte from endpoint
        }

        /* Finalize the stream transfer to receive the last packet */
        Endpoint_ClearOUT();
    }

}

void HID_Task(void)
{
    /* Device must be connected and configured for the task to run */
    if (USB_DeviceState != DEVICE_STATE_Configured)
      return;

    Endpoint_SelectEndpoint(GENERIC_OUT_EPNUM);

    /* Check to see if a packet has been sent from the host */
    if (Endpoint_IsOUTReceived())
    {
        /* Check to see if the packet contains data */
        if (Endpoint_IsReadWriteAllowed())
        {
            /* Read Generic Report Data */
            Endpoint_Read_Stream_LE(&HIDReportOutData, sizeof(HIDReportOutData), NULL);

            /* Process Generic Report Data */
            ProcessHIDReport(HIDReportOutData);
        }

        /* Finalize the stream transfer to send the last packet */
        Endpoint_ClearOUT();
    }

    Endpoint_SelectEndpoint(GENERIC_IN_EPNUM);

    /* Check to see if the host is ready to accept another packet */
    if (Endpoint_IsINReady())
    {
        /* Write Generic Report Data */
        Endpoint_Write_Stream_LE(&HIDReportInData, sizeof(HIDReportInData), NULL);
        
        /* Clean out report data after sending */
        memset(&HIDReportInData, 0, sizeof(HIDReportInData));

        /* Finalize the stream transfer to send the last packet */
        Endpoint_ClearIN();
    }
}

/** Function to process the lest received report from the host.
 *
 *  \param[in] DataArray  Pointer to a buffer where the last report data is stored
 */
void ProcessHIDReport(uint8_t* DataArray)
{
    /*
        This is where you need to process the reports being sent from the host to the device.
        DataArray is an array holding the last report from the host. This function is called
        each time the host has sent a report to the device.
    */

}

/** Processes a given Keyboard LED report from the host, and sets the board LEDs to match. Since the Keyboard
 *  LED report can be sent through either the control endpoint (via a HID SetReport request) or the HID OUT
 *  endpoint, the processing code is placed here to avoid duplicating it and potentially having different
 *  behaviour depending on the method used to sent it.
 */
void Keyboard_ProcessLEDReport(const uint8_t LEDStatus)
{
    uint8_t LEDMask = LEDS_LED2;

    if (LEDStatus & HID_KEYBOARD_LED_NUMLOCK)
      LEDMask |= LEDS_LED2;

    if (LEDStatus & HID_KEYBOARD_LED_CAPSLOCK)
      LEDMask |= LEDS_LED3;

    if (LEDStatus & HID_KEYBOARD_LED_SCROLLLOCK)
      LEDMask |= LEDS_LED4;

    /* Set the status LEDs to the current Keyboard LED status */
    LEDs_SetAllLEDs(LEDMask);
}