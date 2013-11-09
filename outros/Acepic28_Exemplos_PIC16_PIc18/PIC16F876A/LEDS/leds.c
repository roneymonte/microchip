//******************************************************************************
//Kit de desenvolvimento ACEPIC 28
//Projeto: led.c
//Descrição: faz piscar os Leds conectados à porta B em intervalos de 1 segundo
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON a chave 8 do DIP1  e chaves 7 (RA6-OSC1) e 8 (RA7-OSC2) do 
//      DIP2  
//******************************************************************************
#include <16F876A.h>
#use delay(clock=8000000)
#fuses HS,NOWDT,PUT,NOBROWNOUT,NOLVP

#use fast_io(B)

void main()
{
int i, led;

led = 1;
set_tris_b(0x00);
output_b(0x00);

while (true)
         {
         for (i=0;i<=4;i++) {
                             output_b(0x00);
                             delay_ms(100);
                             output_b(0xFF);
                             delay_ms(100);
                            }
         output_b(0x00);
         for (i=0;i<=7;i++) {
                             delay_ms(100);
                             output_b(led);
                             rotate_left(&led,1);
                            }
         rotate_right(&led,1);                   
         for (i=0;i<=7;i++) {
                             delay_ms(100);
                             output_b(led);
                             rotate_right(&led,1);
                             } 
         led = 1;                   
         delay_ms(100);
         }   
}  

