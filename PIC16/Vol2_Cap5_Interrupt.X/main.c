/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 18, 2013, 2:35 PM
 */


#include <xc.h>
#include <htc.h>
//#include <pic16f887_legacy.h>

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


#define LED RD7
#define LED1 RC1
////#define BOTAO RB0

int main(void)
{
    ANSEL=0;    // setta o barramento A como Digital
    ANSELH=0;   // setta o barramento B como Digital
    CM1CON0=0;
    CM2CON0=0;

    
    TRISD=0; // barramento de LEDs da placa 44Debug = OUTPUT
    TRISC=0; // conexao com o DISPLAY 7 SEGs = OUTPUT

    PORTD=0b10101010;
    PORTC=0b10101010;

    TRISB=0xFF; // barramento B onde tem o PUSH-BOTTON
                // sendo igual a 0xFF fica inteiro de INPUT

    LED =1;
    LED1=0;

    //RABPU=0;
    nRBPU=0;    // Use Pull-Ups internos
                // para descobrir essa opcao, tive que clicar o mouse com CTRL
                // apertado em cima da palavra RBPU ou RAPBU, entao ele abriu
                // a definicao "pic16f887.h" indicando a palavra.
                // OBS: ver no FINAL deste codigo fonte, a transcricao do .h

    //OPTION_REG .7=0;

    WPUB0=1;    // Pull-UP na entrada B0

    INTEDG=0;   // Falling Edge Mode, Trigger the Interrupt
    INTE=1;     // Habilita a Interrupcao Externa
    GIE=1;      // Interrupcao Global Habilitada

    while (1==1)
    {

    }

    return 0;
}

void interrupt isr(void)
{
    if (INTF)
    {
        LED=1^LED;
        LED1=1^LED;
        INTF=0;
    }
}


/*
 *
 * Transcricao do arquivo pic16f887.h
 *
// Register: OPTION_REG
extern volatile unsigned char           OPTION_REG          @ 0x081;
#ifndef _LIB_BUILD
asm("OPTION_REG equ 081h");
#endif
// bitfield definitions
typedef union {
    struct {
        unsigned PS                     :3;
        unsigned PSA                    :1;
        unsigned T0SE                   :1;
        unsigned T0CS                   :1;
        unsigned INTEDG                 :1;
        unsigned nRBPU                  :1;
    };
    struct {
        unsigned PS0                    :1;
        unsigned PS1                    :1;
        unsigned PS2                    :1;
    };
} OPTION_REGbits_t;
 */