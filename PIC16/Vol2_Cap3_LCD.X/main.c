/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 14, 2013, 6:55 PM
 */

/*#define _LEGACY_HEADERS //Added for compiler versions 9.81+
#include <htc.h> */

#define _XTAL_FREQ 250000

#include <xc.h>
#include "lcd.h"

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

#define number 0x30


void pause (unsigned short usvalue);



unsigned char b0;



int main(void) {

    ANSEL=0;
    ANSELH=0;
    CM1CON0=0;
    CM2CON0=0;
    TRISA=0;
    TRISC=0;

    lcd_init();

    while (1==1)
    {
        lcd_clear();
        pause(100);
        lcd_goto(0);
        pause(1);
        for (b0=0; b0<10; b0=b0+1)
        {
            lcd_putch( b0 + number );
            pause(2);
        }
        lcd_goto(0x40);
        pause(2);

        _delay(4);

        __delay_us(1000);




        lcd_puts("Hello World");
        pause(20);
        lcd_puts("_EOF");
        pause(10);
    }

    return 0;
}

