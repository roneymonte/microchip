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
 *              Server VM 23.25-b01, System: Linux version 3.11.0-19-
 *
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <plib/i2c.h>
//#include <plib/delays.h>
#include <plib/usart.h>
//#include <math.h>
//#include <string.h>

#include "configbits.h"

#define _XTAL_FREQ 8000000
#define FOSC 8000000
#define Baud 100000

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

void main (void)
{
    TRISC0=0;   // Led Amar
    TRISC1=0;   // Led Verd
    TRISC2=0;   // LED Verm
    TRISC6=0;   // saida TX EUSART
    ADCON1=0x0F;

    unsigned int D0, D1, D2, D3;

    char msg[30];

    LED_VERM=1;
    configuracao_EUSART();

    initExt7SegLCD();
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);

    while (1){

        getDS1307();


        sprintf(msg," ___[%x:%x:%x]___", hora, minuto,segundo );

        while(BusyUSART());
        putsUSART( msg );


        D0 = (hora & 0b00110000) >> 4;

        D1 = hora & 0b00001111;

        D2 = (minuto & 0b11110000) >> 4;

        D3 = minuto & 0b00001111;

        sprintf(msg,"___ %d%d:%d%d ...%xseg_ \r\n",
            D0, D1, D2, D3 ,segundo);

        while(BusyUSART());
        putsUSART( msg );

        StartI2C();
            __delay_us(16);
            WriteI2C(HT16K33_ADDR << 1);

            WriteI2C(0x00);

            WriteI2C(numbertable[ D0 ] & 0xff );
            WriteI2C(numbertable[ D0 ] >> 8);

            WriteI2C(numbertable[ D1  ] );
            WriteI2C(numbertable[ D1 ] >> 8);

            WriteI2C(0xFF);
            WriteI2C(0xFF >> 8);


            WriteI2C(numbertable[ D2 ] & 0xFF );
            WriteI2C(numbertable[ D2 ] >> 8);

            WriteI2C(numbertable[ D3 ] & 0xFF );
            WriteI2C(numbertable[ D3 ] >> 8);

            NotAckI2C();
            __delay_us(16);
        StopI2C();

        // simples delay de aprox 2 seg (nao importante)
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);

        RestartI2C();
            __delay_us(16);
            WriteI2C(HT16K33_ADDR << 1);

            WriteI2C(0x00);

            WriteI2C(numbertable[0]);
            WriteI2C(0x00);

            WriteI2C(numbertable[0]);
            WriteI2C(0x00);

            WriteI2C(0x00);
            WriteI2C(0x00);


            WriteI2C(numbertable[0] );
            WriteI2C(0x00);

            WriteI2C(numbertable[0]);
            WriteI2C(0x00);

            NotAckI2C();
            __delay_us(16);
        StopI2C();

        // simples delay de aprox 2 seg (nao importante)
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);

    }

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

    SSPADD = ((FOSC/4)/Baud)-1;

    PIR2bits.BCLIF=0;       // limpa o flag de COLISAO do barramento
    PIR1bits.SSPIF=0;       // limpa o flag de interrupcao SSPIF
    PIE2bits.BCLIE = 1;     // Enable bus collision interrupts
    PIE1bits.SSPIE = 1;     // Enable MSSP interrupt enable bit

    CloseI2C();
    OpenI2C(MASTER,SLEW_ON);

    LED_AMAR=1;

    StartI2C();
            __delay_us(16);
            WriteI2C(HT16K33_ADDR << 1);
            //AckI2C();
            //IdleI2C();
            //__delay_us(60);
            WriteI2C(HT16K33_CMD_SETUP
                    );
            AckI2C();
            
        StopI2C();

        IdleI2C();

        RestartI2C(); 
            WriteI2C(HT16K33_ADDR << 1);
            //AckI2C();//IdleI2C();
            //__delay_us(60);
             
            WriteI2C( HT16K33_BLINK_CMD | HT16K33_BLINK_2HZ);
            //AckI2C();
            
            __delay_us(16);
        StopI2C();

        IdleI2C();

        RestartI2C();
            __delay_us(16);
             
            WriteI2C(HT16K33_ADDR << 1);
            //AckI2C();//IdleI2C();
            //__delay_us(60);
            WriteI2C(HT16K33_CMD_BRIGHTNESS | 7);
            //AckI2C();
            //__delay_us(16);
        StopI2C();

        RestartI2C();
            __delay_us(16);
            WriteI2C(HT16K33_ADDR << 1);

            WriteI2C(0x00);

            WriteI2C(numbertable[1] | 0x80);
            WriteI2C(0x00);
  
            WriteI2C(numbertable[2]);
            WriteI2C(0x00);

            WriteI2C(0xFF);
            WriteI2C(0x00);


            WriteI2C(numbertable[3]);
            WriteI2C(0x00);

            WriteI2C(numbertable[4] | 0x80);
            WriteI2C(0x00);

            NotAckI2C();
            __delay_us(16);
        StopI2C();

        LED_VERD=1;

        

}

void getDS1307(void)
{
    char msg [40];

    //#define StartI2C()  SSPCON2bits.SEN=1;while(SSPCON2bits.SEN)

    LED_AMAR=1;

    IdleI2C();
    StartI2C();
        //IdleI2C();
        __delay_us(16);
        WriteI2C( 0xD0 );
        //IdleI2C();
        __delay_us(60);
        WriteI2C( 0x00 );
        IdleI2C();
        __delay_us(16);
        //AckI2C();AckI2C();AckI2C();AckI2C();AckI2C();AckI2C();AckI2C();AckI2C();
    StopI2C();
    //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)

    //IdleI2C();
    __delay_us(26);

    RestartI2C();
        __delay_us(16);

        WriteI2C( 0xD1 );
        __delay_us(1);
        IdleI2C();

        segundo    = (int) ReadI2C();
        AckI2C();
        IdleI2C();

        minuto  = (int) ReadI2C();
        AckI2C();
        IdleI2C();

        hora = (int) ReadI2C();
        NotAckI2C();

    StopI2C();
    //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)


    LED_VERM = 0; LED_AMAR=0; LED_VERD=1;

        sprintf(msg,"\r\n%xh:%xm:%xs _ \r\n",
            hora,minuto,segundo);

    while(BusyUSART());
    putsUSART( msg );
    

    LED_VERD=0;
}

void configuracao_EUSART (void)
{
    //#define CloseUSART( ) RCSTA&=0b01001111,TXSTAbits.TXEN=0,PIE1&=0b11001111
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