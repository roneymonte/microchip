/*******************************************************************************
  MEMORY Generated Driver File 

  Company:
    Microchip Technology Inc.

  File Name:
    memory.c

  Summary:
    This is the generated driver implementation file for the MEMORY driver using MPLAB® Code Configurator

  Description:
    This header file provides implementations for driver APIs for MEMORY. 
    Generation Information : 
        Product Revision  :  MPLAB® Code Configurator - v1.0
        Device            :  PIC16F1824
        Version           :  1.1
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.21
        MPLAB             :  MPLAB X 1.90
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/


#include <xc.h>
#include "memory.h"

/**
  Prototype:        uint16_t FLASH_ReadWord(uint16_t flashAddr)
  Input:            flashAddr:  Flash program memory location from which
                                data has to be read
  Output:           Returns data word from the mentioned address in Flash
                    program memory
  Description:      Reads a word from Flash stored at 'flashAddr' address
  Usage:            FLASH_ReadWord(uint16_t flashAddr);
*/
uint16_t FLASH_ReadWord(uint16_t flashAddr)
{
    uint8_t GIEBitValue = 0;

    EEADRL = (flashAddr & 0x00FF);
    EEADRH = ((flashAddr & 0xFF00) >> 8);
    
    EECON1bits.CFGS = 0;        // Deselect Configuration space 
    EECON1bits.EEPGD = 1;   // Select Program Memory
    GIEBitValue = INTCONbits.GIE;
    INTCONbits.GIE = 0;                                         // Disable INTs
    EECON1bits.RD = 1;      // Initiate Read
    asm("NOP");
    asm("NOP");
    INTCONbits.GIE = GIEBitValue;
    
    return ((EEDATH << 8) | EEDATL);
}

/**
  Prototype:        void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word)
  Input:            flashAddr:  Flash program memory location to store word
                    *ramBuf:    Pointer to an array of size 'ERASE_FLASH_BLOCKSIZE'
                                atleast
                    word:       Word to be stored in Flash program memory
  Output:           none
  Description:      API stores a word at the given Flash address.
  Usage:            FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word);
*/
void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word)
{
    uint16_t *blockStartAddr;
    uint16_t *flashMemPtr;
    uint8_t offset = 0;
    uint8_t i;

    offset = (uint16_t)flashAddr % ERASE_FLASH_BLOCKSIZE;
    blockStartAddr = (uint16_t *)(flashAddr - offset);

    flashMemPtr = blockStartAddr;

    for (i=0; i<ERASE_FLASH_BLOCKSIZE; i++)
    {
        ramBuf [i] = flashMemPtr [i] ;

    }

    // load word at startaddress + offset
    ramBuf[offset] = word;
    // Erases current block
    FLASH_EraseBlock((uint16_t)blockStartAddr);
    // Writes words to current block  
    FLASH_WriteBlock((uint16_t)blockStartAddr, ramBuf);
}

/**
  Prototype:        void FLASH_WriteBlock(uint16_t startAddr, uint16_t *flashWordArray)
  Input:            startAddr:          A valid block starting address in
                                        Flash program memory
                    *flashWordArray:    Pointer to an array of size
                                        'WRITE_FLASH_BLOCKSIZE' atleast
  Output:           none
  Description:      API writes words to complete block in Flash program memory
                    starting from 'startAddr'
  Usage:            FLASH_WriteBlock(uint16_t startAddr, uint16_t *flashWordArray);
*/
void FLASH_WriteBlock(uint16_t startAddr, uint16_t *flashWordArray)
{   
    uint8_t i;
    uint8_t GIEBitValue = 0;
    
    // block erase sequence
    FLASH_EraseBlock(startAddr);

    // block write sequence
    GIEBitValue = INTCONbits.GIE;
    INTCONbits.GIE = 0;         // Disable INTs
    // Load lower 8 bits of erase address boundary
    EEADRL = (startAddr & 0xFF);
    // Load upper 6 bits of erase address boundary
    EEADRH = ((flashWordArray [i] & 0xFF00) >> 8);            
    EECON1bits.EEPGD = 1;   // Select Program Memory
    EECON1bits.CFGS = 0;    // Deselect Configuration space
    EECON1bits.WREN = 1;    // Enable wrties
    EECON1bits.LWLO = 1;    // Only load write latches
    
    for (i=0; i<(WRITE_FLASH_BLOCKSIZE-1); i++)
    {
        EEDATL = flashWordArray [i];      // load data in current address
        EEDATH = ((flashWordArray [i] & 0xFF00) >> 8);

        // Start of required sequence to initiate erase
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1;  // Set WR bit to begin write  
        asm("NOP");                                               
        asm("NOP");                                                

        // point to next program location in block
        EEADRL = ((startAddr+i+1) & 0xFF);
        EEADRH = (((startAddr+i+1) & 0xFF00) >> 8);
    }

    EEDATL = flashWordArray [i];
    EEDATH = ((flashWordArray [i] & 0xFF00) >> 8);

    // no more loading latches - actually start Flash program memory write
    EECON1bits.LWLO = 0;    

    // Start of required sequence to initiate write
    EECON2 = 0x55;            
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      // Set WR bit to begin write
    asm("NOP");                                                   
    asm("NOP");                                                    

    EECON1bits.WREN = 0;       // Disable writes
    INTCONbits.GIE = GIEBitValue;
}

/**
  Prototype:        void FLASH_EraseBlock(uint16_t startAddr)
  Input:            startAddr:  A valid block starting address in
                                Flash program memory
  Output:           none
  Description:      Erases complete Flash program memory block mentioned at
                    address 'startAddr'
  Usage:            FLASH_EraseBlock(uint16_t startAddr);
*/
void FLASH_EraseBlock(uint16_t startAddr)
{
    uint8_t GIEBitValue = 0;

    // Disable INTs so required sequences will execute properly
    GIEBitValue = INTCONbits.GIE;
    INTCONbits.GIE = 0;     
    // Load lower 8 bits of erase address boundary
    EEADRL = (startAddr & 0xFF);
    // Load upper 6 bits of erase address boundary
    EEADRH = ((startAddr & 0xFF00) >> 8); 
    EECON1bits.CFGS = 0;    // Deselect Configuration space
    EECON1bits.EEPGD = 1;   // Select Program Memory
    EECON1bits.FREE = 1;    // Specify an erase operation
    EECON1bits.WREN = 1;    // Allows erase cycles

    // Start of required sequence to initiate erase
    EECON2 = 0x55;            
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      // Set WR bit to begin erase
    asm("NOP");                                                    
    asm("NOP");                                                    

    EECON1bits.WREN = 0;       // Disable writes
    INTCONbits.GIE = GIEBitValue;
}

/**
  Prototype:        void DATAEE_WriteByte(uint8_t bAdd, uint8_t bData))
  Input:            bAdd:   Data EEPROM location to which data to be written
    bData:          data to be written to Data EEPROM location
  Output:           none
  Description:      Write a byte to Data EEPROM at the specified address
  Usage:            DATAEE_WriteByte(uint8_t bAdd, uint8_t bData);
*/
void DATAEE_WriteByte(uint8_t bAdd, uint8_t bData)
{
    uint8_t GIEBitValue = 0;

    EEADRL = (bAdd & 0x0ff);    // Data Memory Address to write
    EEDATL = bData;             // Data Memory Value to write
    EECON1bits.EEPGD = 0;   // Point to DATA memory
    EECON1bits.CFGS = 0;        // Deselect Configuration space
    EECON1bits.WREN = 1;        // Enable writes

    GIEBitValue = INTCONbits.GIE;
    INTCONbits.GIE = 0;     // Disable INTs
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      // Set WR bit to begin write
    // Wait for write to complete
    while (EECON1bits.WR)
    {
    }
    INTCONbits.GIE = GIEBitValue;
    EECON1bits.WREN = 0;    // Disable writes
}

/**
  Prototype:        uint8_t DATAEE_ReadByte(uint8_t bAdd)
  Input:            bAdd:   Data EEPROM location from which data has to be read
  Output:           Returns data read from Data EEPROM location
  Description:      Read a byte from Data EEPROM from the specified address
  Usage:            DATAEE_ReadByte(uint8_t bAdd);
*/
uint8_t DATAEE_ReadByte(uint8_t bAdd)
{
    EEADRL = (bAdd & 0x0ff);    // Data Memory Address to read
    EECON1bits.CFGS = 0;    // Deselect Configuration space
    EECON1bits.EEPGD = 0;   // Point to DATA memory
    EECON1bits.RD = 1;      // EE Read
    asm("NOP");                // Nop may be required for latency
    asm("NOP");                // at high frequencies

    return (EEDATL);
}
