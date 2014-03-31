 /*
 * File:   main.c
 * Author: roney
 *
 * Created on 21 de Março de 2014, 16:53    -   Versao 0.1
 * Versao 0.2  - 23mar2014 - implementada comunicacao RS232 e I2C
 *               ainda desajustado com frequencia, SDA e SCL.
 * Versao 0.2a - 24mar2014 - I2C  nao funcionando, SLAVE nao respondendo.
 *               NAKs. troca endereco de 8B para 5C
 * Versao 0.2b - removidas rotinas experimentais desnecessarias.
 * Versao 0.2c - 25mar2014 - I2C/SCL nao funcionam de jeito nenhum.
 *               adicionado RTC com circuito DS1307, e tambem nao funciona.
 *               obs: usando um Arduino, tudo funciona normalmente, sem dor.
 * Versao 0.2d - cada vez pior
 * Versao 0.2e - constatados problemas na PIC18F4550, trocada pela PIC18F2525
 * Versao 0.2f - com a PIC18F2525 medindo niveis de ruido e oscilacao
 *               na alimentacao eletrica via USB, agora com capacitores
 * Versao 0.3  - FINALMENTE FUNCIONANDO, descoberta a falta de ACK na leitura
 *               dos bytes dos dispositivos SLAVEs I2C; Colocando o AckI2C() e
 *               o IdleI2C() resolveu-se o problema. Simples ?
 * Versao 0.3b - Publicando comentarios sobre os configuracoes de inicializacao
 *               e sobre os REGISTRADORES e seus VETORES de configuracao
 * Versao 0.3c - Todos comentarios e documentacoes do Datasheet (PIC18F4550)
 *               estao transcritos, o codigo foi re-organizado em funcoes
 * Versao 0.3d - Todas rotinas re-arrumadas, I2C e timer0 (pausa) funcionando OK
 * Versao 0.4  - Iniciada rotina getPressao com modulo Adafruit BMP180
 * 
 *
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // para uso do comando sprintf()
#include "configbits2525.h"     // outro arquivo config_bits.h para PIC18F4550
#include <plib/delays.h>        // para uso do comando Delay10KTCY(), etc
#include <plib/usart.h>         // para uso da interface Serial
#include <plib/i2c.h>           // para uso da interface MSSP I2C
#include <plib/timers.h>        // para uso formal dos TIMERS

#define _XTAL_FREQ 8000000
#define FOSC       8000000UL
#define Baud 100000

#define LED_AMAR    PORTCbits.RC0
#define LED_VERD    PORTCbits.RC1
#define LED_VERM    PORTCbits.RC2

#define USE_AND_MASKS

/*      ** funcoes e comandos do Protocolo I2C (plib) - Compilador XC8 **
        AckI2C      :   Generate I2C? bus Acknowledgecondition.
        CloseI2C    :   Disable the SSP module.
        DataRdyI2C  :   Is the data available in the I2C buffer?
        getcI2C     :   Read a single byte from the I2C bus.
        getsI2C     :   Read a string from the I2C bus operating in master I2C mode.
        IdleI2C     :   Loop until I2C bus is idle.
        NotAckI2C   :   Generate I2C bus Not Acknowledgecondition.
        OpenI2C     :   Configure the SSP module.
        putcI2C     :   Write a single byte to the I2C bus.
        putsI2C     :   Write a string to the I2C bus operating in either Master or Slave mode.
        ReadI2C     :   Read a single byte from the I2C bus.
        RestartI2C  :   Generate an I2C bus Restartcondition.
        StartI2C    :   Generate an I2C bus Startcondition.
        StopI2C     :   Generate an I2C bus Stopcondition.
        WriteI2C    :   Write a single byte to the I2C bus.
 */

/* Definicao de Funcoes */
void configuracao_I2C (void);
void configuracao_PIC (void);
void configuracao_EUSART (void);
void testaColisao(void);
void getDS1307(void);
void getPressao(void);
void getTemperaturaHumidade (void);
void pausa (unsigned int segundos);
void interrupt global_isr(void);



void main(void)
{
    // Configuracao das portas com LEDs
    TRISCbits.TRISC0=0;    // LED Amarelo para simples sinalizacao
    TRISCbits.TRISC1=0;    // LED Verde para simples sinalizacao
    TRISCbits.TRISC2=0;    // LED Vermelho para simples sinalizacao

    // Configuracao do pino TX da porta serial EUSART / para RS232
    TRISCbits.TRISC6=1;    // TX da EUSART
                           // O programa ira informar na porta serial o status
                           // e logs de funcionamento da coleta de dados I2C

    // agora a CHAMADA para configuracao GLOBAL da PIC
    configuracao_PIC();

    // Preparacao para configuracao do modulo MSSP I2C (com Errata aplicada)
    /* 17. Module: MSSP (ERRATA for PIC18F4550 and PIC18F2525, etc)
     * ================
     *
     *  It has been observed that following a Power-on Reset, I2C mode may not
     *  initialize properly by just configuring the SCL and SDA pins as either
     *  inputs or outputs. This has only been seen in a few unique system
     *  environments. A test of a statistically significant sample of pre-
     *  production systems, across the voltage and current range of the
     *  application's power supply, should indicate if a system is
     *  susceptible to this issue.
     *
     * Work around = Before configuring the module for I2C operation:
     * 1. Configure the SCL and SDA pins as outputs by clearing
     *  their corresponding TRIS bits.
     * 2. Force SCL and SDA low by clearing the corresponding LAT bits.
     * 3. While keeping the LAT bits clear, configure SCL and SDA as
     *  inputs by setting their TRIS bits.
     *
     * Once this is done, use the SSPCON1 and SSPCON2 registers to
     *  configure the proper I2C mode as before.
     */

    TRISCbits.TRISC3=0;    // SCL do I2C colocado como saida por causa de bug*
    TRISCbits.TRISC4=0;    // SDA do I2C colocado como saida por causa de bug*
    LATC3=0;            // bug* pede que zere-se o LAT das portas SCL e SDA
    LATC4=0;            // durante inicializacao do I2C para evitar flutuacoes
                        // eletricas que ficariam nas portas antes de liga-las

    Delay10KTCYx(10);   // simples pausa para troca de estado na SDA e SCL

    TRISCbits.TRISC3=1;    // SCL do I2C, agora corretamente como saida
    TRISCbits.TRISC4=1;    // SDA do I2C, agora corretamente como saida
    // here ends "errata workaround"

    // entao a CHAMADA para diversas configuracoes referentes ao I2C (MSSP)
    configuracao_I2C();

    // e a inicializacao da porta serial EUSART
    configuracao_EUSART();

    while(BusyUSART());
    putrsUSART("\n\r_INIT SERIAL.\n\r");


    /*************************
     *
     * INICIO DO PROGRAMA
     *
     *************************/

    LED_AMAR=0; LED_VERM=1; LED_VERD=0;

    // Inicializacao do MSSP I2C

    CloseI2C(); // simplesmente fechando qualquer possibilidade de I2C anterior
                // comando nao necessario no boot da PIC
                //macro = #define CloseI2C()  SSPCON1 &=0xDF

    while(BusyUSART());
    putrsUSART("SSPAD=");
    putsUSART( itoa(NULL,SSPADD,10) );
    putrsUSART(" (hex=0x");
    putrsUSART( itoa(NULL,SSPADD,16) );
    putrsUSART("); Abrindo MSSP I2C (Master,Slew_off)\n\r");

    OpenI2C(MASTER,SLEW_OFF);   // configuracao implicita da SSPCON1 e SSPSTAT

    while (1)
    {
        testaColisao();
        getDS1307();

        

        //testaColisao();
        getTemperaturaHumidade();

        pausa(10);
    }
}


void getTemperaturaHumidade (void)
{
    unsigned char TEMPL=0, TEMPH=0, HUMIDL=0, HUMIDH=0;
    unsigned char DUMMY=0, OP=0, BT=0;
    float humidade, temperatura;

    char msg[55];

        LED_AMAR=1;

        //#define StartI2C()  SSPCON2bits.SEN=1;while(SSPCON2bits.SEN)

        StartI2C();             // ACORDAR DEVICE
            __delay_us(16);
            WriteI2C(0xB8);     // endereco Slave do AM2315
            __delay_us(135);
        StopI2C();
        //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)

        // com clock de 4 mhz:
        // 10K (100) = 1000 ms
        // 1K  (100) = 100 ms
        // 1K  (10)  = 10 ms
        // 1K  (2)   = 2 ms
        // Delay100TCYx();

        __delay_us(25);

        RestartI2C();           // REQUISITAR PEDIDO DE BYTES
            __delay_us(16);
            WriteI2C(0xB8);     // endereco Slave do AM2315
            __delay_us(60);     // manual do AM2315 recomenda minimo de 30us

            WriteI2C(0x03);     // byte que simboliza a temperatura
            __delay_us(60);

            WriteI2C(0x00);     // start byte para leitura
            __delay_us(60);

            WriteI2C(0x04);     // quantidades de bytes a serem lidos;
            //AckI2C();
            __delay_us(16);
        StopI2C();
        //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)

        __delay_ms(10); // manual do AM2315 recomenda esperar no minimo 10ms

        RestartI2C();
            WriteI2C(0xB9);     // endereco Slave do AM2315
            //AckI2C();         // retirado por nao necessitar (?)
            __delay_us(60);     // manual do AM2315 recomenda minimo de 30us
            IdleI2C();

            OP          = ReadI2C();        // 1o byte
            AckI2C();
            IdleI2C();

            BT          = ReadI2C();        // 2o byte
            AckI2C();
            IdleI2C();

            HUMIDL       = ReadI2C();       // 3o byte
            AckI2C();
            IdleI2C();

            HUMIDH       = ReadI2C();       // 4o byte
            AckI2C();
            IdleI2C();

            TEMPL    = ReadI2C();           // 5o byte
            AckI2C();
            IdleI2C();

            TEMPH    = ReadI2C();           // 6o byte
            AckI2C();
            IdleI2C();

            DUMMY          = ReadI2C();     // 7o byte
            AckI2C();
            IdleI2C();

            DUMMY          = ReadI2C();     // 8 byte
            //__delay_us(16);
        StopI2C();
        //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)

        LED_VERM=0; LED_AMAR=0;LED_VERD=1;


        // Calculos obtidos do exemplo do Arduino
        humidade  = HUMIDL;
        humidade *= 256;
        humidade += HUMIDH;
        humidade /= 10;

        temperatura  = TEMPL;
        temperatura *= 256;
        temperatura += TEMPH;
        temperatura /= 10;

         /* ou ainda
         RH = RHH << 8;
         RH |= RHL;

         TEMP = TEMPH << 8;
         TEMP |= TEMPL;
         */

        sprintf (msg, "Temp= %0.2f, Humid= %0.2f .", temperatura, humidade);

        while(BusyUSART());
        putsUSART(msg);

        while(BusyUSART());
        putrsUSART("\n\r");

        LED_VERD=0;

}

void getDS1307(void)
{
    int hora=0, minuto=0, segundo=0, diasemana=0, dia=0, mes=0, ano=0, dummy=0;
    char msg[40];

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

        segundo    =ReadI2C();
        AckI2C();
        IdleI2C();

        minuto  =ReadI2C();
        AckI2C();
        IdleI2C();

        hora =ReadI2C();
        AckI2C();
        IdleI2C();

        diasemana=ReadI2C();
        AckI2C();
        IdleI2C();

        dia     =ReadI2C();
        AckI2C();
        IdleI2C();

        mes     =ReadI2C();
        AckI2C();
        IdleI2C();

        ano     =ReadI2C();
        AckI2C();
        IdleI2C();

        dummy   =ReadI2C();
        //AckI2C();
        //__delay_us(16);
        //IdleI2C();
        //NotAckI2C();
        //IdleI2C();
    StopI2C();
    //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)


    LED_VERM = 0; LED_AMAR=0; LED_VERD=1;

    sprintf(msg,"%xh:%xm:%xs _ dia %x/%x/%x _ ",
            hora,minuto,segundo,dia,mes,ano);

    while(BusyUSART());
    putsUSART( msg );

    LED_VERD=0;
}

void getPressao(void)
{
    #define BMP085_ADDRESS 0xEE      // I2C address of BMP085
    #define P_CORRECTION   1.5       // in mBars - factor to adjust for elevation to match local weather station pressure
                                     // this value for 14' above sea level (in Boca Raton, Florida)


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

        /*
        segundo    =ReadI2C();
        AckI2C();
        IdleI2C();

        minuto  =ReadI2C();
        AckI2C();
        IdleI2C();

        hora =ReadI2C();
        AckI2C();
        IdleI2C();

        diasemana=ReadI2C();
        AckI2C();
        IdleI2C();

        dia     =ReadI2C();
        AckI2C();
        IdleI2C();

        mes     =ReadI2C();
        AckI2C();
        IdleI2C();

        ano     =ReadI2C();
        AckI2C();
        IdleI2C();

        dummy   =ReadI2C();
        //AckI2C();
        //__delay_us(16);
        //IdleI2C();
        //NotAckI2C();
        //IdleI2C();
    StopI2C();
    //#define StopI2C()  SSPCON2bits.PEN=1;while(SSPCON2bits.PEN)


    LED_VERM = 0; LED_AMAR=0; LED_VERD=1;

    sprintf(msg,"%xh:%xm:%xs _ dia %x/%x/%x _ ",
            hora,minuto,segundo,dia,mes,ano);

         */
        
    while(BusyUSART());
//    putsUSART( msg );

    LED_VERD=0;
}

void configuracao_I2C (void)

{
        //SSPCON1bits.SSPEN=1;  // habilitacao dos pinos fisicos defauls da PIC
                            // nao precisa ser configurado por default (=0)
                            // habilitacao MSSP (Master Syncronous Serial Port)

     /*  O protocolo I2C utiliza os seguintes REGISTRADORES:
      *
      *  MSSP Control Register 1 (SSPCON1)
      *  =================================
      *
      *      WCOL:  Write Collision Detect bit (sem colisao = 0)
      *
      *      SSPOV: Receive Overflow Indicator bit (sem overflow = 0)
      *
      *      SSPEN: Master Synchronous Serial Port Enable bit (habilita pinos)
      *             Os pinos fisicos devem ser setados (TRIS) previamente com 1
      *
      *      CKP: SCK Release Control bit (segura o clock em LOW = 0, e libera o
      *           clock = 1 elevando a voltagem pull-up no pino)
      *
      *      SPM3:SSPM0: Master Synchronous Serial Port Mode Select bit
      *                  Define o TIPO MASTER (2 tipos) ou SLAVE (4 tipos)
      *
      */


    SSPCON2=0x00;   // nao precisaria teoricamente ser zerado, pois eh
                    // controlado pelos comandos da PLIB

     /*  MSSP Control Register 2 (SSPCON2) - Detalhes no Modo Master
      *   =================================
      *
      *      GCEN: General Call Enable bit (Slave mode only) (not used in MASTER)
      *      ACKSTAT: Acknowledge Status bit, Master Transmit Only
      *              (ACK foi recebido do Slave =0; =1 nao recebeu ACK)
      *
      *      ACKDT: Acknowledge Data bit (Recepcao de ACK pelo MASTER)
      *              (ACK foi confirmado, gerou ACK)
      *
      *      ACKEN: Acknowledge Sequence Enable bit (Controla a transmissao de
      *             ACK pelo ACKDT, zerado automaticamente pelo hardware apos)
      *
      *      RCEN: Receive Enable bit (Master Receive mode only)
      *            (1 habilita a recepcao; 0 esta em modo IDLE)
      *
      *      PEN: Stop Condition Enable bit (inicia condicao de STOP = 1, e eh
      *           zerado automaticamente pelo hardware apos)
      *
      *      RSEN: Repeated Start Condition Enable bit (inicia repeticao de
      *            START e eh zerado automaticamente pelo hardware apos)
      *
      *      SEN: Start Condition Enable/Stretch Enable bit
      *      obs: esses vetores (ACKEN, RCEN, PEN, RSEN e SEN) nao podem ser
      *           escritos no momento que o I2C estiver ativo (Startado)
      *
      */

     /*  MSSP Control Register 2 (SSPCON2) - Detalhes no Modo Slave
      *   =================================
      *
      *      GCEN: General Call Enable bit (1= gera interrupcao quando o endereco
      *            0x0000 for recebido no SSPSR)
      *
      *      ACKSTAT: Acknowledge Status bit (nao utilizado no modo slave)
      *      ADMSK5:ADMSK2: Slave Address Mask Select bits (1= mascaramento do
      *                     endereco Slave do SSPADD)
      *
      *      ADMSK1: Slave Address Mask Select bit (=1 habilita mascara SSPADD)
      *
      *      SEN: Stretch Enable bit (1= habilitado o Stretch)
      */

    SSPSTATbits.SMP = 1;        // desabilita o SLEW RATE CONTROL, para 100 khz
                    // comando nao necessario, pois sera controlado pelo
                    // OpenI2C() da PLIB. Vetor dentro do Registrador SSPSTAT.

    //SSPSTAT=0x00; // MSSP Status Register
                    // comando desncecessario (e errado)
                    // pois o default eh boot com 0

    //SSPSTATbits.CKE=0;  // use I2C levels worked also with '0'

     /*
     *  MSSP Status Register (SSPSTAT)
     *  ==============================
     *
     *      obs: Apenas o SMP e CKE setados para 100/400/1000khz e MASTER/SLAVE
     *      DA, P, S, RW, UA e BF sao status durante a transmissao ou recepcao
     *
     *  SMP: Slew Rate Control bit
     *       0 = 100 khz ou 1 mhz
     *       1 = 400 khz
     *
     *  CKE: SMBus Select bit
     *       Habilita o SMBus Specific Inputs
     *
     *  SSPBUF: Serial Receive/Transmit Buffer Register
     *          receive the byte from SSPSR and generate interrupt SSPIF
     *          SSPBUF/SSPRR are used also so send a byte
     *
     *  SSPSR:  Shift Register ? Not directly accessible, used for
     *                           shifting data in or out
      *
     *  SSPADD: Address Register, address when in I2C Slave mode.
     *
     *  When the in Master mode, the lower seven bits of SSPADD act as the
     *  Baud Rate Generator (BRG) reload value
     */

     // FORMULA DO SSPADD (deve-se #definir Baud e FOSC)
     // =================
     //
     //  Exemplo para Clock de 4 mgz
     // SSPADD = (( FOSC/ Bit Rate) /4 ) - 1;
     // SSPADD = ( 4.000.000/100.000 )/4 -1
     // SSPADD = ( 40 ) / 4 -1 = 10-1 = 9

     //  Exemplo para clock de 98 mhz
     // (8.000.000 / 4) / 100.000   - 1;
     //  2.000.000
     //  --------- = 20 - 1 = 19
     //    100.000

     // Definicao e calculo automatico baseados nos valores #DEFINIDOS
     SSPADD = ((FOSC/4)/Baud)-1;

}

void configuracao_PIC (void)

{
    //OSCCONbits.IRCF = 0b111; // definir clock interno para 8 mhz

    /* O Oscilador Interno (INTRC) nao tem precisao para gerar Clock I2C
     * Recomenda-se usar um Oscilador Externo (cristal) para maior precisao
     * Para se usar o INTRC (Oscilador Interno) deve-se mudar o arquivo
     * "configbits.h" com #Pragma config OSC INTRC (ou semelhante)
     */

    /* OSCCON: OSCILLATOR CONTROL REGISTER * (depende do FOSC, abaixo...)
     * ===================================
     *
     * IDLEN: Idle Enable bit
     *
     * IRCF2:IRCF0: Internal Oscillator Frequency Select bits
     *
     * OSTS: Oscillator Start-up Time-out Status bit
     *
     * IOFS: INTOSC Frequency Stable bit
     *
     * SCS1:SCS0: System Clock Select bits
     *
     */

    /* FOSC3:FOSC0: Oscillator Selection bits (exemplo para PIC18F4550)
     * obs: geralmente definido no CONFIG BITS (#Pragma config OSC)
     *
     * 111x = HS oscillator, PLL
     * 110x = HS oscillator (HS)
     * 1011 = Internal oscillator,
     * 1010 = Internal oscillator,
     * 1001 = Internal oscillator,
     * 1000 = Internal oscillator,
     * 0111 = EC oscillator, PLL
     * 0110 = EC oscillator, PLL
     * 0101 = EC oscillator, CLKO function on RA6 (EC)
     * 0100 = EC oscillator, port function on RA6 (ECIO)
     * 001x = XT oscillator, PLL enabled (XTPLL)
     * 000x = XT oscillator (XT)
     *
     */


    CMCON = 0x00;   // aplica configuracao '0' aos Comparators;
                    // teoricamente isso nada influi no funcionamento do I2C
                    // e essa configuracao poderia ser descartada

    /* CMCON: COMPARATOR CONTROL REGISTER
     * ==================================
     *
     * C2OUT: Comparator 2 Output bit
     *
     * C1OUT: Comparator 1 Output bit
     *
     * C2INV: Comparator 2 Output Inversion bit
     *
     * C1INV: Comparator 1 Output Inversion bit
     *
     * CIS: Comparator Input Switch bit
     *
     * CM2:CM0: Comparator Mode bits
     *
     */


    //INTCON = 0x00;  // Interrupt Control Register (desnecessario neste ponto)
                    // Esse registrador quando colocado '0', desabilita todas
                    // as configuracoes de interrupcao nos vetores abaixo:
                    // GIE/GIEH PEIE/GIEL TMR0IE INT0IE RBIE TMR0IF INT0IF RBIF

    //INTCONbits.PEIE=0;  // como o INTCON ja tinha sido zerado anteriormente
    //INTCONbits.GIE=0;   // esses dois vetores nao precisariam estar sendo zerados

    /* INTCON: INTERRUPT CONTROL REGISTER
     * ==================================
     *
     * GIE/GIEH: Global Interrupt Enable bit
     *
     * PEIE/GIEL: Peripheral Interrupt Enable bit
     *
     * TMR0IE: TMR0 Overflow Interrupt Enable bit
     *
     * INT0IE: INT0 External Interrupt Enable bit
     *
     * RBIE: RB Port Change Interrupt Enable bit
     *
     * TMR0IF: TMR0 Overflow Interrupt Flag bit
     *
     * INT0IF: INT0 External Interrupt Flag bit
     *
     * RBIF: RB Port Change Interrupt Flag bit
     *
     */


    //INTCON2bits.RBPU=1;   // pull up interno do PORTB desabilitado
                            // nao necessario para o I2C, default nao interfere
    /*  obs:    The weak pull-up is automatically turned off when the port
     *          pin is configured as an output. The pull-ups are disabled on
     *          a Power-on Reset.
     */

    /* INTCON2: INTERRUPT CONTROL REGISTER 2 (todos bits defaults em 1)
     * =====================================
     *
     * RBPU: PORTB Pull-up Enable bit
     *
     * INTEDG0: External Interrupt 0 Edge Select bit
     *
     * INTEDG1: External Interrupt 1 Edge Select bit
     *
     * INTEDG2: External Interrupt 2 Edge Select bit
     *
     * TMR0IP: TMR0 Overflow Interrupt Priority bit
     *
     * RBIP: RB Port Change Interrupt Priority bit
     *
     */


    PIE1bits.SSPIE = 1;     // Enable MSSP interrupt enable bit

    /* PIE1: PERIPHERAL INTERRUPT ENABLE REGISTER 1
     * ============================================
     *
     * SPPIE: Streaming Parallel Port Read/Write Interrupt Enable bit
     *
     * ADIE: A/D Converter Interrupt Enable bit
     *
     * RCIE: EUSART Receive Interrupt Enable bit
     *
     * TXIE: EUSART Transmit Interrupt Enable bit
     *
     * SSPIE: Master Synchronous Serial Port Interrupt Enable bit
     *
     * CCP1IE: CCP1 Interrupt Enable bit
     *
     * TMR2IE: TMR2 to PR2 Match Interrupt Enable bit
     *
     * TMR1IE: TMR1 Overflow Interrupt Enable bit
     *
     */


    PIE2bits.BCLIE = 1;     // Enable bus collision interrupts

    /* PIE2: PERIPHERAL INTERRUPT ENABLE REGISTER 2
     * ============================================
     *
     * OSCFIE: Oscillator Fail Interrupt Enable bit
     *
     * CMIE: Comparator Interrupt Enable bit
     *
     * USBIE: USB Interrupt Enable bit
     *
     * EEIE: Data EEPROM/Flash Write Operation Interrupt Enable bit
     *
     * BCLIE: Bus Collision Interrupt Enable bit
     *
     * HLVDIE: High/Low-Voltage Detect Interrupt Enable bit
     *
     * TMR3IE: TMR3 Overflow Interrupt Enable bit
     *
     * CCP2IE: CCP2 Interrupt Enable bit
     *
     */


    PIR1bits.SSPIF=0;       // limpa o flag de interrupcao SSPIF
                            // nao necessario neste ponto do programa

    /*  PIR1: PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 1
     *  ====================================================
     *          obs: existe outro PIR2 para mais perifericos
     *
     *      SPPIF: Streaming Parallel Port Read/Write Interrupt Flag bit
     *
     *      ADIF: A/D Converter Interrupt Flag bit
     *
     *      RCIF: EUSART Receive Interrupt Flag bit
     *
     *      TXIF: EUSART Transmit Interrupt Flag bit
     *
     *      SSPIF: Master Synchronous Serial Port Interrupt Flag bit
     *             0 = esperando para receber ou transmitir
     *             1 = a recepcao ou transmissao esta completada
     *
     *      CCP1IF: CCP1 Interrupt Flag bit
     *
     *      TMR2IF: TMR2 to PR2 Match Interrupt Flag bit
     *
     *      TMR1IF: TMR1 Overflow Interrupt Flag bit
     */


    PIR2bits.BCLIF=0;   // limpa o flag de COLISAO do barramento
                        // nao necessario na inicializacao do programa

    /*  PIR2: PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 2
     *  ====================================================
     *
     *      OSCFIF: Oscillator Fail Interrupt Flag bit
     *
     *      CMIF: Comparator Interrupt Flag bit
     *
     *      USBIF: USB Interrupt Flag bit
     *
     *      EEIF: Data EEPROM/Flash Write Operation Interrupt Flag bit
     *
     *      BCLIF: Bus Collision Interrupt Flag bit
     *             1 = houve COLISAO no barramento
     *             obs: tem que ser ZERADO em software
     *
     *      HLVDIF: High/Low-Voltage Detect Interrupt Flag bit
     *
     *      TMR3IF: TMR3 Overflow Interrupt Flag bit
     *
     *      CCP2IF: CCP2 Interrupt Flag bit
     */



    /* ADCON0: A/D CONTROL REGISTER 0 (trata somente conversao A/D)
     * ==============================
     *
     * CHS3:CHS0: Analog Channel Select bits (qual canal sera usado na
     *            conversao)
     *
     * GO/DONE: A/D Conversion Status bit (inicio da conversao e resultado
     *          pronto)
     *
     * ADON: A/D On bit: habilitacao do processo de conversao A/D
     *
     */


     ADCON1=0x0F;              // desabilita todas as portas analogicas e
                              // as torna digitais

     /* ADCON1: A/D CONTROL REGISTER 1
      *  ==============================
      *
      * VCFG1: Voltage Reference Configuration bit (VREF AN2 = 1; VDD = 0)
      *
      * VCFG0: Voltage Reference Configuration bit (VREF AN3 = 1; VDD = 0)
      *
      * PCFG3:PCFG0: A/D Port Configuration Control bits
      *              quais portas fisicas serao digitais ou analogicas
      *              * depende do CONFIG BITS com PBADEN (abaixo)
      *
      * obs: depende do CONFIG BITS (PBADEN) de inicializacao da PIC no boot
      *
      * PBADEN: PORTB A/D Enable bit (Affects ADCON1 Reset state. ADCON1
      * controls PORTB<4:0> pin configuration.)
      * 1 = PORTB<4:0> pins are configured as analog input channels on Reset
      * 0 = PORTB<4:0> pins are configured as digital I/O on Reset
      *
      */

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

void testaColisao(void)
{
    unsigned char data;
    signed char status;

        do
	{
	status = WriteI2C( 0xB8 | 0x00 );	//write the address of slave
		if(status == -1)		//check if bus collision happened
		{
                    while(BusyUSART());
                        putrsUSART("\n\rColisao.\n\r");

                        LED_VERM=1;

			data = SSPBUF;		//upon bus collision detection clear the buffer,
			SSPCON1bits.WCOL=0;	// clear the bus collision status bit
		}
	}
	while (status!=0);		//write untill successful communication

        LED_VERM=0;

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
    //Enable TIMER Interrupt (Int_On)

    /* Calculo do Timer0
     *
     *  (1 seg * 8000000) / (4 * 32)   ... 32 = 1/32 prescaler
     *  8.000.000 / 128 = 62500
     *
     *  Timer0 = 65536 - 62500 = 3035 ou 0xBDB
     */

    WriteTimer0( 0xBDC );

    LED_AMAR=1; LED_VERM=0;

    T0CONbits.TMR0ON = 1;     // Ligue o Timer 0
    //T0CONbits.PSA = 0;      // Use valores atraves do Pre-Scaler
    //T0CONbits.T0CS=0;       // A fonte clock eh Interna (CLKO/FOSC) T0_SOURCE_INT

    // Sem Interrupcoes
    INTCONbits.GIE      = 0;    // Interrupcoes Globais desligadas
    INTCONbits.PEIE     = 0;    // Interrupcoes dos Perifericos desligadas
    INTCONbits.TMR0IE   = 0;    // Interrupcoes do Timer0 desligadas

    INTCONbits.TMR0IF   = 0;    // Zera o contador do Timer0
   
    while (segundos > 0)
    {
        while (!TMR0IF) ;       // enquando o Timer0 nao tiver overflow, espere

        LED_AMAR=~LED_AMAR;     // Led Amarelo pisca a cada segundo
        //segundos--;
        //WriteTimer0( 0xBDC );
        INTCONbits.TMR0IF=0;    // Zera o contador do Timer0
        segundos--;
    }
    
    CloseTimer0();              // Feche o Timer0
    //TMR0ON=0;

    LED_AMAR=0; LED_VERM=0; LED_VERD=0;
}

void interrupt global_isr(void)
{
    if (TMR0IE && TMR0IF)
    {
         LED_VERD=~LED_VERD;        // gera uma mudanca de estado LED Verde

        //WriteTimer0 ( 0x3D );     // Nao eh necessario re-escrever o valor
                                    // de Overflow para o Timer0
        TMR0IF=0;                   // Limpa o status de Overflow do Timer0
        //TMR0IE=1;                 // Nao eh necessario re-habilitar
                                    // a Interrupcao

    }

    LED_VERD=~LED_VERD; 
}