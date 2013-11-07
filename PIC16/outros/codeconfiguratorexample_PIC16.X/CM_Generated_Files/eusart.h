/*******************************************************************************
  EUSART Generated Driver API Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    eusart.h

  Summary:
    This is the generated header file for the EUSART driver using MPLAB® Code Configurator

  Description:
    This header file provides APIs for driver for EUSART. 
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


#ifndef EUSART_H
#define EUSART_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

/**
  Initialization routine that takes inputs from the USART GUI.
  @Prototype        void EUSART_InitializerDefault(void)
  @Param            none
  @Return           none
  @Comment          
  @Usage            EUSART_InitializerDefault();
 */
void EUSART_InitializerDefault(void);
/**
  Initialization routine that takes inputs from the USART GUI.
  @Prototype        void EUSART_InitializerNew(void)
  @Param            none
  @Return           none
  @Comment          
  @Usage            EUSART_InitializerNew();
 */
void EUSART_InitializerNew(void);

/**
  Function to check whether transmitter is busy.
  @Prototype        bool EUSART_IsTxBusy(void)
  @Param            none
  @Return           '1' : If usart transmitter is busy.
                    '0' : If usart transmitter is not busy.
  @Usage            EUSART_IsTxBusy();
 */
bool EUSART_IsTxBusy(void);

/**
  Function to check whether receiver is busy.
  @Prototype        bool EUSART_IsRxBusy(void)
  @Param            none
  @Return           '1' : If usart receiver is busy.
                    '0' : If usart receiver is not busy.
  @Usage            EUSART_IsRxBusy();
 */
bool EUSART_IsRxBusy(void);

 /**
  This routine checks the RCIF flag to see if any data has been
  received by the USART.
  @Prototype        bool EUSART_IsDataReady(void)
  @Param            none
  @Return           '1' : Data is available.
                    '0' : Data is not available.
  @Usage            EUSART_IsDataReady();
 */
bool EUSART_IsDataReady(void);

/**
  This function receives a character from the USART. 
  @Prototype        uint8_t EUSART_GetByte(void)
  @Param            none
  @Return           uint8_t received data
  @Usage            EUSART_GetByte();
 */
uint8_t EUSART_GetByte(void);

/**
  This function transmits a character to the USART.
  @Prototype        void EUSART_PutByte(uint8_t data)
  @Param            data: character to be sent
  @Return           none
  @Usage            EUSART_PutByte(uint8_t data);
 */
void EUSART_PutByte(uint8_t data);

#endif  // EUSART.h

