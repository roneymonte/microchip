/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Acionamento dos Led's conectados à porta B                          *
*Obs.:     Ligar chave 8 do DIP DP1                                            * 
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
short i, led;

led = 1;

TRISB = 0b00000000;            	//Direciona todos os bits da porta D como saída

while(1)
         {
          for (i=0;i<=4;i++)
							{
							PORTB = 0x00;
                            Delay10KTCYx(20);   	//Gera um delay de 100ms
                            PORTB = 0xFF;
                            Delay10KTCYx(20);   	//Gera um delay de 100ms
							}
          PORTB = 0x00;
          for (i=0;i<=7;i++)
							{
							Delay10KTCYx(20);   	//Gera um delay de 100ms
                            PORTB = led;
							led = led<<1;
       						} 
          led = led >> 1;
          for (i=0;i<=7;i++)
							{
							Delay10KTCYx(20);   	//Gera um delay de 100ms
                            PORTB = led;
							led = led>>1;
       						} 
 
          led = 1;    
          Delay10KTCYx(20);   	//Gera um delay de 100ms                   	 
          
         }
}