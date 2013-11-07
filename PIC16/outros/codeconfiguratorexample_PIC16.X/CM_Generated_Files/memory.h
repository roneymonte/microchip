/*******************************************************************************
  MEMORY Generated Driver API Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    memory.h

  Summary:
    This is the generated header file for the MEMORY driver using MPLAB® Code Configurator

  Description:
    This header file provides APIs for driver for MEMORY. 
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


#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdbool.h>
#include <stdint.h>

#define WRITE_FLASH_BLOCKSIZE    32 
#define ERASE_FLASH_BLOCKSIZE    32 
#define END_FLASH                4096

/**
  This API reads a word from Flash stored at 'flashAddr' address

  @prototype        uint16_t FLASH_ReadWord(uint16_t flashAddr)
  @param            flashAddr:  Flash program memory location from which
                                data has to be read
  @return           Returns data word from the mentioned address
                    in Flash program memory
  @usage            FLASH_ReadWord(uint16_t flashAddr);
 */
uint16_t FLASH_ReadWord(uint16_t flashAddr);
 
 /**
 *
  This API stores a word at the given Flash address
  @prototype        void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word)
  @param            flashAddr:  Flash program memory location to store word
                    *ramBuf:    Pointer to an array of size 'ERASE_FLASH_BLOCKSIZE'
                                atleast
                    word:       Word to be stored in Flash program memory
  @return           none
  @usage            FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word);
 */
void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word);

 /**
  This API writes words to complete block in Flash program memory starting
  from 'startAddr'

  @prototype        void FLASH_WriteBlock(uint16_t startAddr, uint16_t *flashWordArray)
  @param            startAddr:          A valid block starting address in
                                        Flash program memory
                    *flashWordArray:    Pointer to an array of size
                                        'WRITE_FLASH_BLOCKSIZE' atleast
  @return           none
  @usage            FLASH_WriteBlock(uint16_t startAddr, uint16_t *flashWordArray);
 */
void FLASH_WriteBlock(uint16_t startAddr, uint16_t *flashWordArray);
 
 /**
  This API erases complete Flash program memory block at the specified address

  @prototype        void FLASH_EraseBlock(uint16_t startAddr)
  @param            startAddr:  A valid block starting address in Flash
                                program memory
  @return           none
  @usage            FLASH_EraseBlock(uint16_t startAddr);
 */
void FLASH_EraseBlock(uint16_t startAddr);

/**
  This API writes a byte to Data EEPROM at the specified address

  @prototype        void DATAEE_WriteByte(uint8_t bAdd, uint8_t bData)
  @param            bAdd:    Data EEPROM location to which data to be written
                    bData:   data to be written to Data EEPROM location
  @return           none
  @usage            DATAEE_WriteByte(uint8_t bAdd, uint8_t bData);
 */
void DATAEE_WriteByte(uint8_t bAdd, uint8_t bData);

/**
  This API reads a byte from Data EEPROM from the specified address

  @prototype        uint8_t DATAEE_ReadByte(uint8_t bAdd)
  @param            bAdd:   Data EEPROM location from which data has to be read
  @return           Returns data read from Data EEPROM location
  @usage            DATAEE_ReadByte(uint8_t bAdd);
 */
uint8_t DATAEE_ReadByte(uint8_t bAdd);

#endif // _MEMORY_H

