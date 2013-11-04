/*
 * File:   main.c
 * Author: roney
 * Version: 0.1a
 * Created on September 27, 2013, 11:36 AM
 * Updated on November 03, 2013, 08:03 AM
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "plib/xlcd.h"
#include "plib/delays.h"
#include "configuration_bits.h"

#define _XTAL_FREQ 32000 // 32 Khz

 //////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////

void DelayFor18TCY(void)
 {
 Delay10TCYx(0x2); //delays 20 cycles
 return;
 }

 void DelayPORXLCD(void)   // minimum 15ms
 {
 Delay100TCYx(0xA0);   // 100TCY * 160
 return;
 }

 void DelayXLCD(void)     // minimum 5ms
 {
 Delay100TCYx(0x36);      // 100TCY * 54
 return;
 }

 //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////

void initLCD(void)
{
        OpenXLCD(FOUR_BIT & LINES_5X7);
        SetDDRamAddr(0x00);
        WriteDataXLCD('1');
	WriteCmdXLCD(DON&CURSOR_OFF&BLINK_OFF);
}

/*
111 = 8 MHz (INTOSC drives clock directly)                  7
110 = 4 MHz                                                 6
101 = 2 MHz                                                 5
100 = 1 MHz(3)                                              4
011 = 500 kHz                                               3
010 = 250 kHz                                               2
001 = 125 kHz                                               1
000 = 31 kHz (from either INTOSC/256 or INTRC directly)     0
*/

 //////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////

void main(void) {

    int onda [] = { 31, 125, 250, 500, 1000, 2000, 4000, 8000 };
    char sNumero [5];
    unsigned short int freq;
    short int i,t;

    ADCON1 = 0b00111111;        	// Make all ADC/IO pins digital
    OSCCON=0b000;   // 31 Khz


    TRISA0=0;   // definida saida do LED para Clock Real
    TRISA1=1;   // definida entrada do PushBotton

    TRISB=0;    // definida saidas para LCD na PORTB
    TRISC=0;    // definida saidas para os LEDs na PORTC

    PORTAbits.RA0=1;    // liga o led vermelho
    initLCD();

    while (1)
    {

    for (freq=0b000; freq <= 0b111 ; freq++)
    {
        //OSCCON=freq;
        OSCCONbits.IRCF=freq;

        for (t=0;t<255;t++) for (i=0b0001;i<0b10000;i=i*2) { PORTC=i;}
        PORTC=0;

        

        while(BusyXLCD());
        WriteCmdXLCD(0x01);

        while(BusyXLCD());
	//putrsXLCD("Frequencia Clock");
        //Delay10KTCYx(1);

        putrsXLCD( ltoa ( sNumero, ((onda[freq]*1000)/16), 10) );
        putrsXLCD(" / ");
        putrsXLCD( ltoa  (sNumero, ((onda[freq]*1000)/4),  10) );

        SetDDRamAddr(0x40);         // segunda linha display
        putrsXLCD(itoa (sNumero,onda[freq],10) );

        if (freq<0b100)
        {      putrsXLCD(" khz "); }
        else { putrsXLCD(" mhz "); }

        putrsXLCD( itoa (sNumero,freq,2));
        putrsXLCD(" / ");
        putrsXLCD( itoa (sNumero,PORTAbits.RA1,2) );

        while (PORTAbits.RA1==1) PORTAbits.RA0=~PORTAbits.RA0;
        PORTAbits.RA0=0;
        while (PORTAbits.RA1==0);
        PORTAbits.RA0=1;

        

        Delay10KTCYx(1);
        

    }

    } // fim while 1
    

    return;
}
