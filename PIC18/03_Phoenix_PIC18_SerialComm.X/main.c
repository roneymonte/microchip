/*
 * File:   main.c
 * Author: roney
 *
 * Created on November 6, 2013, 3:23 PM
 */


#include <xc.h>
#include <plib/usart.h>
#include <plib/xlcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_bits.h"

#define _XTAL_FREQ 8000000

 //////////////////////////////////////////////////////////////////

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

 void initLCD(void)
{
        OpenXLCD(FOUR_BIT & LINES_5X7);
        SetDDRamAddr(0x00);
        //WriteDataXLCD('i');
        WriteCmdXLCD(DON&CURSOR_ON&BLINK_ON);
}

 //////////////////////////////////////////////////////////////////

void main(void) {

    char c;
    short int contDisplay;
    long temperatura;
    char buffer;

    TRISB=0;    // output do LCD
    initLCD();

    TRISAbits.RA1=1;   // canal A1 do Trimpot como Entrada
    TRISAbits.RA3=1;   // canal A3 do sensor de Temperatura como Entrada
    TRISAbits.RA4=1;    // Push Botton 4
    ADCON1bits.PCFG=0b1011;
    TRISC0=0;
    TRISC1=0;
    TRISC2=0;
    TRISC3=0;
    TRISC4=0;   //buzzer
    TRISC5=0;   //led vermelho
    PORTC=0;
    PORTC=1; Delay10TCYx(50); PORTC=0;


    SPEN=1;             //identico abaixo
    RCSTAbits.SPEN=1;   //identico acima
    TRISC6=1;
    TRISC7=1;
    
    //GIE=1;  // deve ser setado apenas se usar Interrupcao
    //PEIE=1; // deve ser setado apenas se usar Interrupcao
    GIE=0;
    PEIE=0;

    // Para modo Assincrono, com Baud Rate Generator (BRG) = 8bits = 0
    SYNC=0;     // de acordo pagina datasheet DS39626E-page 207
    BRGH=0;     // de acordo pagina datasheet DS39626E-page 207 (highbaud)
    BRG16=0;    // de acordo pagina datasheet DS39626E-page 207
    SPBRG=51;     // baudrate 2400 @ 8mhz (51 = reset value)

    // Para modo Assincrono com High BRG = 1
    //SYNC=0;
    //BRGH=1;
    //BRG16=0;
    //SPBRG=207;  // baudrate 2400 @ 8 mhz

    //BAUDCONbits.
    //TXIE=1; // Interrupts desejados na Transmissao
    TXEN=1; // habilita a transmissao
    // TXIF=1; // status vai ser setado automaticamente atraves do TXEN=1
    //TXREG=0x52; // coloca o DADO (byte qualquer) para iniciar a transmissao
    //TRMT = readonly = mostra o status do TSR Register

    //RCIE=1; // Interrupts desejados na Recepcao
    CREN=1; // Habilita a recepcao
    //RCIF = recepcao completa; se INTerrupcao habilitada, gerara INT
    //RCREG = byte recebido (semelhante ao TXREG)

    //OpenUSART();

    while(BusyXLCD());
    WriteCmdXLCD(0x01);
    putrsXLCD ("Serial IO");
    SetDDRamAddr(0x00);

    //while (!DataRdyUSART());

    putcUSART( 0x0C );
    Delay10KTCYx(10);
    putcUSART( 0x0D );
    Delay10KTCYx(10);
    putrsUSART( "INICIO DE LINHA:" );
    for (contDisplay=0;contDisplay<20;contDisplay++)
    {
        putcUSART('0'); Delay10KTCYx(10);
        putcUSART(0x08);Delay10KTCYx(10);
        putcUSART('-'); Delay10KTCYx(10);
        putcUSART(0x08);Delay10KTCYx(10);
        
    }



    contDisplay=0;

    while (1)
    {
        
        while (!DataRdyUSART());

        //c=getcUSART();
        c=ReadUSART();



        PORTCbits.RC5=1;Delay10TCYx(50);PORTCbits.RC5=0;


        if ( c != 'A')
        {

            putcUSART( c );

            if (c==0x0B) {
                contDisplay=0;
                WriteCmdXLCD(0b1);
            }
            else
            if ( c>=32 && c<=126 )
            {
            putcXLCD(c);
            contDisplay++;
            }
            else
            {
                //putcXLCD(itoa(buffer,c,10));

                putrsXLCD( itoa(buffer,c,10) );
                contDisplay=contDisplay+sizeof( itoa(buffer,c,10) );
                if (contDisplay>16) contDisplay=16;
            }

            
            
            if (contDisplay>=32) {
                                 SetDDRamAddr(0x00);
                                 contDisplay=0;
                                 }
            else
                if (contDisplay==16) SetDDRamAddr(0x40);

            Delay10KTCYx(1);
            //printf("\r\nContador = %02lu C.\r\n\r\n", contDisplay);


            ADCON0bits.CHS=0b0011;  //usa o AN3 para CONversao DS39626E-page 223
                                    // AN3 = Termistor
            ADCON0bits.ADON=1;  // liga o AD para CONversao
            Delay1KTCYx(2);     // delay aproximado de 1 ms

            ADCON0bits.GO=1;    // inicia a CONversao
            while (ADCON0bits.GO) ;
                                //  aguarda o termino da CONversao

            temperatura= ((ADRES*5)/1023)+2 ;
            //putcUSART( itoa(buffer,temperatura,10) );
            PORTCbits.RC3=1;Delay10TCYx(5);PORTCbits.RC3=0;
            Delay10KTCYx(1);
            //printf("\r\nTemperatura = %02lu C.\r\n\r\n", temperatura);


        }
        else
        {
            putcUSART(c);
            Delay10KTCYx(1);
            putcUSART(c+1);
            Delay10KTCYx(1);
            putcUSART(c+2);
        }
    }
    

    return;
}

