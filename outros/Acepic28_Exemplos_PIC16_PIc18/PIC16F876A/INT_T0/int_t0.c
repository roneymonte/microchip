//******************************************************************************
//Kit de desenvolvimento ACEPIC 18_28
//Projeto: int_t0.c
//Descrição: Ao pressionar o botão TMR0 por 12 vezes, será gerada a interrupção
//           pelo sinal externo no TIMER0 fazendo com que o LED se
//           alternem entre aceso e apagado dependendo   
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON as chaves 6 (MCL1-MCLR), 7 (LD0-LED1) e 8 (LED) do DIP SW2
//      e chaves 7 (RA6-OSC1) e 8 (RA7-OSC2)
//******************************************************************************

#include <16F876A.h>
#use delay(clock=8000000)
#fuses HS,NOWDT,PUT,NOBROWNOUT,NOLVP

#bit T0IF = 0x0b.2  //Define o bit 2 do resgistrador INTCON como T0IF

int leds=0;         //Declara e inicia variável leds

void main ()
{
OUTPUT_B(0x00);       // Apaga todos os leds da porta D 

/* Configura o Timer 0 para contagem interna e prescaler de 1:256 */
SETUP_TIMER_0(RTCC_EXT_H_TO_L | RTCC_DIV_2);

/* Inicializa Timer 0 com o valor 61 */   
SET_TIMER0(250);

while(true)               //Loop principal
        {
   if (T0IF)             //Se o bit T0IF for igual a 1
           {
           T0IF = 0;            //Faz o bit igual a 0
           SET_TIMER0(250);         //Retoma o valor inicial do Timer 0
           OUTPUT_B(leds=~leds);      //Inverte a situação dos Leds
           } 
        }
}

