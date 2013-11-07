/*
 * File:   main_spi.c
 * Author: pu1rym
 *
 * Created on April 28, 2013, 8:56 PM
 */

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>


#include "lcd.h"
#include "pause.h"
#include "spi_comm.h"

//__CONFIG (INTIO & WDTDIS & MCLRDIS & UNPROTECT);
#pragma config FOSC = XT // XT, LP, RC, HS,
//#pragma config FOSC = INTRC_NOCLKOUT // define oscilador INTERNO sem CLOCK OUT
//#pragma config FOSC = INTRC_CLKOUT // define oscilador INTERNO sem CLOCK OUT
//#pragma config FOSC = INTOSCIO // define oscilador INTERNO sem CLOCK OUT
//#pragma config FOSC = 100
/*
     0111 ? XT PLL 16x ? XT crystal oscillator mode with 16xPLL enabled. 4 ? 10MHz crystal
     0110 ? XT PLL 8x ? XT crystal oscillator mode with 8xPLL enabled. 4 ? 10MHz crystal
     0101 ? XT PLL 4x - XT crystal oscillator mode with 46xPLL enabled. 4 ? 10MHz crystal
     0100 ? XT -  XT crystal oscillator mode (4 ? 10MHz crystal)
     001x - HS - HS crystal oscillator mode (10 ? 25MHz crystal)
     000x ? XTL - XTL crystal oscillator mode (200kHz ? 4MHz crystal)
*/

//#define FOSC 8000000L

#pragma config WDTE = OFF           // watch dog desligado
#pragma config PWRTE = OFF          // temporizador de powerUp desligado
//#pragma config MCLRE = ON           // pino MCLR eh MCLR e nao funcao
#pragma config CP = OFF             // programa memory nao protegido
#pragma config CPD = OFF            // data memory nao protegida
#pragma config BOREN = ON           // BROWN-OUT inicializacao sem reset,
                                    // pelo nivel de voltagem minimo
//#pragma config IESO = OFF           // internal/external switchOver
#pragma config LVP = OFF            // low voltage programming = precisa de
                                    // alta vontagem para programacao do chip
//#pragma config FCMEN = OFF          // fail-safe clock monitor



/*
 BOREN =	Brown-out Reset Enable bit
OFF	BOR disabled
ON	BOR enabled
 *
CPD =	Data EEPROM Memory Code Protection bit
OFF	Data EEPROM code protection off
ON	Data EEPROM code-protected
 *
DEBUG =	In-Circuit Debugger Mode bit
OFF	In-Circuit Debugger disabled, RB6 and RB7 are general purpose I/O pins
ON	In-Circuit Debugger enabled, RB6 and RB7 are dedicated to the debugger
 *
WRT =	Flash Program Memory Write Enable bits
OFF	Write protection off; all program memory may be written to by EECON control
HALF	0000h to 0FFFh write-protected; 1000h to 1FFFh may be written to by EECON control
1FOURTH	0000h to 07FFh write-protected; 0800h to 1FFFh may be written to by EECON control
256	0000h to 00FFh write-protected; 0100h to 1FFFh may be written to by EECON control
 *
FOSC =	Oscillator Selection bits
XT	XT oscillator
LP	LP oscillator
EXTRC	RC oscillator
HS	HS oscillator
 *
WDTE =	Watchdog Timer Enable bit
OFF	WDT disabled
ON	WDT enabled
 *
CP =	Flash Program Memory Code Protection bit
OFF	Code protection off
ON	All program memory code-protected
 *
LVP =	Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
OFF	RB3 is digital I/O, HV on MCLR must be used for programming
ON	RB3/PGM pin has PGM function; low-voltage programming enabled
 *
PWRTE =	Power-up Timer Enable bit
OFF	PWRT disabled
ON	PWRT enabled
 */

//#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
//#endif

#define number 0x30
#define CS  RA5 // no chip pic16f887


//#define CS RC6		//CS = RC6  'Chip select pin
					//SCK = RB6 'Clock pin
					//SDI = RB4	'MISO pin
					//SDO = RC7 'MOSI pin

int digito[10] =
               {0b00111111,    //Dígito 0
                0b00000110,    //Dígito 1
                0b01011011,    //Dígito 2
                0b01001111,    //Dígito 3
                0b01100110,    //Dígito 4
                0b01101101,    //Dígito 5
                0b01111101,    //Dígito 6
                0b00000111,    //Dígito 7
                0b01111111,    //Dígito 8
                0b01101111     //Dígito 9
               };

unsigned short int CONT;
#define PAUSA 100

unsigned char b0;   // Data Storage Byte
unsigned char addr, superaddr; // Address Storage Byte
unsigned char data; // Address Storage Byte
unsigned char c;
char buf[4];


int main(void) {

//    ANSEL=0;
//    ANSELH=0;
//    CM1CON0=0;
//    CM2CON0=0;

    TRISA=0;
    TRISB=0;
    TRISC=0;

    CS=1;   // INICIALIZA O CHIP SELECT COMO HIGH (LIGADO)

    //TRISB2=0;   // pino RS do LCD
    //TRISB3=0;   // pino ENABLE do LCD

    TRISA5=0;   // pino CS como output
    TRISC5=0;   // pino SDO (MOSI) como output
    TRISC3=0;   // pino SCK como output
    TRISC4=1;   // pino SDI (MISO) como input

    RC0=1; //RA5=0;
    for(CONT=9;CONT>0;CONT--) {PORTB=digito[CONT];pause(1000);}
    RC0=0;

    PORTB=0x00;
    c=0xFF;
    RB4 = (c & 0b00010000) >> 4; pause(100);PORTB=0x00;
    RB5 = (c & 0b00100000) >> 5; pause(100);PORTB=0x00;
    RB6 = (c & 0b01000000) >> 6; pause(100);PORTB=0x00;
    RB7 = (c & 0b10000000) >> 7; pause(100);PORTB=0x00;
    /* */
    RB4 = (c & 0b00000001) ;     pause(100);PORTB=0x00;
    RB5 = (c & 0b00000010) >> 1; pause(100);PORTB=0x00;
    RB6 = (c & 0b00000100) >> 2; pause(100);PORTB=0x00;
    RB7 = (c & 0b00001000) >> 3; pause(100);PORTB=0x00;
    pause(1000);

    pause(100);
    PORTB=digito[0]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(1000);
    

    lcd_init();


    pause(4000);
    PORTB=digito[1]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(1000);

    lcd_clear();

    pause(1000);

    PORTB=digito[2]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(500);


    lcd_goto(0);
    pause(2000);

    PORTB=digito[3]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(500);


    lcd_puts("VOL2 CAP8 SPI");
    pause(2000);

    PORTB=digito[4]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(500);

    lcd_goto(0x40);
    pause(2000);

    PORTB=digito[5]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(500);

    lcd_puts("EE LCD");
    pause(2000);

    PORTB=digito[6]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(500);

    lcd_puts("."); pause (1000);
    lcd_puts("."); pause (1000);
    lcd_puts("."); pause (1000);
    lcd_puts("."); pause (1000);
    lcd_puts("."); pause (1000);
    lcd_puts("."); pause (1000);

    PORTB=digito[7]; //RA5=0;
    for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
    PORTB=0x00;pause(500);

    /*  SPI Peripheral Setup    */

    /*  SSPCON Settings */
    #define FOSC=8000000;

    SSPM3=0b0010;
    // 00000001 = SPI Clock Frequency setada para FOSC/16 = 250 khz (para 4mhz)

    /*
     
     * bit 3-0 SSPM3:SSPM0: Synchronous Serial Port Mode Select bits
     0101 = SPI Slave mode, clock = SCK pin. SS pin control disabled. SS can be used as I/O pin.
     0100 = SPI Slave mode, clock = SCK pin. SS pin control enabled.
     0011 = SPI Master mode, clock = TMR2 output/2
     *
     0010 = SPI Master mode, clock = FOSC/64 (no OSCILOSCOPIO deu +- 40 khz)
     *                  8000000 / 64 deveria dar 125 khz
     0001 = SPI Master mode, clock = FOSC/16 (no OSCILOSCOPIO deu ++-- 32 khz)
     *                  8000000 / 16 deveria dar 500 khz
     0000 = SPI Master mode, clock = FOSC/4
     *
     */


    CKP=1;              // Idle State do clock como desligado
            // CKP = Clock Polarity Select (1 = high, 0 = low)
    /*
    CKP: Clock Polarity Select bit
    1 = Idle state for clock is a high level
    0 = Idle state for clock is a low level
    */

    /*  SSPSTAT Settings    */

    // SPI Data Input Sample Phase bit
    SMP=1;      // Sampled ate end of data pulse

    /*
    SMP: Sample bit
    SPI Master mode:
    1 = Input data sampled at end of data output time
    0 = Input data sampled at middle of data output time
    */


    // SPI Clock Edge Select bit
    CKE=0;      // Data Transfered on rising edge of clock

    /* para p PIC16F876A
     *
    CKE: SPI Clock Select bit
    1 = Transmit occurs on transition from active to Idle clock state
    0 = Transmit occurs on transition from Idle to active clock state
    */

    /*  SPI Peripheral on   */
    // Syncronous Serial Port Enable bit
    SSPEN=1;    // Enable SPI Peripheral

    /*  Load Data into EEPROM   */

   // while (1==1)
   // {
        lcd_clear();
        lcd_goto(0x00);
        lcd_puts("ESCREVENDO");

        lcd_goto(0x40);
        lcd_puts("EEPROM");
        pause(1000);

        /*
        PORTB=digito[8]; //RA5=0;
        for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
        PORTB=0x00;pause(500);
         */

        for(superaddr=0; superaddr<0b00010000; superaddr++)
        {
        for(addr=0; addr<0xFF; addr=addr+1)
        {
            lcd_goto(0x47);
            lcd_putch(0x58); // escreve caractere X
            //pause(50);
            data=(addr>>1);
                WR_BYTE(superaddr, addr,data);
            //pause(10);
            lcd_goto(0x47);
            lcd_putch(0x4F); //escreve caractere O
            //pause(50);
        }
        }
        lcd_goto(0x49);
        lcd_puts("OK");
        pause(1000);

        lcd_clear();
        lcd_goto(0);
        lcd_puts("INICIANDO LOOP");
        pause(1000);

        while (1==1)
       {

        /*
        PORTB=digito[9]; //RA5=0;
        for(CONT=0;CONT<5;CONT++) {RC0=1; pause(PAUSA); RC0=0; pause(PAUSA);}
        PORTB=0x00;pause(500);
         */

        lcd_clear();
        lcd_goto(0);
        lcd_puts("ADDR VALOR");
        //pause(1000);

        for(superaddr=0; superaddr<0b0100000; superaddr++)
        {
        for(addr=0; addr<0xFF; addr=addr+1)
        {
            b0=RD_BYTE(superaddr, addr);
            lcd_goto(0x40);

            itoa(buf, superaddr, 16);
            lcd_puts(buf);

            itoa(buf, addr, 16);
            lcd_puts(buf);

            lcd_puts("=");
                itoa(buf, b0, 10);
            lcd_puts(buf);
            lcd_puts("   ");
            //pause(100);
        }

        }

        //pause(1000);

        /*
        for(addr=0; addr<0xFF; addr=addr+1)
        {
            b0=RD_BYTE(addr);
            lcd_goto(0x40);
            lcd_putch(addr+number);
            lcd_puts("_");
            lcd_putch(b0+number);
            pause(200);
        }

        pause(1000);

        for(addr=0; addr<0xFF; addr=addr+1)
        {
            b0=RD_BYTE(addr);
            lcd_goto(0x40);
            lcd_putch(addr+number);
            lcd_puts("_");
            lcd_putch(b0+number);
            pause(200);
            
        }
        /*


    //RC0=1;


    /*
    for(CONT=999;CONT>0;CONT--)
    {
        PORTB=digito[ (CONT/100)       ]; RC0=1; pause(1); RC0=0;
        PORTB=digito[((CONT/10) % 10 ) ]; RC1=1; pause(1); RC1=0;
        //PORTB=digito[ (CONT     % 10 ) ]; RA5=1; pause(1); RA5=0;
        
    }
    */
    

    }
    
    return 0;
}
