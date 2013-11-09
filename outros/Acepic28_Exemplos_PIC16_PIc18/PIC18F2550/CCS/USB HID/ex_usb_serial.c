/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_serial.c                          ////
////                                                                 ////
//// A demonstration of the USB CDC API that is provided by CCS.     ////
//// The USB CDC API that CCS provides will create a virtual UART    ////
//// port.  USB CDC drivers are included with most versions of       ////
//// Microsoft Windows, and when properly loaded will create a COMx  ////
//// port from which you can write and read to your PIC device       ////
//// like any serial device that has a COMx port.                    ////
////                                                                 ////
//// This example creates a USB<->UART converter.  Open              ////
//// Hyperterminal to COM1 (or whatever COM port is your usual RS232 ////
//// serial COM port).  Plug the PIC to USB.  Open Hypertimernal to  ////
//// the new COM port that is the USB<->UART COM port (for this      ////
//// example say it is COM2).  Typing a character in COM1 will cause ////
//// it to be sent out COM2, and vice-versa.                         ////
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
 #DEFINE LED1  PIN_A5
 #include <18F4550.h>
 #fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
 #use delay(clock=48000000)
#else //use the National USBN960x peripheral
 #DEFINE LED1  PIN_B3
 #include <18F452.h>
 #fuses HS,NOWDT,NOPROTECT,NOLVP
 #use delay(clock=20000000)
#endif   //endif check to see which peripheral to use

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

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


#include <usb_cdc.h>

/////////////////////////////////////////////////////////////////////////////
//
// Configure the demonstration I/O
//
/////////////////////////////////////////////////////////////////////////////
#define LED2 PIN_B4
#define LED3 PIN_B5
#DEFINE BUTTON PIN_A4
#define LED_ON output_low
#define LED_OFF output_high


/////////////////////////////////////////////////////////////////////////////
//
// usb_debug_task()
//
// When called periodically, displays debugging information over serial
// to display enumeration and connection states.  Also lights LED1 based upon
// enumeration and status.
//
/////////////////////////////////////////////////////////////////////////////
void usb_debug_task(void) {
   static int8 last_connected;
   static int8 last_enumerated;
   int8 new_connected;
   int8 new_enumerated;
   static int8 last_cdc;
   int8 new_cdc;

   new_connected=usb_attached();
   new_enumerated=usb_enumerated();
   new_cdc=usb_cdc_connected();

   if (new_enumerated)
      LED_ON(LED1);
   else
      LED_OFF(LED1);

   if (new_cdc)
      LED_ON(LED2);
   else
      LED_OFF(LED2);

   if (usb_cdc_carrier.dte_present)
      LED_ON(LED3);
   else
      LED_OFF(LED3);

   if (new_connected && !last_connected)
      printf("USB connected, waiting for enumaration...\r\n\n");
   if (!new_connected && last_connected)
      printf("USB disconnected, waiting for connection...\r\n\n");
   if (new_enumerated && !last_enumerated)
      printf("USB enumerated by PC/HOST\r\n\n");
   if (!new_enumerated && last_enumerated)
      printf("USB unenumerated by PC/HOST, waiting for enumeration...\r\n\n");
   if (new_cdc && !last_cdc) {
      printf("Serial program initiated on USB<->UART COM Port\r\n\n");
      printf(usb_cdc_putc, "\r\n\nCCS CDC (Virtual RS232) Example\r\n\n");
   }

   last_connected=new_connected;
   last_enumerated=new_enumerated;
   last_cdc=new_cdc;
}

void main(void) {
   char c;

   LED_OFF(LED1);
   LED_OFF(LED2);
   LED_OFF(LED3);

   printf("\r\n\nCCS CDC (Virtual RS232) Example\r\n");

  #ifdef __PCH__
   printf("PCH: v");
   printf(__PCH__);
  #else
   printf("PCM: v");
   printf(__PCM__);
  #endif
   printf("\r\n");

   usb_init();

  #if !(__USB_PIC_PERIF__)
   printf("USBN: 0x%X", usbn_get_version());
   printf("\r\n\n");
  #endif


   while (TRUE) {
      usb_task();
      usb_debug_task();

      if (kbhit()) {
         c=getc();
         if (c=='\n') {usb_cdc_putc('\r'); usb_cdc_putc('\n');}
         if (c=='\r') {usb_cdc_putc('\r'); usb_cdc_putc('\n');}
         else {usb_cdc_putc(c);}
      }
      if (usb_cdc_kbhit()) {
         c=usb_cdc_getc();
         if (c=='\n') {putc('\r'); putc('\n');}
         if (c=='\r') {putc('\r'); putc('\n');}
         else {putc(c);}
      }
   }
}
