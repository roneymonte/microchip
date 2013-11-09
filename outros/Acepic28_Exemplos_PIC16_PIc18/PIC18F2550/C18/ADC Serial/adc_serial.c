/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Leitura dos canais analógicos AN0 e AN3 com envio serial.           *
*Obs.:     Ligar chave 1 do DIP DP1 para Trimpot ADC1 para conexão ao canal    *
*          analógico AN0 e chave 3 do DIP conecta o Sensor de Temperatura LM35 *
*          ao canal analógico AN3.                                             *
*		   A chave 6 do DIP DP2 conecta o circuito de aquecimento ao pino RC2. *
********************************************************************************/
#include <p18F2550.h>
#include <delays.h>
#include <usart.h>
#include <stdio.h>

// Frequencia do oscilador = 8MHz
// Ciclo de máquina  = 1/(Fosc/4) = 0,5us

#pragma config FOSC = HS       		//Configura o oscilador a cristal
#pragma config CPUDIV = OSC1_PLL2 	//Postscale clock do sistema 
#pragma config WDT = OFF  			//Desabilita o Watchdog Timer (WDT).
#pragma config PWRT = ON   			//Habilita o Power-up Timer (PWRT).
#pragma config BOR = ON				//Habilita Brown-out reset 
#pragma config BORV = 1	        	//Tensão do BOR é 4,33V.
#pragma config LVP = OFF       		//Desabilita o Low Voltage Program.

unsigned long res_ad;	//variável de armazenamento da conversão AD
unsigned long res_temp;	//variável de armazenamento da conversão AD
unsigned long tensao;	//variavel de resultado em tensão
unsigned long temp;     //variável de resultado da temperatura

void main()
{
int i;

TRISAbits.TRISA0 = 1;   //Direciona o pino 0 da porta A como entrada
TRISAbits.TRISA3 = 1;   //Direciona o pino 3 da porta A como entrada
TRISCbits.TRISC2 = 0;	//Direciona o pino 2 da porta C como saída (Aquecimento)
TRISAbits.TRISA4 = 1;   //Direciona o pino 4 da porta A como entrada (B2) 

OpenUSART(USART_TX_INT_OFF		//desabilita interrupção de transmissão
			& USART_RX_INT_OFF	//desabilita interrupção de recepção
			& USART_ASYNCH_MODE	//modo assíncrono
			& USART_EIGHT_BIT		//transmissão e recepção em 8 bits
			& USART_BRGH_HIGH,	//Baud Rate em alta velocidade
			25);					//SPBRG p/ 19200 bps 
                   
ADCON2 = 0b10100001;     /*ADFM1 = 1 -> Resultado da conversão AD
                                    ... justificado à direita
                                    -
                                    ****Velocidade de aquisição em 8TAD
                                    ACQT2 = 1
                                    ACQT1 = 0
                                    ACQT0 = 0
                                    ****Fonte de clock em Fosc/8
                                    ADCS2 = 0
                                    ADCS1 = 0
                                    ADCS0 = 1*/        
 
  
ADCON1 = 0b00001011;      /* -
                           	 -
                             VCFG1 = 0 -> Vref- = terra
                             VCFG0 = 1 -> Vref+ = VDD
                             *****Seleciona os canais AN0 e AN3 como analógicos
                             PCFG3 = 1
                             PCFG2 = 1
                             PCFG1 = 0
                             PCFG0 = 1*/                                                 


while(1)
	{
    if (PORTAbits.RA4)		//Se Chave B2 foi pressionada
		{
		PORTCbits.RC2 = 1;	//Liga circuito de aquecimento
  		}      

     ADCON0 = 0b00000001;  		/*Seleciona Canal 0 para conversão
                                  ...e habilita o conversor AD*/
     Delay1KTCYx(2);			//Atraso de 1ms
     ADCON0bits.GO = 1;         //Inicializa a conversão
     while(ADCON0bits.GO);      //Aguarda o término da conversão
 
     res_ad = ADRES;            //atribui o valor convertido à variável res_ad0
 
     tensao = (res_ad*5000) / 1023;    //Calcula o valor em mV para o resultado obtido     
 
     ADCON0 = 0b00001101;         /*Seleciona Canal 3 para conversão        
                                   ...e habilita o conversor AD*/

     Delay1KTCYx(2);              //Atraso de 1ms
 
     ADCON0bits.GO = 1;           //Inicializa a conversão
     while(ADCON0bits.GO);        //Agurarda o término da conversão
 
     res_temp = ADRES;            //atribui o valor convertido à variável res_temp
 
     temp = (res_temp*500) / 1023;    //Calcula o valor em mV para o resultado obtido

	 if (temp > 40)
		PORTCbits.RC2 = 0;			//Desliga circuito de aquecimento.
 
     
     printf("AN0 = %04lu mV - Temp. = %02lu C.\r\n", tensao, temp); //Envia a string
      
     Delay10KTCYx(200);                                //Atraso de 1000ms
     }  
}