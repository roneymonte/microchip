/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Teste das chaves                                                    *
********************************************************************************/
#include <p18F2550.h>
#include <delays.h>

// Frequencia do oscilador = 8MHz
// Ciclo de máquina  = 1/(Fosc/4) = 0,5us

#pragma config FOSC = HS       		//Configura o oscilador a cristal
#pragma config CPUDIV = OSC1_PLL2 	//Postscale clock do sistema 
#pragma config WDT = OFF  			//Desabilita o Watchdog Timer (WDT).
#pragma config PWRT = ON   			//Habilita o Power-up Timer (PWRT).
#pragma config BOR = ON				//Habilita Brown-out reset 
#pragma config BORV = 1	        	//Tensão do BOR é 4,33V.
#pragma config LVP = OFF       		//Desabilita o Low Voltage Program.


void main()
{
TRISA = 0b00010100;
TRISC = 0b00000011;
TRISB = 0b00000000;

PORTB = 0b00000000;

ADCON1 = 0x0F;

while(1)
	{
	if(PORTAbits.RA2)
		{
        PORTBbits.RB0 = ~PORTBbits.RB0;
        while(PORTAbits.RA2);
		}
    if(PORTAbits.RA4)
		{
        PORTBbits.RB1 = ~PORTBbits.RB1;
        while(PORTAbits.RA4);
		}
    if(PORTCbits.RC0)
		{
        PORTBbits.RB2 = ~PORTBbits.RB2;
        while(PORTCbits.RC0);
		}
    if(PORTCbits.RC1)
		{
        PORTBbits.RB3 = ~PORTBbits.RB3;
        while(PORTCbits.RC1);
		}
		
	}

}