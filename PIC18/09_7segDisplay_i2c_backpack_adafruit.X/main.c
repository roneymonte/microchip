/* Display 7 Segmentos via I2C Backpack (Produto Adafruit)
 *
 * "Adafruit 0.56" 4-Digit 7-Segment Display w/I2C Backpack - Red - ID: 878"
 *
 * por Roney Monte
 * MCU utilizada PIC18F2525
 *
 * Versao inicial - 30/mar/2014
 * Versao 0.1 - Exemplificacao com relogio digital, lendo a hora do relogio
 *              DS1307 via I2C, e exibindo no modulo 7-Segmentos I2C (ht16k33),
 *              e debug do log na serial EUSART (TX pino C6)
 *            - atualizacao dentro do Linux Ubuntu 12.04 com SVN v 1.7.9
 *              e MPLAB X IDE v2.05, Java: 1.7.0_25; Java HotSpot(TM) 64-Bit,
 *              Server VM 23.25-b01, System: Linux version 3.11.0-19
 * Versao 0.2 - 01/abr/2014 - sistema mais estabilizado, pisca ":" e contador
 *              de pausa via TIMER0
 *
 * Versao 0.3 - 02/abr/2014 - primeiros passos no Watchdog Timer e Sleep Mode
 *              apenas um esboco dos principais comandos e registradores, sem
 *              funcionar.
 *
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <plib/i2c.h>
#include <plib/usart.h>
#include <plib/timers.h>
#include "configbits.h"
#include <plib/dpslp.h>
#include <pic18.h>
#include <plib/reset.h>

#define _XTAL_FREQ 8000000
#define FOSC 8000000
#define Baud 100000     // velocidade do barramento SCL (clock I2C)

#define USE_AND_MASKS


#define HT16K33_BLINK_CMD       0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF       0
#define HT16K33_BLINK_2HZ       1
#define HT16K33_BLINK_1HZ       2
#define HT16K33_BLINK_HALFHZ    3

#define HT16K33_CMD_SETUP       0x21
#define HT16K33_CMD_BRIGHTNESS  0x0E
#define HT16K33_ADDR            0x70

#define LED_AMAR    PORTCbits.RC0
#define LED_VERD    PORTCbits.RC1
#define LED_VERM    PORTCbits.RC2

int hora, minuto, segundo;
const unsigned char numbertable[] =
{
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x77, /* a */
    0x7C, /* b */
    0x39, /* C */
    0x5E, /* d */
    0x79, /* E */
    0x71, /* F */
};


void initExt7SegLCD (void);
void getDS1307(void);
void configuracao_EUSART (void);
void piscaSeg (char a);
void pausa (unsigned int segundos);
void dormir (void);
void acordar (void);

void main (void)
{
    TRISC0=0;   // Led Amar
    TRISC1=0;   // Led Verd
    TRISC2=0;   // LED Verm
    TRISC6=0;   // saida TX EUSART
    ADCON1=0x0F;    // coloca pinos I/O em modo digital (nao analogico)

    acordar();

    unsigned int D0, D1, D2, D3, minAnterior, contador, segInicial;
    char msg[30];

    LED_VERM=1;
     // inicia porta serial com 9600 bps @ 8mhz

    initExt7SegLCD();   // inicia barramento I2C e escreve ".12:34."
    pausa(1); piscaSeg(0);

    // Inicio do Relogio e Display

    contador=60;    // o contador esta acima do limite (59)
    minAnterior=10; // o minuto nunca chegaria a 10 (forca display)

    while (1)
    {

        if (contador > 59)  // ira requisitar hora I2C somente 1 vez ao minuto
        {
        getDS1307();

            sprintf(msg," ___[%x:%x:%x]___", hora, minuto,segundo );
            while(BusyUSART());
            putsUSART( msg );

        D0 = (hora & 0b00110000) >> 4;  // utiliza somente 2 bits da nibble esq
        D1 = hora & 0b00001111;         // utiliza somente 4 bits da nibble dir
        D2 = (minuto & 0b11110000) >> 4;// utiliza somente 4 bits da nibble esq
        D3 = minuto & 0b00001111;       // utiliza somente 4 bits da nibble dir


        contador = segundo; //atualiza a contagem interna do loop com o seg real
        }

            /*
            sprintf(msg,"___ %d%d:%d%d ...%xseg_ \r\n",
                D0, D1, D2, D3 ,segundo);
            while(BusyUSART());
            putsUSART( msg );
             */

        if (D3 != minAnterior)  // se mudou o minuto, entao mostre novo horario
        {
            StartI2C();
                __delay_us(16); // delay experimental
                WriteI2C(HT16K33_ADDR << 1);

                WriteI2C(0x00);

                WriteI2C(numbertable[D0] & 0xff ); // preenche todos os bits
                WriteI2C(numbertable[D0] >> 8);    // qualquer byte
                // obs: acima como foram usados somente 2 bits, alguma "sujeira"
                // poderia ter ficado escrita no registrador, por isso a necessidade
                // de "preencher" os outros bits com & 0xff

                WriteI2C(numbertable[ D1 ] & 0xFF ); // preenche todos os bits
                WriteI2C(numbertable[ D1 ] >> 8);    // qualquer byte

                WriteI2C(0xFF); // escreve ":" sempre no segundo 0 de mostrar
                WriteI2C(0xFF >> 8);    // qualquer byte

                WriteI2C(numbertable[ D2 ] & 0xFF );// preenche todos os bits
                WriteI2C(numbertable[ D2 ] >> 8);    // qualquer byte

                WriteI2C(numbertable[ D3 ] & 0xFF );// preenche todos os bits
                WriteI2C(numbertable[ D3 ] >> 8);    // qualquer byte

                NotAckI2C();    // encerra envio
                __delay_us(16); // delay experimental
            StopI2C();
            minAnterior=D3; // variavel para proxima checagem e mudanca
        }

        pausa(1);   // utiliza TIMER0 para realizar contagem precisa de 1 seg
        contador++; // incrementa controle de contagem de segundos
        
        if ( (contador % 2) == 0) piscaSeg(1);  // gera a piscada ":" par
        else piscaSeg(0); // ou impar " ", a cada mudanca de contador (seg)

        
        sprintf(msg,"\r\nSeg=%d, Contador=%d",segundo,contador );
            while(BusyUSART());
            putsUSART( msg );


            // rotina abaixo de calculo de desvio de segundos
            // ainda deve ser corrigida para o algoritmo correto
        if (segundo < 50)
            if (contador > (segundo+10) ) 
                {contador=60;dormir();}

            else
                if ((contador<segundo)  &&
                    (contador > ( 60-segundo ) ) )
                    {contador=60; dormir;}
        
    }
}

void piscaSeg (char a)
{
        StartI2C();
            WriteI2C(HT16K33_ADDR << 1);
            WriteI2C(0x04); // posicao "dois pontos central" :
                if ( a == 1)
                    WriteI2C(0xFF); // dois pontos ":" aceso
                else
                    WriteI2C(0x00); // dois pontos ":" apagado
            NotAckI2C();
        StopI2C();
}

void initExt7SegLCD (void)
{
    TRISCbits.TRISC3=0;    // SCL do I2C colocado como saida por causa de bug*
    TRISCbits.TRISC4=0;    // SDA do I2C colocado como saida por causa de bug*
    LATC3=0;            // bug* pede que zere-se o LAT das portas SCL e SDA
    LATC4=0;            // durante inicializacao do I2C para evitar flutuacoes
                        // eletricas que ficariam nas portas antes de liga-las

    Delay10KTCYx(10);   // simples pausa para troca de estado na SDA e SCL

    TRISCbits.TRISC3=1;    // SCL do I2C, agora corretamente como saida
    TRISCbits.TRISC4=1;    // SDA do I2C, agora corretamente como saida

    SSPADD = ((FOSC/4)/Baud)-1; // na definicao inicial de FOSC e BAUD

    // parametros de interrupcao abaixo nao sao necessarios de especificar
    //PIR2bits.BCLIF=0;       // limpa o flag de COLISAO do barramento
    //PIR1bits.SSPIF=0;       // limpa o flag de interrupcao SSPIF
    //PIE2bits.BCLIE = 1;     // Enable bus collision interrupts
    //PIE1bits.SSPIE = 1;     // Enable MSSP interrupt enable bit

    CloseI2C(); //fecha qualquer previa sessao I2C pre-iniciada (nao necessario)
    OpenI2C(MASTER,SLEW_OFF);   // inicia I2C em modo Master e clock p/ 100 khz

    LED_AMAR=1;

    StartI2C();
            __delay_us(16); // delays de maneira experimental para testes
            WriteI2C(HT16K33_ADDR << 1);
            WriteI2C(HT16K33_CMD_SETUP);    // liga o display
            AckI2C();      
        StopI2C();

        IdleI2C();

        RestartI2C();   // na pratica nao esta funcionando para o Adafruit
            WriteI2C(HT16K33_ADDR << 1);
            WriteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_2HZ);
            __delay_us(16); // delays de maneira experimental para testes
        StopI2C();

        IdleI2C();

        RestartI2C();   // na pratica nao esta funcionando para o Adafruit
            __delay_us(16); // delays de maneira experimental para testes
            WriteI2C(HT16K33_ADDR << 1);
            WriteI2C(HT16K33_CMD_BRIGHTNESS | 7);
        StopI2C();

        RestartI2C();       // apenas valida o display colocando ".12:34."
            __delay_us(16); // delays de maneira experimental para testes
            WriteI2C(HT16K33_ADDR << 1);

            WriteI2C(0x00);

            WriteI2C(numbertable[1] | 0x80);    // numero 1 + ponto (0x80)
            WriteI2C(0x00);                     // qualquer caractere
  
            WriteI2C(numbertable[2]);           // numero 2
            WriteI2C(0x00);                     // qualquer caractere

            WriteI2C(0xFF);                     // dois pontos ":" ativado
            WriteI2C(0x00);                     // qualquer coisa


            WriteI2C(numbertable[3]);           // numero 3
            WriteI2C(0x00);                     // qualquer coisa

            WriteI2C(numbertable[4] | 0x80);    // numero 4 + ponto (0x80)
            WriteI2C(0x00);                     // qualquer coisa

            NotAckI2C();                        // finalizacao do envio
            __delay_us(16); // delays de maneira experimental para testes
        StopI2C();

        LED_VERD=1;
}

void getDS1307(void)
{
    // rotina para LER a hora do circuito DS1307 via I2C

    char msg [40];

    LED_AMAR=1;

    IdleI2C();

    StartI2C();

        __delay_us(16); // delay de maneira experimental para testes
        WriteI2C( 0xD0 );

        __delay_us(60); // delay de maneira experimental para testes
        WriteI2C( 0x00 );
        IdleI2C();
        __delay_us(16); // delay de maneira experimental para testes

    StopI2C();
    //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)

    //IdleI2C();
    __delay_us(26);

    RestartI2C();
        __delay_us(16); // delay de maneira experimental para testes

        WriteI2C( 0xD1 );
        __delay_us(1); // delay de maneira experimental para testes
        IdleI2C();

        segundo = ReadI2C();
        AckI2C();
        IdleI2C();

        minuto = ReadI2C();
        AckI2C();
        IdleI2C();

        hora = ReadI2C();
        NotAckI2C(); // finalizacao do envio

    StopI2C();

    LED_VERM = 0; LED_AMAR=0; LED_VERD=1;

    /*
    sprintf(msg,"\r\n%xh:%xm:%xs _ ",
        hora,minuto,segundo);
    while(BusyUSART());
    putsUSART( msg );
    */
    LED_VERD=0;
}

void configuracao_EUSART (void)
{
    CloseUSART();   // fecha qualquer USART que estaria supostamente aberta antes
                    // just closes any previous USART open port

    OpenUSART(  USART_TX_INT_OFF &
                USART_RX_INT_OFF &
                USART_ASYNCH_MODE &
                USART_EIGHT_BIT &
                USART_CONT_RX &
                USART_BRGH_LOW,
                12
                );
                // em 4 Mhz, com BRGH LOW (Bit Rate Generator LOW):
                // 51 = 1200bps; 25 = 2400bps; 6 = 9600bps (Err);

                // em 8 Mhz, com BRGH LOW (Bit Rate Generator LOW):
                // 103 = 1200bps; 25 = 4800bps; 51 = 2400bps; 12 = 9600bps
}

void pausa (unsigned int segundos)
{
    if (segundos <= 0) return;

    OpenTimer0( TIMER_INT_OFF &
                T0_16BIT &
                T0_PS_1_32 &
                T0_SOURCE_INT &
                T0_EDGE_FALL
            );

    __delay_ms(10); // delay nao necessario obrigatoriamente

    WriteTimer0( 0xBDC );   // tempo de 1 segundo em 8mhz e pre 1/32

    T0CONbits.TMR0ON = 1;     // Liga o Timer 0

    INTCONbits.GIE      = 0;    // Interrupcoes Globais desligadas
    INTCONbits.PEIE     = 0;    // Interrupcoes dos Perifericos desligadas
    INTCONbits.TMR0IE   = 0;    // Interrupcoes do Timer0 desligadas
    INTCONbits.TMR0IF   = 0;    // Zera o contador do Timer0

    while (segundos > 0)
    {
        while (!TMR0IF) ;       // enquando o Timer0 nao tiver overflow, espere

        LED_VERD=~LED_VERD;     // Led Amarelo pisca a cada segundo
        INTCONbits.TMR0IF=0;    // Zera o contador do Timer0
        segundos--;
    }

    CloseTimer0();              // Feche o Timer0
}

/* Funcoes do Deep Sleep:
 * 
 * void     DeepSleepWakeUpSource( SRC* ptr)
 * void     gotoDeepSleep( unsigned int config )
 * signed char IsResetFromDeepSleep( void)
 * void     ReadDSGPR( CONTEXT* ptr)
 * void     ULPWakeUpEnable( void)
 * 
 * Tipos de Wakeup/Reset (Wakeup source WKSRC):
 * DS_POR   //wake up source is Power on reset
 * DS_MCLR  //wake up source is master reset pin set to low
 * DS_RTC   //wake up source is rtcc alarm
 * DS_WDT   //wake up source is deep sleep watch dog timer expiry
 * DS_FLT   //wake up due to fault in deep sleep configuration corruption
 * DS_INT0  //wake up source is external interrupt
 * DS_BOR   //Brown out occured during deep sleep
 * DS_ULP   //wake up source is ultra low power wake up
 * DS_ICD   //wake up source is In-circuit debugger
 * DS_EXT   //wake up source is external interrupt
 * 
 * Registradores:
 * Reg0;                   //Stores the DSGPR0 register contents
 * Reg1;                   //Stores the DSGPR0 register content
 
 * Macros do Deep Sleep:
 * 
 * Write_DSGPR(data1,data2)
 * ReleaseDeepSleep()
 *
 * #define	ClrWdt()	asm(" clrwdt")
 * #define	Reset()		asm(" reset")
 * #define Sleep()		asm(" sleep")
 */

#include <plib/portb.h>
#include <plib/timers.h>
#include <plib/dpslp.h>
#include <plib/pconfig.h>
#include <plib/pps.h>
#include <plib/rtcc.h>
#include <plib/portb.h>

#define WATCHDOG
//#define DPSLP_V1_1
//#define DPSLP_ULPWU_ENABLE

#define USE_OR_MASKS
#include <p18cxxx.h>
#include <plib/dpslp.h>
#include <plib/portb.h>
#include <plib/rtcc.h>

void dormir (void)
{
    LED_AMAR=1;
    while(BusyUSART());
    putrsUSART("\r\n...entrando em modo Sleep...");
    CloseUSART();

    IDLEN=0;
    SLEEP();

    //ClrWdt();

    //WDTCONbits.SWDTEN=1;
    //WDTCONbits.SWDTE=1;
    //WDT_ENABLED;

    //OSTS_bit;
    //IOFS_bit;
    //T1RUN_bit;
    configuracao_EUSART();
    while(BusyUSART());
    putrsUSART("\r\n...acordando dentro do subcomando Sleep...");
    LED_AMAR=0;

}

void acordar (void)
{
    short int razao=0;
    char msg[55];
    short int RI=0, TO=0, PD=0, POR=0, BOR=0;

    configuracao_EUSART();

    RI=RCON &  0b10000 >> 4;
    TO=RCON &  0b01000 >> 3;
    PD=RCON &  0b00100 >> 2;
    POR=RCON & 0b00010 >> 1;
    BOR=RCON & 0b00001;

    razao = RCON & 0b11111; // pega somente os 5 primeiros bits de RCON

    sprintf(msg,"\r\nWakeup: [0x%X] RI=%d, TO=%d, PD=%d, POR=%d, BOR=%d\r\n", razao);
    while(BusyUSART());
    putsUSART( msg );

    while (razao-- >= 0)
    {
        LED_VERD=1;
        Delay10KTCYx(50);
        LED_VERD=0;
        Delay10KTCYx(50);
    }

    Delay10KTCYx(100);

}

/*
 * The power-managed Sleep mode in the PIC18F2525/
2620/4525/4620 devices is identical to the legacy
Sleep mode offered in all other PIC devices. It is
entered by clearing the IDLEN bit (the default state on
device Reset) and executing the SLEEP instruction.
This shuts down the selected oscillator (Figure 3-5). All
clock source status bits are cleared.
 *
Entering the Sleep mode from any other mode does not
require a clock switch. This is because no clocks are
needed once the controller has entered Sleep. If the
WDT is selected, the INTRC source will continue to
operate. If the Timer1 oscillator is enabled, it will also
continue to run.
 *
When a wake event occurs in Sleep mode (by interrupt,
Reset or WDT time-out), the device will not be clocked
until the clock source selected by the SCS1:SCS0 bits
becomes ready (see Figure 3-6), or it will be clocked
from the internal oscillator block if either the Two-Speed
Start-up or the Fail-Safe Clock Monitor are enabled
(see Section 23.0 ?Special Features of the CPU?). In
either case, the OSTS bit is set when the primary clock
is providing the device clocks. The IDLEN and SCS bits
are not affected by the wake-up.
 */

/*
 23.2
Watchdog Timer (WDT)
 *
For PIC18F2525/2620/4525/4620 devices, the WDT is
driven by the INTRC source. When the WDT is
enabled, the clock source is also enabled. The nominal
WDT period is 4 ms and has the same stability as the
INTRC oscillator.
 *
The 4 ms period of the WDT is multiplied by a 16-bit
postscaler. Any output of the WDT postscaler is
selected by a multiplexer, controlled by bits in Configu-
ration Register 2H. Available periods range from 4 ms
to 131.072 seconds (2.18 minutes). The WDT and
postscaler are cleared when any of the following events
occur: a SLEEP or CLRWDT instruction is executed, the
IRCF bits (OSCCON<6:4>) are changed or a clock
failure has occurred.
 *
FIGURE 23-1:
SWDTEN
WDTEN
 *
Note 1: The CLRWDT and SLEEP instructions
clear the WDT and postscaler counts
when executed.
 *
2: Changing the setting of the IRCF bits
(OSCCON<6:4>) clears the WDT and
postscaler counts.
 *
3: When a CLRWDT instruction is executed,
the postscaler count will be cleared.
 * 
23.2.1
CONTROL REGISTER
Register 23-14 shows the WDTCON register. This is a
readable and writable register which contains a control
bit that allows software to override the WDT enable
Configuration bit, but
 *
 */