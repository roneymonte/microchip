/*
 * File:   main.c
 * Author: roney
 *
 * Created on March 18, 2014, 2:37 PM
 * Versao Funcional em 21-mar-2014
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "configbits.h"
#include <plib/timers.h>
#include <plib/pwm.h>

#define LED_VERM PORTDbits.RD7  // Led recebera pisca-pisca via software while
#define LED_AMAR PORTDbits.RD6  // Led recebera pisca-pisca via software while
#define LED_BRCO PORTDbits.RD3  // Led recebera PISCA 1 Segundo via TIMER1 16bit
#define LED_VERD PORTDbits.RD2  // Led recebera PISCA 0,1 Segungo via TIMER0 8bit
// OBS: foi colocado um Led AZUL na portC2 (pino 17 da PIC18F4550) para PWM1

#define _XTAL_FREQ 4000000      // Cristal externo na frequencia de 4mhz
#define FOSC 4000000UL          // Oscilador funcionando na mesma freq de 4mhz
#define USE_AND_MASKS           // Forma de selecao de palavras pre-definidas
// nas bibliotecas implicitas do compilador XC8

/*
 *
 */

void interrupt global_isr(void);
void configTimers (void);

int main(int argc, char** argv) {

    unsigned int dutycycle=0;
    bool crescente=true;

    TRISD=0;     // PORTD inteiro em Output
    TRISCbits.RC2=0;    // Output do PWM1/P1A

    PORTD=0x00; // zera os leds dao PortD.

    LED_BRCO=0;
    LED_VERD=0;
    LED_VERM=0;
    LED_AMAR=1; // Inicialmente para fins de debug, acende o Led Amarelo

    configTimers(); //  Inicializa os Timers 0, 1 e 2 (PWM)

    while (1)
    {

        LED_AMAR=1;  LED_VERM=0;    // Pisca-Pisca
        __delay_ms(10);

                if (crescente)  dutycycle=dutycycle+4;  // incrementa a intensidade
                else            dutycycle=dutycycle-4;  // descrementa a intensidade

        LED_AMAR=0;  LED_VERM=1;    // Pisca-Pisca
        __delay_ms(10);

                SetDCPWM1(dutycycle);   // Atualiza o novo dutyCycle

                if (dutycycle<=4)       crescente=true; // Led Azul Acendendo
                else
                if (dutycycle >= 1019)  crescente=false; // Led Azul Apagando
    }

    return (EXIT_SUCCESS);  // nao necessario, porem default do main C.
}

void configTimers (void)
{
    //TIMER0
    OpenTimer0 (    TIMER_INT_ON &
            T0_8BIT &
            T0_EDGE_FALL &
            T0_PS_1_256 &
            T0_SOURCE_INT
            );              // Clock Interno, contador de 8 bits e Prescaler 1/256
    WriteTimer0 ( 0x3D );   // Gera um intervalo de 0,1 segundo a 4 mhz

    //TMR0ON=1;               // Liga o Timer0
    //extern volatile __bit TMR0ON              @ (((unsigned) &T0CON)*8) + 7;
    //#define               TMR0ON_bit          BANKMASK(T0CON), 7

    TMR0IF=0;               // Zera o Overflow do Timer0
    TMR0IE=1;               // Habilita a Interrupcao para o Overflow do Timer0

    //////////////////////////////////////////////////////////////////////

    // TIMER1
    OpenTimer1(TIMER_INT_ON &
               T1_SOURCE_EXT &
               T1_SYNC_EXT_OFF &
               T1_PS_1_1 &
               T1_OSC1EN_ON &
               T1_16BIT_RW
               );
    //WriteTimer1( 0xBE5  );   // equivalemnte a 1/2 segundo em 4,00 mhz
    WriteTimer1( 0x8000 );   // equivalente a 1 segundo em 32,768 khz

    //TMR1ON = 1;   //  Liga o Timer1 -> Igual ao TIMER_INT_ON
    TMR1IF = 0;     // Zera o Overflow para o Timer1
    TMR1IE=1;       // Habilita Interrupcao para o Overflow do Timer1
    //////////////////////////////////////////////////////////////////////

     // PWM TIMER2
     OpenPWM1(0x7C);    // Inicializa o PWM com intervalo de 2khz e PS_1/16
                        // num clock de 4 mhz
     SetDCPWM1(0x000F);  // Configura o Duty Cycle Inicial

     OpenTimer2(
             TIMER_INT_OFF &
             T2_PS_1_16
             );             // Para o PWM funcionar corretamente, a Interrupcao
                            // TIMER_INT_OFF (ou TMR2IE) deve ser desabilitada

     // obs: como o PWM funciona como um "temporizador", nao precisa executar
     // interrupcao ou acao; a propria porta de saida PWM1 ja executa

     //TMR2=0;    // Clear Timer2 -> Nao necessario para este exemplo
     //T2CKPS1=1; // Pre-Scaler 16 (ja setado no T2_PS1_16), redundante

     SetOutputPWM1( SINGLE_OUT , PWM_MODE_1 );    // Configura o CCP1CON
     // apenas o PWM1: P1A modulated; P1B, P1C, P1D assigned as port pins
     // no modo PxA,PxC active high, PxB,PxD active high */

     //CCP1IE=1;  // Habilita Interrupcao no modulo CCP1, Nao Necessario
     //TMR2IF=0;  // Limpando o flag de overflow do Timer2, Nao Necessario
     TMR2ON=1;  // Ligando o Timer2
     //TMR2IE=0;    // -> TIMER_INT_OFF , redundante

     // Pisca Verde/Vermelho 2 vezes para sinalizar inicio do PWM
    __delay_ms(100);LED_VERD=1;__delay_ms(100);LED_VERD=0;
    __delay_ms(100);LED_VERM=1;__delay_ms(100);LED_VERM=0;
    __delay_ms(100);LED_VERD=1;__delay_ms(100);LED_VERD=0;
    __delay_ms(100);LED_VERM=1;__delay_ms(100);LED_VERM=0;

    PEIE=1; //  Habilita As Interrupcoes dos Perifericos
    GIE=1;  //  Habilita as Interrupcoes Globais
}

void interrupt global_isr(void)
{
    if (TMR1IE && TMR1IF)
    {
        LED_BRCO=~LED_BRCO;         // gera uma mudanca de estado LED Branco

        //WriteTimer1( 0x8000 );    // Nao eh necessario re-escrever o valor
                                    // de Overflow para o Timer1

        TMR1IF=0;                   // Limpa o status de Overflow do Timer1
        //TMR1IE=1;                 // Nao eh necessario re-habilitar
                                    // a Interrupcao
    }

    if (TMR0IE && TMR0IF)
    {
         LED_VERD=~LED_VERD;        // gera uma mudanca de estado LED Verde

        //WriteTimer0 ( 0x3D );     // Nao eh necessario re-escrever o valor
                                    // de Overflow para o Timer0
        TMR0IF=0;                   // Limpa o status de Overflow do Timer0
        //TMR0IE=1;                 // Nao eh necessario re-habilitar
                                    // a Interrupcao
    }

    //if(CCP1IF)
    //{//interrupção do ccp1
    //  CCP1IF = 0;//não será usado
    //}

}
