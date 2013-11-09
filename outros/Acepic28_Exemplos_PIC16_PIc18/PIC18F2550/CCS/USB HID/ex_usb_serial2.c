/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_serial2.c                         ////
////                                                                 ////
//// A demonstration of the USB CDC API that is provided by CCS.     ////
//// The USB CDC API that CCS provides will create a virtual UART    ////
//// port.  USB CDC drivers are included with most versions of       ////
//// Microsoft Windows, and when properly loaded will create a COMx  ////
//// port from which you can write and read to your PIC device       ////
//// like any serial device that has a COMx port.                    ////
////                                                                 ////
//// This example is a conversion of the original EX_INTEE.C to use  ////
//// the USB CDC API to read and display serial data over USB.       ////
////                                                                 ////
//// Only two lines were added to initialize USB:                    ////
////   usb_init() - init USB and enable USB interrupt.               ////
////   while(!usb_cdc_connected()) - wait until user opens           ////
////       Hyperterminal before displaying serial data.  This line   ////
////       is not necessary.                                         ////
////                                                                 ////
//// Two other changes were also made to convert to USB.  First,     ////
//// printf will call usb_cdc_putc() to put the character out USB    ////
//// instead of the normal RS232 stream.  Second, gethex() was       ////
//// replaced with gethex_usb().  All input functions normally found ////
//// in input.c have been converted to use the USB CDC API in        ////
//// usb_cdc.h, and gethex_usb() is one of these converted           ////
//// functions.                                                      ////
////                                                                 ////
//// See usb_cdc.h for API documentation.                            ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// VERSION HISTORY                                                 ////
////                                                                 ////
//// July 1st, 2005: Initial Release.                                ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2005 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

//set to 1 to use a PIC's internal USB Peripheral
//set to 0 to use a National USBN960x peripheral
#define __USB_PIC_PERIF__ 1

#if !defined(__PCH__)
 #error USB CDC Library requires PIC18
#endif

#if __USB_PIC_PERIF__
  #include <18F4550.h>

  //configure a 20MHz crystal to operate at 48MHz
  #fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
  #use delay(clock=48000000)
#else //use the National USBN960x peripheral
  #include <18F452.h>
  #fuses HS,NOWDT,NOPROTECT,NOLVP
  #use delay(clock=20000000)
#endif   //endif check to see which peripheral to use

/////////////////////////////////////////////////////////////////////////////
//
// If you are using a USB connection sense pin, define it here.  If you are
// not using connection sense, comment out this line.  Without connection
// sense you will not know if the device gets disconnected.
//       (connection sense should look like this:
//                             100k
//            VBUS-----+----/\/\/\/\/\----- (I/O PIN ON PIC)
//                     |
//                     +----/\/\/\/\/\-----GND
//                             100k
//        (where VBUS is pin1 of the USB connector)
//
/////////////////////////////////////////////////////////////////////////////
//#define USB_CON_SENSE_PIN PIN_B2

// Includes all USB code and interrupts, as well as the CDC API
#include <usb_cdc.h>

#rom int 0xf00000={1,2,3,4}

void main() {
   BYTE i, j, address, value;

   usb_init();
   while(!usb_cdc_connected()) {}

   do {
      printf(usb_cdc_putc, "\r\n\nEEPROM:\r\n");              // Display contents of the first 64
      for(i=0; i<=3; ++i) {                     // bytes of the data EEPROM in hex
         for(j=0; j<=15; ++j) {
            printf(usb_cdc_putc, "%2x ", read_eeprom( i*16+j ) );
         }
         printf(usb_cdc_putc, "\n\r");
      }
      printf(usb_cdc_putc, "\r\nLocation to change: ");
      address = gethex_usb();
      printf(usb_cdc_putc, "\r\nNew value: ");
      value = gethex_usb();

      write_eeprom( address, value );

   } while (TRUE);
}
