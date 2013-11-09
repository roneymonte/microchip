/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo: Acionamento dos Led's conectados à porta B                          *
*Obs.:     Ligar chave 8 do DIP DP1 e chaves 7 (RA6-OSC1) e 8 (RA7-OSC2) do    * 
*          DIP2                                                                * 
********************************************************************************/
#include <18F2550.h>
#use delay(clock=8000000)
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP, CPUDIV1

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

