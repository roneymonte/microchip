/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 24, 2013, 2:52 PM
 */


#include <xc.h>

#include <stdio.h>
#include "usart.h"

/* Pic16f887 configuration */

//#pragma config FOSC = INTRC_NOCLKOUT // define oscilador INTERNO sem CLOCK OUT
//#pragma config FOSC = INTRC_CLKOUT // define oscilador INTERNO sem CLOCK OUT
//#pragma config FOSC = INTOSCIO // define oscilador INTERNO sem CLOCK OUT
//#pragma config FOSC = 100

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

__CONFIG(INTIO);

int main(void) {

    unsigned char input;

    ANSEL=0;
    ANSELH=0;
    CM1CON0=0;
    CM2CON0=0;

    INTCON=0;   // DESABILITA AS INTERRUPCOES

    init_comms();    // prepara a USART com configuracoes definidas em usart.h

    while (1==1)
    {
        unsigned char count;
        for ( count=0x31; count <= 0x39; count=count+1)
        {
            putch(count);
            input = getch();
            putch(input);

        }
        while (1)
        {
            printf("Press a key and I will echo it back:\r\n");
            input = getch();
            printf("I detected [%c]\r\n",input);

        }
    }

    return 0;
}
