/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*                                                                              *
*Objetivo : Teste LCD                                                          *
*Obs.: Ligar as chaves 7 (RA6-OSC1) e 8 (RA7-OSC2) do DIP2                     *
*******************************************************************************/
#include <18F2550.h>
#use delay(clock=8000000)
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP, CPUDIV1 

#include "LCD_4B.c"

void main() {

  lcd_ini();
      
  printf(lcd_escreve,"\f   PIC18F2550");
  printf(lcd_escreve,"\nTEST LCD DISPLAY");

  while(true) {
               delay_ms(2000);
               printf(lcd_escreve,"\f"); //Limpa Display
               printf(lcd_escreve,"Test LCD Display");
               printf(lcd_escreve,"\n   PIC18F2550   ");
               delay_ms(2000);
               printf(lcd_escreve,"\f   PIC18F2550");
               printf(lcd_escreve,"\nTEST LCD DISPLAY");
              }

}
