/////////////////////////////////////////////////////////////////////////
////                        ex_usb_scope.c                           ////
////                                                                 ////
//// This example shows how to develop a USB bulk device, and is     ////
//// accompanied by an example Windows program (oscope.exe) and      ////
//// an example Windows USB Bulk driver (usbdemo.sys).  This example ////
//// will run on CCS's USB Prototype board.                          ////
////                                                                 ////
//// NOTE: THIS DOES NOT WORK PIC16C7x5 BECAUSE THAT USB PERIPHERAL  ////
////       IS A SLOW SPEED PERIPHERAL, AND SLOW SPEED ONLY SUPPORTS  ////
////       CONTROL AND INTERRUPT TRANSFERS.                          ////
////                                                                 ////
//// When a PIC with this code is connected to the PC, an install    ////
//// driver wizard should pop-up in Windows.  Install the            ////
//// provided usbdemo.sys driver.  You may have to reboot the        ////
//// machine.  Once installed, you can use oscope.exe to see the     ////
//// data from the PIC.                                              ////
////                                                                 ////
////  This example will work with the  PCH compiler.                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// VERSION HISTORY                                                 ////
////                                                                 ////
//// June 20th, 2005:  18Fxx5x intial Release                        ////
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

//set to 1 to use a PIC with internal USB Peripheral
//set to 0 to use a National USBN960x peripheral
#define __USB_PIC_PERIF__ 1

#if __USB_PIC_PERIF__ //use the internal USB peripheral
 #define LED1 PIN_A5
 #if defined(__PCM__)
   #error The PIC16C7x5 USB Peripheral does not support bulk mode
 #else
  #include <18F4550.h>
  //#define USB_CON_SENSE_PIN PIN_B2   //CCS 18F4550 development kit has optional conection sense pin
  //~~~ 20MHZ OSCILLATOR CONFIGS ~~~//
  #fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
  #use delay(clock=48000000)
 #endif
#else //use the National USBN960x peripheral
 #define LED1 PIN_B3
 #if defined(__PCM__)
   #error This example requires PCH because of RAM requirements
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
#DEFINE USB_HID_DEVICE     FALSE
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK  //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK  //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    64  //size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    8   //size to allocate for the rx endpoint 1 buffer


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
  #include <pic18_usb.h>
#else
 #include <usbn960x.c>   //National 960x hardware layer for usb.c
#endif
#include <usb_desc_scope.h>	//USB Configuration and Device descriptors for this UBS device
#include <usb.c>        	//handles usb setup tokens and get descriptor reports


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

//We will send a 512byte message to the PC.  Since the packet size is 64, this
//will be accomplished by sending 8 64byte packets and 1 0byte packet.
#DEFINE OSCDEMO_MESSAGE_SIZE 512

//a sine table is saved to ROM to simulate adc readings
#define SINE_TABLE_SIZE 256
const char sine_table[SINE_TABLE_SIZE]={
   127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169,
   172, 175, 178, 181, 184, 186, 189, 192, 195, 197, 200, 202, 205, 207, 210,
   212, 214, 216, 219, 221, 223, 225, 227, 229, 230, 232, 234, 236, 237, 239,
   240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252, 253,
   253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250,
   249, 248, 247, 246, 245, 244, 242, 241, 240, 238, 237, 235, 234, 232, 230,
   228, 226, 224, 222, 220, 218, 216, 214, 211, 209, 207, 204, 202, 199, 197,
   194, 191, 189, 186, 183, 180, 178, 175, 172, 169, 166, 163, 160, 157, 154,
   151, 148, 145, 142, 138, 135, 132, 129, 127, 124, 121, 118, 114, 111, 108,
   105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 73, 70, 67, 64,
   62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 37, 35, 33, 31, 29,
   27, 25, 23, 22, 20, 18, 17, 15, 14, 12, 11, 10, 9, 8, 7,
   6, 5, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9, 11, 12,
   13, 14, 16, 17, 19, 21, 22, 24, 26, 28, 30, 32, 34, 36, 38,
   41, 43, 45, 48, 50, 53, 55, 58, 60, 63, 66, 69, 71, 74, 77,
   80, 83, 86, 89, 92, 95, 98, 101, 104, 107, 110, 113, 116, 119, 122, 125
};

//oscope.exe will send us a message containg two bytes.  The first byte
//is the sample_rate, or time division between ticks.  The second byte is
//the threshold, or trigger point of the scope.
int8 rxdata[2];
#define sample_rate rxdata[1]
#define threshold rxdata[0]


/////////////////////////////////////////////////////////////////////////////
//
// read_simulated_adc()
//
// Simulates reading an adc by using the sine_table[] array.  Also will
// simulate reading at faster/slower rates by using the sample_rate
// to change the speed at which we index the sine_table[] array.
//
/////////////////////////////////////////////////////////////////////////////
int8 read_simulated_adc(void) {
   static int8 index;
   int8 ret;
   ret=sine_table[index];
   index+=sample_rate+1;
   return(ret);
}

/////////////////////////////////////////////////////////////////////////////
//
// usb_scope_task()
//
// Reads an ADC value, and once it raises above the threshold read and
// sample 512 bytes and then transmit those 512 readings to the PC.
//
/////////////////////////////////////////////////////////////////////////////
void usb_scope_task(void) {
   int8 adc;
   static int8 last_adc;
   int8 message[OSCDEMO_MESSAGE_SIZE];
   int16 i;

   adc=read_simulated_adc();

   if ((adc >= threshold)&&(last_adc <= threshold)) {
      message[0]=adc;
      for (i=1;i<OSCDEMO_MESSAGE_SIZE;i++) {
         message[i]=read_simulated_adc();
      }
      usb_puts(1,message,OSCDEMO_MESSAGE_SIZE,50);
   }
   else {
      last_adc=adc;
   }
}

void main(void) {
   int1 run=0;
   int16 ms_counter;

   LED_ON(LED1);
   LED_OFF(LED2);
   LED_OFF(LED3);

   printf("\r\n\nCCS USB Bulk Example\r\nPCH: v");
   printf(__PCH__);

   usb_init();

  #if !(__USB_PIC_PERIF__)
   printf("\r\nUSBN: 0x%X", usbn_get_version());
  #ENDIF
   printf("\r\n\n");

   while (TRUE) {
      usb_task();
      usb_debug_task();

      if(usb_enumerated()) {
         if (run)
            usb_scope_task();

         if (usb_kbhit(1)) {
            ms_counter=0;
            usb_get_packet(1,rxdata,2);
            printf("\r\n--> Received 2 bytes: Thresh=%U Delay=%U",threshold,sample_rate);
            run=1;
         }
      }
   }
}
