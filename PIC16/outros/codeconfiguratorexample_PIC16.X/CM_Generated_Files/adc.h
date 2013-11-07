/*******************************************************************************
  ADC Generated Driver API Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    adc.h

  Summary:
    This is the generated header file for the ADC driver using MPLAB® Code Configurator

  Description:
    This header file provides APIs for driver for ADC. 
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


#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>
#include <stdbool.h>
/**
 *  result size of an A/D conversion
 */
 
typedef uint16_t adc_result_t;

/**
 *  user selected A/D channels definition 
 */
typedef enum 
{
    channel_Temp =  0x1D,
    channel_DAC =  0x1E,
    channel_FVR =  0x1F,
    channel_AN0 =  0x00,
    channel_AN1 =  0x01,
    channel_AN2 =  0x02,
    channel_AN3 =  0x03
} adc_channel_t;

 
/**
 * @prototype       void ADC_InitializerDefault(void)
 * @param           none
 * @return          none
 * @description     custom init function for the InitializerDefault channel
 *                  to be called prior to the ADC_getConversion() function
 * @comment         
 * @usage           ADC_InitializerDefault();
 */
void ADC_InitializerDefault(void);



/**
 * Starts a conversion on the desired analog channel.
 * 
 * @prototype       void ADC_StartConversion(void)
 * @param           the name set for the channel inside the MPLAB X plugin in channel_[name] format
 * @return          none
 * @usage           ADC_StartConversion(battery_channel);
 */
void ADC_StartConversion(adc_channel_t channel);


/**
 * Returns the state of the conversion process. Should be used to check is done after a call to ADC_StartConversion(...);
 * 
 * @prototype       void ADC_IsConversionDone(void)
 * @param           none
 * @return          bool value indicating if the conversion process is finished
 * @usage           ADC_StartConversion(battery_channel);
 *                  while(ADC_IsConversionDone() == false) {}
 * 
 */
bool ADC_IsConversionDone();

/**
 * Provides the result of the A/D conversion. It should be called after a ADC_StartConversion(...) was called and the conversion has finished 
 * 
 * @prototype       adc_result_t ADC_GetConversion(void)
 * @param           none
 * @return          the 10-bit A/D conversion result in the adc_result_t format
 * Description:     provides the result of the A/D conversion
 *                  To be called after a ADC_StartConversion(...) was called and the conversion has finished
 * @usage           ADC_StartConversion(channel_AN0);
 *                  while(ADC_IsConversionDone == false) {}
 *                  adc_result_t result = ADC_GetConversionResult();
 */
adc_result_t ADC_GetConversionResult(void);

/**
 * Provides the conversion result of the desired analog channel.
 * To be called after the initializer function.
 * 
 * @prototype       adc_result_t ADC_GetConversion(void)
 * @param           the name set for the channel inside the MPLAB X plugin in [name]_channel format
 * @return          the 10-bit A/D conversion result in the adc_result_t format (see above)
 * @usage           ADC_InitBatteryVoltage();
 *                  batteryVoltage_u16 = adc_GetConversion(battery_channel);
 */
adc_result_t ADC_GetConversion(adc_channel_t channel);



//***User Area Begin->code:adc_h***
    //Add your own functions here
//***User Area End->code:adc_h***
#endif

