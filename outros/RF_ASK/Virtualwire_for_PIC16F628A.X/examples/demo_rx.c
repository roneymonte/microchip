#include <htc.h>
#include "..\stdint.h"
#include "..\virtualwire.h"
#include "rs232.h"

__CONFIG(MCLREN & PWRTEN & BOREN & LVPDIS & WDTDIS & INTIO);

static bank1 uint8_t text[VW_MAX_MESSAGE_LEN];

void interrupt global_isr(void)
{
	if(T0IF)
		vw_isr_tmr0();
}

void main(void)
{
	uint16_t i;
	
	serialInit(9600);
	vw_setup(600);

	puts("PIC Receiver Demo\n");

	vw_rx_start();

	while(1)
	{
		if (vw_have_message())
		{
			uint8_t len = VW_MAX_MESSAGE_LEN;

			if (vw_recv(text, &len))
			{
				for (i = 0; i < len; i++)
					putchar(text[i]);

				putchar('\n');
			}
		}
	}
	
}
