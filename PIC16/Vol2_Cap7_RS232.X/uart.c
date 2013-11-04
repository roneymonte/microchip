#include <xc.h>
#include <stdio.h>
#include "usart.h"

void putch(unsigned char byte)
{
    while(!TXIF) continue;
    TXREG=byte;
}

unsigned char getch()
{
    while(!RCIF) continue;
    return RCREG;
}

void init_comms(void)
{
    RX_PIN=1;
    TX_PIN=0;
    SPBRG=baudsetting;

    RCSTA=0x90; // SPEN e CREN bit = 1, RX9EN = 0
    TXSTA=0x20; // TXEN=1, BRGH, SYNC = 0
    BAUDCTL=0;  // BRG16=0

}