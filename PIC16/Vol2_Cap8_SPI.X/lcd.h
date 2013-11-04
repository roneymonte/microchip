/*
 Declaracoes
 */

/*
extern void lcd_write(unsigned char);
extern void lcd_clear(void);
extern void lcd_puts(const char * s);
extern void lcd_goto(unsigned char pos);
extern void lcd_init(void);
extern void lcd_putch(char);
*/

extern void lcd_write(unsigned char);
extern void lcd_write_byte(unsigned char);
/* Clear and home the LCD */

extern void lcd_clear(void);

/* write a string of characters to the LCD */

extern void lcd_puts(const char * s);

/* Go to the specified position */

extern void lcd_goto(unsigned char pos);

/* intialize the LCD - call before anything else */

extern void lcd_init(void);

/* write a character to the LCD */

extern void lcd_putch(char);