//******************************************************************************
//Kit de desenvolvimento ACEPIC 28
//Projeto: EEPROM.c
//Descrição: Programa de demonstração para armazenamento de dados na memória EEPROM
//           do microcontrolador e informações enviadas pela serial.
//           Quando resetada a placa, são armazenados nos endereços 0 a 5 da EEPROM
//           caracteres formando a palavra ACEPIC eapós isso, o microcontrolador
//           envia pela serial a informação que o usuário deve pressionar o botão B1
//           para verificar o conteúdo armazenado nestes endereços.
//           
//**********************************************************************************
#include <16F876A.h>
#use delay(clock=8000000)
#fuses HS,NOWDT,PUT,NOBROWNOUT,NOLVP

//Definições para a comunicação serial
#use rs232(baud=4800, XMIT=PIN_C6, RCV=PIN_C7)


void main()      
{
//Envia a string pela serial e muda o cursor para a próxima linha
printf("Microcontroladores PIC 16F876A.\r\n");

//Envia a string pela serial e muda o cursor para a próxima linha
printf("Gravando na  EEPROM...\r\n");

//Escrevendo na EEPROM
write_eeprom(0,'A');       //Escreve o caractere 'A' no endereço 0
write_eeprom(1,'C');       //Escreve o caractere 'C' no endereço 1
write_eeprom(2,'E');       //Escreve o caractere 'E' no endereço 2
write_eeprom(3,'P');       //Escreve o caractere 'P' no endereço 3
write_eeprom(4,'I');       //Escreve o caractere 'I' no endereço 4
write_eeprom(5,'C');       //Escreve o caractere 'C' no endereço 5
delay_ms(500);      //Atraso de 500ms

//Envia a string pela serial e muda o cursor para a próxima linha
printf("Pressione B1 para ver.\r\n");

while(true)
     {
     if (input(PIN_A2))      //Se o botão B0 for pressionado
        {
        /*Envia as strings pela serial formatadas com os valores lido dos enderecos
        0 até 5 da EEPROM*/
        printf("Valor lido no endereco 0: %c\r\n",read_eeprom(0));
        printf("Valor lido no endereco 1: %c\r\n",read_eeprom(1));
        printf("Valor lido no endereco 2: %c\r\n",read_eeprom(2));
        printf("Valor lido no endereco 3: %c\r\n",read_eeprom(3));
        printf("Valor lido no endereco 4: %c\r\n",read_eeprom(4));
        printf("Valor lido no endereco 5: %c\r\n",read_eeprom(5));
        delay_ms(300);        //Atraso de 300ms
        }
     } 
}
