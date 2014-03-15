/*

 * Monitor de Temperatura e Luminosidade
 * (Duas Entradas Analogicas AN0 e AN1)
 *
 * VERSAO 1.0 = apenas o teste inicial de LCD
 *
 * por Roney Monte em 03-mar-2014
 * VERSAO 1.1 = adicao do sensor LM35 e LDR com 10 bits de precisao (0 a 1023)
 *
 * Projeto idealizado com PIC18F4550
 * rodando com cristal externo de 4mhz
 *
 * LCD de 20x4 colunas, conectado ao PORTB
 * com biblioteca default da PLIB - PIC18
 *
 * VERSAO 1.2 = adicionada a biblioteca Virtual Wire, para transmissao
 * dos dados coletados atraves de RF, utilizando o pino RD4 para TX
 *
 * Os dados transmitidos sao: contador de sequencia, temperatura, luminosidade;
 *
 * Obs: o modulo RX esta programado em um Arduino. Ambos se comunicam a 600 bps
 *
 */


#include <xc.h>
#include <plib/delays.h>
#include <plib/xlcd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "WV_PIC_LIB/virtualwire.h"

#include "config_bits.h"

#define _XTAL_FREQ 4000000

#define LED1 PORTDbits.RD6  // simples LED para monitoramento
#define LED2 PORTDbits.RD7  // simples LED para monitoramento
#define LEDI PORTDbits.RD3  // simples LED para monitoramento
#define LDR  PORTAbits.AN1  // Sensor LDR conectado ao pino AN1
#define LM35 PORTAbits.AN0  // Sensor LM45 conectado ao pino AN0


 //////////////////////////////////////////////////////////////////
// Pre-Definicao de Funcoes

void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);
void initLCD(void);
void piscaVermelho (void);
void interrupt global_isr(void);
void delay(unsigned int delay);

 //////////////////////////////////////////////////////////////////

void DelayFor18TCY(void)
 {
 //Delay10TCYx(0x2); //delays 20 cycles

 	Delay10TCYx(1);
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
	Delay1TCY();
        Delay1TCY();
	Delay1TCY();

 return;
 }


// void DelayPORXLCD (void) {
// Delay of 15ms
// Cycles = (TimeDelay * Fosc) / 4
// Cycles = (15ms * 12MHz) / 4
// Cycles = 15,000

//Delay1KTCYx(100);     //20ms
  //Delay1KTCYx(60); // Delay of 15ms
  // Cycles = (TimeDelay * Fosc) / 4
  // Cycles = (15ms * 16MHz) / 4
  // Cycles = 60,000

 void DelayPORXLCD(void)   // minimum 15ms
 {
 //Delay100TCYx(0xA0);   // 100TCY * 160

 	Delay1KTCYx(15);	// Delay of 15 ms
						// Cycles = (TimeDelay * Fosc)/4
						// Cycles = (15ms * 4MHz)/4
						// Cycles = 15,000

 return;
 }

 /*
  * 1 mhz = 1 milhao de hertz = 1 milhao ciclos/seg = 250 mil instrucoes / seg
  * 
  */

 /* Delay100TCYx
  * Delay multiples of 100 Tcy
  * Passing 0 (zero) results in a delay of 25,600 cycles.
  */


  //Delay1KTCYx(12); // Delay of 5ms
       // Cycles = (TimeDelay * Fosc) / 4
       // Cycles = (5ms * 12MHz) / 4
       // Cycles = 5,000
 
        //Delay1KTCYx(40);    //delay of 6ms

           //Delay1KTCYx(20); // Delay of 5ms
           // Cycles = (TimeDelay * Fosc) / 4
           // Cycles = (5ms * 16MHz) / 4
           // Cycles = 20,000

 void DelayXLCD(void)     // minimum 5ms
 {
 //Delay100TCYx(0x36);      // 100TCY * 54

 Delay1KTCYx(5); 	// Delay of 5 ms
						// Cycles = (TimeDelay * Fosc)/4
						// Cycles = (5ms * 4MHz)/4
						// Cycles = 5,000
 return;
 }

 void initLCD(void)
{
     
        OpenXLCD(FOUR_BIT & LINES_5X7);
        WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF);

 }

 //////////////////////////////////////////////////////////////////

void interrupt global_isr(void)
{
	if(T0IF)
        {
            LEDI=1;
		vw_isr_tmr0();
            LEDI=0;
        }
}

void delay(unsigned int delay)
{
	while(delay--);
}

 //////////////////////////////////////////////////////////////////

void main (void)
{
    unsigned short int i, temperatura, luminosidade, temp0, lumi0;
    unsigned short int min=255;
    unsigned short int max=0;
    unsigned short int lmin=255;
    unsigned short int lmax=0;
    unsigned short int contador = 0;

    bool change;
    //char null_0 [sizeof(unsigned long)*8+1];
    //char null_1 [sizeof(unsigned long)*8+1];

    //FOSC = INTOSC_EC, the actual value for FOSC<3:0> = b'1001', which accesses the internal clock and sets RA6 as a Fosc/4 pin.
    // OSCCON=0b110; // 4 mhz
    // OSCCON=0b111; // 8 mhz
    // OSCCON=0b11110010; // 8 mhz ,  SCS<1:0> = b'10', which activates the internal oscillator.

    //IRCF0=0;
    //IRCF1=1;
    //IRCF2=1;

   // SCS1=1;
   // SCS0=0;

    /*  REGISTER 2-2: OSCCON: OSCILLATOR CONTROL REGISTER

        IDLEN IRCF2 IRCF1 IRCF0 OSTS IOFS SCS1 SCS0
        bit 7 ................................ bit 0

        bit 7 IDLEN:Idle Enable bit
            1= Device enters Idle mode on SLEEPinstruction
            0= Device enters Sleep mode on SLEEPinstruction

        bit 6-4 IRCF2:IRCF0:Internal Oscillator Frequency Select bits
            111= 8 MHz (INTOSC drives clock directly)
            110= 4 MHz
            101= 2 MHz
            100= 1 MHz

            011= 500 kHz
            010= 250 kHz
            001= 125 kHz
            000= 31 kHz (from either INTOSC/256 or INTRC directly)

        bit 3 OSTS:Oscillator Start-up Time-out Status bit
            1= Oscillator Start-up Timer time-out has expired; primary oscillator is running
            0= Oscillator Start-up Timer time-out is running; primary oscillator is not ready

        bit 2 IOFS:INTOSC Frequency Stable bit
            1= INTOSC frequency is stable
            0= INTOSC frequency is not stable

        bit 1-0 SCS1:SCS0:System Clock Select bits
            1x= Internal oscillator
            01= Timer1 oscillator
            00= Primary oscillator

        Note 1: Depends on the state of the IESO Configuration bit.
        2: Source selected by the INTSRC bit (OSCTUNE<7>), see text.
        3: Default output frequency of INTOSC on Reset
     */

    TRISB=0x00; // configura PORTB para saida do LCD
    TRISD6=0;   // configura LED
    TRISD7=0;   // configura LED
    TRISD3=0;   // configura LED de Interrupcao
    //TRISD5=1;   // suposto modulo RX
    //TRISD4=0;   // Modulo TX
    TRISA0=1;   // configura ENTRADA do TERMISTOR LM35
    TRISA1=1;   // configura ENTRADA do LDR

    ADCON1bits.PCFG=0b1101; // Configura somente as portas AN0 e AN1 como AD

    ADFM=1;      // utiliza o total de 10 bits no ADRES
    /*
     *  Página 33 de 74O bit de ADFM tem a função de organizar o resultado da
     *  conversão A/D, de forma que o osvalores convertidos sejam justificados
     *  a direita ou a esquerda nos registradores ADRESH e ADRESL. Caso venhamos
     *  configurar ADFM = 1, organizamos o valor da conversão a direita,ou seja,
     *  os oitos bits menos significativo será armazendo em ADRESL, e os 2 bits
     *  maissignificativo serão armazenados em ADRESH.Caso ADFM = 0,
     *  justificaremos a esquerda os valores de conversão, desta forma os
     *  oitosbits mais significativos ficarão em ADRESH e os 2 menos
     *  significativo ficará em ADRESL.
     */

    for(i=0;i<10;i++)
    {
        LED2=1; LED1=0;  Delay1KTCYx(50);  //  50 ms
        LED2=0; LED1=1;  Delay1KTCYx(50);

    }
    LED1=0;
    
    initLCD();

    //#######################################################################
    //#######################################################################
    
    //#######################################################################
    //#######################################################################

    while(BusyXLCD());
        WriteCmdXLCD(0x01);
        
    SetDDRamAddr(0x00);
              //0123456789_123456789
    //putrsXLCD ("Projeto:  PIC18F4550");
    putrsXLCD ("PIC18F4550 LuzTempRF");

    //SetDDRamAddr(0x40);
    //putrsXLCD ("Luz & Temp & RF TX");

    //SetDDRamAddr(0x14);
    //putrsXLCD ("Linha 3: PIC18F4550");
    //SetDDRamAddr(0x54);
    //putrsXLCD ("Linha 4: PIC18F4550");

    //const char text[] = "Hello from PIC ";
    // excessao para teste de concatenacao de strings
    //char *static_msg = "256,30,100";    // 10 + 1 posicoes
    //char msg[ sizeof (static_msg) + 1 ];
    char msg [12];  //  = "256,30,100 ";

    //sprintf(msg, "%s %d", static_msg, var);

    vw_setup(600);  // inicializa o modulo RF com 600 bps

    // apenas um teste para imprimir o tamanho do float
    sprintf(msg, "%s%d%c", "I", sizeof(float), NULL);
    Delay1KTCYx(100);
            vw_send(msg, sizeof(msg)-1);

    while (1)
    {
        piscaVermelho();
        ///////////////////////////////////////////////////

        ADCON0bits.CHS=0b0000;  //usa o AN0 para CONversao DS39626E-page 223
                                // AN0 = Termistor LM35
        ADCON0bits.ADON=1;  // liga o AD para CONversao
        Delay1KTCYx(1);     // delay aproximado de 1 ms

        ADCON0bits.GO=1;    // inicia a CONversao
        while (ADCON0bits.GO) ;
                            //  aguarda o termino da CONversao

            while(BusyXLCD()) ;
            SetDDRamAddr(0x14); //linha 2
                      //0123456789_123456789
            putrsXLCD ("Temp: ");
            temperatura = ( ADRES * 500) / 1023 ;

            //putrsXLCD ( ltoa (null_0,((ADRES*5)/1023)+9,10) ); // era usado com o ADRESL e ADRESH invertido errado
            //putrsXLCD ( ltoa (null_0,( ADRES * 500) / 1023 ,10) );
            while(BusyXLCD()) ;
            putrsXLCD ( ltoa (NULL, temperatura ,10) );
            /*
             * Equacao realizada para ajustar a temperatura, com
             * ajuste fino de +9 graus centigrados para temperatura local
             *
             */
            while(BusyXLCD()) ;
            putcXLCD(0xDF); // imprime o caractere de "grau" (degree)
            putrsXLCD ("c ");

        ///////////////////////////////////////////////////



        piscaVermelho();
        //Delay10KTCYx(50);     // delay aproximado de 100 ms

        ///////////////////////////////////////////////////

        ADCON0bits.CHS=0b0001;  //usa o AN0 para CONversao DS39626E-page 223
                                // AN1 = Termistor LDR
        ADCON0bits.ADON=1;  // liga o AD para CONversao
        Delay1KTCYx(1);     // delay aproximado de 1 ms

        ADCON0bits.GO=1;    // inicia a CONversao
        while (ADCON0bits.GO) ;
                            //  aguarda o termino da CONversao
        //corrente = ADRES;

            while(BusyXLCD()) ;
                //SetDDRamAddr(0x54); //linha 2
                SetDDRamAddr(0x20);
                          //0123456789_123456789
                putrsXLCD ("Luz: ");
                luminosidade = ( ( ADRES / 8 )  ) ;

            while(BusyXLCD()) ;
                putrsXLCD ( ltoa (NULL,luminosidade,10) );
                //putrsXLCD ( ltoa (null_1,ADRES,10) );
                putrsXLCD ("% ");

        ///////////////////////////////////////////////////

    if (temperatura != temp0)   { temp0=temperatura; change=true; }
    else
    if (luminosidade != lumi0)  { lumi0=luminosidade; change=true;}

    if (change)
        {
        contador++;
        if (temperatura < min) min=temperatura;
        else
            if (temperatura > max) max=temperatura;

        if (luminosidade < lmin) lmin=luminosidade;
        else
            if (luminosidade > lmax) lmax=luminosidade;

        while(BusyXLCD()) ;
        SetDDRamAddr(0x40);
            putrsXLCD ("Tmin"); putrsXLCD (ltoa (NULL,min,10));
            putrsXLCD ("/");    putrsXLCD (ltoa (NULL,max,10));
            putrsXLCD (" Luz"); putrsXLCD (ltoa (NULL,lmin,10));
            putrsXLCD ("/");    putrsXLCD (ltoa (NULL,lmax,10));
            putrsXLCD (" ");


        while(BusyXLCD()) ;
            SetDDRamAddr(0x66); //linha 4 nas duas ultimas posicoes
                  //456789abcdef0123456_
            putrsXLCD ("TX");

            //i=sizeof(msg)-1; // apenas um teste para mostrar o tamanho da msg

        sprintf(msg, "%u,%u,%u%s", contador, temperatura, luminosidade, ";");

        while(BusyXLCD()) ;
            SetDDRamAddr(0x54);
            putrsXLCD (msg);
            putrsXLCD ("  ");

        LED1=1;

            Delay1KTCYx(100);
            vw_send(msg, sizeof(msg)-1);
            //vw_send("Testando T",10);
            //Delay10KTCYx(200);
            //vw_send(text, sizeof(text)-1);
                //vw_send("Testando T",10);
                //Delay10KTCYx(200);
        LED1=0;

        while(BusyXLCD()) ;
            SetDDRamAddr(0x66); //linha 2
                  //456789abcdef0123456_
            //putrsXLCD ("__");
            putcXLCD ('_');putcXLCD ('_');
            
        change=false;
        }
    }



}

void piscaVermelho (void){
    LED2=1;
    Delay1KTCYx(15);
    LED2=0;
    Delay1KTCYx(85);
    
}