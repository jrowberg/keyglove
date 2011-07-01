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

/** Global structure to hold the current keyboard interface HID report, for transmission to the host */
static USB_KeyboardReport_Data_t KeyboardReportData;

/** Global structure to hold the current mouse interface HID report, for transmission to the host */
static USB_MouseReport_Data_t MouseReportData;

/** Global structure to hold the current joystick interface HID report, for transmission to the host */
static USB_JoystickReport_Data_t JoystickReportData;

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
		Keyboard_HID_Task();
		Mouse_HID_Task();
		Joystick_HID_Task();
		CDC_Task();
		USB_USBTask();
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

	/* Setup Keyboard HID Report Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
	                                            HID_EPSIZE, ENDPOINT_BANK_SINGLE);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_OUT,
	                                            HID_EPSIZE, ENDPOINT_BANK_SINGLE);

	/* Setup Mouse HID Report Endpoint */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MOUSE_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
	                                            HID_EPSIZE, ENDPOINT_BANK_SINGLE);

	/* Setup Joystick HID Report Endpoint */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
	                                            HID_EPSIZE, ENDPOINT_BANK_SINGLE);

	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncodingData.BaudRateBPS = 0;

	/* Indicate endpoint configuration success or failure */
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
	uint8_t* ReportData;
	uint8_t  ReportSize;

	/* Handle HID Class specific requests */
	switch (USB_ControlRequest.bRequest)
	{
		case HID_REQ_GetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Determine if it is the mouse or the keyboard data that is being requested */
				if (USB_ControlRequest.wIndex == 2)
				{
					ReportData = (uint8_t*)&KeyboardReportData;
					ReportSize = sizeof(KeyboardReportData);
				}
				else if (USB_ControlRequest.wIndex == 3)
				{
					ReportData = (uint8_t*)&MouseReportData;
					ReportSize = sizeof(MouseReportData);
				}
				else if (USB_ControlRequest.wIndex == 4)
				{
					ReportData = (uint8_t*)&JoystickReportData;
					ReportSize = sizeof(JoystickReportData);
                                }
                                else
                                {
                                        break; // wtf, hmm
                                }

				/* Write the report data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(ReportData, ReportSize);
				Endpoint_ClearOUT();

				/* Clear the report data afterwards */
				memset(ReportData, 0, ReportSize);
			}

			break;
		case HID_REQ_SetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Wait until the LED report has been sent by the host */
				while (!(Endpoint_IsOUTReceived()))
				{
					if (USB_DeviceState == DEVICE_STATE_Unattached)
					  return;
				}

				/* Read in the LED report from the host */
				uint8_t LEDStatus = Endpoint_Read_8();

				Endpoint_ClearOUT();
				Endpoint_ClearStatusStage();

				/* Process the incoming LED report */
				Keyboard_ProcessLEDReport(LEDStatus);
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

/** Keyboard task. This generates the next keyboard HID report for the host, and transmits it via the
 *  keyboard IN endpoint when the host is ready for more data. Additionally, it processes host LED status
 *  reports sent to the device via the keyboard OUT reporting endpoint.
 */
void Keyboard_HID_Task(void)
{
	//uint8_t JoyStatus_LCL = Joystick_GetStatus();

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* Check if board button is not pressed, if so mouse mode enabled */
	if (false) //(Buttons_GetStatus() & BUTTONS_BUTTON1))
	{
		/* Make sent key uppercase by indicating that the left shift key is pressed */
		KeyboardReportData.Modifier = HID_KEYBOARD_MODIFER_LEFTSHIFT;
		KeyboardReportData.KeyCode[0] = HID_KEYBOARD_SC_A;
	}

	/* Select the Keyboard Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_IN_EPNUM);

	/* Check if Keyboard Endpoint Ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write Keyboard Report Data */
		Endpoint_Write_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* Clear the report data afterwards */
		memset(&KeyboardReportData, 0, sizeof(KeyboardReportData));
	}

	/* Select the Keyboard LED Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_OUT_EPNUM);

	/* Check if Keyboard LED Endpoint Ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Read in and process the LED report from the host */
		Keyboard_ProcessLEDReport(Endpoint_Read_8());

		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}

/** Mouse task. This generates the next mouse HID report for the host, and transmits it via the
 *  mouse IN endpoint when the host is ready for more data.
 */
void Mouse_HID_Task(void)
{
	//uint8_t JoyStatus_LCL = Joystick_GetStatus();

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* Check if board button is pressed, if so mouse mode enabled */
	if (false) //Buttons_GetStatus() & BUTTONS_BUTTON1)
	{
                MouseReportData.Y =  1;
                //MouseReportData.Button |= (1 << 0);
	}

	/* Select the Mouse Report Endpoint */
	Endpoint_SelectEndpoint(MOUSE_IN_EPNUM);

	/* Check if Mouse Endpoint Ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write Mouse Report Data */
		Endpoint_Write_Stream_LE(&MouseReportData, sizeof(MouseReportData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* Clear the report data afterwards */
		memset(&MouseReportData, 0, sizeof(MouseReportData));
	}
}

/** Joystick task to manage joystick HID report generation and transmission to the host. */
void Joystick_HID_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* Select the Joystick Report Endpoint */
	Endpoint_SelectEndpoint(JOYSTICK_EPNUM);

	/* Check to see if the host is ready for another packet */
	if (Endpoint_IsINReady())
	{
		/* Write Joystick Report Data */
		Endpoint_Write_Stream_LE(&JoystickReportData, sizeof(JoystickReportData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* Clear the report data afterwards */
		memset(&JoystickReportData, 0, sizeof(JoystickReportData));
	}
}

/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
	char*       ReportString    = NULL;
	static bool ActionSent      = false;
	
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	PORTD |= (1 << 6);
	_delay_ms(100);

        ReportString = "Test serial output\r\n";
        //ActionSent = false;

	/* Flag management - Only allow one string to be sent per action */
	if ((ReportString != NULL) && (ActionSent == false) && LineEncodingData.BaudRateBPS)
	{
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

	PORTD &= ~(1 << 6);
	_delay_ms(250);
}

