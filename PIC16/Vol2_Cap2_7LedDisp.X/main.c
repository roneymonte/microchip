/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 14, 2013, 12:19 PM
 */


#include <xc.h>
const zero = 0b00000010;
const one =  0b01111010;
const two =  0b00100001;
const tree = 0b00110000;
const four = 0b01011000;
const five = 0b00010100;
const six =  0b00000100;
const seven= 0b00111010;
const eight= 0b00000000;
const nine = 0b00010000;

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

void pause(unsigned short msvalue);

int main(void) {

    unsigned char state_led=0;

    ANSEL=0;
    ANSELH=0;
    CM1CON0=0;
    CM2CON0=0;

    PORTC=0X00;
    TRISC=0X00;

    while (1==1)
    {
        state_led++;
        switch (state_led)
        {
            case 1:
                PORTC=one;
                break;
            case 2:
                PORTC=two;
                break;
            case 3:
                PORTC=tree;
                break;
            case 4:
                PORTC=four;
                break;
            case 5:
                PORTC=five;
                break;
            case 6:
                PORTC=six;
                break;
            case 7:
                PORTC=seven;
                break;
            case 8:
                PORTC=eight;
                break;
            case 9:
                PORTC=nine;
                break;
            default:
                state_led=0;
                PORTC=zero;
                break;



        }
        pause(150);
    }

    return 0;
}
