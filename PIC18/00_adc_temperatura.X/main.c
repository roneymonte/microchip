/*
 * File:   main.c
 * Author: roney
 *
 * Created on November 4, 2013, 4:53 PM (on GNU/Linux)
 * Version 0.2
 * Updated Version 1.0.1    @ 2013NOV10 - 00:18
 * via GitHub on MPLABX and Subversion 1.6.17 (for win)
 *
 */


#include <xc.h>
#include <plib/xlcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <plib/delays.h>
#include "main.h"
#include "configuration_bits.h"
#define _XTAL_FREQ 8000000

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

 void initLCD(void)
{
        OpenXLCD(FOUR_BIT & LINES_5X7);
        SetDDRamAddr(0x00);
        WriteDataXLCD('i');
        WriteCmdXLCD(DON&CURSOR_OFF&BLINK_OFF);
}

 //////////////////////////////////////////////////////////////////

void main(void) {

    char buffer [sizeof(unsigned long)*8+1];
    unsigned long corrente, anteriorP, anteriorT;
    unsigned short int temperatura, tempAnterior;
    unsigned short int tMax=0, tMin=50;

    TRISC=0;        //  desnecessario
    piscarC();

    TRISB=0;            // display de LCD como saida
    initLCD();

    TRISCbits.RC2=0;    // AQUECEDOR de Resistor 5w (devera ser acionado por PWM)
    PORTCbits.RC2=0;    // inicia com o AQUECEDOR desligado
    TRISCbits.RC0=0;    // led verde
    TRISCbits.RC1=0;    // led verde
    TRISCbits.RC3=0;    // led verde


    TRISAbits.RA1=1;   // canal A1 do Trimpot como Entrada
    TRISAbits.RA3=1;   // canal A3 do sensor de Temperatura como Entrada
    TRISAbits.RA4=1;    // Push Botton 4
    //ADCON1bits_t.PCFG1=1;
    //ADCON1bits_t.PCFG3=1;
    //ou
    //           PCFG 3210 (bits)
    ADCON1bits.PCFG=0b1011; // pagina DS39626E-page 224 do Datasheet 18F2525
    //o valor de 1011 esta na tabela para definir AN0 a AN3 como analogicos
    //e o restante dos outros ANs como digitais

    /*The I/O configuration for these
    pins is determined by the setting of the PCFG3:PCFG0
    bits (ADCON1<3:0>). Therefore, device current is
    minimized when analog inputs are present at Reset
    time.*/

    //CMCONbits_t.CIS=0;
    /*
    A device Reset forces the CMCON register to its Reset
    state, causing the comparator modules to be turned off
    (CM2:CM0 = 111). However, the input pins (RA0
    through RA3) are configured as analog inputs by
    default on device Reset
    */
    //piscarC();

    while(BusyXLCD());
    WriteCmdXLCD(0x01);
    //putrsXLCD ("");

    while (1)
    {
        

        ADCON0bits.CHS=0b0001;  //usa o AN1 para CONversao DS39626E-page 223
                                // AN1 = Trimpot
        ADCON0bits.ADON=1;  // liga o AD para CONversao
        Delay1KTCYx(2);     // delay aproximado de 1 ms

        ADCON0bits.GO=1;    // inicia a CONversao
        while (ADCON0bits.GO) ;
                            //  aguarda o termino da CONversao
        corrente = ADRES;

        if ( corrente != anteriorP )
        {
        while(BusyXLCD()) ;
        SetDDRamAddr(0x40); //linha 2
        //putrsXLCD ("p");
        putrsXLCD ( ltoa (buffer,ADRES,10) );
        putrsXLCD (" ");
        anteriorP=corrente;
        }
        //lerTemperatura();


        ADCON0bits.CHS=0b0011;  //usa o AN3 para CONversao DS39626E-page 223
                                // AN3 = Termistor
        ADCON0bits.ADON=1;  // liga o AD para CONversao
        Delay1KTCYx(2);     // delay aproximado de 1 ms

        ADCON0bits.GO=1;    // inicia a CONversao
        while (ADCON0bits.GO) ;
                            //  aguarda o termino da CONversao
        corrente = ADRES;

        temperatura= ((corrente*5)/1023)+2 ;

        if ( temperatura < tMin)
        {
            tMin=temperatura;
            SetDDRamAddr(0x00);
            putrsXLCD ("Min:");
            putrsXLCD ( itoa(buffer,tMin, 10));
            putrsXLCD ("c ");
        }

        if ( temperatura > tMax)
        {
            tMax=temperatura;
            SetDDRamAddr(0x09);
            putrsXLCD ("Max:");
            putrsXLCD ( itoa(buffer,tMax, 10));
            putrsXLCD ("c ");
        }

        if ( temperatura != tempAnterior )
        {
        while(BusyXLCD()) ;
        SetDDRamAddr(0x46);
        //putrsXLCD (" t");
        putrsXLCD ( ltoa (buffer, corrente, 10) );
        putrsXLCD (":");
        putrsXLCD ( ltoa (buffer, temperatura,10));
        putrsXLCD ("c ");
        tempAnterior=temperatura;
        }
        Delay10KTCYx(10);

        if (temperatura>30) { PORTCbits.RC4=1;Delay10KTCYx(1);PORTCbits.RC4=0; }
        if (temperatura>31) PORTCbits.RC2 = 0;
        if (PORTCbits.RC2==1) { PORTCbits.RC5=1;Delay10KTCYx(10);PORTCbits.RC5=0; }

        while (PORTAbits.RA4==1)
        {   
            piscarC() ;
            PORTCbits.RC2=~PORTCbits.RC2;
        }
        ;
        //while (PORTAbits.RA4==0);
        ;

    }

    //return;
}

long lerTemperatura (void)
{
    return PORTAbits.RA3;
}

void piscarC (void)
{
    unsigned short int i;
    PORTC=0;
    for (i=0;i<6;i++)
    {
        PORTC=~PORTC;
        Delay10KTCYx(10);
    }
    PORTC=0;
}

