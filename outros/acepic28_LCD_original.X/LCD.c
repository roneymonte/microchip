/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Teste LCD                                                           *
********************************************************************************/


#include <p18f2525.h>
#include <delays.h>
#include <stdio.h>

//#include "configuracoes.h"


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
#define FOSC 8000000

#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)

#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTRB = OFF

#include "lcd_4b.h"

char buf_lcd[16];		//matriz de armazenamento da mensagem para o LCD  
 

void main()
{
int i;
ADCON1 = 0xFF;


lcd_inicio();

while (1)
        {          
        //sprintf(buf_lcd,"    ACEPIC28    ");
        sprintf(buf_lcd,"aB");
        lcd_posicao(1,1);                         //Posiciona o cursor do LCD na coluna 1 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD 

        //sprintf(buf_lcd,"   PIC18F2550   ");
        sprintf(buf_lcd,"Bc");
        lcd_posicao(2,1);                         //Posiciona o cursor do LCD na coluna 2 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD 
          
        for (i=0;i<20;i++) Delay10KTCYx(20);  //Atraso de 2 segundos

        //sprintf(buf_lcd,"   PIC18F2550   ");
        sprintf(buf_lcd,"Cd");
        lcd_posicao(1,1);                         //Posiciona o cursor do LCD na coluna 2 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD

        sprintf(buf_lcd,"De");
        lcd_posicao(2,1);                         //Posiciona o cursor do LCD na coluna 1 da linha 1
     	lcd_escreve_bf(buf_lcd);                  //Envia a variável buf_lcd para o LCD           
        
        for (i=0;i<20;i++) Delay10KTCYx(20);  //Atraso de 2 segundos
        }
}