/*******************************************************************************
  Generated MPLAB� Code Configurator File 

  Company:
    Microchip Technology Inc.

  File Name:
    code_manager.c

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


#include "mcc.h"

// Configuration bits: selected in the GUI
#pragma config FOSC = INTOSC    // Oscillator Selection->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = OFF    // Watchdog Timer Enable->WDT disabled
#pragma config PWRTE = OFF    // Power-up Timer Enable->PWRT disabled
#pragma config MCLRE = ON    // MCLR Pin Function Select->MCLR/VPP pin function is MCLR
#pragma config CP = OFF    // Flash Program Memory Code Protection->Program memory code protection is disabled
#pragma config CPD = OFF    // Data Memory Code Protection->Data memory code protection is disabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = ON    // Internal/External Switchover->Internal/External Switchover mode is enabled
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config PLLEN = ON    // PLL Enable->4x PLL enabled
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LVP = OFF    // Low-Voltage Programming Enable->High-voltage on MCLR/VPP must be used for programming

void SYSTEM_Initializer(void)
{   
    OSCILLATOR_InitializerDefault();
    PIN_MANAGER_Initializer();
    EUSART_InitializerDefault();
    TMR0_InitializerDefault();
    TMR1_InitializerDefault();
    I2C_InitializerDefault();
    ADC_InitializerDefault();
}


void OSCILLATOR_InitializerDefault(void)
{
    // SPLLEN disabled; SCS FOSC; IRCF 500KHz_MF; 
    OSCCON = 0x38;
    // OSTS intosc; HFIOFR disabled; HFIOFS not0.5percent_acc; PLLR disabled; T1OSCR disabled; MFIOFR disabled; HFIOFL not2percent_acc; LFIOFR disabled; 
    OSCSTAT = 0x00;
    // OSCTUNE 0x0; 
    OSCTUNE = 0x00;
}
