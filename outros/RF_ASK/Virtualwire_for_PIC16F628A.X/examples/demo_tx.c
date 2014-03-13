#include <htc.h>
#include "..\virtualwire.h"

/*
 
 * Demonstracao de biblioteca TX do Virtual Wire
 * adaptada para PIC18F4550
 * 
 * O modulo RX esta rodando num Arduino com a propria Sketch
 * de exemplo de recepcao. O Baud Rate foi setado em ambos
 * com o valor de 600 bps
 * 
 * Esta funcionando corretamente agora.
 * 
 * Versao 1.0
 * 
 * Adaptacao por Roney Monte (em 13-marco-2014)
 
 */


//__CONFIG(MCLREN & PWRTEN & BOREN & LVPDIS & WDTDIS & INTIO);

#include "../configbits.h"
#define _XTAL_FREQ 4000000


void interrupt global_isr(void)
{
	if(T0IF)
		vw_isr_tmr0();
}

void delay(unsigned int delay)
{
	while(delay--);
}

void main(void)
{

    TRISD6=0;
    TRISD7=0;

	const char text[] = "Hello from PIC ";

    CMCON = 0x07;	// analog comparator disabled
//	VRCON = 0x00;	// voltage reference module disabled
    CVRCON=0x00;

	vw_setup(600); // era 600

	while(1)
	{
            LATDbits.LATD6=1;
            LATDbits.LATD7=0;
		vw_send(text, sizeof(text)-1);
                Delay10KTCYx(200);
                vw_send("Testando T",10);
                Delay10KTCYx(200);
            LATDbits.LATD6=0;
            LATDbits.LATD7=1;
                delay(40000);
	}
	
}
