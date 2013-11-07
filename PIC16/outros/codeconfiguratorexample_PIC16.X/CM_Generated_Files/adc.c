/*******************************************************************************
  ADC Generated Driver File 

  Company:
    Microchip Technology Inc.

  File Name:
    adc.c

  Summary:
    This is the generated driver implementation file for the ADC driver using MPLAB® Code Configurator

  Description:
    This source file provides implementations for driver APIs for ADC. 
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
#include "adc.h"



/**********************************************************************************************
 *
 * Prototype:       void ADC_InitializerDefault(void)
 * Input:           none
 * Output:          none
 * Description:     custom init function for the InitializerDefault channel
 *                  To be called prior to the ADC_getConversion() function
 * Comment          
 * Usage:           ADC_InitializerDefault();
 *
 **********************************************************************************************/
void ADC_InitializerDefault(void)
{
    // set the ADC to the options selected in the User Interface

    // GO_nDONE stop; ADON enabled; CHS AN0; 
    ADCON0 = 0x01;

    // ADPREF chip_VDD; ADNREF chip_VSS; ADFM left; ADCS FOSC/16; 
    ADCON1 = 0x50;

    // ADRESL 0x0; 
    ADRESL = 0x00;

    // ADRESH 0x0; 
    ADRESH = 0x00;
}


/**********************************************************************************************
 *
 * Prototype:       void ADC_StartConversion (adc_channel_t channel)
 * Input:           the name set for the channel inside the MPLAB X plugin in channel_[name] format
 * Output:          none
 * Description:     starts an A/D conversion
 *                  To be called after the init function corresponding to the desired channel
 * Usage:           ADC_StartConversion(battery);
 *
 **********************************************************************************************/
void ADC_StartConversion(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;

    // turn on the ADC module
    ADCON0bits.ADON = 1;
		
    // start the conversion
    ADCON0bits.GO_nDONE = 1;

}

/**********************************************************************************************
 *
 * Prototype:       bool ADC_IsConversionDone ()
 * Input:           none
 * Output:          bool value indicating if the conversion process is finished
 * Description:     starts an A/D conversion
 *                  To be called after a conversion is started
 * Usage:           
 *
 **********************************************************************************************/
bool ADC_IsConversionDone()
{
    // start the conversion
    return ADCON0bits.GO_nDONE;

}

/**********************************************************************************************
 *
 * Prototype:       adc_result_t ADC_GetConversionResult(void)
 *
 * Input:           none
 * Output:          the 10-bit A/D conversion result in the adc_result_t format
 * Description:     provides the result of the A/D conversion
 *                  To be called after a ADC_StartConversion(...) was called and the conversion has finished
 * Usage:           ADC_StartConversion(channel_AN0);
 *                  while(ADC_IsConversionDone == false) {}
 *                  adc_result_t result = ADC_GetConversionResult();
 *
 **********************************************************************************************/
adc_result_t ADC_GetConversionResult(void)
{    
    // conversion finished, return the result
    return (ADRESH << 8) + ADRESL;
}

/**********************************************************************************************
 *
 * Prototype:       adc_result_t ADC_GetConversion (adc_channel channel)
 *
 * Input:           the name set for the channel inside the MPLAB X plugin in [name]_channel format
 * Output:          the 10-bit A/D conversion result in the adc_result_t format
 * Description:     provides the result of the A/D conversion
 *                  To be called after the init function corresponding to the desired channel
 * Usage:           ADC_InitBatteryVoltage();
 *                  batteryVoltage_u16 = ADC_GetConversion(battery);
 *
 **********************************************************************************************/
adc_result_t ADC_GetConversion(adc_channel_t channel)
{
    adc_result_t result;
    // select the A/D channel
    ADCON0bits.CHS = channel;

    // turn on the ADC module
    ADCON0bits.ADON = 1;
		
    // start the conversion
    ADCON0bits.GO_nDONE = 1;

    // wait for the conversion to finish
    while (ADCON0bits.GO_nDONE)
    {
    }
	
    result = (ADRESH << 8) + ADRESL;
    // conversion finished, return the result
    return result;
}

