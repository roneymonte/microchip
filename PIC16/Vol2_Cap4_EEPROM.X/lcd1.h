/* 
 * File:   lcd1.h
 * Author: pu1rym
 *
 * Created on April 18, 2013, 2:21 PM
 */

#ifndef LCD1_H
#define	LCD1_H

#ifdef	__cplusplus
extern "C" {
#endif



extern void lcd_write(unsigned char);
extern void lcd_clear(void);
extern void lcd_puts(const char * s);
extern void lcd_goto(unsigned char pos);
extern void lcd_init(void);
extern void lcd_putch(char);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD1_H */

