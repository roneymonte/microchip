#include <htc.h>
#include "..\virtualwire.h"

__CONFIG(MCLREN & PWRTEN & BOREN & LVPDIS & WDTDIS & INTIO);


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
	const char text[] = "Hello from PIC";

    CMCON = 0x07;	// analog comparator disabled
	VRCON = 0x00;	// voltage reference module disabled


	vw_setup(600);

	while(1)
	{
		vw_send(text, sizeof(text)-1);
		delay(20000);
	}
	
}
