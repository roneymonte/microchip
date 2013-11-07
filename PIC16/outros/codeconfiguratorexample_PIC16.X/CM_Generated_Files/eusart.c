/*******************************************************************************
  EUSART Generated Driver File 

  Company:
    Microchip Technology Inc.

  File Name:
    eusart.c

  Summary:
    This is the generated driver implementation file for the EUSART driver using MPLAB® Code Configurator

  Description:
    This header file provides implementations for driver APIs for EUSART. 
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
#include "eusart.h"

/**
  Prototype:        void EUSART_InitializerDefault(void)
  Input:            none
  Output:           none
  Description:      EUSART_InitializerDefault is an
                    initialization routine that takes inputs from the USART GUI.
  Comment:          
  Usage:            EUSART_InitializerDefault();
*/
void EUSART_InitializerDefault(void)
{
    // WUE disabled; ABDEN enabled; ABDOVF no_overflow; RCIDL idle; SCKP async_noninverted_sync_fallingedge; BRG16 16bit_generator; 
    BAUDCON = 0x49;
    // ADDEN disabled; RX9 8-bit; RX9D 0x0; CREN disabled; FERR no_error; SPEN enabled; SREN disabled; OERR no_error; 
    RCSTA = 0x80;
    // SPBRGL 51; 
    SPBRGL = 0x33;
    // SPBRGH 0; 
    SPBRGH = 0x00;
    // TXREG 0x0; 
    TXREG = 0x00;
    // RCREG 0x0; 
    RCREG = 0x00;
    // CSRC slave_mode; TRMT TSR_empty; TXEN enabled; BRGH hi_speed; SYNC asynchronous; SENDB sync_break_complete; TX9D 0x0; TX9 8-bit; 
    TXSTA = 0x26;
}
/**
  Prototype:        void EUSART_InitializerNew(void)
  Input:            none
  Output:           none
  Description:      EUSART_InitializerNew is an
                    initialization routine that takes inputs from the USART GUI.
  Comment:          
  Usage:            EUSART_InitializerNew();
*/
void EUSART_InitializerNew(void)
{
    // WUE disabled; ABDEN disabled; ABDOVF no_overflow; RCIDL idle; SCKP async_noninverted_sync_fallingedge; BRG16 16bit_generator; 
    BAUDCON = 0x48;
    // ADDEN disabled; RX9 8-bit; RX9D 0x0; CREN disabled; FERR no_error; SPEN disabled; SREN disabled; OERR no_error; 
    RCSTA = 0x00;
    // SPBRGL 12; 
    SPBRGL = 0x0C;
    // SPBRGH 0; 
    SPBRGH = 0x00;
    // TXREG 0x0; 
    TXREG = 0x00;
    // RCREG 0x0; 
    RCREG = 0x00;
    // CSRC slave_mode; TRMT TSR_empty; TXEN disabled; BRGH hi_speed; SYNC asynchronous; SENDB sync_break_complete; TX9D 0x0; TX9 8-bit; 
    TXSTA = 0x06;
}


/**
  Prototype:        bool EUSART_IsTxBusy(void)
  Input:            none
  Output:           '1' : If usart transmitter is busy.
                    '0': If usart transmitter is not busy.
  Description:      Function to check whether EUSART transmitter is
                    busy.
  Usage:            EUSART_IsTxBusy();
*/
bool EUSART_IsTxBusy(void)
{
    //check if a byte can be written to the USART by checking the TRMT bit
    return !TXSTAbits.TRMT;
}

/**
  Prototype:        bool EUSART_IsRxBusy(void)
  Input:            none
  Output:           '1: If usart receiver is busy.
                    '0': If usart receiver is not busy.
  Description:      Function to check whether EUSART receiver is
                    busy.
  Usage:            EUSART_IsRxBusy();
*/
bool EUSART_IsRxBusy(void)
{
    //check if receiver is idle by checking the RCIDL bit
    return !BAUDCONbits.RCIDL;
}

/**
  
  Prototype:        bool EUSART_IsDataReady(void)
  Input:            none
  Output:           TRUE '1' : Data is available.
                    FALSE '0' : Data is not available.
  Description:      This routine checks the RCIF flag to see if any data
                    has been received by the USART.
  Usage:            EUSART_IsDataReady();
*/
bool EUSART_IsDataReady(void)
{
    //check if  data is received by checking the RCIF bit in PIR register
    return PIR1bits.RCIF;
}

/**
  
  Prototype:        uint8_t EUSART_GetByte(void)
  Input:            none
  Output:           uint8_t received data
  Description:      EUSART_GetByte function receives a character
                    from the USART and returns it.
  Usage:            EUSART_GetByte();
*/
uint8_t EUSART_GetByte(void)
{
    // Wait for data to be received
    while(!EUSART_IsDataReady())
    {

    }
    return RCREG;      // Return the received data.
}

/**
  Prototype:        void EUSART_PutByte(uint8_t txData)
  Input:            txData: character to be sent
  Output:           none
  Description:      EUSART_PutByte function transmits a
                    character to the USART.
  Usage:            EUSART_PutByte(uint8_t txData);
*/
void EUSART_PutByte(uint8_t txData)
{
    while(EUSART_IsTxBusy())
    {
        
    }
    TXREG = txData;    // Write the data byte to the USART.
    
}

