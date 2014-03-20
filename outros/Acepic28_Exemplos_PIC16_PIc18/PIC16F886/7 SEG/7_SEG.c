//******************************************************************************
//Kit de desenvolvimento ACEPIC 18_28
//Projeto: Usart.c
//Descrição: recebe um caracter pela serial e o envia novamente
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON a chave 6 (MCL1-MCLR) do DIP SW2 e
//      as chaves 3 (RA1-DSP1), 5 (RA2-DSP2) e 6 (RA3-DSP3) do DIP SW1
//******************************************************************************

#include <16F886.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,PUT,NOBROWNOUT,NOLVP,MCLR


int conta=0;
int i;
int16 d1, d2, d3;

/*Matriz de 10 posições contendo o valor a ser enviado para a porta D a fim de
   mostrar o dígito referente nos displays */
int digito[10] =
               {0b00111111,    //Dígito 0
                0b00000110,    //Dígito 1
                0b01011011,    //Dígito 2
                0b01001111,    //Dígito 3
                0b01100110,    //Dígito 4
                0b01101101,    //Dígito 5
                0b01111101,    //Dígito 6
                0b00000111,    //Dígito 7
                0b01111111,    //Dígito 8
                0b01101111     //Dígito 9
               };

#INT_TIMER0
void trata_t0()
{
conta++;
if (conta>3) conta = 1;

switch(conta)
          {
         case 1: OUTPUT_B(digito[d1]);
                 OUTPUT_A(0X02);
                       break; 
         case 2: OUTPUT_B(digito[d2]);
                 OUTPUT_A(0X04);
                       break; 
         case 3: OUTPUT_B(digito[d3]);
                 OUTPUT_A(0X08);
                       break; 
         } 

SET_TIMER0(100); 
}


void main()
{
ENABLE_INTERRUPTS(GLOBAL | INT_TIMER0);
SETUP_TIMER_0(RTCC_INTERNAL | RTCC_DIV_16);
SET_TIMER0(100);

while(true)
     {
     i++;
     if (i>100) i = 0;
     
     d1 = i/100;
     d2 = (i/10) % 10;
     d3 = (i/1) % 10;
     
     delay_ms(500);    
     }
}
