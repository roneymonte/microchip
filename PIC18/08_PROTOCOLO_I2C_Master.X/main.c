/* 
 * File:   main.c
 * Author: roney
 *
 * Created on 21 de Março de 2014, 16:53
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_bits.h"
#include <plib/i2c.h>


/*
 * 
 */
int main(int argc, char** argv) {


    OpenI2C();
    StartI2C();

    unsigned char data = ReadI2C();

    IdleI2C();

    getcI2C();

    WriteI2C();


    CloseI2C();

}

void SSPISR(void)
{
    unsigned char temp,data;
    temp = SSPSTAT &0x2d;

    if((temp ^0x09) ==0x00)
    {  // 1: write operation, last byte was  address
    data =ReadI2C();
    // Do something with data, or just return
    }

    else if((temp ^0x29) ==0x00)
    {  // 2: write operation, last byte was data
    data =ReadI2C();
    // Do something with data, or just return
    }

    else if((temp ^0x0c) ==0x00)
    {  // 3: read operation, last byte was address
    // Do something, then write something to I2C
    WriteI2C(0x00);
    }

    else if((temp ^0x2c) ==0x00)
    {  // 4: read operation, last byte was data
    // Do something, then write something to I2C
    WriteI2C(0x00);
    }

    else{  // 5: slave logic reset by NACK from master
    // Don't do anything, clear a buffer, reset, whatever
    }
}
