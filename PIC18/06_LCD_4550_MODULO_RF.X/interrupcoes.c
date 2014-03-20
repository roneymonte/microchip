#include "interrupcoes.h"


void configTimers (void)
{
    unsigned char inicializacaoT1;

    // 31 khz
    //OSCCONbits.IRCF0 = 1;
    //OSCCONbits.IRCF1 = 1;
    //OSCCONbits.IRCF2 = 1;


    //OSCCONbits.OSTS = 1;    // Oscillator Start-up Timer
    //OSCCONbits.IOFS = 1;    // internal oscillator block has stabilized and is
                            // providing the device clock in RC Clock modes

    //INTCONbits.GIE=0;   // Desabilita Todas Interrupcoes durante o processo de configuracao

    /*
     *  The OSTS, IOFS and T1RUN bits indicate which clock
        source is currently providing the device clock. The OSTS
        bit indicates that the Oscillator Start-up Timer (OST) has
        timed out and the primary clock is providing the device
        clock in primary clock modes. The IOFS bit indicates
        when the internal oscillator block has stabilized and is
        providing the device clock in RC Clock modes. The
        T1RUN bit (T1CON<6>) indicates when the Timer1 oscil-
        lator is providing the device clock in secondary clock
        modes. In power-managed modes, only one of these
        three bits will be set at any time. If none of these bits are
        set, the INTRC is providing the clock or the internal
        oscillator block has just started and is not yet stable.
     */

    T1RUN=1;    // bit 8 do T1CON
    /*
        T1RUN: Timer1 System Clock Status bit
        1 = Device clock is derived from Timer1 oscillator
        0 = Device clock is derived from another source
     */


    //***** TIMER0
    //PSA =0;
              //  PSA: Timer0 Prescaler Assignment bit
              //  1 = TImer0 prescaler is NOT assigned. Timer0 clock input bypasses prescaler.
              //  0 = Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
    //T0SE=0;
              // T0SE: Timer0 Source Edge Select bit
              // 1 = Increment on high-to-low transition on T0CKI pin
              // 0 = Increment on low-to-high transition on T0CKI pin
    //T0CS=0;
              //  T0CS: Timer0 Clock Source Select bit
              //  1 = Transition on T0CKI pin
              //  0 = Internal instruction cycle clock (CLKO)


    //RCONbits.IPEN = 1;    //Enable priority levels on interrupts
    //RCONbits.SBOREN = 0;  //Disable BOR

//T1CON (bit8)RD16  T1RUN(bit7) (bit6e5) T1CKPS1/T1CKPS0 (bit4)T1OSCEN (bit3)T1SYNC (bit2)TMR1CS (bit1)TMR1ON
// obs: ? parece que existe uma ERRATA de troca entre os bits 7 e 8 ?

                   // bit 2           bit 5e6     bit 4          bit 7         bit 3             bit 8
    inicializacaoT1 = T1_SOURCE_EXT & T1_PS_1_8 & T1_OSC1EN_ON & T1_16BIT_RW & T1_SYNC_EXT_OFF;  //& TIMER_INT_ON ;
                   // TMR1CS          T1CKPS      T1OSCEN        RD16          T1SYNC

    OpenTimer1(inicializacaoT1);


    //T1CONbits.RD16=1;   // bit 7 do T1CON = T1_16BIT_RW ou T1_8BIT_RW
    /*
     *  RD16: 16-Bit Read/Write Mode Enable bit
     *   1 = Enables register read/write of Timer1 in one 16-bit operation
     *   0 = Enables register read/write of Timer1 in two 8-bit operations
     */

    // T1CKPS1:T1CKPS0 = T1_PS_x_y (bits 5 e 6)

    //T1OSCEN = 1;    // bit 4 do T1CON = T1_OSC1EN ON ou OFF
    /*
     *   An on-chip crystal oscillator circuit is incorporated
     *   between pins T1OSI (input) and T1OSO (amplifier
     *   output). It is enabled by setting the Timer1 Oscillator
     *   Enable bit, T1OSCEN (T1CON<3>). The oscillator is a
     *   low-power circuit rated for 32 kHz crystals. It will
     *   continue to run during all power-managed modes. The
     *   circuit for a typical LP oscillator is shown in Figure 12-3.
     *   Table 12-1 shows the capacitor selection for the Timer1
     *   oscillator.
     */

    //T1SYNC=0;   // bit 3 do T1CON = T1_SYNC_EXT ON ou OFF
    /*
        T1SYNC: Timer1 External Clock Input Synchronization Select bit
        When TMR1CS = 1:
        1 = Do not synchronize external clock input
        0 = Synchronize external clock input
    */

    //TMR1CS=0;   // bit 2 do T1CON = T1_SOURCE EXT ou INT
    /*
     *  TMR1CS: Timer1 Clock Source Select bit
     *  1 = External clock from RC0/T1OSO/T13CKI pin (on the rising edge)
     *  0 = Internal clock (F OSC /4)
     *  Internal instruction cycle clock (CLKO) acts as source of clock
     *  In select modes, OSC2 pin outputs CLKO which has 1/4 the
     *  frequency of OSC1 and denotes the instruction cycle rate.
     *
     * The operating mode is determined by the clock select
     * bit, TMR1CS (T1CON<1>). When TMR1CS is cleared
     * (= 0), Timer1 increments on every internal instruction
     * cycle (F OSC /4). When the bit is set, Timer1 increments
     * on every rising edge of the Timer1 external clock input
     * or the Timer1 oscillator, if enabled.
     */

    //SCS0=0; // bit 0 do OSCCON
    //SCS1=0; // bit 1 do OSCCON
    /*  SCS1:SCS0: System Clock Select bits
     *  1x = Internal oscillator
     *  01 = Timer1 oscillator
     *  00 = Primary oscillator
     *
     * Primary ? all oscillator modes (00).
     * This is the normal full-power execution mode.
     */


    // LOAD_TIMER = (estouro do timer) - ( tempo desejado * ( (Clock CPU/4) * prescaler ) )  )
    // Preescaler de 1:8, clock interno (48MHz) e tempo 180. Utilizamos aqui uma variavel para
    // aumentar o prescaler, pois neste caso (Timer1) o maximo eh 8.
    // #define INICIO_TMR1     0


    // 1 seg = 1.000 ms (10^-3) = 1.000.000 us (10^-6) = 1.000.000.000 ns (10^-9)

    // 1 ps (picosegundo)   = 1 * e^-12
    // 1 ns (nansegundo)    = 1 * e^-9
    // 1 us (microsegundo)  = 1 * e^-6
    // 1 ms (milisegundo)   = 1 * e^-3

    // 500 us  =  500 * 10-6 = 0,0005 segs
    // 20  ms  =  20  * 10-3 = 0.02 segs
    // 500 ms  =  500 * 10-3 = 0.5 segs

    // quero um delay de 1/2 segundo ou 500 ms = 0,5 seg

    //      Utilizando o Cristal default de 4 mhz
    //      (65536) - [ 0,5 * (4.000.000/4) * (1/8) ]
    //       65536  -   0,5 *  1.000.000    * 0.125
    //       65536  -   0,5 *  125.000
    //       65536  -   62500
    //       igual a 3045 decimal ou 0xBE5

    //      Utilizando um Cristal de 32.768 mhz (para 0,5 seg)
    //      65536   -   [ 0,5 * (32.768 / 4) * (1/8) ]
    //      65536   -   [ 0,5 *     8192     *  0.125 ]
    //      65536   -   [ 0,5 * 1024 ]
    //      65536   -   [ 512 ]
    //      igual a 65024 ou 0xFE00

    //      para 1 seg
    //      65536   -   [ 1   * 1024 ]
    //      igual a 64512 ou 0xFC00

    WriteTimer1( 0xFC00 );   // equivalente a 1/2 segundo

    TMR1IF = 0; // interrupt flag do Timer1, bit 0 do PIR1
    //PIR1bits.TMR1IF=0; // seria a mesma coisa ???

    ei();   // Enable Interrupt (!!!????!!!!) ei() = (INTCONbits.GIE = 1)
            // Interrupts of Hi/Lo Priority or Peripheral interrupts

    GIE=1;  // Faz o mesmo comando que o ei() acima; di() desabilita
    //INTCONbits.GIE=1; // o mesmo de cima

    //TMR0IE=0;   // ** DESABILITA TIMER0 **
    TMR1IE=1;   // Timer1 Interrupt Enable bit bit 0 do PIE1
    //PIE1bits.TMR1IE=1; // seria a mesma coisa ???

    TMR1ON=1;   // bit0 do T1CON1

    LED2=1;

}

 //////////////////////////////////////////////////////////////////

void interrupt global_isr(void)
{
        // TIMER 0
	if(TMR0IE && TMR0IF)
        {
            //TMR0IE=0;

            LED2=1;
		//vw_isr_tmr0();
                //return; // volta antes de testar o Timer1
            //TMR0IE=1;
        }



        // TIMER 1
        if (TMR1IF)
        {
            //TMR1IE=0;

            LEDI=1;
            LEDI=0;

            if (relogio >= 60)
            {
                aferir();   // Afere, Mostra no LCD, e Trasmite
                relogio=0;
            }
            else relogio++;


                // Mostra o contador de segundos no LCD
                while(BusyXLCD()) ;
                    SetDDRamAddr(0x66); //linha 4 nas duas ultimas posicoes
                    putrsXLCD ( ltoa ( NULL , relogio , 10) );

            TMR1IF=0;
            WriteTimer1( 0xFC00 );

            //TMR1IE=1;
        }
}