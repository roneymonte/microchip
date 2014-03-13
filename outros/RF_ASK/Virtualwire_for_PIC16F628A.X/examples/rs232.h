/*
 * RS232.H
 * Serial port routines for the PIC microcontroller.
 * No interrupts are in use.
 * 
 */

#define	BS  0x08    // Backspace caracter
#define ESC 0x1B    // ESC key caracter
#define EOT 0x04    // End Of Transmission (CTRL+D)

void serialInit(unsigned long baudrate);

void putchar(unsigned char ch);

unsigned char getchar(void);

unsigned char getchar_nb(void);

void puts(const unsigned char *str);

void gets(unsigned char *str, unsigned int len);
