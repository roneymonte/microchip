/*******************************************************************************
*                        Kit de desenvolvimento ACEPIC 28                      *
*                      ACEPIC Tecnologia e Treinamento LTDA                    *
*                               www.acepic.com.br                              * 
*Objetivo: Leitura do Canal Analógico AN0 e visualização dos valores de        *
*           0 a 5.00 V                                                         *
*Obs.: Ligar as chaves 4 (RC0-DSP1), 5 (RC1-DSP2) e 6 (RA5-DSP3) do DIP1       *
*      e as chaves 7 (RA6-OSC1) e 8 (RA7-OSC2) do DIP2                         *
*******************************************************************************/

#include <18F2550.h>
#device ADC = 10              //Define 10 bits para o resultado da conversão AD
#use delay(clock=8000000)
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP, CPUDIV1 

#use fast_io(a)

int conta=0;
int32 ad0;
int32 d1, d2, d3;

/*Matriz de 10 posições contendo o valor a ser enviado para a porta D a fim de
   mostrar o dígito referente nos displays */
int digito[10] =
               {0b00111111,    //Dígito 0
                0b00000110,    //Dígito 1
                0b01011011,    //Dígito 2
                0b01001111,    //Dígito 3
                0b01100110,    //Dígito 4
                0b01101101,    //Dígito 5
                0b01111101,    //Dígito 6
                0b00000111,    //Dígito 7
                0b01111111,    //Dígito 8
                0b01101111     //Dígito 9
               };

#INT_TIMER0
void trata_t0()
{
conta++;
if (conta>3) conta = 1;

switch(conta)
          {
         case 1: OUTPUT_B(digito[d1] | 0b10000000);
                 OUTPUT_A(0b00000000);
                 OUTPUT_C(0b00000001);
                 break; 
         case 2: OUTPUT_B(digito[d2]);
                 OUTPUT_C(0b00000010);
                 break; 
         case 3: OUTPUT_B(digito[d3]);
                 OUTPUT_C(0b00000000); 
                 OUTPUT_A(0b00100000);
                 break; 
         } 

SET_TIMER0(100); 
}


void main()
{
SET_TRIS_A(0b00000001);

SETUP_ADC_PORTS(AN0);

SETUP_ADC(ADC_CLOCK_INTERNAL);

ENABLE_INTERRUPTS(GLOBAL | INT_TIMER0);
SETUP_TIMER_0(RTCC_INTERNAL | RTCC_8_BIT| RTCC_DIV_16);
SET_TIMER0(100);

while(true)
     {
     SET_ADC_CHANNEL(0);        //Configura o canal de leitura 0
     delay_us(100);             //Tempo de ajuste do canal (necessário) 
     ad0 = READ_ADC();          //Faz a conversão AD e a salva na variável ad0
   
     ad0 = (ad0 * 5000)/1023;
          
     d1 = ad0/1000;
     d2 = (ad0/100) % 10;
     d3 = (ad0/10) % 10;
     
     delay_ms(500);    
     }
}
