
/*******************************************************************************
  I2C Generated Driver File 

  Company:
    Microchip Technology Inc.

  File Name:
    i2c.c

  Summary:
    This is the generated driver implementation file for the I2C driver using MPLAB® Code Configurator

  Description:
    This source file provides implementations for driver APIs for I2C. 
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
#include "i2c.h"

enum 
{
    I2C_START,
    I2C_SEND_ADDRESS,
    I2C_RECEIVE_ENABLE,
    I2C_READ,
    I2C_LAST_BYTE,
    I2C_STOP_READ,
    I2C_RESET_AFTER_READ
}I2C_READ_STATE_MACHINE;

enum 
{
    I2C_INITIATE_START,
    I2C_SEND_ADDR,
    I2C_WRITE,
    I2C_STOP,
    I2C_RESET_AFTER_WRITE
}I2C_WRITE_STATE_MACHINE;

enum 
{
    I2C_BUS_IDLE,
    I2C_BUS_BUSY,
    I2C_BUS_COLLISION,
    I2C_ACK_NOT_RECEIVED,
    I2C_IN_PROGRESS
}I2C_STATE;


#define I2C_READ_HEAD_SIZE (8-1)  
#define I2C_WRITE_HEAD_SIZE (8-1) 

uint8_t i2cReadBuffer[8];
uint8_t i2cWriteBuffer[8];

uint8_t i2cModuleReadHeadIndex = 0;
uint8_t i2cModuleWriteHeadIndex = 0;
uint8_t i2cCpuReadHeadIndex = 0;
uint8_t i2cCpuWriteHeadIndex = 0;

uint8_t isi2cRead;
uint8_t i2cFrameLength ;
uint8_t i2cSlaveAddress;

/************* THIS IS THE USER GENERATED I2C SOURCE FILE ************************************/

/**
  Prototype:        void I2C_InitializerDefault(void)
  Input:            none
  Output:           none
  Description:      custom init function for InitializerDefault
  Comment:          
  Usage:            I2C_InitializerDefault();
*/
void I2C_InitializerDefault(void)
{
    // set the I2C module to the options selected in the User Interface

    // BF RCinprocess_TXcomplete; UA dontupdate; SMP midsample_enable; P stopbit_notdetected; S startbit_notdetected; R_nW write_noTX; CKE tx_on_idle_to_active; D_nA lastbyte_address; 
    SSP1STAT = 0x00;

    // SSPEN disabled; WCOL no_collision; SSPOV no_overflow; CKP lo_hold; SSPM I2CMaster_FOSC/4_SSPxADD; 
    SSP1CON1 = 0x08;

    // ACKSTAT received; RCEN disabled; RSEN disabled; ACKEN disabled; ACKDT acknowledge; SEN disabled; GCEN disabled; PEN disabled; 
    SSP1CON2 = 0x00;

    // BOEN disabled; AHEN disabled; SBCDE disabled; SDAHT 100nshold; ACKTIM ackseq; DHEN disabled; PCIE disabled; SCIE disabled; 
    SSP1CON3 = 0x00;

    // SSPBUF 0x0; 
    SSP1BUF = 0x00;

    // SSPMSK 0x0; 
    SSP1MSK = 0x00;

    // SSPADD 255; 
    SSP1ADD = 0xFF;

    PIE1bits.SSP1IE = 1;
    PIE2bits.BCL1IE = 1;
}



/**
  Prototype:        void I2C_ReadFrame(uint8_t slaveAddress, uint8_t frameLength)
  Input:            slaveAddress: Address of slave device
                    frameLength: Number of bytes to be read from the I2C bus
  Output:           none
  Description:      This function is used to read a byte from the I2C bus. This is a non blocking function.
  Usage:            I2C_ReadFrame(slaveAddress, frameLength);
*/
void I2C_ReadFrame(uint8_t slaveAddress, uint8_t frameLength)
{
   i2cSlaveAddress = slaveAddress;
   i2cFrameLength = frameLength;
   isi2cRead = 1;
   I2C_STATE = I2C_IN_PROGRESS;
   switch (I2C_READ_STATE_MACHINE)
   {
       case I2C_START : if (SSP1STATbits.S)
                         {
                            I2C_STATE = I2C_BUS_BUSY;
                         }
                         else
                         {
                            SSP1CON2bits.SEN = 1;
                            I2C_READ_STATE_MACHINE++;
                         }
                         break;

       case I2C_SEND_ADDRESS : SSPBUF = i2cSlaveAddress | 0x01;
                                I2C_READ_STATE_MACHINE++;
                                break;

       case I2C_RECEIVE_ENABLE : if (SSP1CON2bits.ACKSTAT)
                                  {
                                      I2C_STATE = I2C_ACK_NOT_RECEIVED;
                                      I2C_READ_STATE_MACHINE = I2C_RESET_AFTER_READ;
                                      SSP1CON2bits.PEN = 1;
                                  }
                                  else
                                  {
                                      SSP1CON2bits.RCEN = 1;
                                      I2C_READ_STATE_MACHINE++;
                                      if (frameLength == 1)
                                      I2C_READ_STATE_MACHINE = I2C_LAST_BYTE;
                                  }
                                  break;

       case I2C_READ : i2cReadBuffer[i2cModuleWriteHeadIndex++] = SSPBUF;
                        SSP1CON2bits.ACKDT = 0;
                        SSP1CON2bits.ACKEN = 1;
                        I2C_READ_STATE_MACHINE = I2C_RECEIVE_ENABLE;
                        i2cFrameLength--;
                        i2cModuleWriteHeadIndex &= I2C_READ_HEAD_SIZE;
                        break;

       case I2C_LAST_BYTE : i2cReadBuffer[i2cModuleWriteHeadIndex++] = SSPBUF;
                             SSP1CON2bits.ACKDT = 1;
                             SSP1CON2bits.ACKEN = 1;
                             I2C_READ_STATE_MACHINE = I2C_STOP_READ;
                             i2cModuleWriteHeadIndex &= I2C_READ_HEAD_SIZE;
                             break;

       case I2C_STOP_READ : SSP1CON2bits.PEN = 1;
                             I2C_STATE = I2C_BUS_IDLE;
                             I2C_READ_STATE_MACHINE++;
                             break;

       case I2C_RESET_AFTER_READ : I2C_READ_STATE_MACHINE = I2C_START;
                                    break;
   }
}
    

/**
  Prototype:        void I2C_WriteFrame(uint8_t slaveAddress, uint8_t frameLength)
  Input:            slaveAddress: Address of slave device
                    frameLength: Number of bytes to write into the I2C bus.
  Output:           none
  Description:      This function is used to write a frame of data into the I2C bus. This is a non blocking function.
  Usage:            I2C_WriteFrame(slaveAddress, frameLength);
*/
void I2C_WriteFrame(uint8_t slaveAddress, uint8_t frameLength)
{
    i2cSlaveAddress = slaveAddress;
    i2cFrameLength = frameLength;
    isi2cRead = 0;
    I2C_STATE = I2C_IN_PROGRESS;
    switch (I2C_WRITE_STATE_MACHINE)
    {
        case I2C_INITIATE_START : if (SSP1STATbits.S)
                                   {
                                       I2C_STATE = I2C_BUS_BUSY;
                                   }
                                   else
                                   {
                                       SSP1CON2bits.SEN = 1;
                                       I2C_WRITE_STATE_MACHINE++;
                                   }
                                   break;

        case I2C_SEND_ADDR : SSPBUF = i2cSlaveAddress;
                              I2C_WRITE_STATE_MACHINE++;
                              break;

	case I2C_WRITE : if (SSP1CON2bits.ACKSTAT)
                          {
                              I2C_STATE = I2C_ACK_NOT_RECEIVED;
                              I2C_WRITE_STATE_MACHINE = I2C_RESET_AFTER_WRITE;
                              SSP1CON2bits.PEN = 1;
                          }
                          else
                          {
                              SSPBUF = i2cWriteBuffer[i2cModuleReadHeadIndex++];
                              i2cFrameLength--;
                              i2cModuleReadHeadIndex &= I2C_WRITE_HEAD_SIZE;
                              if (!i2cFrameLength)
                              {
                                 I2C_WRITE_STATE_MACHINE = I2C_STOP;
                              }
                          }
                          break;

        case I2C_STOP : SSP1CON2bits.PEN = 1;
                         I2C_WRITE_STATE_MACHINE = I2C_RESET_AFTER_WRITE;
                         break;

        case I2C_RESET_AFTER_WRITE : I2C_WRITE_STATE_MACHINE = I2C_INITIATE_START;
                                      break;
    }
}   
 
/**
  Prototype:        uint8_t I2C_CheckStatus(void)
  Input:            none
  Output:           I2C operation status
  Description:      The function will return the present status of I2C operation.
  Usage:            I2C_CheckStatus();
*/
uint8_t I2C_CheckStatus(void)
{
    uint8_t presentStatus = I2C_STATE;
    I2C_STATE = 0;
    return presentStatus;
} 

/**
  Prototype:        void I2C_WriteTo_i2cWriteBuffer(uint8_t i2cData)
  Input:            i2cData : byte to be written in the buffer
  Output:           none
  Description:      This function will write a byte into the i2c write buffer.
  Usage:            I2C_WriteTo_i2cWriteBuffer(i2cData);
*/

void I2C_WriteTo_i2cWriteBuffer(uint8_t i2cData)
{
    i2cCpuWriteHeadIndex &= I2C_WRITE_HEAD_SIZE;
    i2cWriteBuffer[i2cCpuWriteHeadIndex++] = i2cData;
}

/**
  Prototype:        uint8_t I2C_ReadFrom_i2cReadBuffer(void)
  Input:            none
  Output:           Data byte from I2C read buffer
  Description:      This function will read a byte from the I2C read buffer.
  Usage:            I2C_ReadFrom_i2cReadBuffer();
*/

uint8_t I2C_ReadFrom_i2cReadBuffer(void)
{
    i2cCpuReadHeadIndex &= I2C_READ_HEAD_SIZE;
    return (i2cReadBuffer[i2cCpuReadHeadIndex++]);
}

/**
   Prototype:        bool I2C_IsDataAvailableIni2cReadBuffer(void)
   Input:            none
   Output:           TRUE : Data is available in the I2C read buffer
                     FALSE : Data is not available in the I2C read buffer
   Description:      This function will check whether data is available in the I2C read buffer.
   Usage:            I2C_IsDataAvailableIni2cReadBuffer();
*/

bool I2C_IsDataAvailableIni2cReadBuffer(void)
{
    if (i2cModuleWriteHeadIndex == i2cCpuReadHeadIndex)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
  Prototype:       bool I2C_Isi2cWriteBufferFull(void)
  Input:           none
  Output:          TRUE : I2C write buffer is full
                   FALSE : I2C write buffer is not full
  Description:     This function will check whether the I2C write buffer is full or not.
  Usage:           I2C_Isi2cWriteBufferFull();
*/

bool I2C_Isi2cWriteBufferFull(void)
{
    if (i2cCpuWriteHeadIndex > I2C_WRITE_HEAD_SIZE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
  Prototype:        void I2C_ISR(void)
  Input:            none
  Output:           none
  Description:      I2C Interrupt Service Routine. Called by the Interrupt Manager.
                    Place your I2C Interrupt code here.
  Usage:            I2C_ISR();
*/
void I2C_ISR(void)
{
    PIR1bits.SSP1IF = 0;
    if (isi2cRead)
    {
        I2C_ReadFrame(i2cSlaveAddress, i2cFrameLength);
    }
    else
    {
        I2C_WriteFrame(i2cSlaveAddress, i2cFrameLength);
    }
}

/**
  Prototype:        void I2C_BusCollisionISR(void)
  Input:            none
  Output:           none
  Description:      Bus Collision Interrupt Service Routine. Called by the Interrupt Manager.
                    Place your Bus Collision Interrupt code here.
  Usage:            I2C_BusCollisionISR();
*/
void I2C_BusCollisionISR(void)
{
    PIR2bits.BCL1IF = 0;
    I2C_STATE = I2C_BUS_COLLISION;
}

