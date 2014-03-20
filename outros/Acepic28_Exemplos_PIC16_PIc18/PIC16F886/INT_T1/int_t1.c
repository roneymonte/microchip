//******************************************************************************
//Kit de desenvolvimento ACEPIC 18_28
//Projeto: int_t0.c
//Descrição: Ao pressionar o botão TMR0 por 12 vezes, será gerada a interrupção
//           pelo sinal externo no TIMER0 fazendo com que o LED se
//           alternem entre aceso e apagado dependendo   
//Desenvolvimento: Eng.: Carlos Eduardo Sandrini Luz
//                 ACEPIC Tecnologia e Treinamento LTDA
//
//Obs.: Colocar em ON as chaves 6 (MCL1-MCLR) e 8 (LED) do DIP SW2
//******************************************************************************

#include <16F886.h>
#use delay(clock=4000000)
#fuses INTRC_IO,NOWDT,PUT,NOBROWNOUT,NOLVP

#bit TIMER1IF = 0x0c.0         //Define o bit 0 do resgistrador PIR1 como TIMER1IF

int led=0;         //Declara e inicia variável leds


void main ()
{
OUTPUT_B(led);       // Apaga todos os leds da porta D 

/* Configura o Timer 1 para contagem interna e prescaler de 1:8 */
SETUP_TIMER_1(T1_EXTERNAL|T1_DIV_BY_1);

/* Inicializa Timer 1 com o valor 3036 */   
SET_TIMER1(65530);



while(true)                  //Loop principal
        {
        if (TIMER1IF)             //Se o bit T0IF for igual a 1
           {
           TIMER1IF = 0;      //Faz o bit igual a 0
           SET_TIMER1(65530);   //Retoma o valor inicial do Timer 0
           OUTPUT_B(led=~led);      //Inverte a situação dos Leds
           }
       }
}
