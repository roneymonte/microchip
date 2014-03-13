/*
 * RS232.C
 * Serial port routines for the PIC microcontroller.
 * No interrupts are in use.
 * 
 */

#include <htc.h>
#include "rs232.h"

/* xtal */
#include "..\common.h"

/**
 * Serial port initialization at the specified baud/bit rate.
 * @param baudrate desired bit rate (1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, etc.).
 */
void serialInit(unsigned long baudrate)
{
    TRISB2 = 0; // out pin configure - tx
    TRISB1 = 1; // out pin configure - rx

    SPBRG = (char) (FOSC / (16UL * baudrate)); /* Baud Rate register value...*/

    BRGH = 1; // High baud rate select
    SYNC = 0; // USART operate in assynchronous mode
    SPEN = 1; // Serial port is enable
    TXEN = 1; // Enable transmission
    CREN = 1; // Enable reception
}

/**
 * Sends one character via serial port.
 * @param ch character to send.
 */
void putchar(unsigned char ch)
{
    while (TXIF == 0);
    TXREG = ch;
}

/**
 * Reads a character from the serial port, waiting if none is available.
 * @return one character.
 */
unsigned char getchar(void)
{
    while (RCIF == 0)
    {
        /* clears any error on the serial port (overrun or framing) */
        if (OERR || FERR)
        {
            CREN = 0;
            CREN = 1;
        }
    }
    RCIF = 0;
    return RCREG;
}

/**
 * Reads a character from the serial port, non-blocking.
 * @return the char if available or '\0' if not.
 */
unsigned char getchar_nb(void)
{
    if (RCIF == 0)
        return 0;
    RCIF = 0;
    return RCREG;
}

/**
 * Sends a null terminated string via the serial port. The finalizing NULL is not sent.
 * @param str the string to send via serial port.
 */
void puts(const unsigned char *str)
{
    while (*str != '\0')
        putchar(*str++);
}

/**
 * Reads a string up to len-1 bytes max and returns pointer to it.
 * @param str pointer to a suitable buffer
 * @param len buffer size
 * @return the null terminated string.
 */
void gets(unsigned char *str, unsigned int len)
{
    unsigned char index, ch;

    index = 0;
	do {
    	ch = getchar();
	} while (ch == '\n' || ch == '\r');
    
    while (ch != EOT && ch != '\r' && ch != '\n' && index < len)
    {
        str[index] = ch;
        index++;
        ch = getchar();
    }
    
    str[index] = '\0';
}

