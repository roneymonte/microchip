/*
 * File:   main.c
 *
 * Created on August 16, 2010, 12:09 PM
 */


// Agora adicionado Sharing interno do Ubuntu com Windows
// Recebido tambem no Ubuntu ThinkPad

#include "p18f2525.h"

// PIC18F2525 Configuration Bit Settings

#include <xc.h>
#include "plib/delays.h"
#include "plib/xlcd.h"


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1H
//#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
//#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config OSC = INTIO7     // Oscillator Selection bits (Internal oscillator block, CLKOUT function on RA6, port function on RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
//#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
//#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

#define _XTAL_FREQ 1000000 // 1 mhz
//#define _XTAL_FREQ (20000000) //20 mhz

//#define _XTAL_FREQ 125000 // 125 khz
//#define _XTAL_FREQ 4000000 // 4mhz
//#define _XTAL_FREQ 8000000 // 4mhz



void delay();

void delay() {
    //__delay_ms(10);

    
    
    int counter = 0;
    for (counter = 0; counter<10000; counter++) {
        ;
    }
    
}

void DelayFor18TCY(void)
 {
 Delay10TCYx(0x2); //delays 20 cycles
 return;
 }

 void DelayPORXLCD(void)   // minimum 15ms
 {
 Delay100TCYx(0xA0);   // 100TCY * 160
 return;
 }

 void DelayXLCD(void)     // minimum 5ms
 {
 Delay100TCYx(0x36);      // 100TCY * 54
 return;
 }

void main(void) {

    //IRCF0_bit=0b0;
    //IRCF1_bit=0b0;
    //IRCF2_bit=0b1;
    //OSCCON=0b100; // 1mhz
    //OSCCON=0b001; // 125 khz

    //OSCCONbits.SCS1=1; // internal oscillator block
    OSCCONbits.IRCF0 = 0b1;
    OSCCONbits.IRCF1 = 0b1;
    OSCCONbits.IRCF2 = 0b1;


    //para 100 (osconbits 210), osciloscopio acusou 250 khz


    short int cont;
    TRISB = 0;
    TRISC = 0;
    /*
    TRISCbits.RC3=0;
    TRISCbits.RC2=0;
    TRISCbits.RC1=0;
    */

    PORTC = 0x00;

     for (cont=0; cont<2; cont++)
        {
         //LATC3=1;
         //LATC2=1;
         //LATC1=1;
         PORTC=0b1111;

        Delay10KTCYx(1000);

         //LATC3=0;
         //LATC2=0;
         //LATC1=0;
         PORTC=0b0000;

        Delay10KTCYx(1000);
        }

    OSCCONbits.IRCF0 = 0b0;
    OSCCONbits.IRCF1 = 0b1;
    OSCCONbits.IRCF2 = 0b1; // 4mhz

    LATC3=0;
    LATC2=0;
    LATC1=0;
    
        Delay10KTCYx(2000);

    	OpenXLCD(FOUR_BIT & LINES_5X7);
        WriteDataXLCD('H');
	WriteCmdXLCD(DON&CURSOR_ON&BLINK_ON);

        char data[]="Finalmente Rodou";
        
    while (1) {

        PORTC=0;

        for (cont= 0b0001 ; cont <= 0b1111 ; cont=cont*2)
        {
        PORTC = cont;
        Delay10KTCYx(10);
        }
        PORTC=0;

        OSCCONbits.IRCF0 = 0b1;
        OSCCONbits.IRCF1 = 0b1;
        OSCCONbits.IRCF2 = 0b0; // 500 khz
        
        while(BusyXLCD());
        WriteCmdXLCD(0x01);
        


	Delay10KTCYx(250);

        OSCCONbits.IRCF0 = 0b0;
        OSCCONbits.IRCF1 = 0b0;
        OSCCONbits.IRCF2 = 0b0; // 31 khz

        while(BusyXLCD());
	putrsXLCD("Lcd Testando..");
        Delay10KTCYx(2);

         /* Set DDRam address to 0x40 to display data in the second line */
        SetDDRamAddr(0x40);
        putrsXLCD(data);

        Delay10KTCYx(2);
        OSCCONbits.IRCF0 = 0b0;
        OSCCONbits.IRCF1 = 0b0;
        OSCCONbits.IRCF2 = 0b1; // 1 mhz
        Delay10KTCYx(1000);
        }

}


