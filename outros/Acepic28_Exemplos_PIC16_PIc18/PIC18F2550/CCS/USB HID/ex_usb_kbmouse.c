/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_kbmouse.c                         ////
////                                                                 ////
//// An example of creating a mouse and keyboard combo USB device,   ////
//// implemented by using multiple interfaces.  One interface is     ////
//// used for the mouse, another interface for the keyboard.         ////
//// Uses your operating system's HID drivers, which on most systems ////
//// should already be installed.                                    ////
////                                                                 ////
//// Once enumrated, the PIC will move the mouse cursor around in    ////
//// a circle if the button is held down.  Also, whenever the        ////
//// button is held down the PIC will send an 'a' as a keyboard      ////
//// press.  (open a text editor to see the 'a' being sent)          ////
////                                                                 ////
//// For a more generic USB HID example (as well as some HID         ////
//// documentation) see ex_usb_hid.c                                 ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// VERSION HISTORY                                                 ////
////                                                                 ////
//// June 20th, 2005: Initial 18fxx5x release.                       ////
////                                                                 ////
//// May 13th, 2005: Initial Beta.  Only works with 18Fxx5x          ////                                                                                 ////
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


#if __USB_PIC_PERIF__ //use the PIC16C7x5 peripheral
 #DEFINE LED1  PIN_A5
 #if defined(__PCM__)
    #include <16C765.h>
    #device *=16
    #fuses HS,NOWDT,NOPROTECT
    #use delay(clock=24000000)
 #elif defined(__PCH__)
    #include <18F4550.h>
    #fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
    #use delay(clock=48000000)
 #endif
#else //use the National USBN960x peripheral
 #DEFINE LED1  PIN_B3
 #if defined(__PCM__)
  #include <16F877A.h>
  #device *=16
  #fuses HS,NOWDT,NOPROTECT,NOLVP
 #elif defined(__PCH__)
  #include <18F452.h>
  #fuses HS,NOWDT,NOPROTECT,NOLVP
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

//Tells the CCS PIC USB firmware to include HID handling code.
#DEFINE USB_HID_DEVICE  TRUE

//the following defines needed for the CCS USB PIC driver to enable the TX endpoint 1
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_TX_SIZE 8

#define USB_EP2_TX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP2 for IN bulk/interrupt transfers
#define USB_EP2_TX_SIZE 8

#define USB_EP2_RX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP2 for IN bulk/interrupt transfers
#define USB_EP2_RX_SIZE 4


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
  #include <pic18_usb.h>
 #endif
#else
 #include <usbn960x.c>   //National 960x hardware layer for usb.c
#endif
#include <usb_desc_kbmouse.h>    //USB Configuration and Device descriptors for this UBS device
#include <usb.c>        //handles usb setup tokens and get descriptor reports


/////////////////////////////////////////////////////////////////////////////
//
// Configure the demonstration I/O
//
/////////////////////////////////////////////////////////////////////////////
#define BUTTON PIN_A4
#define LED2 PIN_B4
#define LED3 PIN_B5
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

/////////////////////////////////////////////////////////////////////////////
//
// usb_mouse_task()
//
// Sends a packet of data containing mouse information.  The protocol was
// defined in the HID report descriptor (see usb_desc_kbmouse.h), and is:
//   out_data[0]=button state
//   out_data[1]=x delta
//   out_data[2]=y delta
//   out_data[3]=wheel delta
//
/////////////////////////////////////////////////////////////////////////////
void usb_mouse_task(void) {
   #define MOUSE_SEQUENCE_STEPS  16
   const char mouse_seq[MOUSE_SEQUENCE_STEPS]=
      {0, 1, 3, 4, 4, 4, 3, 1, 0, -1, -3, -4, -4, -4, -3, -1};

   int8 out_data[4];
   static int8 x_seq=0;  static int8 y_seq=MOUSE_SEQUENCE_STEPS/4;
   static int8 skip=0;

   if (!input(BUTTON)) {
      out_data[1]=mouse_seq[x_seq];
      out_data[2]=mouse_seq[y_seq];
   }
   else {
      out_data[1]=0;
      out_data[2]=0;
   }
   out_data[0]=0;
   out_data[3]=0;

   if (usb_put_packet(1,out_data,4,USB_DTS_TOGGLE)) {
      skip++;
      if (skip>10) {
         if (++x_seq>=MOUSE_SEQUENCE_STEPS) {x_seq=0;}
         if (++y_seq>=MOUSE_SEQUENCE_STEPS) {y_seq=0;}
         skip=0;
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// usb_keyboard_task()
//
// Sends a packet of keyboard data.  The protocol was specified in the HID
// report descriptor (see usb_desc_kbmouse.h), and is:
//     tx_msg[0] = modifier (an 8bit bitmap of shift, tab, alt keypress)
//     tx_msg[1] = const 0
//     tx_msg[2:6] = an array of held down keys.  a=4, b=5, etc.
//                   if msg[2:7]={0} then no keys are held down
//
//     rx_msg[0] = 5bit bitmap of led status
//
/////////////////////////////////////////////////////////////////////////////
void usb_keyboard_task(void) {
   static int8 tx_msg[7]={0,0,0,0,0,0,0};
   static int8 leds;

   if (!input(BUTTON))
      tx_msg[2]=4;
   else
      tx_msg[2]=0;

   usb_put_packet(2,tx_msg,sizeof(tx_msg),USB_DTS_TOGGLE);

   //receive NUM LOCK, CAPS LOCK, etc LED status from PC.
   //we won't do anything with it.
   if (usb_kbhit(2)) {
      usb_get_packet(2, &leds, 1);
   }
}

void main(void) {
   LED_ON(LED1);
   LED_OFF(LED2);
   LED_OFF(LED3);

   printf("\r\n\nCCS HID Mouse/Keyboard Multiple Interface Demo");

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
         usb_mouse_task();
         usb_keyboard_task();
         delay_ms(10);
      }
   }
}

