
/*******************************************************************************
  TMR0 Generated Driver API Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    tmr0.h

  Summary:
    This is the generated header file for the TMR0 driver using MPLAB® Code Configurator

  Description:
    This header file provides APIs for driver for TMR0. 
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



#ifndef TMR0_H
#define TMR0_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t timer_size_t;
 
/**
  This initializer configure the Timer control registers.
  @Prototype        void TMR0_InitializerDefault(void)
  @Param            parameters required to initialize comparator are provided by UI
  @Return           none
  @Comment          
  @Usage            TMR0_InitializerDefault();
*/
void TMR0_InitializerDefault(void);

/**
  This routine starts the timer
  @Prototype        void TMR0_StartTimer(void)
  @Param            none
  @Return           none
  @Usage            TMR0_StartTimer function();
*/
void TMR0_StartTimer(void);

/**
  This routine stops the timer
  @Prototype        void TMR0_StopTimer(void)
  @Param            none
  @Return           none
  @Usage            TMR0_StopTimer();
*/
void TMR0_StopTimer(void);

/**
  This routine reads the Timer register value 
  @Prototype        uint8_t TMR0_ReadTimer(void)
  @Param            none
  @Return           Timer value  at the time of the function call
                    read as a 8-bit value
  @Usage            TMR0_ReadTimer();
*/
uint8_t TMR0_ReadTimer(void);

/**
  This routine writes to Timer register
  @Prototype        void TMR0_WriteTimer(uint8_t TimerVal)
  @Param            TimerVal : Timer value to be loaded
  @Return           none
  @Usage            TMR0_WriteTimer(TimerVal);
*/
void TMR0_WriteTimer(uint8_t TimerVal);

/**
  This routine is used to reload value to TMR register
  @Prototype        void TMR0_Reload(void)
  @Param            none
  @Return           none
  @Usage            TMR0_Reload();
*/
void TMR0_Reload(void);

/**
  Boolean routine to poll or to check for the overflow flag on the fly
  @Prototype        bool TMR0_IsOverflowOccured(void)
  @Param            none
  @Return           Timer overflow or no timer overflow
  @Usage            TMR0_IsOverflowOccured();
*/
bool TMR0_IsOverflowOccured(void);

#endif // tmr0.h