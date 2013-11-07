#include <xc.h>


void msecbase (void)
{
    OPTION_REG=0b00000001;
    TMR0=0xd;
    while(!T0IF);
    T0IF=0;
}