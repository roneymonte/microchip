#include <delays.h>

void lcd_comando(unsigned char cmd);
void lcd_envia_byte(unsigned char control, unsigned char dado);
void lcd_escreve(const rom char *matriz_lcd);
void lcd_escreve_bf(char *s_caracteres);
void lcd_posicao(unsigned char linha, unsigned char coluna);
void lcd_inicio();

#define D_EN TRISBbits.TRISB3	//Define o nome D_EN para o bit 3 do reg. TRISB (EN do LCD)
#define D_RS TRISBbits.TRISB2	//Define o nome D_RS para o bit 2 do reg. TRISB (RS do LCD)

#define D_D4 TRISBbits.TRISB4 	//Define o nome D_D4 para o bit 4 do reg. TRISB (dado 4 do LCD)
#define D_D5 TRISBbits.TRISB5 	//Define o nome D_D5 para o bit 5 do reg. TRISB (dado 5 do LCD)
#define D_D6 TRISBbits.TRISB6 	//Define o nome D_D6 para o bit 6 do reg. TRISB (dado 6 do LCD)
#define D_D7 TRISBbits.TRISB7 	//Define o nome D_D7 para o bit 7 do reg. TRISB (dado 7 do LCD)

#define EN PORTBbits.RB3		//Define o nome EN para o bit 3 da porta B (EN do LCD)
#define RS PORTBbits.RB2		//Define o nome RS para o bit 2 da porta B (RS do LCD)

#define D4 PORTBbits.RB4 		//Define o nome D4 para o bit 4 da porta B (dado 4 do LCD)
#define D5 PORTBbits.RB5 		//Define o nome D5 para o bit 5 da porta B (dado 5 do LCD)
#define D6 PORTBbits.RB6 		//Define o nome D6 para o bit 6 da porta B (dado 6 do LCD)
#define D7 PORTBbits.RB7 		//Define o nome D7 para o bit 7 da porta B (dado 7 do LCD)

void lcd_comando(unsigned char cmd)
{
D7 = cmd>>7;
D6 = cmd>>6;
D5 = cmd>>5;
D4 = cmd>>4;

EN = 1;
EN = 0;

D7 = cmd>>3;
D6 = cmd>>2;
D5 = cmd>>1;
D4 = cmd;

EN = 1;
EN = 0;

}

void lcd_envia_byte(unsigned char control, unsigned char dado)
{
RS = 0;
RS = control;
Delay100TCYx(2);
EN = 0;
lcd_comando(dado);
}

void lcd_escreve(const rom char *matriz_lcd)
{
while (*matriz_lcd!=0)
	{
		lcd_envia_byte(1,*matriz_lcd);
		matriz_lcd++;
	}

}

void lcd_escreve_bf(char *s_caracteres)
{
unsigned char tam_buffer=16;
	while (tam_buffer--)
	{
		lcd_envia_byte(1,*s_caracteres);
		s_caracteres++;
	}
}

void lcd_posicao(unsigned char linha, unsigned char coluna)
{
	switch (linha)
	{
		case 1:
			lcd_envia_byte (0,0x80+coluna-1);
			break;
		case 2:
			lcd_envia_byte (0,0xc0+coluna-1);
			break;
		case 3:
			lcd_envia_byte (0,0x94+coluna-1);
			break;
		case 4:
			lcd_envia_byte (0,0xd4+coluna-1);
			break;
	}	
}


void lcd_inicio()
{
char conta;

D_RS = 0;
D_EN = 0;
D_D4 = 0;
D_D5 = 0;
D_D6 = 0;
D_D7 = 0;

EN = 0;
RS = 0;
D4 = 0;
D5 = 0;
D6 = 0;
D7 = 0;

Delay10KTCYx(3);	//Gera delay de 15ms (30000 ciclos de máquina) 
					//Delay = 15m * 2M = 30000

for (conta = 1; conta <= 3; conta++)
	{
	lcd_comando(0x03);
	Delay10KTCYx(1);		//Gera delay de 5ms (10000 ciclos de máquina)
     }

lcd_comando(0x02);
lcd_envia_byte(0,0x28);
lcd_envia_byte(0,0x01);
Delay1KTCYx(8); 
lcd_envia_byte(0,0x0C);
lcd_envia_byte(0,0x06);
Delay10KTCYx(1);

}
        



