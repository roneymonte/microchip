#include <xc.h>
/* #include "lcd.h" */



#define LCD_RS RA7
#define LCD_RW RA4
#define LCD_EN RA6

#define LCD_DATA PORTC
#define LCD_STROBE()    ((LCD_EN = 1),(LCD_EN=0))

void pause( unsigned short usvalue );


void lcd_write (unsigned char c)
{
    pause(1);
    LCD_DATA= ( ( c >> 4 ) & 0x0F );
    LCD_STROBE();
    LCD_DATA=( c & 0x0F );
    LCD_STROBE();
     

   

}

void lcd_clear(void)
{
    LCD_RS=0;
    lcd_write(0x1);
    pause(2);
}

void lcd_puts(const char * s)
{
    LCD_RS=1;
    while(*s)   lcd_write(*s++);
}

void lcd_putch(char c)
{
    LCD_RS=1;
    lcd_write(c);
}

void lcd_goto(unsigned char pos)
{
    LCD_RS=0;
    lcd_write(0x80+pos);
}

void lcd_init()
{



    char init_value;

    ANSEL=0;

    init_value=0x3;
    TRISA=0;
    TRISC=0;
    LCD_RS=0;
    LCD_EN=0;
    LCD_RW=0; //nao precisaria

    pause(15);
    LCD_DATA=init_value;
    LCD_STROBE();

    pause(10);
    LCD_STROBE();
    pause(10);
    LCD_STROBE();
    pause(10);

    LCD_DATA=2;
    LCD_STROBE();

    lcd_write(0b00101000);  //0x28 = duas linhas e char 5x8
                                // com 4 bits de transferencia

    //lcd_write(0b00100100);  // 0x24 = uma linha com caracter 5x10
                            // SE o display suportasse esse modo
                            // com 4 bits de transferencia

    lcd_write(0xF);
    lcd_clear();
    lcd_write(0x6);
     

    
}