
/*******************************************************************************
  TMR1 Generated Driver File 

  Company:
    Microchip Technology Inc.

  File Name:
    tmr1.c

  Summary:
    This is the generated driver implementation file for the TMR1 driver using MPLAB® Code Configurator

  @Description
    This header file provides implementations for driver APIs for TMR1. 
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
#include "tmr1.h"

uint16_t Timer1ReloadVal;

/**
  @Prototype        void TMR1_InitializerDefault(void)
  @Input            none
  @Output           none
  @Description      custom initializer function for TMR1
  @Comment          
  @Usage            TMR1_InitializerDefault();
*/
void TMR1_InitializerDefault(void)
{
// set the Timer to the options selected in the GUI    
    //T1OSCEN disabled; nT1SYNC synchronize; TMR1CS FOSC/4; T1CKPS 1:8; TMR1ON disabled; 
    T1CON = 0x30; 
    //T1GSPM disabled; T1GSS T1G; T1GTM disabled; T1GPOL low; TMR1GE disabled; T1GGO done; 
    T1GCON = 0x00; 
    //TMR1H 0; 
    TMR1H = 0x00; 
    //TMR1L 0; 
    TMR1L = 0x00; 
    
    // Load the TMR value to reload variable
    Timer1ReloadVal=TMR1;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1IF = 0;    
}

/**
  @Prototype        void TMR1_StartTimer(void)
  @Input            none
  @Output           none
  @Description      TMR1 Start function.
  @Usage            TMR1_StartTimer();
*/
void TMR1_StartTimer(void)
{
    //Start the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 1;
}

/**
  @Prototype        void TMR1_StopTimer(void)
  @Input            none
  @Output           none
  @Description      TMR1Stop function.
  @Usage            TMR1_StopTimer();
*/
void TMR1_StopTimer(void)
{
    //Stop the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 0;
}

/**
  @Prototype        uint16_t TMR1_ReadTimer(void)
  @Input            none
  @Output           Timer value at the time of the function call
                    read as a 8-bit value
  @Description      TMR1 Read function
  @Usage            TMR1_ReadTimer();
*/
uint16_t TMR1_ReadTimer(void)
{
    uint16_t ReadVal;

    ReadVal = TMR1;
    return ReadVal;
}

/**
  @Prototype        void TMR1_WriteTimer(uint16_t TimerVal)
  @Input            TimerVal : timer value to be loaded
  @Output           none
  @Description      TMR1 Write function
  @Usage            TMR1_WriteTimer(TimerVal);
*/
void TMR1_WriteTimer(uint16_t TimerVal)
{
    if (T1CONbits.nT1SYNC == 1)
    {
        //Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        //Write to the Timer1 register
        TMR1H = TimerVal>> 8;
        TMR1L = TimerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON =1;

    }
    else
    {
        //Write to the Timer1 register
        TMR1H = TimerVal>> 8;
        TMR1L = TimerVal;
    }
}

/**
  @Prototype        void TMR1_Reload(void)
  @Input            none
  @Output           none
  @Description      TMR1 Reload function
  @Usage            TMR1_Reload();
*/
void TMR1_Reload(void)
{
    //Write to the Timer1 register
    TMR1H = Timer1ReloadVal>> 8;
    TMR1L = Timer1ReloadVal;

}

/**
  @Prototype        bool TMR1_IsOverflowOccured(void)
  @Input            none
  @Output           Timer overflow or no timer overflow
  @Description      Boolean routine to poll or to check for the overflow flag on the fly
  @Usage            TMR1_IsOverflowOccured();
*/
bool TMR1_IsOverflowOccured(void)
{
       
    //check if  overflow has occurred by checking the TMRIF bit
    return(PIR1bits.TMR1IF);
}