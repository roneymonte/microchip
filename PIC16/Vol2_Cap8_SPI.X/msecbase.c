#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

void msecbase (void)
{
    OPTION_REG=0b00000010;
    TMR0=0xD;
    while (!T0IF);
    T0IF=0;
}

