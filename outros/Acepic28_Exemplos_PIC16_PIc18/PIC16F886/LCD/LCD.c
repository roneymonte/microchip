//******************************************************************************
//Kit de desenvolvimento ACEPIC 18_28
//Projeto: LCD.c
//Descrição: Mostra as mensagens 'PIC16F628A' na primeira linha e 'Test LCD DISPLAY
//           na segunda linha, após 1,5 segundo, as linhas se invertem
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON a chave 6 (RA5-MCLR) do DIP SW2
//******************************************************************************

#include <16F886.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,PUT,NOBROWNOUT,NOLVP


#include "LCD_4B.c"

void main() {

  lcd_ini();
  
  //delay_ms(500);
  
  printf(lcd_escreve,"\f   PIC16F886");
  printf(lcd_escreve,"\nTEST LCD DISPLAY");

  while(true) {
               delay_ms(2000);
               printf(lcd_escreve,"\f"); //Limpa Display
               printf(lcd_escreve,"Test LCD Display");
               printf(lcd_escreve,"\n   PIC16F886   ");
               delay_ms(2000);
               printf(lcd_escreve,"\f   PIC16F886");
               printf(lcd_escreve,"\nTEST LCD DISPLAY");
              }

}
