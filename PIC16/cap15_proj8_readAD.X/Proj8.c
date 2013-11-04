/*
 * File:   Proj8.c
 * Author: pu1rym
 *
 * Created on April 4, 2013, 10:52 AM
 */


#include <xc.h>


/* Pic16f887 configuration */

#pragma config FOSC = INTRC_NOCLKOUT // define oscilador INTERNO sem CLOCK OUT
#pragma config WDTE = OFF           // watch dog desligado
#pragma config PWRTE = OFF          // temporizador de powerUp desligado
#pragma config MCLRE = ON           // pino MCLR eh MCLR e nao funcao
#pragma config CP = OFF             // programa memory nao protegido
#pragma config CPD = OFF            // data memory nao protegida
#pragma config BOREN = ON           // BROWN-OUT inicializacao sem reset,
                                    // pelo nivel de voltagem minimo

#pragma config IESO = OFF           // internal/external switchOver

#pragma config LVP = OFF            // low voltage programming = precisa de
                                    // alta vontagem para programacao do chip

#pragma config FCMEN = OFF          // fail-safe clock monitor

#define DS1_on  0b00000001;
#define DS2_on  0b00000010
#define DS3_on  0b00000100;
#define DS4_on  0b00001000;
#define DS5_on  0b00010000;
#define DS6_on  0b00100000;
#define DS7_on  0b01000000;
#define DS8_on  0b10000000;

#define _XTAL_FREQ 4000000; //8MHz OSCILLATOR

int advalue=0;

int main(void) {

    PORTA=0;
    TRISA=0xFF; // setta a PORTA como I/O Inputs
    CM1CON0=0;
    ANSEL=1;    // liga o modulo A/D
    ADCON0=0b00000001;  //AtoD on, Left justified, canal AN0/RA0 selecionado
    ADCON1=0b00111000;  //clock interno para A/D conversion

    PORTD=0;
    TRISD=0;    // output dos leds na PORTD

    while (1==1)
    {
        GO_DONE=1;   //starta o processo A/D
        while (GO_DONE==1)
        {
            // espera A/D terminar
        }

        advalue=ADRESH; // coloca o valor do A/D na variavel advalue

        if(advalue<50)  {PORTD=DS8_on;}
        if(advalue<90)  {PORTD=DS7_on;}

        if(advalue<120) {PORTD=DS6_on;}
        if(advalue<150) {PORTD=DS5_on;}

        if(advalue<180) {PORTD=DS4_on;}
        if(advalue<210) {PORTD=DS3_on;}

        if(advalue<230) {PORTD=DS2_on;}
        if(advalue<250) {PORTD=DS1_on;}

        if(advalue>=250)
        {
            PORTD=0;
        }
    }

    
    return 0;
}

