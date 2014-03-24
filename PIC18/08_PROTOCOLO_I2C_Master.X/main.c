/* 
 * File:   main.c
 * Author: roney
 *
 * Created on 21 de Março de 2014, 16:53    -   Versao 0.1
 * 
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_bits.h"
#include <plib/i2c.h>
#include <plib/usart.h>

#define _XTAL_FREQ 4000000
#define FOSC 4000000UL
#define Baud 100000

#define LED_VERM PORTDbits.RD7  // Led recebera pisca-pisca via software while
#define LED_AMAR PORTDbits.RD6  // Led recebera pisca-pisca via software while
#define LED_VERD1 PORTDbits.RD3  // Led recebera PISCA 1 Segundo via TIMER1 16bit
#define LED_VERD2 PORTDbits.RD2  // Led recebera PISCA 0,1 Segungo via TIMER0 8bit
#define LED_AZUL PORTCbits.RC2  //

/*
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

void ciclo (void);
void inicializacao (void);
void SSPISR(void);
void readDevice2(void);
void teste(void);

/*
 * 
 */

void inicializacao (void)
{
	//SSPCON1 = 0x28;	// I2C ENB, FOSC / (4 * (SSPADD+1))
	//SSPADD = ((FOSC/4)/Baud)-1;
	//SSPIF = 0;
}

int main(int argc, char** argv) {

    unsigned char data;
    signed char status;

    // Definicao de portas para os LEDs
    TRISDbits.RD7=0; TRISDbits.RD6=0; TRISDbits.RD3=0; TRISDbits.RD2=0;
    TRISCbits.RC2=0;
    
    TRISBbits.RB0=1;
    TRISBbits.RB1=1;
    TRISCbits.RC6=0;    // Saida TX EUSART

    ADCON1=0x0F;

    PORTD=0x00; PORTCbits.RC2=0;

    

    
    // SSPADD = (FOSC/ Bit Rate) /4 - 1
    // SSPADD = ( 4.000.000/100.000 )/4 -1
    // SSPADD = ( 40 ) / 4 -1 = 10-1 = 9

    //SSPADD = ((FOSC/4)/Baud)-1;
    //SSPADD =  ( 4000000/4 / 100000) -1;
    //SSPADD =    1.000.000 / 100.000  - 1
    //SSPADD = 10 - 1 = 9
    
    OpenI2C(MASTER,SLEW_OFF);
    SSPADD=9;

    

    CloseUSART();   // fecha qualquer USART que estaria supostamente aberta antes
                    // just closes any previous USART open port

    OpenUSART(  USART_TX_INT_OFF &
                USART_RX_INT_ON &
                USART_ASYNCH_MODE &
                USART_EIGHT_BIT &
                USART_CONT_RX &
                USART_BRGH_LOW,
                25
                );  // 51 = 1200bps@4mhz; 6=9600bps@4mhz; 25=2400bps@4mhz

                while(BusyUSART());
                putrsUSART("\n\rINIT SERIAL; SSPAD=");

                while(BusyUSART());
                putsUSART( itoa(NULL,SSPADD,10) );

                while(BusyUSART());
                putrsUSART(" decimal ");
                putcUSART( SSPADD );
                putrsUSART("\n\r");

    

    LED_VERM = 1;

    StartI2C();
        WriteI2C( 0x5C );
        IdleI2C();
        AckI2C();
        WriteI2C( 0x03 );
        IdleI2C();
        AckI2C();
        WriteI2C( 0x00 );
        IdleI2C();
        AckI2C();
        WriteI2C( 0x00 );
        IdleI2C();
        AckI2C();
    StopI2C();

    LED_AMAR = 1;
    Delay10KTCYx(50);
    //#define StartI2C()  SSPCON2bits.SEN=1;while(SSPCON2bits.SEN)

    /*
    	do
	{
	status = WriteI2C( 0xB8 | 0x00 );	//write the address of slave
		if(status == -1)		//check if bus collision happened
		{
                    while(BusyUSART());
                        putrsUSART("\n\rColisao.\n\r");

			data = SSPBUF;		//upon bus collision detection clear the buffer,
			SSPCON1bits.WCOL=0;	// clear the bus collision status bit
		}
	}
	while(status!=0);		//write untill successful communication
     */ 

        LED_VERM=0; LED_AMAR=0; LED_VERD1=1;
        Delay10KTCYx(50);
        LED_VERD1=0;

    // ---//---

    //while ( SSPCON2bits.SEN );


    // ---//---

    /*
    if (DataRdyI2C())
    {
        unsigned char data = getcI2C();
        LED_VERD1=1;
        Delay10KTCYx(100);
        LED_VERD1=0;
        while(BusyUSART());
            putcUSART(data);
    }*/

        // reading humidity
        //rx_byte =  i2c_read(0x00);
        //rx_byte1 = i2c_read(0x01);

        //read temp
        //rx_byte2 = i2c_read(0x02);
        //rx_byte3 = i2c_read(0x03);

        //printf ("rx_byte1 = 0x%2.2X\r   0x%2.2X\r \n", rx_byte, rx_byte1);
        //printf ("rx_byte1 = 0x%2.2X\r   0x%2.2X\r \n", rx_byte2, rx_byte3);


    //IdleI2C();

    //getcI2C();

    //WriteI2C();

    //LED_AMAR=1;
    //CloseI2C();

    //Delay10KTCYx(100);
    //LED_AMAR=0; LED_VERD1=1;
    //Delay10KTCYx(100); LED_VERD1=0;

    ciclo();

    while (1);


}





/*

 *
 *      (versao correta; abaixo teria bugs)
        I2C1_Start();              // issue I2C start signal
        I2C1_Wr(0xB9);             // send byte via I2C  (device address + W)
 *
        FC = I2C1_Rd(1);       // Read the data (acknowledge)
        RN = I2C1_Rd(1);       // Read the data (acknowledge)
 *
        RHL = I2C1_Rd(1);       // Read the data (acknowledge)
        RHH = I2C1_Rd(1);       // Read the data (acknowledge)
 *
        TEMPL = I2C1_Rd(1);       // Read the data (acknowledge)
        TEMPH = I2C1_Rd(1);       // Read the data (acknowledge)
 *
        CRC = I2C1_Rd(1);       // Read the data (acknowledge)
        I2C1_Stop();
 *
 *
    I2C1_Init(100000);         // initialize I2C communication
    I2C1_Start();              // issue I2C start signal
    I2C1_Wr(0xB8);             // send byte via I2C  (device address + W)
    I2C1_Wr(0x03);             // send byte (address of EEPROM location)
    I2C1_Wr(0x00);             // send data (data to be written)
    I2C1_Wr(0x00);
    I2C1_Stop();               // issue I2C stop signal

    I2C1_Start();              // issue I2C start signal
    I2C1_Wr(0xB9);             // send byte via I2C  (device address + W)
 *
    FC = I2C1_Rd(1);           // Read the data (acknowledge)
    RN = I2C1_Rd(1);           // Read the data (acknowledge)
 *
    RHL = I2C1_Rd(1);          // Read the data (acknowledge)
    RHH = I2C1_Rd(1);          // Read the data (acknowledge)
 *
    TEMPL = I2C1_Rd(1);        // Read the data (acknowledge)
    TEMPH = I2C1_Rd(1);        // Read the data (acknowledge)
 *
    RHL = I2C1_Rd(1);          // Read the data (acknowledge)
    RHH = I2C1_Rd(1);          // Read the data (acknowledge)
 *
    CRC = I2C1_Rd(1);          // Read the data (acknowledge)
    I2C1_Stop();               // issue I2C stop signal
 *
 *
 */
void SSPISR(void)
{
    unsigned char temp,data;
    temp = SSPSTAT &0x2d;   // 0b-10-1101

    if((temp ^0x09) ==0x00) // 0b-1001
    {  // 1: write operation, last byte was  address
    data = ReadI2C();
    // Do something with data, or just return
    }

    else if((temp ^0x29) ==0x00)    //0b-10-1001
    {  // 2: write operation, last byte was data
    data = ReadI2C();
    // Do something with data, or just return
    }

    else if((temp ^0x0c) ==0x00)    // 0b-1100
    {  // 3: read operation, last byte was address
    // Do something, then write something to I2C
    WriteI2C(0x00);
    }

    else if((temp ^0x2c) ==0x00)    // 0b-10-1100
    {  // 4: read operation, last byte was data
    // Do something, then write something to I2C
    WriteI2C(0x00);
    }

    else{  // 5: slave logic reset by NACK from master
    // Don't do anything, clear a buffer, reset, whatever
    }
}

void readDevice2(void){
    /*
     I2C1_Start();              // issue I2C start signal
     I2C1_Write(0x5C);          // send byte via I2C  (device address + W)

     I2C1_Write(0x03);          // send byte (address of EEPROM location)
     I2C1_Write(0x00);          // send data (data to be written)
     I2C1_Write(0x04);
     I2C1_Stop();               // issue I2C stop signal

     I2C1_Start();              // issue I2C start signal
     I2C1_Write(0x5C);          // send byte via I2C  (device address + W)
     I2C1_Write(0x03);          // send byte (address of EEPROM location)
     I2C1_Write(0x00);          // send data (data to be written)
     I2C1_Write(0x04);
     I2C1_Stop();

     I2C1_Start();                  // issue I2C start signal
     I2C1_Write(0x5C);              // send byte via I2C  (device address + W)
     dummy = I2C1_Read(1);          // Read the data (acknowledge)
     dummy = I2C1_Read(1);          // Read the data (acknowledge)
     FC = I2C1_Read(1);             // Read the data (acknowledge)
     RN = I2C1_Read(1);             // Read the data (acknowledge)
     RHL = I2C1_Read(1);            // Read the data (acknowledge)
     RHH = I2C1_Read(1);            // Read the data (acknowledge)
     TEMPL = I2C1_Read(1);          // Read the data (acknowledge)
     TEMPH = I2C1_Read(1);          // Read the data (acknowledge)
     I2C1_Stop();

     RH = RHH << 8;
     RH |= RHL;

     TEMP = TEMPH << 8;
     TEMP |= TEMPL;
     */

}

void teste (void)
{
    unsigned char rx_byte;
    unsigned char rx_byte1;



    printf ("Start i2c \n");
    StartI2C();
    __delay_ms(50);

    WriteI2C(0xB8);  /* Device Address */
    __delay_ms(50);
    WriteI2C(0x03);
    __delay_ms(100);

    // reading humidity
    rx_byte = ReadI2C();
    __delay_ms(50);
    rx_byte1 =ReadI2C();
    __delay_ms(50);

    //read temp
    rx_byte = ReadI2C();
    __delay_ms(50);
    rx_byte1 = ReadI2C();
    __delay_ms(50);
    printf ("rx_byte1 = 0x%2.2X\r\n", rx_byte1);
    printf ("rx_byte1 = 0x%2.2X\r\n", rx_byte1);
    StopI2C();
    printf ("End\n");

}

void ciclo (void)
{
    unsigned char TEMPL=0, TEMPH=0, PRESSAOL=0, PRESSAOH=0;
    unsigned char DUMMY=0, OP=0, BT=0;
    char msg[55];

    Delay10KTCYx(100); //Passing 0 (zero) results in a delay of 2,560,000 cycles

    while(BusyUSART());
    putrsUSART("\n\rINIT SERIAL.\n\r");

    while (1)
    {
        //putrsUSART("\r\n____OPEN I2C; ");
        //OpenI2C(MASTER,SLEW_OFF);
        LED_VERM=1;

        while(BusyUSART());
        putrsUSART("\r\n____START I2C; ");

        StartI2C();
            WriteI2C(0x5C);     // endereco Slave do AM2315
            WriteI2C(0x03);     // byte que simboliza a temperatura
            WriteI2C(0x00);     // start byte para leitura
            WriteI2C(0x04);     // quantidades de bytes a serem lidos;
            Delay1KTCYx(2);    // 2 milisegundos (800 us a 3 ms)
            __delay_ms(1);            
        StopI2C();

        

        // 10K (100) = 1000 ms
        // 1K  (100) = 100 ms
        // 1K  (10)  = 10 ms
        // 1K  (2)   = 2 ms
        // Delay100TCYx();
        __delay_ms(2);

        StartI2C();
            WriteI2C(0x5C);     // endereco Slave do AM2315
            WriteI2C(0x03);     // byte que simboliza a temperatura
            WriteI2C(0x00);     // start byte para leitura
            WriteI2C(0x04);     // quantidades de bytes a serem lidos;
            AckI2C();
        StopI2C();

        //AckI2C();

        __delay_us(1500);

        //LED_VERD2=1;

       // Delay10KTCYx(1);
        StartI2C();
        WriteI2C(0x5C);     // endereco Slave do AM2315
        IdleI2C(); AckI2C();
        //AckI2C();
            //__delay_us(30);
            OP          = ReadI2C();
            BT          = ReadI2C();
            TEMPL       = ReadI2C();
            TEMPH       = ReadI2C();
            PRESSAOL    = ReadI2C();
            PRESSAOH    = ReadI2C();
            DUMMY       = ReadI2C();

        StopI2C();

        //AckI2C();

        
        LED_VERM=0; LED_AMAR=0;LED_VERD1=0;LED_VERD2=0;
        LED_AZUL=1;

        sprintf (msg, "OP=%2.2X BT=%2.2X PL=%2.2X %2.2X TL=%2.2X %2.4X Dy=%2.4X \r\n",OP,BT,PRESSAOH,PRESSAOL,TEMPL,TEMPH,DUMMY);
        //printf ("TEMPL = 0x%2.2X TEMPH= 0x%2.2X \r ",TEMPL,TEMPH);
        
        while(BusyUSART());
        //putrsUSART(msg);
        putsUSART(msg);

        //,

        Delay10KTCYx(100); LED_AZUL=0;
        while(BusyUSART());
        putrsUSART("\n\r");
        Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);Delay10KTCYx(100);Delay10KTCYx(100);
        Delay10KTCYx(100);Delay10KTCYx(100);
         
    }



}