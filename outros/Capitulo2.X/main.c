/*
 * File:   main.c
 * Author: Administrator
 *
 * Created on March 25, 2013, 4:23 PM
 */


#include <xc.h>
//#include <pic.h>

//__CONFIG(INTIO & WDTDIS  & MCLRDIS & UNPROTECT );
//__CONFIG(INTIO & WDTDIS & MCLRDIS & UNPROTECT );
__CONFIG(INTIO&WDTDIS&PWRTDIS&MCLREN&UNPROTECT&DUNPROTECT);
__CONFIG(BOREN&IESODIS&LVPDIS&FCMDIS);


void msecbase (void);
void pause (unsigned short usvalue);

int main(void) {
    ANSEL=0;
    CM1CON0=0;
    CM2CON0=0;

    PORTD=0X00;
    TRISD=0X00;

    RD0=1;
    RD1=1;

    while (1==1)
    {
        RD0=1;
        RD1=0;
        pause(500);

        RD0=0;
        RD1=1;
        pause(500);
    }

    return 0;
}

void pause (unsigned short usvalue)
{
    unsigned short x;
    //unsigned int counter = 65535;
    for (x=0; x<=usvalue; x++)
    {
        msecbase();
    }

    /*do
    {
        counter = counter - 1;
    }
    while (counter>0);
     */

}

void msecbase (void)
{
    OPTION_REG = 0b00000001; // no livro era apenas OPTION
    TMR0=0x0D;
    while(!T0IF);
    T0IF=0;
}

