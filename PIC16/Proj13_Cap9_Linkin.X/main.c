/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 4, 2013, 10:08 AM
 */


#include <xc.h>

__CONFIG(INTIO&WDTDIS&PWRTDIS&MCLREN&UNPROTECT&DUNPROTECT);
__CONFIG(BOREN&IESODIS&LVPDIS&FCMDIS);

unsigned short delay = 80;
unsigned short potenciometro;

void pause(unsigned short msvalue);

int main(void) {

    ANSEL=0;
    CM1CON0=0;
    CM2CON0=0;

    PORTD=0x00;
    TRISD=0x00;


    PORTA=0;
    TRISA=0xFF; // setta a PORTA como I/O Inputs
    ANSEL=1;    // liga o modulo A/D
    ADCON0=0b00000001;  //AtoD on, Left justified, canal AN0/RA0 selecionado
    ADCON1=0b00111000;  //clock interno para A/D conversion

    while (1==1)
    {
        int x;
        for(x=1; x<129; x=x*2)
        {
            PORTD=x;
            pause(delay);

        }

        GO_DONE=1;   //starta o processo A/D
        while (GO_DONE==1)
        {
            // espera A/D terminar
        }

        //potenciometro=(ADRESH<<8)+ADRESL;
        potenciometro=ADRESH;
        delay=potenciometro;
    }


    return 0;
}
