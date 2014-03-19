/*
 * File:   main.c
 * Author: roney
 *
 * Created on November 20, 2013, 9:40 AM
 * Version 0.1
 *
 * This program was created in order to show the hability to send and receive
 * DATA trought the Serial Port to a PC running TTY emulator, and showing the
 * characters in LCD, flashing LEDs to display status.
 *
 */


#include <xc.h>
#include <plib/usart.h>
#include <plib/xlcd.h>
#include <stdbool.h>
#include <stdlib.h>

#include "configbits.h"

#define _XTAL_FREQ 8000000

#define LED1 PORTCbits.RC0
#define LED2 PORTCbits.RC1
#define LED3 PORTCbits.RC2
#define LED4 PORTCbits.RC3
#define BUZZ PORTCbits.RC4
#define LED_VERMELHO PORTCbits.RC5
#define BOTAO PORTAbits.RA2     // Botao da Placa AcePic28, para debug texto
                                // Pushbotton for serial text debug

void interrupt Interrupcao(void);
void initLCD(void);


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
        WriteCmdXLCD(DON&CURSOR_ON&BLINK_ON);
        // utilizei um display padrao 16x2 caracteres
        // I used a common LCD of 16x2 characters
}

 //////////////////////////////////////////////////////////////////


char chRX;
bool SerialEcho = true;
bool LcdDisplay = true;
short int contadorDisplay=0;


void main(void) {

    char mensagem[]="Pronto > ";

    ADCON1=0xF;    // torna todas portas AN0 a AN12 como digitais
                    // na PIC18F2525 somente AN0 a AN4 e AN8 a AN12
                    // nas PICs com 40 pinos, todos ANs de 0 a 12
                    //
                    // PCFG3:PCFG0: A/D Port Configuration Control bits
                    // Note 1:
                    // The POR value of the PCFG bits depends on the value of
                    // the PBADEN Configuration bit. When PBADEN = 1,
                    // PCFG<2:0> = 000; when PBADEN = 0, PCFG<2:0> = 111.


    TRISB=0;    // output do LCD na PORTB / LCD output in PORTB
    initLCD();  // inicia comandos de configuracao do LCD
                // LCD init commands
    /*
     * obs: a biblioteca XLCD.H da PLIB para PIC18, considera como default o LCD
     * conectado na PORTB com as seguintes pinagens:
     *
     Lower Nibble = LCD DATA in PORT B0, B1, B2, B3 (DATA_PORT)
     RW_PIN   in B6   		( PORT for LCD RW , can also be grounded)
     RS_PIN   in B5   		( PORT for LCD RS )
     E_PIN    in B4  		( PORT for LCD Enable Pin )
     */

    TRISA2=1;   // entrada do BOTAO / push-BOTTON input

    TRISC0=0;   // led verde 1
    TRISC1=0;   // led verde 2
    TRISC2=0;   // led verde 3
    TRISC3=0;   // led verde 4
    TRISC4=0;   //buzzer
    TRISC5=0;   //led vermelho

    LED_VERMELHO=1;
    LED1=1;

    Delay10KTCYx(1000);


    while(BusyXLCD());
    WriteCmdXLCD(0x01); // comando para limpar LCD / command to clear LCD

    while(BusyXLCD());
    putrsXLCD ("Serial IO EUSART"); // somente logotipo / just a start logo
    SetDDRamAddr(0x40);     // Linha 2 do Display LCD / second line of LCD

    while(BusyXLCD());
    putrsXLCD ("RS232 PIC18F2525"); // somente logotipo / just a start logo

    SetDDRamAddr(0x00); // volta cursor para linha 0 e posicao 0 do LCD
                        // put cursor in position 0,0 of LCD

    contadorDisplay=0;      // zerando a posicao de caracteres do LCD
                            // reseting the LCD character count

    // Habilitacao dos pinos C6 e C7 para uso da EUSART (explicacao abaixo):
    // Enabling pins C6 and C7 for EUSART (explanation bellow):
    TRISC6=1;
    TRISC7=1;
    RCSTAbits.SPEN=1;
    /*
    The pins of the Enhanced USART are multiplexed
    with PORTC. In order to configure RC6/TX/CK and
    RC7/RX/DT as a USART:
    ? SPEN bit (RCSTA<7>) must be set (= 1)
    ? TRISC<7> bit must be set (= 1)
    ? TRISC<6> bit must be set (= 1)
    Note:
    The EUSART control will automatically
    reconfigure the pin from input to output as
    needed.

     */


    LED1=1;
    LED2=0;
    LED_VERMELHO=0;

    CloseUSART();   // fecha qualquer USART que estaria supostamente aberta antes
                    // just closes any previous USART open port

    
    Delay10KTCYx(1); //Passing 0 (zero) results in a delay of 2,560,000 cycles
    Delay10KTCYx(1000);

    //PORTC=1; Delay10TCYx(50); PORTC=0;

    OpenUSART(  USART_TX_INT_OFF &
                USART_RX_INT_ON &
                USART_ASYNCH_MODE &
                USART_EIGHT_BIT &
                USART_CONT_RX &
                USART_BRGH_LOW,
                51
                );
    // Baud Rate "51" para 2400bps @ 8mhz em modo assincrono
    // de acordo com DS39626E-page 207 do Datasheet em PDF da PIC18F2525

    // These are common comands for 2400 bps running at 8 mhz, assyncronous mode
    // just as being showed in PIC18F2525 Datasheet (DS39626E-page 207)

    //baudUSART (BAUD_8_BIT_RATE | BAUD_AUTO_OFF);

    // page 1158 do pic18_plib.pdf (capitulo 8.17.1.3.3 baud_USART)
    // Set the baud rate configuration bits for enhanced usart operation
    // These functions are only available for processors with
    // enhanced usart capability (EUSART)
    /*
          The Enhanced Universal Synchronous Asynchronous
    Receiver Transmitter (EUSART) module is one of the
    two serial I/O modules. (Generically, the USART is also
    known as a Serial Communications Interface or SCI.)
    The EUSART can be configured as a full-duplex
    asynchronous system that can communicate with
    peripheral devices, such as CRT terminals and
    personal computers. It can also be configured as a half-
    duplex synchronous system that can communicate
    with peripheral devices, such as A/D or D/A integrated
    circuits, serial EEPROMs, etc.
    The Enhanced USART module implements additional
    features, including automatic baud rate detection and
    calibration, automatic wake-up on Sync Break recep-
    tion and 12-bit Break character transmit. These make it
    ideally suited for use in Local Interconnect Network bus
    (LIN bus) systems. (DS39626E-page 201)
     */


    INTCONbits.PEIE = 1;  // interrupcoes para perifericos
    INTCONbits.GIE  = 1;  // interrupcoes globais

    while(BusyUSART());
    putrsUSART("\n\rLed1 e LedVermelho = Interrupcao; Led2 = while wait; Led3/4 = Err");

    while(BusyUSART());
    putrsUSART("\n\rComandos ^E (echo), ^P (lcd), ^L (cls), ^S (status)");

    while(BusyUSART());
    putrsUSART("\n\rTerminal Serial: ");

    while(BusyUSART());
    putsUSART(mensagem);

    LED_VERMELHO=0;
    LED1=1;
    LED2=1;
    
    //LED4=1;

    while (1){

        //LED4=RCIF;
        // o LED4 mostra o status da Interrupcao da RX Serial
        // Led4 shows the status of RX interrupt
      
        //LED3=TXIF;
        // o LED3 mostra o status da suposta interrupcao de TX
        // Led3 shows the suposed TX interrupt state

        LED3=OERR;

        LED4=FERR;
        /*
        bit 2 FERR: Framing Error bit
        1 = Framing error (can be cleared by reading RCREG register and receiving next valid byte)
        0 = No framing error
         *
        bit 1 OERR: Overrun Error bit
        1 = Overrun error (can be cleared by clearing bit, CREN)
        0 = No overrun error
         */


        LED2=~LED2;
        Delay10KTCYx(10);
        // quando esta no modo de espera de tecla* (interrupcao), fica piscando
        // os tres leds para demonstrar a despreocupacao do loop while
        // *na verdade essa espera de tecla eh o RX da serial

        // when being in wait mode (for interrupt *keypress )just flashes the tree
        // leds to demonstrate the un-commitment of while loop to keypress
        // * this keypress event is the serial RX



        // Check for overrun error condition
		if (OERR == 1)
		{
			// Clear the overrun error condition
                        BUZZ=1;
			CREN = 0;
			CREN = 1;
                        Delay100TCYx(10); BUZZ=0;
                }
        LED_VERMELHO = RCIF;    // buffer de recepcao cheio

    }

    return;
}

void interrupt Interrupcao(void){

    char buffer[4];
    /* a string buffer ira guardar um numero decimal (0 a 32) traduzido pela
     * funcao itoa, onde ira transformar o numero inteiro em string com apontador
     * o tamanho poderia ser 3, mas o inteiro vai ate 3 posicoes + null
     *
     * the buffer string variable will hold a decimal number (0 to 32) that was
     * converted by the itoa function, changing a integer number into a string
     * pointer
     * its size could be only 3, but the interger goes up to 3 positions + null end
     */


    // LED_VERMELHO = ~LED_VERMELHO;
    // muda o status do LED_VERMELHO quando a MCU entrar em interrupcao
    // changes the LED_VERMELHO when the MCU interrupts

    // BUZZ=1; Delay100TCYx(10); BUZZ=0;
    // toca o buzzer para cada interrupcao
    // plays the buzzer for each interrupt

    if (RCIF)   // se existe interrupcao aguardando...
                // teoricamente este RCIF nao precisaria de verificacao
                // pois se a rotina de Interrupcao ja foi chamada, entao
                // nem seria necessario verifica-la
                // mas os LED1 e LED_VERMELHO servem justamente para comprovar
                // que o RCIF sempre ira existir, quando mudam juntos (leds)

                // Theoretically this RCIF "if" check should never be necessary
                // but just to confirm the RCIF, the two leds (LED1 and
                // LED_VERMELHO) will always change together.
        {

        RCIE = 0;   // desabilita interrupts de RX para tratar a entrada
                    // disables RX interrupts to treat input

        LED1 = ~LED1;
        // muda o status do primeiro LED
        // inverts the LED1 status

        chRX = ReadUSART();
        // le caractere da Serial / read a character from Serial

        if ( chRX>31 && chRX<127 && !BOTAO)
            // filtra apenas os caracteres texto comuns
            // just filter the common readable text characters
        {

            // Se o Echo Serial esta ativo, entao imprima o caractere na Serial
            // If Serial Echo is on, then send the character to Serial Port
            if (SerialEcho)
                {
                    while (BusyUSART()); // espera nao ocupado / waits non busy
                    WriteUSART(chRX);    // envia caractere na Porta Serial
                                         // sends the character in Serial Port
                }

            // Se o LcdDisplay esta ativo, entao imprima o caractere
            // If Lcd Display is enabled, print the character in LCD
            if (LcdDisplay)
            {
                while (BusyXLCD());
                WriteDataXLCD(chRX);

                contadorDisplay++;      // contador de caracteres para tabular
                                        // LCD character tabulation count
                if (contadorDisplay==16)    // se chegar ao final da primeira linha
                                            // if reaches the end of first line
                {
                    SetDDRamAddr(0x40); // comando para pular para segunda linha
                                        // command for going to second line
                    if (SerialEcho)
                    {
                        while (BusyUSART());
                        putrsUSART("\r\n"); // imprime RETURN e NEWLINE (inicio)
                                            // prints Carriage RETURN and Newline
                    }

                }

                if (contadorDisplay==32)    // se chegar ao final da segunda linha
                                            // if reaches the end of second line
                {
                    SetDDRamAddr(0x00);     // pula novamente para primeira linha
                                            // go again to first line
                    contadorDisplay=0;  // ja na primeira linha, limpa contador
                                        // in first line, clear the char counter
                    if (SerialEcho)
                    {
                        while (BusyUSART());
                        putrsUSART("\r\n"); // imprime RETURN e NEWLINE (inicio)
                                            // prints Carriage RETURN and Newline
                    }
                }

            }



        }
        else            // filtra todos demais Controls e Caracteres Especiais
                        // this is the state of non-text characters being filtered

        if (BOTAO)  // se o BOTAO de debug estiver pressionado, nao filtra controle
                    //if pushBOTTON is pressed, do not filter control characters
        {
                    while (BusyUSART());
                    WriteUSART(chRX);
        }
        else
        switch (chRX)
        {
            case 0x05 :      // Caractere ^E
                SerialEcho=!SerialEcho;
                while(BusyUSART());
                putrsUSART("\r\n[ECHO ");
                if (SerialEcho) putrsUSART("ON]"); else putrsUSART("OFF]");
                break;
                /*
                 * Control-E:
                 * Rotina para ligar e desligar o ECHO na porta serial
                 * Routine to turn on and off the ECHO in serial port
                 */

            case 0x10 :      // Caractere ^P
                LcdDisplay=!LcdDisplay;
                while(BusyUSART());
                putrsUSART("\r\n[LCD ");
                if (LcdDisplay) putrsUSART("ON]"); else putrsUSART("OFF]");
                break;
                /*
                 * Contro-P:
                 * Rotina para ligar e desligar a amostragem de Caractere no LCD
                 * Routine to turn on and off the display of characters in LCD
                 */

            case 0x0C :     // Caractere ^L (FormFeed)
                putrsUSART("\r\n[LCD CLS]");
                while(BusyXLCD());
                WriteCmdXLCD(0x01); // Comando para limpar o LCD
                contadorDisplay=0;
                break;
                /*
                 * Control-L: (LimpaTela / FormFeed)
                 * Rotina para Limpar a Tela
                 * Routine to CLear Screen (CLS)
                 */

            case 0x13 :     // Caractere ^S
                putrsUSART("\r\n[Status Lcd:");
                if (LcdDisplay) putrsUSART("On"); else putrsUSART("Off");
                putrsUSART(" Echo:");
                if (SerialEcho) putrsUSART("On"); else putrsUSART("Off");
                putrsUSART(" charLCD:");
                
                itoa ( buffer, contadorDisplay, 10);
                // itoa necessita da biblioteca <stdlib.h>
                // itoa needs the include stdlib.h
                putrsUSART( buffer );
                putrsUSART("]\r\n");
                /*
                 * Control-S:
                 * Mostra o Status do Echo, do LCD, e da quantidade de caracteres
                 * no LCD
                 * Shows the Status of Echo, LCD and characteres number in LCD
                 */
                
                break;

            default:
               
                ;
        }

        




        // two Peripheral Interrupt Request (Flag) registers (PIR1 and PIR2)
        // RCIF: EUSART Receive Interrupt Flag bit

        //PIR1bits.RCIF = 0; // PIR1 no registro RCIF
        RCIE = 1; // Re-Habilita a Interrupcao de Recepcao
                  // Re-Enable RX Interrupts

        RCIF = 0; // PIR1 no registro RCIF
        // limpa o registrador de interrupcao, e sai da interrupcao
        // clear the interrupt register, and quits it

        }
}