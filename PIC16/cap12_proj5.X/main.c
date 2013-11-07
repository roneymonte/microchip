/*
 * File:   main.c
 * Author: pu1rym
 *
 * Created on April 1, 2013, 7:55 PM
 */


#include <xc.h>

//__CONFIG(INTIO & WDTDIS & MCLRDIS & UNPROTECT & DUNPROTECT);
//__CONFIG(BOREN & IESODIS & LVPDIS & FCMDIS & PWRTDIS);
//__CONFIG(INTIO&WDTDIS&PWRTDIS&MCLREN&UNPROTECT&DUNPROTECT);

//__CONFIG(WDTDIS&PWRTDIS&MCLREN&UNPROTECT&DUNPROTECT);
//__CONFIG(BOREN&IESODIS&LVPDIS&FCMDIS);
//__CONFIG(INTIO);

#pragma config FOSC = INTRC_NOCLKOUT // define oscilador INTERNO sem CLOCK OUT
#pragma config WDTE = OFF           // watch dog desligado
#pragma config PWRTE = OFF          // temporizador de powerUp desligado
#pragma config MCLRE = ON           // pino MCLR eh MCLR e nao funcao
#pragma config CP = OFF             // programa memory nao protegido
#pragma config CPD = OFF            // data memory nao protegida
#pragma config BOREN = ON           // BROWN-OUT inicializacao sem reset,
                                    // pelo nivel de voltagem minimo

#pragma config IESO = OFF           // internal/external switchOver

#pragma config LVP = OFF            // low voltage programming = precisa de
                                    // alta vontagem para programacao do chip

#pragma config FCMEN = OFF          // fail-safe clock monitor

/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog
//#fuses MCLR,NOPROTECT,NOWDT

// CONFIG1

//FOSC =    Oscillator Selection
//ECM    ECM, External Clock, Medium Power Mode (0.5-4 MHz): device clock supplied to CLKIN pin
//XT    XT Oscillator, Crystal/resonator connected between OSC1 and OSC2 pins
//LP    LP Oscillator, Low-power crystal connected between OSC1 and OSC2 pins
//EXTRC    EXTRC oscillator: External RC circuit connected to CLKIN pin
//ECH    ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin
//ECL    ECL, External Clock, Low Power Mode (0-0.5 MHz): device clock supplied to CLKIN pin
//INTOSC    INTOSC oscillator: I/O function on CLKIN pin
//HS    HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins
//#pragma config FOSC = 0b101

//WDTE =    Watchdog Timer Enable
//OFF    WDT disabled
//ON    WDT enabled
//NSLEEP    WDT enabled while running and disabled in Sleep
//SWDTEN    WDT controlled by the SWDTEN bit in the WDTCON register
//#pragma config WDTE = OFF

//PWRTE =    Power-up Timer Enable
//OFF    PWRT disabled
//ON    PWRT enabled
//#pragma config PWRTE = OFF

//MCLRE =    MCLR Pin Function Select
//OFF    MCLR/VPP pin function is digital input
//ON    MCLR/VPP pin function is MCLR
//#pragma config MCLRE = OFF

//CP =    Flash Program Memory Code Protection
//OFF    Program memory code protection is disabled
//ON    Program memory code protection is enabled
//#pragma config CP = OFF

//CPD =    Data Memory Code Protection
//OFF    Data memory code protection is disabled
//ON    Data memory code protection is enabled
//#pragma config CPD = OFF

//BOREN =    Brown-out Reset Enable
//OFF    Brown-out Reset disabled
//ON    Brown-out Reset enabled
//NSLEEP    Brown-out Reset enabled while running and disabled in Sleep
//SBODEN    Brown-out Reset controlled by the SBOREN bit in the BORCON register
//#pragma config BOREN = OFF

//CLKOUTEN =    Clock Out Enable
//OFF    CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
//ON    CLKOUT function is enabled on the CLKOUT pin
////#pragma config CLKOUTEN = ON

//IESO =    Internal/External Switchover
//OFF    Internal/External Switchover mode is disabled
//ON    Internal/External Switchover mode is enabled
//#pragma config IESO = OFF

//FCMEN =    Fail-Safe Clock Monitor Enable
//OFF    Fail-Safe Clock Monitor is disabled
//ON    Fail-Safe Clock Monitor is enabled
//#pragma config FCMEN = OFF

// CONFIG2

//WRT =    Flash Memory Self-Write Protection
//OFF    Write protection off
//BOOT    000h to 1FFh write protected, 200h to 1FFFh may be modified by EECON control
//HALF    000h to FFFh write protected, 1000h to 1FFFh may be modified by EECON control
//ALL    000h to 1FFFh write protected, no addresses may be modified by EECON control
//#pragma config WRT = OFF

//PLLEN =    PLL Enable
//OFF    4x PLL disabled 
//ON    4x PLL enabled
////#pragma config PLLEN = OFF

//STVREN =    Stack Overflow/Underflow Reset Enable
//OFF    Stack Overflow or Underflow will not cause a Reset
//ON    Stack Overflow or Underflow will cause a Reset
////#pragma config STVREN = OFF

//BORV =    Brown-out Reset Voltage Selection
//LO    Brown-out Reset Voltage (Vbor), low trip point selected.
//HI    Brown-out Reset Voltage (Vbor), high trip point selected.
//#pragma config BORV = 19

//LVP =    Low-Voltage Programming Enable
//OFF    High-voltage on MCLR/VPP must be used for programming
//ON    Low-voltage programming enabled
//#pragma config LVP = OFF


void Pause(unsigned short msvalue);
void msecbase (void);


int main(void) {

    ANSEL=0;
    CM1CON0=0;
    CM2CON0=0;

    PORTD=0x00;
    TRISD=0x00; // output na PORT D;

    ANSELH=0;
    TRISB=0xFF; // input na PORT B;

    RD7=1;
    RD6=1;
    RD4=1;

    while (1==1)
    {
        RD4=0;
        Pause(100);

        

        if (RB0==1)
        {
            RD0=1;
            //PORTD=0b00000010;
            ;
        }

        else

        {
            RD0=0;
            //PORTD=0b00000100;
        }
        

        //PORTD=!PORTB;
        
        RD4=1;
        Pause(100);
    }

    return 0;
}

void Pause(unsigned short msvalue)
{
    unsigned short x;
    for (x=0; x<=msvalue; x++)
    {
        msecbase();
    }
}

void msecbase (void)
{
    OPTION_REG=0b00000001;
    TMR0=0xd;
    while(!T0IF);
    T0IF=0;
}