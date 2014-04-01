/* 
 * File:   main.c
 * Author: roney
 *
 * Created on April 1, 2014, 11:42 AM
 *
 * Based on Software Example by Nick ODell (4 Aug 2011) for C18 compiler
 * and Austin Schaller (21 Aug 2011)
 *
 * Versao 0.1   -   01/Abr/2014 - Programa ja funcionando OK e coletando
 *                                dados do modulo BM180, porem as formulas
 *                                matematicas ainda nao estao 100%. o calculo
 *                                da pressao barometrica ainda nao esta correto
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib/i2c.h>
#include <plib/usart.h>
#include <plib/delays.h>
#include "configbits.h"
#include <math.h>

#define BMP085_ADDRESS 0xEE      // I2C address of BMP085 (ou BMP180) que eh
                                 // o mesmo do modulo 085 ou 180; ja eh um ende-
                                 // reco de 8 bits, nao precisa << 1
#define P_CORRECTION   1.5       // in mBars - factor to adjust for elevation to match local weather station pressure
                                 // this value for 14' above sea level (in Boca Raton, Florida)
#define OSS 1       // BMP180 Over Sampling Settings

/*
 *                    OSS samples conv time (ms)
lowpower                0   1         4.5
standard                1   2         7.5
high resolution         2   4         13.5
ultra high resolution   3   8         25.5
 */

#define _XTAL_FREQ 8000000
#define FOSC 8000000
#define Baud 100000

#define LED_AMAR    PORTCbits.RC0
#define LED_VERD    PORTCbits.RC1
#define LED_VERM    PORTCbits.RC2



// variaveis da BOSCH para calibragem do medidor de pressao e temperatura
short ac1;  // pressao
short ac2;  // pressao
short ac3;  // pressao
short b1;
short b2;
short mb;
short mc;           // temperatura
short md;           // temperatura
unsigned short ac4; // pressao
unsigned short ac5; // temperatura
unsigned short ac6; // temperatura

/*
 * 
 */
void iniciarI2c(void);
long lerBMP180 (unsigned char endereco);
long lerBMP180temperatura (void);
long lerBMP180pressao (void);
void BMP180conversor (long *temp, long *pressao);
void BMP180calibragem (void);
void configuracao_EUSART (void);


int main(int argc, char** argv) {

    long temperatura=0, pressao=0;
    char msg[40];

    TRISC0 = 0; // Led Amar
    TRISC1 = 0; // Led Verd
    TRISC2 = 0; // LED Verm

    ADCON1=0x0F;    // coloca pinos I/O em modo digital (nao analogico)

    LED_VERM=1;
    iniciarI2c();

    configuracao_EUSART();
    while (BusyUSART());
        putrsUSART("__Iniciando UART 9600 bps\r\n");

    LED_AMAR=1;
    BMP180calibragem();

    Delay10KTCYx(200);
    while (BusyUSART());
        putrsUSART("__Calibragem OK, iniciando...\r\n");

    // Iniciando

    while (1)
    {
        LED_VERD=1;
        BMP180conversor( &temperatura, &pressao );
        LED_VERM=0;

        sprintf(msg,"_Temperatura %2.1d c, Pressao %d Pa \r\n", (temperatura/10), pressao);
        
        while (BusyUSART());
        putsUSART(msg);
        LED_AMAR=0;

        Delay10KTCYx(200);LED_VERD=0;
        Delay10KTCYx(200);Delay10KTCYx(200);Delay10KTCYx(200);
        Delay10KTCYx(200);Delay10KTCYx(200);Delay10KTCYx(200);Delay10KTCYx(200);
        Delay10KTCYx(200);Delay10KTCYx(200);

        
    }

    return (EXIT_SUCCESS);
}

void iniciarI2c(void)
{
    Delay10KTCYx(200);      // Aguarda +- 1 seg

    TRISCbits.TRISC3=0;    // SCL do I2C colocado como saida por causa de bug*
    TRISCbits.TRISC4=0;    // SDA do I2C colocado como saida por causa de bug*
    LATC3=0;            // bug* pede que zere-se o LAT das portas SCL e SDA
    LATC4=0;            // durante inicializacao do I2C para evitar flutuacoes
                        // eletricas que ficariam nas portas antes de liga-las

    Delay10KTCYx(200);   // simples pausa para troca de estado na SDA e SCL

    TRISCbits.TRISC3=1;    // SCL do I2C, agora corretamente como saida
    TRISCbits.TRISC4=1;    // SDA do I2C, agora corretamente como saida

    // Definicao e calculo automatico baseados nos valores #DEFINIDOS
     SSPADD = ((FOSC/4)/Baud)-1;

     CloseI2C();
     OpenI2C(MASTER,SLEW_OFF);
}

long lerBMP180 (unsigned char endereco)
{
    unsigned short msb, lsb, dado;
    char msg[40];

    StartI2C();
        WriteI2C(BMP085_ADDRESS );  // modo default de escrita
        
        WriteI2C(endereco);
        
    // nao eh realmente necessario Stop antes de um Restart ? Nao,
    // o proprio Restart ja gera um Stop implicitamente

    RestartI2C();
        WriteI2C( BMP085_ADDRESS  | 0x01);   // modo de leitura
        
        msb = ReadI2C();    // out_msb (0xF6)
        AckI2C();

        lsb = ReadI2C();    // out_lsb (0xF7)
        NotAckI2C();
        
    StopI2C();

    dado = msb;
    dado *= 256;
    dado |= lsb;
    sprintf(msg,"[0x%x,0x%x=0x%X]", msb, lsb, dado);
    while (BusyUSART());
        putsUSART(msg);

    return dado;
}

long lerBMP180temperatura (void)
{
    StartI2C();
        WriteI2C(BMP085_ADDRESS );

        WriteI2C(0xF4); // campo de pedido
                        // ctrl_meas, 2bits_oss / sco / 4bits_controle
        //AckI2C();
        WriteI2C(0x2E); // pedindo as variaveis de temperatura
    StopI2C();

        __delay_ms(5);  // espere pelo menos 4.5 ms
                        // (conversion time pressure max) para OSS 0 (1 sample)
                        // obs: PDF da Bosch somente indica delay para pressao

        return lerBMP180(0xF6); // ira ler o campo de resultado 0xF6 (out_msb)
                                // sendo long, mesclara com 0xF7 (out_lsb)
}

long lerBMP180pressao (void)
{
    StartI2C();
        WriteI2C(BMP085_ADDRESS );
        
        WriteI2C(0xF4); // campo de pedido
                        // ctrl_meas, 2bits_oss / sco / 4bits_controle
        //AckI2C();
        WriteI2C(0x34); // pedindo as variaveis de pressao descompensada
                        // 0x34 + (oss<<6)
    StopI2C();

    __delay_ms(5);  // espere pelo menos 4.5 ms
                    // (conversion time pressure max) para OSS 0 (1 sample)

    return lerBMP180(0xF6); // ira ler o campo de resultado 0xF6 (out_msb)
                                // sendo long, mesclara com 0xF7 (out_lsb)
}

void BMP180conversor (long *temp, long *pressao)
{
    char msg[40];
    /* pagina 15 do manual da Bosch
     * BST-BMP180-DS000-09 (revisao 2.5 - 5 April 2013 )
     */

    	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;

	// ler a temperatura antes da pressao
	ut = lerBMP180temperatura();
	up = lerBMP180pressao();

	x1 = ((long) ut - ac6) * ac5 >> 15;
	x2 = ((long) mc * 1 << 11) / (x1 + md);
	b5 = x1 + x2;

	*temp = (b5 + 8) >> 4;


	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;

        sprintf(msg,"    ( x1=%d x2=%d x3=%d )\r\n", x1, x2, x3);
        while (BusyUSART()); putsUSART(msg);
        /////////////////////////////////////////////////////

	//b3 = (( ((long) ac1 * 4 + x3) + 2) << 2);
        b3 = ( ( ((long) ac1 * 4 + x3) << OSS) +2 ) >> 2; // (mesmo que / 4)

        sprintf(msg,"    ( b3=%d e ac1=%d )\r\n", b3, ac1);
        while (BusyUSART()); putsUSART(msg);
        /////////////////////////////////////////////////////

	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;

        sprintf(msg,"    ( x1=%d x2=%d x3=%d )\r\n", x1, x2, x3);
        while (BusyUSART()); putsUSART(msg);
        /////////////////////////////////////////////////////


	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);

        sprintf(msg,"    ( b4=%d b7=%d up=%d )\r\n", b4, b7, up);
        while (BusyUSART()); putsUSART(msg);
        /////////////////////////////////////////////////////

	p = ( b7 < 0x80000000 ) ? ( (b7 * 2) / b4 ) : ( (b7 / b4) * 2) ;

        sprintf(msg,"    ( p=%d b7=%d b4=%d )\r\n", p, b7,b4 );
        while (BusyUSART()); putsUSART(msg);
        /////////////////////////////////////////////////////

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;

        sprintf(msg,"    ( x1=%d x2=%d x3=%d )\r\n", x1, x2, x3);
        while (BusyUSART()); putsUSART(msg);
        /////////////////////////////////////////////////////


	*pressao = ( p + ((x1 + x2 + 3791) ))  >> 4;

	__delay_ms(10);
}

void BMP180calibragem (void)
{
    char msg[40];

	ac1 = lerBMP180(0xAA);
	ac2 = lerBMP180(0xAC);
	ac3 = lerBMP180(0xAE);
	ac4 = lerBMP180(0xB0);
	ac5 = lerBMP180(0xB2);
	ac6 = lerBMP180(0xB4);
	b1  = lerBMP180(0xB6);
	b2  = lerBMP180(0xB8);
	mb  = lerBMP180(0xBA);
	mc  = lerBMP180(0xBC);
	md  = lerBMP180(0xBE);

	sprintf(msg,"\tAC1 = %d\r\n", ac1); while (BusyUSART());putsUSART(msg);

	sprintf(msg,"\tAC2 = %d\r\n", ac2); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tAC3 = %d\r\n", ac3); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tAC4 = %d\r\n", ac4); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tAC5 = %d\r\n", ac5); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tAC6 = %d\r\n", ac6); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tB1 = %d\r\n", b1); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tB2 = %d\r\n", b2); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tMB = %d\r\n", mb); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tMC = %d\r\n", mc); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"\tMD = %d\r\n", md); while (BusyUSART());putsUSART(msg);
	sprintf(msg,"------------------------\r\n"); while (BusyUSART());putsUSART(msg);
}

void configuracao_EUSART (void)
{

    TRISC6=0;   // saida TX EUSART na PIC18F2525

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
