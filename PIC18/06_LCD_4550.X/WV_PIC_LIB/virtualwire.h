/*
 * Simplified Virtual Wire compatible library for PIC microcontrollers.
 * It is based on the original VirtualWire 1.20 available for Arduino at
 * http://www.airspayce.com/mikem/arduino/
 *
 * Some simplifications were made to make it fit inside PIC16 mid-range devices.
 * Half-duplex mode only, with a shared buffer for TX and RX.
 *
 * Original license:
 ** This software is Copyright (C) 2008 Mike McCauley. Use is subject to license
 ** conditions. The main licensing options available are GPL V2 or Commercial:
 *
 ** Open Source Licensing GPL V2
 *
 ** This is the appropriate option if you want to share the source code of your
 ** application with everyone you distribute it to, and you also want to give them
 ** the right to share who uses it. If you wish to use this software under Open
 ** Source Licensing, you must contribute all your source code to the open source
 ** community in accordance with the GPL Version 2 when your application is
 ** distributed. See http://www.gnu.org/copyleft/gpl.html
 **
 ** Commercial Licensing
 *
 ** This is the appropriate option if you are creating proprietary applications
 ** and you are not prepared to distribute and share the source code of your
 ** application. Contact info@open.com.au for details.
 */

#ifndef VIRTUALWIRE_H
#define VIRTUALWIRE_H

#include <htc.h>
#include "stdint.h"

#define VW_MAX_MESSAGE_LEN 24   // Maximum number of bytes in a message,
                                // including the byte count and FCS (3 bytes)


void vw_setup(uint16_t brate);
bit vw_send(const char *buf, uint8_t len);
void vw_wait_tx(void);

bit vw_have_message(void);
bit vw_recv(uint8_t *buf, uint8_t *len);

void vw_rx_stop(void);
void vw_rx_start(void);

void vw_isr_tmr0(void);

#endif
