/* 
 * File:   uart.h
 * Author: pu1rym
 *
 * Created on April 24, 2013, 3:09 PM
 */

#define BAUD 2400
#define FOSC 4000000L
#define baudsetting ((int)(FOSC/(64UL * BAUD)-1))

#define RX_PIN TRISC7
#define TX_PIN TRISC6

void putch (unsigned char);
unsigned char getch (void);
unsigned char getche (void);
void init_comms(void);
