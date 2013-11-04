/*
 *
 * LCD Interface Example
 * usa a rotina de delay.c
 * esse codigo interfaceira com o LCD HD44780 e usa 4 bits
 *
 */



#include <xc.h>

#include "lcd.h"
#include "pause.h"

#define LCD_RS RB2 // pino 4 do LCD
#define LCD_EN RB3 // pino 6 do LCD
//#define LCD_RW RB4 // nao precisa pois RW (pino 5) do LCD foi aterrado

#define LCD_DATA    PORTB   //  PORTB = eb0,rb1,rb2,rb3, RB4, RB5, RB6, RB7
                            // pinos LCD:xx, xx, xx, xx,  13,  14,  15,  16

#define LCD_STROBE()  ((LCD_EN=1),(LCD_EN=0))


void lcd_write(unsigned char c)
{
    pause(1);
    LCD_RS=0;
    LCD_EN=0;

    RB4 = (c & 0b00010000) >> 4;
    RB5 = (c & 0b00100000) >> 5;
    RB6 = (c & 0b01000000) >> 6;
    RB7 = (c & 0b10000000) >> 7;
    //LCD_DATA=( ((c>>4) & 0x0F) | 0b0010 0000 );
    LCD_STROBE();

    RB4 = (c & 0b00000001) ;
    RB5 = (c & 0b00000010) >> 1;
    RB6 = (c & 0b00000100) >> 2;
    RB7 = (c & 0b00001000) >> 3;

    //LCD_DATA=( (c>>4) & 0x0F );
    //LCD_DATA=( c>>4 );
    LCD_STROBE();

    //LCD_DATA=( c      & 0x0F);
    //LCD_STROBE();

    /*
   // envia a primeira parte do byte
   lcd_envia_nibble(dado >> 4);

   // envia a segunda parte do byte
   lcd_envia_nibble(dado & 0x0f);
     */

    /*
     	PORTB = (PORTB & 0xF0) | ((c & 0xF0) >> 4);
	LCD_STROBE;
	PORTB = (PORTB & 0xF0) | (c & 0x0F);
	LCD_STROBE;
     */


}

void lcd_write_byte(unsigned char c)
{ // para escrever caracteres, o RS tem que estar em 1
    //pause(1);
    LCD_RS=0;
    LCD_RS=1;
    
    pause(1);
    LCD_EN=0;

    RB4 = (c & 0b00010000) >> 4;
    RB5 = (c & 0b00100000) >> 5;
    RB6 = (c & 0b01000000) >> 6;
    RB7 = (c & 0b10000000) >> 7;
    //LCD_DATA=( ((c>>4) & 0x0F) | 0b0010 0000 );
    LCD_STROBE();

    RB4 = (c & 0b00000001) ;
    RB5 = (c & 0b00000010) >> 1;
    RB6 = (c & 0b00000100) >> 2;
    RB7 = (c & 0b00001000) >> 3;   
    //LCD_DATA=(( c      & 0x0F) | 0b0010 0000 );
    LCD_STROBE();
    LCD_RS=0;

}

void lcd_clear(void)
{
    LCD_RS=0;
    lcd_write(0x01);
    pause(2);

}

void lcd_puts(const char * s)
{
    LCD_RS=1;
    while (*s)  lcd_write_byte(*s++);

}

void lcd_putch(char c)
{
    LCD_RS=1;
    lcd_write_byte(c);
}

void lcd_goto (unsigned char pos)
{
    LCD_RS=0;
    lcd_write(0x80 + pos);

}

void lcd_init()
{
    //TRISB=0;
    //PORTB=0b00000000;
    char init_value = 0x03;

    LCD_RS=0;
    LCD_EN=0;

    pause(15);

    
    // no bloco abaixo, vai colocar 0x03 (init_value) e strobar 3 vezes
    LCD_DATA=init_value;
    LCD_STROBE();
    pause(100);

    LCD_DATA=init_value;
    LCD_STROBE();
    pause(100);

    LCD_DATA=init_value;
    LCD_STROBE();
    pause(100);


    // depois envia 0x02 uma unica vez e stroba
    // 02 eh utilizado para indicar operacao em 4 bits
    LCD_DATA=0x02;
    LCD_STROBE();

    //pause(300);
    
    // depois envia a "string de inicializacao composta por 4 bytes
    //lcd_write( 0x20 | (2 << 2) );
    lcd_write ( 0x28 );

    /*
     * Programming Tip: The LCD command 38h is really the sum of a number
     *  of option bits. The instruction itself is the instruction 20h 
     * ("Function set"). However, to this we add the values 10h to indicate
     *  an 8-bit data bus plus 08h to indicate that the display is a 
     * two-line display.
     */


    lcd_write(0x0C); // no LCD da ACEPIC, este valor eh 0xC;
                    // no LCD padrao, o  valor eh 0xF;
    /*
     * Programming Tip: The command 0Eh is really the instruction
     *  08h plus 04h to turn the LCD on. To that an additional 02h
     *  is added in order to turn the cursor on.
     */
    lcd_clear(); // escreve 0x01 (definido dentro da subrotina lcd_clear)

    lcd_write(0x06); //Programming Tip: The command 06h is really the
                    //instruction 04h plus 02h to configure the LCD such
                    //that every time we send it a character, the cursor
                    //position automatically moves to the right.

    /*#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 linhas
      #define lcd_seg_lin 0x40    // Endereço da segunda linha na RAM do LCD

    // a constante abaixo define a seqüência de inicialização do módulo LCD
    byte CONST INI_LCD[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6};
     */

}