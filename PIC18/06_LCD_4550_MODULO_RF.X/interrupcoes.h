/* 
 * File:   interrupcoes.h
 * Author: roney
 *
 * Created on March 18, 2014, 2:31 PM
 */

#ifndef INTERRUPCOES_H
#define	INTERRUPCOES_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPCOES_H */

#include <plib/timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void configTimers (void);
void interrupt global_isr(void);
