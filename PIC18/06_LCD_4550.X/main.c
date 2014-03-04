#include <xc.h>
#include <plib/delays.h>
#include <plib/xlcd.h>

#include "config_bits.h"

#define _XTAL_FREQ 4000000

#define LED1 PORTDbits.RD6
#define LED2 PORTDbits.RD7


 //////////////////////////////////////////////////////////////////

void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);
void initLCD(void);
void piscaVermelho (void);

 //////////////////////////////////////////////////////////////////

void DelayFor18TCY(void)
 {
 //Delay10TCYx(0x2); //delays 20 cycles

 	Delay10TCYx(1);
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();

        Delay1TCY();
	Delay1TCY();

 return;
 }


// void DelayPORXLCD (void) {
// Delay of 15ms
// Cycles = (TimeDelay * Fosc) / 4
// Cycles = (15ms * 12MHz) / 4
// Cycles = 15,000

//Delay1KTCYx(100);     //20ms
  //Delay1KTCYx(60); // Delay of 15ms
  // Cycles = (TimeDelay * Fosc) / 4
  // Cycles = (15ms * 16MHz) / 4
  // Cycles = 60,000
 void DelayPORXLCD(void)   // minimum 15ms
 {
 //Delay100TCYx(0xA0);   // 100TCY * 160

 	Delay1KTCYx(15);	// Delay of 15 ms
						// Cycles = (TimeDelay * Fosc)/4
						// Cycles = (15ms * 4MHz)/4
						// Cycles = 15,000

 return;
 }

 /*
  * 1 mhz = 1 milhao de hertz = 1 milhao ciclos/seg = 250 instrucoes / seg
  * 
  */

 /* Delay100TCYx
  * Delay multiples of 100 Tcy
  * Passing 0 (zero) results in a delay of 25,600 cycles.
  */


  //Delay1KTCYx(12); // Delay of 5ms
       // Cycles = (TimeDelay * Fosc) / 4
       // Cycles = (5ms * 12MHz) / 4
       // Cycles = 5,000
          //Delay1KTCYx(40);    //delay of 6ms
           //Delay1KTCYx(20); // Delay of 5ms
           // Cycles = (TimeDelay * Fosc) / 4
           // Cycles = (5ms * 16MHz) / 4
           // Cycles = 20,000
 void DelayXLCD(void)     // minimum 5ms
 {
 //Delay100TCYx(0x36);      // 100TCY * 54

 Delay1KTCYx(5); 	// Delay of 5 ms
						// Cycles = (TimeDelay * Fosc)/4
						// Cycles = (5ms * 4MHz)/4
						// Cycles = 5,000
 return;
 }

 void initLCD(void)
{
     
        OpenXLCD(FOUR_BIT & LINES_5X7);

        //OpenXLCD(FOUR_BIT);
        
        //SetDDRamAddr(0x00);
        
        //WriteDataXLCD('i');
        
        WriteCmdXLCD(DON & CURSOR_ON & BLINK_ON);

 }

 //////////////////////////////////////////////////////////////////
/*
 /****************************************************
 * Delay functions									*
 ****************************************************
void DelayFor18TCY(void) // Delay of 18 cycles
{
	Delay10TCYx(1);
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
}

void DelayPORXLCD(void)
{
	Delay1KTCYx(15);	// Delay of 15 ms
						// Cycles = (TimeDelay * Fosc)/4
						// Cycles = (15ms * 4MHz)/4
						// Cycles = 15,000
}

void DelayXLCD(void)
{
	Delay1KTCYx(5); 	// Delay of 5 ms
						// Cycles = (TimeDelay * Fosc)/4
						// Cycles = (5ms * 4MHz)/4
						// Cycles = 5,000
}
*/





void main (void)
{
    // 2 segundos de delay na inicializacao
    Delay10KTCYx(50);Delay10KTCYx(50);Delay10KTCYx(50);Delay10KTCYx(50);

    short i;
    //FOSC = INTOSC_EC, the actual value for FOSC<3:0> = b'1001', which accesses the internal clock and sets RA6 as a Fosc/4 pin.
    //OSCCON=0b110; // 4 mhz
    // OSCCON=0b111; // 8 mhz
    //OSCCON=0b11110010; // 8 mhz ,  SCS<1:0> = b'10', which activates the internal oscillator.

    //IRCF0=0;
    //IRCF1=1;
    //IRCF2=1;

   // SCS1=1;
   // SCS0=0;



    /*

     * REGISTER 2-2: OSCCON: OSCILLATOR CONTROL REGISTER

        IDLEN IRCF2 IRCF1 IRCF0 OSTS IOFS SCS1 SCS0
        bit 7 ................................ bit 0

        bit 7 IDLEN:Idle Enable bit
            1= Device enters Idle mode on SLEEPinstruction
            0= Device enters Sleep mode on SLEEPinstruction

        bit 6-4 IRCF2:IRCF0:Internal Oscillator Frequency Select bits
            111= 8 MHz (INTOSC drives clock directly)
            110= 4 MHz
            101= 2 MHz
            100= 1 MHz

            011= 500 kHz
            010= 250 kHz
            001= 125 kHz
            000= 31 kHz (from either INTOSC/256 or INTRC directly)

        bit 3 OSTS:Oscillator Start-up Time-out Status bit
            1= Oscillator Start-up Timer time-out has expired; primary oscillator is running
            0= Oscillator Start-up Timer time-out is running; primary oscillator is not ready

        bit 2 IOFS:INTOSC Frequency Stable bit
            1= INTOSC frequency is stable
            0= INTOSC frequency is not stable

        bit 1-0 SCS1:SCS0:System Clock Select bits
            1x= Internal oscillator
            01= Timer1 oscillator
            00= Primary oscillator

        Note 1: Depends on the state of the IESO Configuration bit.
        2: Source selected by the INTSRC bit (OSCTUNE<7>), see text.
        3: Default output frequency of INTOSC on Reset
     */

   /*
    SPPEN=0;
    CMCON=0;
    PSPIE=0;
    CCP1=0b0000;
    CCP1CON=0;
    P1M0=0b0;
    P1M1=0b0;
    //ECCP1CON
*/

    TRISB=0x00;
    TRISD6=0;
    TRISD7=0;

    piscaVermelho();
    Delay10KTCYx(10); // 1 seg a 4 mhz

    
    for(i=0;i<3;i++)
    {
        LED2=1;
        Delay10KTCYx(50);  // Delay of 10 ms * 50 = 500 ms
        LED2=0;
        Delay10KTCYx(50);	// Delay of 100 ms
    }
    
    Delay10KTCYx(50);piscaVermelho();Delay10KTCYx(50);piscaVermelho();Delay10KTCYx(50);
    Delay10KTCYx(200);

    initLCD();

    //#######################################################################
    //#######################################################################
    
    //#######################################################################
    //#######################################################################

    piscaVermelho();Delay10KTCYx(50);piscaVermelho();Delay10KTCYx(50);
    Delay10KTCYx(50);piscaVermelho();Delay10KTCYx(50);

    while(BusyXLCD());
        WriteCmdXLCD(0x01);
        
    SetDDRamAddr(0x00);
    putrsXLCD ("PIC18F4550");
    SetDDRamAddr(0x40);
    putrsXLCD ("PIC18F4550");
    SetDDRamAddr(0x80);
    putrsXLCD ("PIC18F4550");



    while (1)
    {
        LED2=1;
        Delay10KTCYx(50);     // delay aproximado de 100 ms
        LED2=0;
        Delay10KTCYx(50);     // delay aproximado de 100 ms
    }



}

void piscaVermelho (void){
    LED2=1;
    Delay10KTCYx(20);
    LED2=0;
}