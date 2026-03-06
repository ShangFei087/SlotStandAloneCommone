/*******************************************************************************
* Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*
* Description: Human Interface Device Class (Keyboard) over USB
* $Id: descriptors.h 16373 2015-01-16 21:01:36Z jeremy.brodt $
*
********************************************************************************
*/

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <stdint.h>
#include "usb.h"
#include "qs_cfg.h"

#define BLUK_DATA_MAX_PACKET_SIZE       64   /* Endpoint IN & OUT Packet size */

#define USB_BLUK_CONFIG_DESC_SIZ        32
#define USBD_VID                        0x4B58
#ifdef QS_SENV_IO_ADDRESS
#define USBD_PID                        QS_SENV_IO_ADDRESS
#else
#define USBD_PID                        0x0004
#endif

#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))

usb_device_descriptor_t __attribute__((aligned(4))) device_descriptor = 
{
	0x12,	                  // bLength = 18d
	0x01,		                // bDescriptorType = Device (1)
	//0x0110,		              // bcdUSB(L/H) USB spec rev (BCD)
	0x0200,		              // bcdUSB(L/H) USB spec rev (BCD)	
	0x00,0x00,0x00,         // bDeviceClass, bDeviceSubClass, bDeviceProtocol
	0x40,		                // bMaxPacketSize0 EP0 is 64 bytes
	USBD_VID,               // idVendor
	USBD_PID,               // idVendor
	0x0200,	                // bcdDevice--1234
	1,2,3,		              // iManufacturer, iProduct, iSerialNumber
	1,                      // bNumConfigurations
};

__attribute__((aligned(4)))
struct __attribute__((packed)) {
  usb_configuration_descriptor_t  config_descriptor;
  usb_interface_descriptor_t      interface_descriptor;
  usb_endpoint_descriptor_t       endpoint_descriptor_in;
  usb_endpoint_descriptor_t       endpoint_descriptor_out;
} config_descriptor =
{
  {
	0x09,		// bLength
	0x02,		// bDescriptorType = Config
	USB_BLUK_CONFIG_DESC_SIZ, // wTotalLength(L/H)
	0x01,		// bNumInterfaces
	0x01,		// bConfigValue
	0x00,		// iConfiguration
	0xC0,		// bmAttributes. b7=1 b6=self-powered b5=RWU supported
	0xFA,		// MaxPower is 100 ma
  },
  { /*  First Interface Descriptor */
	0x09,		// length = 9
	0x04,		// type = IF
	0x00,		// IF #0
	0x00,		// bAlternate Setting
	0x02,		// bNum Endpoints
	0x00,		// bInterfaceClass = Bulk
	0x00,0x00,	// bInterfaceSubClass, bInterfaceProtocol
	0x00,		// iInterface
  },
  { /*  IN Endpoint 3 (Descriptor #1) */
	/*Endpoint IN Descriptor*/
	0x07,   /* bLength: Endpoint Descriptor size */
	0x05,      /* bDescriptorType: Endpoint */
	0x83,                         /* bEndpointAddress , IN */
	0x02,                              /* bmAttributes: Bulk */
	BLUK_DATA_MAX_PACKET_SIZE,  /* wMaxPacketSize: */
	0x00,                              /* bInterval: ignore for Bulk transfer */
  },
  { /*  OUT Endpoint 3 (Descriptor #1) */
	/*Endpoint OUT Descriptor*/
	0x07,   /* bLength: Endpoint Descriptor size */
	0x05,      /* bDescriptorType: Endpoint */
	0x01,                        /* bEndpointAddress , OUT */
	0x02,                              /* bmAttributes: Bulk */
	BLUK_DATA_MAX_PACKET_SIZE,  /* wMaxPacketSize: */
	0x00,                              /* bInterval: ignore for Bulk transfer */
  }
};

__attribute__((aligned(4)))
uint8_t lang_id_desc[] = {
  0x04,         /* bLength */
  0x03,         /* bDescriptorType */
  0x09, 0x04    /* bString = wLANGID (see usb_20.pdf 9.6.7 String) */
};

__attribute__((aligned(4)))
uint8_t mfg_id_desc[] = {
	20,				// bLength
	0x03,			// bDescriptorType = string KAIXINMAN
	'K',0,'A',0,'I',0,'X',0,'I',0,'N',0,'M',0,'A',0,'N',0 // text in Unicode
};

__attribute__((aligned(4)))
uint8_t prod_id_desc[] = {
  10,         /* bLength */
	0x03,			// bDescriptorType = string
	'I',0,'D',0,'E',0,'A',0,
};

__attribute__((aligned(4)))
uint8_t serial_id_desc[] = {
  24,         /* bLength */
	0x03,			// bDescriptorType = string
	'S',0,
	'/',0,
	'N',0,
	'2',0,
	'0',0,
	'1',0,
	'8',0,
	'0',0,
  '6',0,
  '0',0,
  '1',0,	
};

#endif /* _DESCRIPTORS_H_ */
