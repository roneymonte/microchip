//******************************************************************************
//Kit de desenvolvimento ACEPIC 18_28
//Projeto: comp.c
//Descrição: compara as tensões nos trimpots CMP1 e CMP2, se a tensão em CMP2
//             for maior que em CMP1, acende o LED L4
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON as chaves 5 (RA5-MCLR) e 8 (LED) do DIP SW1
//      e 1 (RA1-CMP1) e 4 (RA2-CMP2) do DIP SW2
//******************************************************************************

#include <16F628A.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,PUT,NOBROWNOUT,NOLVP

#define LED         PIN_B3         // Define a saída para o led   

void Inicializa(void);               // Prototipagem da função de Inicialização

void main(void)
{
Inicializa();
Output_B(0X00);

while(true)                  // Entra em loop infinito
     {
     if (C2OUT)
        {output_high(LED);   // Se V2 > V1 liga o led
        } 
     else {output_low(LED);} // senão apaga o led
      }
}      

void Inicializa(void)
{
setup_comparator(NC_NC_A1_A2);   //Configura RA1 e RA2 como entradas para o comparador
}   
