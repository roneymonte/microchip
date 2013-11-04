/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 18, 2013, 3:39 PM
 */


#include <xc.h>


/* Pic16f887 configuration */

#pragma config FOSC = INTRC_NOCLKOUT // define oscilador INTERNO sem CLOCK OUT
#pragma config WDTE = OFF           // watch dog desligado
#pragma config PWRTE = OFF          // temporizador de powerUp desligado
#pragma config MCLRE = ON           // pino MCLR eh MCLR e nao funcao
#pragma config CP = OFF             // programa memory nao protegido
#pragma config CPD = OFF            // data memory nao protegida
#pragma config BOREN = ON           // BROWN-OUT inicializacao sem reset,
                                    // pelo nivel de voltagem minimo
#pragma config IESO = OFF           // internal/external switchOver
#pragma config LVP = OFF            // low voltage programming = precisa de
                                    // alta vontagem para programacao do chip
#pragma config FCMEN = OFF          // fail-safe clock monitor



unsigned char TOC;

void interrupt isr(void)
{
    if ( TMR1IF=1)
    {
        //TMR1H=0x0B;     // coloca 3035 decimal no timer 1
        //TMR1L=0xDB;     // usando o HEX 0x0BDB (hi e low)
                        // esse valor de TIMER gera 8 * 62.5 msec = 500 msec
                        // o "8 *" eh o prescaler de 1:8

        TOC=TOC+1;      // incrementa o contador de overflow
        TMR1IF=0;       // limpa o flag de overflow
    }
}

int main(void) {

    ANSEL=0;
    ANSELH=0;
    CM1CON0=0;
    CM2CON0=0;

    TRISA=0;    // barramando A como output
    TRISD=0;    // barramento D de acender os LEDs

    TMR1H=0x0B; // comentarios dentro da funcao de interrupcao
    TMR1L=0xDB;

    //T1CON= 0b00110000;  // coloca o Timer1 com pre-escala de 1:8
                        // na PIC16F690 ou na PIC16F887

    // mas vou querer esse prescaler com 1:2, para gerar 1/8 segundo
    T1CON= 0b00010000;


    PIE1=  0b00000001;  // habilita o Timer1 Interrupt somente
    INTCON=0b11000000;  // habilita interrupcoes GIE = 1. PIE = 1
    TMR1ON=1;           // liga o Timer1

    /* Ports inicialicacao*/

    PORTD=1;            // desliga todos os leds, ligando o primeiro 1 LED
    TRISD=0;            // redundancia, esse comando ja foi dado acima

    TOC=0;

    while (1==1)
    {
       

        if (PORTD==0b00000000) // no exemplo eram apenas 4 leds, entao PORTD>8 (4o bit)
        {
            PORTD=1;
        }

         if (TOC==2) // no exemplo eram apenas 4 leds, entao TOC==4
        {
            PORTD=PORTD<<1;
            TOC=0;
        }

    }

    return 0;
}


