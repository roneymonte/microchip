/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 18, 2013, 2:18 PM
 */


#include <xc.h>
#include <htc.h>

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


#include "lcd1.h"
#include "pause1.h"
#include <eeprom_routines.h>

unsigned short int B0,B1;

int main(void) {

    ANSEL=0;
    ANSELH=0;
    CM1CON0=0;
    CM2CON0=0;

    lcd_init();
    while(1==1)
    {
        for(B0=0;B0<10;B0=B0+1)
        {
            
            EEPROM_WRITE(B0,B0);
            pause(10);

        }

        for(B0=0;B0<10;B0=B0+1)
        {
            B1=EEPROM_READ(B0);
            lcd_clear;
            lcd_goto(0);
            lcd_puts("Value Read:");
            lcd_putch(B1 + 0x30);
            pause(500);

        }
    }


    return 0;
}
