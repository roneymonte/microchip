#ifndef _COMMON_H
#define _COMMON_H

#define FOSC 4000000UL

/**
 * Compatibility with pic16f88
 */
#ifndef TMR0IF
#define TMR0IF T0IF
#endif
#ifndef TMR0IE
#define TMR0IE T0IE
#endif

#endif
