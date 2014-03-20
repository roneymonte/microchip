/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_mouse.c                           ////
////                                                                 ////
//// An example of creating a USB mouse.  Uses your operating        ////
//// system's HID drivers, which on most systems should already be   ////
//// installed.                                                      ////
////                                                                 ////
//// If everything is working, the mouse cursor will move in a       ////
//// circle when connected to a PC.                                  ////
////                                                                 ////
//// For a more generic USB HID example (as well as some HID         ////
//// documentation) see ex_usb_hid.c                                 ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// NOTE ABOUT ENDPOINT BUFFER SIZE:                                ////
////                                                                 ////
//// Although this application sends 4 bytes to the PC, this demo    ////
//// defines USB_EP1_TX_SIZE  to 8 to allocate 8 bytes for this      ////
//// endpoint.  This constant is also used in the endpoint           ////
//// descriptor to specify the endpoint max size.  If you were       ////
//// concious of RAM savings you could redefine these to 4           ////
//// (or even 1!), but you would lose throughput.The reason for      ////
//// throughput loss is that if you send a packet that is the same   ////
//// size as the max packet size then you need to send a 0 len       ////
//// packet to specify end of message  marker.  The routines         ////
//// usb_puts() and usb_gets() send and  receive multiple packet     ////
//// message, waiting for a 0 len packet or a packet that is smaller ////
//// than max-packet size.                                           ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// VERSION HISTORY                                                 ////
////                                                                 ////
//// June 20th, 2005: 18Fxx5x Release                                ////
////                                                                 ////
//// March 21st, 2005: Initial PIC18Fxx5x support                    ////
////                                                                 ////
//// June 24th, 2004:  Cleanup to work with updated USB API.         ////
////                   Mouse now goes in a circle.                   ////
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

//set to 1 to use a PIC with an internal USB Peripheral
//set to 0 to use a National USBN960x peripheral
#define __USB_PIC_PERIF__ 1

#if __USB_PIC_PERIF__ //use the PIC16C7x5 peripheral
 #define LED1 PIN_A5
 #if defined(__PCM__)
  #include <16C765.h>
  #device *=16
  #fuses HS,NOWDT,NOPROTECT
  #use delay(clock=24000000)
 #else
  #include <18F4550.h>
  //~~~ 20MHZ OSCILLATOR CONFIGS ~~~//
  //// FULL SPEED
  #fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
  #use delay(clock=48000000)

  ////SLOW SPEED
  //#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV3,VREGEN
  //#use delay(clock=24000000)
  //#define USB_USE_FULL_SPEED  FALSE
 #endif
#else //use the National USBN960x peripheral
 #define LED1 PIN_B3
 #if defined(__PCM__)
  #include <16F877A.h>
  #device *=16
  #fuses HS,NOWDT,NOPROTECT,NOLVP
 #elif defined(__PCH__)
  #include <18F452.h>
  #fuses HS,NOPROTECT,NOLVP,NOWDT
 #endif
 #use delay(clock=20000000)
#endif   //endif check to see which peripheral to use

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

/////////////////////////////////////////////////////////////////////////////
//
// CCS Library dynamic defines.  For dynamic configuration of the CCS Library
// for your application several defines need to be made.  See the comments
// at usb.h for more information
//
/////////////////////////////////////////////////////////////////////////////
#DEFINE USB_HID_DEVICE  TRUE  //Tells the CCS PIC USB firmware
                              //to include HID handling code.

//turn on EP1 for IN interrupt transfers.  (IN = PIC -> PC)
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT
#define USB_EP1_TX_SIZE 8     //max packet size of this endpoint

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

/////////////////////////////////////////////////////////////////////////////
//
// Include the CCS USB Libraries.  See the comments at the top of these
// files for more information
//
/////////////////////////////////////////////////////////////////////////////
#if __USB_PIC_PERIF__
 #if defined(__PCM__)
  #include <pic_usb.h>   //Microchip PIC16C765 hardware layer for usb.c
 #else
  #include <pic18_usb.h>   //Microchip PIC18Fxx5x hardware layer for usb.c
 #endif
#else
 #include <usbn960x.c>   //National 960x hardware layer for usb.c
#endif
#include <usb_desc_mouse.h>    //USB Configuration and Device descriptors for this UBS device
#include <usb.c>        //handles usb setup tokens and get descriptor reports

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
// to display enumeration and connection states.  Also lights LED2 and LED3
// based upon enumeration and connection status.
//
/////////////////////////////////////////////////////////////////////////////
void usb_debug_task(void) {
   static int8 last_connected;
   static int8 last_enumerated;
   int8 new_connected;
   int8 new_enumerated;

   new_connected=usb_attached();
   new_enumerated=usb_enumerated();

   if (new_connected)
      LED_ON(LED2);
   else
      LED_OFF(LED2);

   if (new_enumerated)
      LED_ON(LED3);
   else
      LED_OFF(LED3);

   if (new_connected && !last_connected)
      printf("\r\n\nUSB connected, waiting for enumaration...");
   if (!new_connected && last_connected)
      printf("\r\n\nUSB disconnected, waiting for connection...");
   if (new_enumerated && !last_enumerated)
      printf("\r\n\nUSB enumerated by PC/HOST");
   if (!new_enumerated && last_enumerated)
      printf("\r\n\nUSB unenumerated by PC/HOST, waiting for enumeration...");

   last_connected=new_connected;
   last_enumerated=new_enumerated;
}

void main(void) {
   #define MOUSE_SEQUENCE_STEPS  16
   const char mouse_seq[MOUSE_SEQUENCE_STEPS]=
      {0, 1, 3, 4, 4, 4, 3, 1, 0, -1, -3, -4, -4, -4, -3, -1};

   int8 out_data[4];
   int8 x_seq=0;  int8 y_seq=MOUSE_SEQUENCE_STEPS/4;
   int8 count=0;

   LED_ON(LED1);
   LED_OFF(LED2);
   LED_OFF(LED3);

   printf("\r\n\nCCS USB Mouse Example");
  #ifdef __PCH__
   printf("\r\nPCH: v");
   printf(__PCH__);
  #else
   printf("\r\n\PCM: v");
   printf(__PCM__);
  #endif

   usb_init();

  #if !(__USB_PIC_PERIF__)
   printf("\r\nUSBN: 0x%X", usbn_get_version());
  #endif
   printf("\r\n");

   while (TRUE) {
      usb_task();
      usb_debug_task();
      if (usb_enumerated()) {
         out_data[0]=0; //button state goes here
         out_data[1]=mouse_seq[x_seq];
         out_data[2]=mouse_seq[y_seq];
         out_data[3]=0; //wheel state goes here
         if (usb_put_packet(1,out_data,4,USB_DTS_TOGGLE))
            count++;
         if (count > 10) {
            if (++x_seq>=MOUSE_SEQUENCE_STEPS) {x_seq=0;}
            if (++y_seq>=MOUSE_SEQUENCE_STEPS) {y_seq=0;}
            count=0;
         }
         delay_ms(10);
      }
   }
}
