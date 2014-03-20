//******************************************************************************
//Kit de desenvolvimento ACEPIC 18_28
//Projeto: led.c
//Descrição: faz piscar os Leds conectados à porta B em intervalos de 1 segundo
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON as chaves 6 (MCL1-MCLR), 7 (LD0-LED1) e 8 (LED) do DIP SW2
//******************************************************************************

#include <16F886.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,PUT,NOBROWNOUT,NOLVP

void main()
{  

while (true)
         {
         output_B(0xFF);   //Apaga todos os Led's da Porta D
         delay_ms(1000);   //Atraso de 1000ms (1 segundo)
         output_B(0x00);   //Acende Todos os Led's da Porta D
         delay_ms(1000);   //Atraso de 1000ms (1segundo)
         }   
}  

