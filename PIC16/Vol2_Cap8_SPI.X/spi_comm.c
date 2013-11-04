#include <xc.h>
#include "spi_comm.h"
#include "pause.h"

//    TRISA5=0;   // pino CS como output
//    TRISC5=0;   // pino SDO (MOSI) como output
//    TRISC3=0;   // pino SCK como output

//    TRISC4=1;   // pino SDI (MISO) como input

#define CS  RA5 // no chip pic16f887
#define SCK RC3 // CLOCK (LED
#define SDI RC4 // MISO
#define SDO RC5 // MOSI


char SPI_OUTPUT (char spi_byte)
{
    SSPBUF=spi_byte;
    do
    {

    } while (!BF);
    return SSPBUF;
}

void WR_BYTE (char E0_address, char EE_address, char EE_data)
{
    char dummy;

    /* WRITE ENABLE SEQUENCE */
    CS=0;   // Selecione a EEPROM colocando CS em low
    dummy=SPI_OUTPUT( 0x06 );    // envia write enable command
    CS=1;   //  libera a EEPROM colocando o CS em high
    pause(5);


    CS=0;  // Seleciona habilitando a EEPROM em high
    dummy=SPI_OUTPUT (0x02);    // envia o write command


    //dummy=SPI_OUTPUT ( EE_address >> 8 ); // envia o address location = 0
    dummy=SPI_OUTPUT ( E0_address ); // envia o address location
    dummy=SPI_OUTPUT ( EE_address ); // envia o address location
    /*
    SPI1_transfer( address >> 8 );
    SPI1_transfer( address );
     */

    dummy=SPI_OUTPUT ( EE_data );    // envia os dados
    CS=1;   // libera a EEPROM para permitir o byte para ser escrito
}

char RD_BYTE (char E0_address, char EE_address)
{
    char dummy;
    char spi_received=0;

    CS=0;   // seleciona o chip low
    dummy=SPI_OUTPUT(0x03); // envia read command

    //dummy=SPI_OUTPUT(EE_address >> 8);   // envia o endereco
    dummy=SPI_OUTPUT(E0_address);   // envia o endereco
    dummy=SPI_OUTPUT(EE_address);   // envia o endereco

    spi_received=SPI_OUTPUT(0x00);  // recebe o valor armazenado

    CS=1;   // coloca o chip select em high para terminar o comando de leitura

    return spi_received;
}