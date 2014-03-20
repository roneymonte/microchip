/* 
 * File:   main.c
 * Author: roney
 *
 * Created on March 18, 2014, 2:37 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "configbits.h"
#include <plib/timers.h>

#define LED_VERM PORTDbits.RD7
#define LED_AMAR PORTDbits.RD6
#define LED_BRCO PORTDbits.RD3
#define LED_VERD PORTDbits.RD2
#define _XTAL_FREQ 4000000
#define USE_AND_MASKS

/*
 * 
 */

void interrupt global_isr(void);
void configTimers (void);
//void interrupt interrupcao(void);


int main(int argc, char** argv) {

    TRISD=0;

    LED_BRCO=1;
    LED_VERD=0;

    configTimers();

    while (1)
    {
        LED_VERM=1; LED_AMAR=0;
        __delay_ms(100);

        LED_AMAR=1; LED_VERM=0;
        __delay_ms(100);
    }

    return (EXIT_SUCCESS);
}

void configTimers (void)
{
     OpenTimer1(TIMER_INT_ON &
                T1_SOURCE_EXT &
                T1_SYNC_EXT_OFF &
                T1_PS_1_1 &
                T1_OSC1EN_ON &
                T1_16BIT_RW

                );

     //WriteTimer1( 0xBE5  );   // equivalemnte a 1/2 segundo em 4,00 mhz
     WriteTimer1( 0x8000 );   // equivalente a 1 segundo em 32,768 khz

     TMR1ON = 1;
     TMR1IF = 0;

     TMR1IE=1;
     PEIE=1;
     GIE=1;

}
 
void interrupt global_isr(void)
{
    //if (TMR1IF==1)
    //{
        LED_BRCO=~LED_BRCO;
        LED_VERD=~LED_VERD;

        //WriteTimer1( 0xBE5 );
        WriteTimer1( 0x8000 );

        TMR1IF=0;
        TMR1IE=1;
    //}
}
