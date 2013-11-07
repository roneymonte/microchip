/* 
 * File:   spi_comm.h
 * Author: pu1rym
 *
 * Created on April 28, 2013, 10:07 PM
 */

#ifndef SPI_COMM_H
#define	SPI_COMM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_COMM_H */

char SPI_OUTPUT(char spi_byte); // send byte routine
void WR_BYTE(char E0_address, char EE_address, char EE_data);    // write memory byte
char RD_BYTE(char E0_address, char EE_address);  // read memory byte
