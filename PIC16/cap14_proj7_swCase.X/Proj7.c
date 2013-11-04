/*
 * File:   Proj7.c
 * Author: pu1rym
 *
 * Created on April 4, 2013, 10:25 AM
 */


#include <xc.h>
#include "pause.h"

#define SW1 RB0

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


int main(void) {

    unsigned char state_led = 0;

    ANSEL=0;
    ANSELH=0;

    CM1CON0=0;
    CM2CON0=0;

    PORTD=0X00;
    TRISD=0X00;
    PORTB=0XFF;

    while (1==1)
    {
        if (SW1==0)
        {
            state_led=state_led+1;
            switch(state_led)
            {
                case 1:
                    RD0=1;
                    RD1=0;
                    RD2=0;
                    RD3=0;
                    RD4=0;
                    RD5=0;
                    RD6=0;
                    RD7=0;
                    break;
                case 2:
                    RD0=0;
                    RD1=1;
                    RD2=0;
                    RD3=0;
                    RD4=0;
                    RD5=0;
                    RD6=0;
                    RD7=0;
                    break;
                case 3:
                    RD0=0;
                    RD1=0;
                    RD2=1;
                    RD3=0;
                    RD4=0;
                    RD5=0;
                    RD6=0;
                    RD7=0;
                    break;
                case 4:
                    RD0=0;
                    RD1=0;
                    RD2=0;
                    RD3=1;
                    RD4=0;
                    RD5=0;
                    RD6=0;
                    RD7=0;
                    break;
                case 5:
                    RD0=0;
                    RD1=0;
                    RD2=0;
                    RD3=0;
                    RD4=1;
                    RD5=0;
                    RD6=0;
                    RD7=0;
                    break;
                case 6:
                    RD0=0;
                    RD1=1;
                    RD2=0;
                    RD3=0;
                    RD4=0;
                    RD5=1;
                    RD6=0;
                    RD7=0;
                    break;
                case 7:
                    RD0=0;
                    RD1=0;
                    RD2=0;
                    RD3=0;
                    RD4=0;
                    RD5=0;
                    RD6=1;
                    RD7=0;
                    break;
                case 8:
                    RD0=0;
                    RD1=0;
                    RD2=0;
                    RD3=0;
                    RD4=0;
                    RD5=0;
                    RD6=0;
                    RD7=1;
                    break;
                default:

                    state_led=0;
                    RD0=0;
                    RD1=0;
                    RD2=0;
                    RD3=0;
                    RD4=0;
                    RD5=0;
                    RD6=0;
                    RD7=0;
                    break;

            }
            while (!SW1);
            pause (10);
            while (!SW1);
        }
    }

    return 0;
}
