/*
 * File:   main.c
 * Author: roney
 *
 * Created on June 22, 2013, 7:03 PM
 */


#include <xc.h>
//#include <htc.h>
#include "pause.h"

#pragma config FOSC = XT
#pragma config WDTE = OFF           // watch dog desligado
#pragma config PWRTE = OFF          // temporizador de powerUp desligado
#pragma config CP = OFF             // programa memory nao protegido
#pragma config CPD = OFF            // data memory nao protegida
#pragma config BOREN = ON           // BROWN-OUT inicializacao sem reset,
                                    // pelo nivel de voltagem minimo
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000

//ANSEL=0;
//ANSEL_H=0;
//CM1CON0=0;
//CM2CON0=0;

/* PWM Setup */





void main(void) {

    TRISC=0xFF;
    CCP1CON=0;      // modulo CCP desligado

    //PR2=124;        // Periodo do PWM para Clock de 4 mhz
    PR2=249;        // Periodo do PWM para Clock de 8 mhz
    CCPR1L=0b00111110;  // Duty Cycle = 50% (decimal = 62)
    TMR2IE=0;       // Desabilita a Interrupcao do Timer2
    T2CKPS0=1;      // Coloca o Pre-Escaler do Timer2 em 16
    T2CKPS1=1;

    CCP1CON=0b00001100; // Configura o Modo PWM, colocando os
                        // DB Bits em 00 (decimal = 12)

    TMR2ON=1;       // Liga o Timer2

    while (TMR2IF==0)
    {}

    TRISC2=0;       // onde vai estar o PWM (led RC2 na placa AcePic28)

    /* A/D SetUp */

    TRISA=1;        // Coloca o pino RA0/AN0 como Input
    //ANSEL=0b00000001;   // Configura RA0 co o Analog to Digital (AD)


    ADCON0=0b00000000;  // Configura o AD para usar AN0, left justified

    while(1)
    {
        ADON=1;     // liga o conversor AD
        GO_DONE=1;   // inicia a conversao AD no pino AN0
        while (GO_DONE==1)
            ;

        // Ajusta o PWM Duty Cycle com AN0 AD Value
        CCPR1L=ADRESH;

        //PR2=

        pause(250);
        
    }



    return;
}
