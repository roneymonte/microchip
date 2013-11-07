/*******************************************************************************
  Generated MPLAB� Code Configurator Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    code_manager.h

  Summary:
    This is the MPLAB� Code Configurator file generated using MPLAB� Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI. 
    Generation Information : 
        Product Revision  :  MPLAB� Code Configurator - v1.0
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


#ifndef CODE_MANAGER_H
#define	CODE_MANAGER_H
#include <xc.h>
#include "pin_manager.h"
#include "interrupt_manager.h"
#include "eusart.h"
#include "tmr0.h"
#include "tmr1.h"
#include "memory.h"
#include "i2c.h"
#include "adc.h"

#define _XTAL_FREQ  500000

/**
 * @prototype       void SYSTEM_Initializer(void)
 * @param           none
 * @return          none
 * @description     Initializes the device to the default states configured in the 
 *                  PCG GUI
 * @usage           SYSTEM_Initializer(void);
 */
void SYSTEM_Initializer(void);

/**
 * @prototype       void OSCILLATOR_Initializer(void)
 * @param           none
 * @return          none
 * @description     Initializes the oscillator to the default states configured in the 
 *                  PCG GUI
 * @usage           OSCILLATOR_Initializer(void);
 */
void OSCILLATOR_InitializerDefault(void);
#endif	/* CODE_MANAGER_H */
