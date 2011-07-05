/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
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
 *  Main source file for the KeyboardMouse demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */

#include "Keyglove.h"

/** Contains the current baud rate and other settings of the virtual serial port. While USB virtual serial does not use
 *  the physical USART and thus does not use these settings, they must still be retained and returned to the host
 *  upon request or the host will assume the device is non-functional.
 *
 *  These values are set by the host via a class-specific request, however they are not required to be used accurately.
 *  It is possible to completely ignore these value or use other settings as the host is completely unaware of the physical
 *  serial link characteristics and instead sends and receives data in endpoint streams.
 */
static CDC_LineEncoding_t LineEncodingData = { .BaudRateBPS = 0,
                                           .CharFormat  = CDC_LINEENCODING_OneStopBit,
                                           .ParityType  = CDC_PARITY_None,
                                           .DataBits    = 8                            };

/** Static buffer to hold the last received report from the host, so that it can be echoed back in the next sent report */
static uint8_t LastReceived[GENERIC_REPORT_SIZE];

long counter = 0;

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    SetupHardware();

    LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
    sei();

    for (;;)
    {
        CDC_Task();
        HID_Task();
        USB_USBTask();
        counter++;
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    //clock_prescale_set(clock_div_1);
        CPU_PRESCALE(CPU_8MHz); // 3.3v AVR shouldn't run faster than 8MHz

    /* Hardware Initialization */
    LEDs_Init();
    USB_Init();
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
                uint8_t GenericData[GENERIC_REPORT_SIZE];
                CreateGenericHIDReport(GenericData);

                Endpoint_ClearSETUP();

                /* Write the report data to the control endpoint */
                Endpoint_Write_Control_Stream_LE(&GenericData, sizeof(GenericData));
                Endpoint_ClearOUT();
            }

            break;
        case HID_REQ_SetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                uint8_t GenericData[GENERIC_REPORT_SIZE];

                Endpoint_ClearSETUP();

                /* Read the report data from the control endpoint */
                Endpoint_Read_Control_Stream_LE(&GenericData, sizeof(GenericData));
                Endpoint_ClearIN();

                ProcessGenericHIDReport(GenericData);
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

/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
    char*       ReportString    = NULL;
    static bool ActionSent      = false;
    
    /* Device must be connected and configured for the task to run */
    if (USB_DeviceState != DEVICE_STATE_Configured)
      return;

    if (counter % 10000 == 0)
    {
        ReportString = "Test serial output\r\n";
    }
    else
    {
        ActionSent = false;
        if (counter % 10000 == 1000)
        {
            PORTD &= ~(1 << 6);
        }
    }

    /* Flag management - Only allow one string to be sent per action */
    if ((ReportString != NULL) && (ActionSent == false) && LineEncodingData.BaudRateBPS)
    {
        PORTD |= (1 << 6);
        ActionSent = true;

        /* Select the Serial Tx Endpoint */
        Endpoint_SelectEndpoint(CDC_TX_EPNUM);

        /* Write the String to the Endpoint */
        Endpoint_Write_Stream_LE(ReportString, strlen(ReportString), NULL);

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

    /* Select the Serial Rx Endpoint */
    Endpoint_SelectEndpoint(CDC_RX_EPNUM);

    /* Throw away any received data from the host */
    if (Endpoint_IsOUTReceived())
      Endpoint_ClearOUT();

}

/** Function to process the lest received report from the host.
 *
 *  \param[in] DataArray  Pointer to a buffer where the last report data is stored
 */
void ProcessGenericHIDReport(uint8_t* DataArray)
{
    /*
        This is where you need to process the reports being sent from the host to the device.
        DataArray is an array holding the last report from the host. This function is called
        each time the host has sent a report to the device.
    */

    for (uint8_t i = 0; i < GENERIC_REPORT_SIZE; i++)
      LastReceived[i] = DataArray[i];
}

/** Function to create the next report to send back to the host at the next reporting interval.
 *
 *  \param[out] DataArray  Pointer to a buffer where the next report data should be stored
 */
void CreateGenericHIDReport(uint8_t* DataArray)
{
    /*
        This is where you need to create reports to be sent to the host from the device. This
        function is called each time the host is ready to accept a new report. DataArray is
        an array to hold the report to the host.
    */

    for (uint8_t i = 0; i < GENERIC_REPORT_SIZE; i++)
      DataArray[i] = LastReceived[i];
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
            /* Create a temporary buffer to hold the read in report from the host */
            uint8_t GenericData[GENERIC_REPORT_SIZE];

            /* Read Generic Report Data */
            Endpoint_Read_Stream_LE(&GenericData, sizeof(GenericData), NULL);

            /* Process Generic Report Data */
            ProcessGenericHIDReport(GenericData);
        }

        /* Finalize the stream transfer to send the last packet */
        Endpoint_ClearOUT();
    }

    Endpoint_SelectEndpoint(GENERIC_IN_EPNUM);

    /* Check to see if the host is ready to accept another packet */
    if (Endpoint_IsINReady())
    {
        /* Create a temporary buffer to hold the report to send to the host */
        uint8_t GenericData[GENERIC_REPORT_SIZE];

        /* Create Generic Report Data */
        CreateGenericHIDReport(GenericData);

        /* Write Generic Report Data */
        Endpoint_Write_Stream_LE(&GenericData, sizeof(GenericData), NULL);

        /* Finalize the stream transfer to send the last packet */
        Endpoint_ClearIN();
    }
}

