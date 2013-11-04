/*
 * File:   main2.c
 * Author: pu1rym
 *
 * Created on April 1, 2013, 4:21 PM
 */


#include <xc.h>
#include <pic.h>

//__CONFIG(INTIO & WDTDIS & MCLRDIS & UNPROTECT);
__CONFIG(INTIO&WDTDIS&PWRTDIS&MCLREN&UNPROTECT&DUNPROTECT);
__CONFIG(BOREN&IESODIS&LVPDIS&FCMDIS);

unsigned short delay = 250;

void Pause(unsigned short msvalue);
void msecbase (void);



int main(void) {

    ANSEL=0;
    CM1CON0=0;
    CM2CON0=0;

    PORTD=0x00;
    TRISD=0x00;

    while (1==1)
    {
        int x;
        for(x=1; x<129; x=x*2)
        {
            PORTD=x;
            Pause(delay);

        }
    }


    return 0;
}

void Pause(unsigned short msvalue)
{
    unsigned short x;
    for (x=0; x<=msvalue; x++)
    {
        msecbase();
    }
}

void msecbase (void)
{
    OPTION_REG=0b00000001;
    TMR0=0xd;
    while(!T0IF);
    T0IF=0;
}