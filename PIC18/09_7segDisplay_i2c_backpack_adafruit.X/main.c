/* Display 7 Segmentos via I2C Backpack (Produto Adafruit)
 *
 * por Roney Monte
 *
 * Versao inicial - 30/mar/2014
 *
 */

#include <xc.h>
#include <plib/i2c.h>

#include "configbits.h"

#define _XTAL_FREQ 8000000
#define Baud 100000

#define USE_AND_MASKS

#define HT16K33_BLINK_CMD       0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF       0
#define HT16K33_BLINK_2HZ       1
#define HT16K33_BLINK_1HZ       2
#define HT16K33_BLINK_HALFHZ    3

#define HT16K33_CMD_SETUP       0x21
#define HT16K33_CMD_BRIGHTNESS  0x0E
#define HT16K33_ADDR            0x70

#define BUFFER_SIZE             8

unsigned short displaybuffer[ BUFFER_SIZE ];
const unsigned char numbertable[] =
{
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x77, /* a */
    0x7C, /* b */
    0x39, /* C */
    0x5E, /* d */
    0x79, /* E */
    0x71, /* F */
};

void main (void)
{


}