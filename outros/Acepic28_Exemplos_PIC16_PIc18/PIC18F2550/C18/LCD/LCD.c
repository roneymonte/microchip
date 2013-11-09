/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Teste LCD                                                           *
********************************************************************************/

//#include <pic18F2525.h>
#include <p18f2525.h>
#include <delays.h>
#include <stdio.h>
#include "configuracoes.h"


// Frequencia do oscilador = 8MHz
// Ciclo de máquina  = 1/(Fosc/4) = 0,5us

/*
//#pragma config FOSC = HS       		//Configura o oscilador a cristal
//#pragma config CPUDIV = OSC1_PLL2 	//Postscale clock do sistema
#pragma config WDT = OFF  			//Desabilita o Watchdog Timer (WDT).
#pragma config PWRT = ON   			//Habilita o Power-up Timer (PWRT).
//#pragma config BOR = ON				//Habilita Brown-out reset
#pragma config BORV = 1	        	//Tensão do BOR é 4,33V.
#pragma config LVP = OFF       		//Desabilita o Low Voltage Program.
*/

#include "lcd_4b.h"

char buf_lcd[16];		//matriz de armazenamento da mensagem para o LCD  
 

void main()
{
int i;
ADCON1 = 0xFF;

lcd_inicio();

while (1)
        {          
        sprintf(buf_lcd,"    ACEPIC28    ");
        lcd_posicao(1,1);                         //Posiciona o cursor do LCD na coluna 1 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD 

        sprintf(buf_lcd,"   PIC18F2550   ");
        lcd_posicao(2,1);                         //Posiciona o cursor do LCD na coluna 2 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD 
          
        for (i=0;i<20;i++) Delay10KTCYx(20);  //Atraso de 2 segundos

        sprintf(buf_lcd,"   PIC18F2550   ");
        lcd_posicao(1,1);                         //Posiciona o cursor do LCD na coluna 2 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD

        sprintf(buf_lcd,"    ACEPIC28    ");
        lcd_posicao(2,1);                         //Posiciona o cursor do LCD na coluna 1 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD           
        
        for (i=0;i<20;i++) Delay10KTCYx(20);  //Atraso de 2 segundos
        }
}