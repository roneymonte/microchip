/*******************************************************************************
  I2C Generated Driver API Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    i2c.h

  Summary:
    This is the generated header file for the I2C driver using MPLAB® Code Configurator

  Description:
    This header file provides APIs for driver for I2C. 
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


#ifndef I2C_H
#define I2C_H

#include <xc.h> 
#include <stdint.h>
#include <stdbool.h>


/********* THIS IS THE USER GENERATED I2C HEADER ***********/

/**
  @Prototype        void  I2C_InitializerDefault(void)
  @Param            none - parameters required to initialize i2c are provided in UI
  @Return           none
  @Comment          
  @Usage            I2C_InitializerDefault();
*/
void  I2C_InitializerDefault(void);


/**
  This function will read a frame of data from the i2c bus.

  @Prototype        void I2C_ReadFrame(uint8_t slaveAddress, uint8_t frameLength)
  @Param            slaveAddress: Address of slave device
                    frameLength: Number of bytes to be read from the i2c bus
  @Return           none
  @Usage            I2C_ReadFrame(slaveAddress, frameLength);
*/
void I2C_ReadFrame(uint8_t slaveAddress, uint8_t frameLength);


/**
  This function will write a frame from into the i2c bus.

  @Prototype        void I2C_WriteFrame(uint8_t slaveAddress, uint8_t frameLength)
  @Param            slaveAddress: Address of slave device
                    frameLength: Number of bytes to write into the i2c bus.
  @Return           none
  @Usage            I2C_WriteFrame(slaveAddress, frameLength);
*/
void I2C_WriteFrame(uint8_t slaveAddress, uint8_t frameLength);

/**
  This function will write a byte into the i2c write buffer.

  @Prototype        void I2C_WriteTo_i2cWriteBuffer(uint8_t i2cData)
  @Param            i2cData : byte to be written in the buffer
  @Return           none
  @Usage            I2C_WriteTo_i2cWriteBuffer(i2cDdata);
*/
void I2C_WriteTo_i2cWriteBuffer(uint8_t i2cData);

/**
  This function will read a byte from the i2c read buffer.

  @Prototype        uint8_t I2C_ReadFrom_i2cReadBuffer(void)
  @Param            none
  @Return           Data byte from i2c read buffer
  @Usage            I2C_ReadFrom_i2cReadBuffer();
*/
uint8_t I2C_ReadFrom_i2cReadBuffer(void);

/**
  This function will check whether the i2c write buffer is full or not.

  @Prototype        bool I2C_Isi2cWriteBufferFull(void)
  @Param            none
  @Return           TRUE : i2c write buffer is full
                    FALSE : i2c write buffer is not full
  @Usage            I2C_Isi2cWriteBufferFull();
*/
bool I2C_Isi2cWriteBufferFull(void);

/**
  This function will check whether data is available in the i2c read buffer.

  @Prototype        bool I2C_IsDataAvailableIni2cReadBuffer(void)
  @Param            none
  @Return           TRUE : Data is available in the i2c read buffer
                    FALSE : Data is not available in the i2c read buffer
  @Usage            I2C_IsDataAvailableIni2cReadBuffer();
*/
bool I2C_IsDataAvailableIni2cReadBuffer(void);

/**
  This function will return the status of i2c.

  @Prototype       uint8_t I2C_CheckStatus(void)
  @Param           none
  @Return          i2c state
  @Usage           I2C_CheckStatus();
*/
uint8_t I2C_CheckStatus(void);

/**
  I2C Interrupt Service Routine. Called by the Interrupt Manager.
  Place your i2c interrupt code here.

  @Protoype        void I2C_ISR(void)
  @Param           none
  @Return          none
  @Usage           I2C_ISR();
*/
void I2C_ISR(void);
 
/**
  I2C bus collision Interrupt Service Routine. Called by the Interrupt Manager.
  Place your bus collision interrupt code here.

  @Protoype        void I2C_BusCollisionISR(void)
  @Param           none
  @Return          none
  @Usage           I2C_BusCollisionISR();
*/
void I2C_BusCollisionISR(void);

#endif  // I2C.h

