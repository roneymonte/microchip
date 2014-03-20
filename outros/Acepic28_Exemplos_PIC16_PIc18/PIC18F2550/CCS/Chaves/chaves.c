/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              *
*                                                                              *
*Objetivo: Leitura dos pinos RA2, RA4, RC0 e RC1, chaves B1, B2, B3 e B4       *
*Obs.: Ligar 8 do DIP1  e chaves 7 (RA6-OSC1) e 8 (RA7-OSC2) do DIP2           *  
*******************************************************************************/
#include <18F2550.h>
#use delay(clock=8000000)
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP, CPUDIV1 

#use fast_io(A)
#use fast_io(B)
#use fast_io(C)

#define LED1 PIN_B0
#define LED2 PIN_B1
#define LED3 PIN_B2
#define LED4 PIN_B3

#define B1   PIN_A2
#define B2   PIN_A4
#define B3   PIN_C0
#define B4   PIN_C1


void main()
{
Set_tris_A(0b00010100);
Set_tris_B(0b00000000);
Set_tris_C(0b00000011);

Output_B(0b00000000);


while(true)
     {
     if (input(B1))
         {
         output_toggle(LED1);
         delay_ms(300);
         }
     if (input(B2))
         {
         output_toggle(LED2);
         delay_ms(300);
         }
     if (input(B3))
         {
         output_toggle(LED3);
         delay_ms(300);
         }
     if (input(B4))
         {
         output_toggle(LED4);
         delay_ms(300);
         }    
     }
} 

