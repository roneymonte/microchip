/* 
 * File:   main.c
 * Author: roney
 *
 * Created on 11 de Fevereiro de 2014, 11:46
 */

#include <xc.h>
#include "config.h"
#include <plib/delays.h>


#define _XTAL_FREQ 8000000


#define DSP1 PORTCbits.RC2
#define DSP2 PORTCbits.RC1
#define DSP3 PORTAbits.RA5
#define SEG PORTB

void SevenSegment(short int num);







int main(void) {

    short int x, d1, d2, d3;

    TRISB = 0x00;
    SEG = 0x00;

    //TRISCbits.DSP1 = 0x00;
    TRISCbits.RC2 = 0x00;
    TRISCbits.RC1 = 0x00;
    TRISAbits.RA5 = 0x00;

    TRISAbits.RA1=1;   // canal A1 do Trimpot como Entrada
    ADCON1bits.PCFG=0b1011; // pagina DS39626E-page 224 do Datasheet 18F2525
    //o valor de 1011 esta na tabela para definir AN0 a AN3 como analogicos
    //e o restante dos outros ANs como digitais




    SEG=0xFF;
    for(x=0;x<5;x++)
    {
        DSP1=0b1; DSP2=0b1; DSP3=0b1;
        Delay10KTCYx(100);
        DSP1=0b0; DSP2=0b0; DSP3=0b0;
        Delay10KTCYx(100);
    }

    x=0;
    while (1)

    {



        //d1 = x / 100;
        //DSP1=0b1; DSP2=0b0; DSP3=0b0;
        //SevenSegment(d1);

        d2 = x / 10;
        DSP2=0b1; DSP3=0b0;
        SevenSegment(d2);

        PORTCbits.RC0 = 0b1;

        ADCON0bits.CHS=0b0001;  //usa o AN1 para CONversao DS39626E-page 223
                                // AN1 = Trimpot
        ADCON0bits.ADON=1;  // liga o AD para CONversao
        Delay1KTCYx(2);     // delay aproximado de 1 ms

        ADCON0bits.GO=1;    // inicia a CONversao
        while (ADCON0bits.GO) ;
                            //  aguarda o termino da CONversao
        Delay1KTCYx( ADRESH );


        d3 = x % 10;
        DSP2=0b0; DSP3=0b1;
        SevenSegment(d3);

        PORTCbits.RC0 = 0b0;

        //DSP1=0b0; DSP2=0b0; DSP3=0b0;

        ADCON0bits.CHS=0b0001;  //usa o AN1 para CONversao DS39626E-page 223
                                // AN1 = Trimpot
        ADCON0bits.ADON=1;  // liga o AD para CONversao
        Delay1KTCYx(2);     // delay aproximado de 1 ms

        ADCON0bits.GO=1;    // inicia a CONversao
        while (ADCON0bits.GO) ;
                            //  aguarda o termino da CONversao
        Delay1KTCYx( ADRESH );



        x++;
        if (x>99) x=0;

        

    }
}

void SevenSegment(short num)
{
   switch(num)
   {
      case 0:
         SEG = 0b00111111;
         break;

      case 1:
         SEG = 0b00000110;
         break;

      case 2:
         SEG = 0b01011011;
         break;

      case 3:
         SEG = 0b01001111;
         break;

      case 4:
         SEG = 0b01100110;
         break;

      case 5:
         SEG = 0b01101101;
         break;

      case 6:
         SEG = 0b01111101;
         break;

      case 7:
         SEG = 0b00000111;
         break;

      case 8:
         SEG = 0b01111111;
         break;

      case 9:
         SEG = 0b01101111;
         break;
   }
}
