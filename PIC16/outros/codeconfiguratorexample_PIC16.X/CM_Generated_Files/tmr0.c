
/*******************************************************************************
  TMR0 Generated Driver File 

  Company:
    Microchip Technology Inc.

  File Name:
    tmr0.c

  Summary:
    This is the generated driver implementation file for the TMR0 driver using MPLAB® Code Configurator

  @Description
    This header file provides implementations for driver APIs for TMR0. 
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
#include "tmr0.h"

uint8_t Timer0ReloadVal;

/**
  @Prototype        void TMR0_InitializerDefault(void)
  @Input            none
  @Output           none
  @Description      custom initializer function for TMR0
  @Comment          
  @Usage            TMR0_InitializerDefault ();
*/
void TMR0_InitializerDefault(void)
{
    // set TMR0 to the options selected in the User Interface
    OPTION_REG = 0xFF; 
    // set TMR0 to the options selected in the User Interface
    TMR0 = 0x00; 
    
    // Load the TMR value to reload variable
    Timer0ReloadVal=TMR0;

    // Clearing IF flag before enabling the interrupt.
    INTCONbits.TMR0IF = 0;    
}

/**
  @Prototype        void TMR0_StartTimer(void)
  @Input            none
  @Output           none
  @Description      TMR0 Start function.
  @Usage            TMR0_StartTimer();
*/
void TMR0_StartTimer(void)
{
    //Start the Timer by writing to TMR0CS bit
    OPTION_REGbits.TMR0CS = 0;
}

/**
  @Prototype        void TMR0_StopTimer(void)
  @Input            none
  @Output           none
  @Description      Function to stop the timer.
  @Usage            TMR0_StopTimer();
*/
void TMR0_StopTimer(void)
{
    //Stop the Timer by writing to TMR0CS bit
    OPTION_REGbits.TMR0CS = 1;
}

/**
  @Prototype        uint8_t TMR0_ReadTimer(void)
  @Input            none
  @Output           Value of Timer at the time of the function call
                    read as a 8-bit value
  @Description      TMR0_ReadTimer function
  @Usage            TMR0_ReadTimer();
*/
uint8_t TMR0_ReadTimer(void)
{
    uint8_t ReadVal;
    ReadVal = TMR0;
    return ReadVal;
}

/**
  @Prototype        void TMR0_WriteTimer(uint8_t TimerVal)
  @Input            TimerVal : Timer value to be loaded
  @Output           none
  @Description      TMR0_WriteTimer function
  @Usage            TMR0_WriteTimer(TimerVal);
*/
void TMR0_WriteTimer(uint8_t TimerVal)
{   
    //Write to the Timer0 register
    TMR0 = TimerVal;
	
 }

/**
  @Prototype        void TMR0_Reload(void)
  @Input            none
  @Output           none
  @Description      TMR0_Reload function
  @Usage            TMR0_Reload();
*/
void TMR0_Reload(void)
{   
    //Write to the Timer0 register
    TMR0 = Timer0ReloadVal;
}

/**
  @Prototype        bool TMR0_IsOverflowOccured(void)
  @Input            none
  @Output           Timer overflow or no timer overflow
  @Description      Boolean routine to poll or to check for the overflow flag on the fly
  @Usage            TMR0_IsOverflowOccured();
*/
bool TMR0_IsOverflowOccured(void)
{
       
    //check if  overflow has occurred by checking the TMRIF bit
    return(INTCONbits.TMR0IF);
}