////////////////////////////////////////////////////////////////////////////////////////////////////////
// Programa:  Controle de dispositivo utilizando a comunicação USB no modo HID
// Descrição: Este programa tem a finalidade de mostrar o uso da porta USB em projetos
//           de controle. Neste exemplo, a placa ACEPIC PRO v2.0 (PIC18F4550) enviará
//           os valores de conversão AD referentes aos canais 0, 1 e 3 e receberá do
//           computador os comandos para acionamento dos LEDS L1, L2, L3, e L4.
// 
// Desenvolvido por: Eng. Carlos Eduardo Sandrini Luz
//                          ACEPIC Tecnologia e Treinamento LTDA
// Hardware: Kit de desenvolvimento ACEPIC PRO V2.0 (PIC18F4550)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////          

#DEFINE LED1  PIN_B7

#include <18F2550.h>
#device ADC=10;
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL2,CPUDIV1,VREGEN
#use delay(clock=48000000)

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

/////////////////////////////////////////////////////////////////////////////
//
// CCS Library dynamic defines.  For dynamic configuration of the CCS Library
// for your application several defines need to be made.  See the comments
// at usb.h for more information
//
/////////////////////////////////////////////////////////////////////////////

//Tells the CCS PIC USB firmware to include HID handling code.
#DEFINE USB_HID_DEVICE  TRUE

//the following defines needed for the CCS USB PIC driver to enable the TX endpoint 1
// and allocate buffer space on the peripheral
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_TX_SIZE    11  //aloca 8 bytes para transmissão
//#define USB_EP1_TX_SIZE    8  //allocate 8 bytes in the hardware for transmission

//the following defines needed for the CCS USB PIC driver to enable the RX endpoint 1
// and allocate buffer space on the peripheral
#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_RX_SIZE    8  //allocate 8 bytes in the hardware for reception

/////////////////////////////////////////////////////////////////////////////
//
// Include the CCS USB Libraries.  See the comments at the top of these
// files for more information
//
/////////////////////////////////////////////////////////////////////////////

#include <pic18_usb.h>   //Microchip 18Fxx5x hardware layer for usb.c

//#include <Descriptor_easyHID.h>   //USB Configuration and Device descriptors for this UBS device
#include <usb_desc_hid_ACEPIC.h>   //USB Configuration and Device descriptors for this UBS device
#include <usb.c>        //handles usb setup tokens and get descriptor reports


/////////////////////////////////////////////////////////////////////////////
//
// Configure the demonstration I/O
//
/////////////////////////////////////////////////////////////////////////////
#define L1 PIN_B0
#define L2 PIN_B1
#define L3 PIN_B2
#define L4 PIN_B3
#define RB0 PIN_A2
#define RB1 PIN_A4
#define RB2 PIN_C0
#define RA4 PIN_C1
#define VENT PIN_B4
#define AQC PIN_C2
#define LED_ON output_high
#define LED_OFF output_low

int8 in_data[8];

/////////////////////////////////////////////////////////////////////////////
//
// Variáveis
//
/////////////////////////////////////////////////////////////////////////////
int16 valor_adc01;
int16 valor_adc02;
int16 valor_adc03;
int i;

/////////////////////////////////////////////////////////////////////////////
//
// usb_debug_task()
//
// When called periodically, displays debugging information over serial
// to display enumeration and connection states.  Also lights LED1 based upon
// enumeration and status.
//
/////////////////////////////////////////////////////////////////////////////
void usb_debug_task(void) {
   static int8 last_connected;
   static int8 last_enumerated;
   int8 new_connected;
   int8 new_enumerated;

   new_connected=usb_attached();
   new_enumerated=usb_enumerated();

   if (new_enumerated)
      LED_ON(LED1);
   else
      LED_OFF(LED1);

   if (new_connected && !last_connected)
      printf("\r\n\nUSB conectado, aguardando enumeracao...");
   if (!new_connected && last_connected)
      printf("\r\n\nUSB desconectado, aguardando conexao...");
   if (new_enumerated && !last_enumerated)
      printf("\r\n\nUSB enumerado pelo PC/HOST");
   if (!new_enumerated && last_enumerated)
      printf("\r\n\nUSB nao enumerado pelo PC/HOST, aguardando enumeracao...");

   last_connected=new_connected;
   last_enumerated=new_enumerated;
}

void main() {
   int8 out_data[20];
   //int8 in_data[8];
   int8 send_timer=0;
   int8 send_adc_Timer=0;

   Output_B(0x00);
   
   printf("\r\n\nACEPIC PRO V2.0 (PIC18F4550) - Exemplo USB/HID");

 
   printf("\r\nPCH: v");
   printf(__PCH__);


   usb_init();

//  #if !(__USB_PIC_PERIF__)
//   printf("\r\nUSBN: 0x%X", usbn_get_version());
//  #endif
   printf("\r\n");

  
   setup_adc_ports(AN0_TO_AN3);
 

   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   while (TRUE) {
      usb_task();
      usb_debug_task();
      if (usb_enumerated()) {
         if (!send_timer) {
            send_timer=250;               //Retorna o valor 250 à variável send_timer
            send_adc_timer++;               //Incrementa a variável send_adc_timer
            if (send_adc_timer>=4)            //Envia os valores ADC em aprox. 1 segundo
               {
                set_adc_channel(0);            //Seleciona o canal analógico 0
                delay_us(100);               //Atraso de 100us
                valor_adc01 = read_adc();      //Lê o canal 0 e armazena o valor em valor_adc01
            
                set_adc_channel(1);            //Seleciona o canal analógico 1
                delay_us(100);               //Atraso de 100us
                valor_adc02 = read_adc();      //Lê o canal 1 e armazena o valor em valor_adc02   

                set_adc_channel(3);            //Seleciona o canal analógico 3
                delay_us(100);                //Atraso de 100us
                valor_adc03 = read_adc();      //Lê o canal 3 e armazena o valor em valor_adc03
                send_adc_timer=0;            //zera a variável send_adc_timer
                } 
            
       //Valores a serem armazenados nas posições 0 a 8 da matriz out_data
            out_data[0]= valor_adc01>>8;      //Armazena a parte alta da variável valor_adc01
            out_data[1]=(int) valor_adc01;      //Armazena a parte baixa da variável valor_adc01
            out_data[2]= valor_adc02>>8;      //Armazena a parte alta da variável valor_adc02   
            out_data[3]=(int) valor_adc02;      //Armazena a parte baixa da variável valor_adc02
            out_data[4]= valor_adc03>>8;      //Armazena a parte alta da variável valor_adc03   
            out_data[5]=(int) valor_adc03;      //Armazena a parte baixa da variável valor_adc03
            out_data[6]=!input(RB0);         //Armazena o resultado do pressionamento do botão RB0
            out_data[7]=!input(RB1);         //Armazena o resultado do pressionamento do botão RB1
            out_data[8]=!input(RB2);         //Armazena o resultado do pressionamento do botão RB2
            out_data[9]=   input_state(AQC); 
            out_data[10] = input_state(VENT);
            
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////
           //Envia a matriz out_data à USB 
           //Ao enviar, também faz a verificação através da declaração 'if'. O envio estando ok, faz a
      //impressão na serial dos valores enviados
       //////////////////////////////////////////////////////////////////////////////////////////////////////////  
      if (usb_put_packet(1, out_data, 11, USB_DTS_TOGGLE));
          //printf("\r\n<-- Enviando: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X", out_data[0], out_data[1], out_data[2], out_data[3], out_data[4], out_data[5], out_data[6], out_data[7], out_data[8]);
         }

         if (usb_kbhit(1)) {
            usb_get_packet(1, in_data, 8);
            printf("\r\n--> Dado recebido: %c %c %c %c",in_data[0],in_data[1], in_data[2], in_data[3]);
            if (in_data[0] == 'L' && in_data[1] == 'D')
                {
                 switch(in_data[2])
                           {
                              case '1': Output_toggle(L1);
                                        break;
                              case '2': Output_toggle(L2);
                                        break;
                              case '3': Output_toggle(L3);
                                        break;
                              case '4': Output_toggle(L4);
                                        break;
                           }
                }
            if (in_data[0] == 'D' && in_data[1] == 'A' && in_data[2] == 'Q')
                  {
                  output_low(AQC);
                  output_high(VENT);
                  }
            if (in_data[0] == 'D' && in_data[1] == 'V' && in_data[2] == 'T')
                  {
                  output_low(VENT);
                  } 
            if (in_data[0] == 'L' && in_data[1] == 'A' && in_data[2] == 'Q')
                  {
                  output_low(VENT);
                  output_high(AQC);
                  }       
                  
         }
         if (input(RA4)) output_high(AQC);   
         send_timer--;
         delay_ms(1);
      }
   }
}
