#include	<htc.h>
#include	"virtualwire.h"
#include	"crc16.h"
#include	"string.h"

/*************************************
 *	Tunable parameters
 */

/* Transmit and Receive port bits */
#define RxData RB3
#define TxData RB4
#define RxTris TRISB3
#define TxTris TRISB4

#define _XTAL_FREQ 4000000


#include "common.h"
#define OVERSAMPLING	8

/*
 *	Don't change anything else
 ************************************/


// -----------------------------------------------------------------------------
// TX
// -----------------------------------------------------------------------------
static uint8_t vw_tx_len = 0; // Number of symbols in vw_tx_buf to be sent
static uint8_t vw_tx_index = 0; // Index of the next symbol to send, [0..vw_tx_len)
static bit vw_tx_header; // flag to decide if transmitting header or payload

static uint8_t vw_tx_sample = 0; // Sample number for each bit (oversampling)
static uint8_t vw_tx_bit = 0; // Bit number of next bit to send
static bit vw_tx_enabled = 0; // Flag tx enabled?

static uint8_t vw_tmr0_value;

// -----------------------------------------------------------------------------
// RX
// -----------------------------------------------------------------------------
static volatile bit vw_rx_done = 0; // flag new message is available
static uint16_t vw_rx_bits = 0; // last 12 bits to look for the start symbol
static bit vw_rx_enabled = 0; // flag receiver enabled
static bit vw_rx_enabled_last = 0; // flag receiver enabled reposition
static uint8_t vw_rx_bit_count = 0; // bits of message received (0..12)
static uint8_t vw_rx_count = 0; // expected message length
static uint8_t vw_rx_len = 0; // incoming buffer used so far

static uint8_t vw_rx_sample = 0; // current receiver sample
static uint8_t vw_rx_last_sample = 0; // last receiver sample
static bit vw_rx_active = 0; // start sequence found and now decoding data

// -----------------------------------------------------------------------------
// PLL
// -----------------------------------------------------------------------------
/// The size of the receiver ramp. Ramp wraps modulu this number
#define VW_RX_RAMP_LEN 160

/// Number of samples per bit
#define VW_RX_SAMPLES_PER_BIT 8
// Ramp adjustment parameters
// Standard is if a transition occurs before VW_RAMP_TRANSITION (80) in the ramp,
// the ramp is retarded by adding VW_RAMP_INC_RETARD (11)
// else by adding VW_RAMP_INC_ADVANCE (29)
// If there is no transition it is adjusted by VW_RAMP_INC (20)
/// Internal ramp adjustment parameter
#define VW_RAMP_INC (VW_RX_RAMP_LEN/VW_RX_SAMPLES_PER_BIT)
/// Internal ramp adjustment parameter
#define VW_RAMP_TRANSITION VW_RX_RAMP_LEN/2
/// Internal ramp adjustment parameter
#define VW_RAMP_ADJUST 9
/// Internal ramp adjustment parameter
#define VW_RAMP_INC_RETARD (VW_RAMP_INC-VW_RAMP_ADJUST)
/// Internal ramp adjustment parameter
#define VW_RAMP_INC_ADVANCE (VW_RAMP_INC+VW_RAMP_ADJUST)
static uint8_t vw_rx_pll_ramp = 0;
static uint8_t vw_rx_integrator = 0;

// 4 bit to 6 bit symbol converter table
// Used to convert the high and low nybbles of the transmitted data
// into 6 bit symbols for transmission. Each 6-bit symbol has 3 1s and 3 0s 
// with at most 3 consecutive identical bits
const uint8_t symbols[] = {
    0xd, 0xe, 0x13, 0x15, 0x16, 0x19, 0x1a, 0x1c,
    0x23, 0x25, 0x26, 0x29, 0x2a, 0x2c, 0x32, 0x34
};

#define VW_HEADER_LEN 8
#define VW_MAX_PAYLOAD VW_MAX_MESSAGE_LEN-3	/// The maximum payload length

const uint8_t vw_tx_buf_header[VW_HEADER_LEN]
        = {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x38, 0x2c};

static bank1 uint8_t vw_tx_buf[VW_MAX_MESSAGE_LEN * 2];

void vw_setup(uint16_t brate)
{
    // setup pins
    RxTris = 1;
    TxTris = 0;

    // idle output pin
    TxData = 0;

    /* Set up the timer (warning 8 bit only) */
    vw_tmr0_value = FOSC / (4 * brate * OVERSAMPLING);

    T0CS = 0; // Set timer mode for Timer0.
    TMR0 = (2 - vw_tmr0_value); // +2 as timer stops for 2 cycles
    // when writing to TMR0
    TMR0IE = 1; // Enable the Timer0 interrupt.
    GIE = 1;
}

// ----------------------------------------------------------------------------
// TX
// ----------------------------------------------------------------------------

void vw_wait_tx(void)
{
    while (vw_tx_enabled);
}

void vw_tx_stop(void)
{
    TxData = 0;
    vw_tx_enabled = 0;
}

void vw_tx_start(void)
{
    vw_tx_index = 0;
    vw_tx_bit = 0;
    vw_tx_sample = 0;

    vw_tx_header = 1;

    // Next tick interrupt will send the first bit
    vw_tx_enabled = 1;
}

bit vw_send(const char *buf, uint8_t len)
{
    uint8_t i;
    uint8_t index = 0;
    uint16_t crc = 0xffff;
    uint8_t *p = vw_tx_buf;
    uint8_t count = len + 3; // Added byte count and FCS to get total number of bytes

    if (len > VW_MAX_PAYLOAD)
        return 0;

    // Wait for transmitter to become available
    vw_wait_tx();
    vw_rx_enabled = 0; // disable the receiver so it does corrupt our message

    // Encode the message length
    crc = _crc_ccitt_update(crc, count);
    p[index++] = symbols[count >> 4];
    p[index++] = symbols[count & 0xf];

    // Encode the message into 6 bit symbols. Each byte is converted into
    // 2 6-bit symbols, high nybble first, low nybble second
    for (i = 0; i < len; i++)
    {
        crc = _crc_ccitt_update(crc, buf[i]);
        p[index++] = symbols[buf[i] >> 4];
        p[index++] = symbols[buf[i] & 0xf];
    }

    // Append the fcs, 16 bits before encoding (4 6-bit symbols after encoding)
    // Caution: VW expects the _ones_complement_ of the CCITT CRC-16 as the FCS
    // VW sends FCS as low byte then hi byte
    crc = ~crc;
    p[index++] = symbols[(crc >> 4) & 0xf];
    p[index++] = symbols[crc & 0xf];
    p[index++] = symbols[(crc >> 12) & 0xf];
    p[index++] = symbols[(crc >> 8) & 0xf];

    // Total number of 6-bit symbols to send
    vw_tx_len = index + VW_HEADER_LEN;

    // Start the low level interrupt handler sending symbols
    vw_tx_start();

    return 1;
}

// ----------------------------------------------------------------------------
// RX
// ----------------------------------------------------------------------------

// Compute CRC over count bytes.
// This should only be ever called at user level, not interrupt level

uint16_t vw_crc(uint8_t *ptr, uint8_t count)
{
    uint16_t crc = 0xffff;

    while (count-- > 0)
        crc = _crc_ccitt_update(crc, *ptr++);
    return crc;
}

// Convert a 6 bit encoded symbol into its 4 bit decoded equivalent

uint8_t vw_symbol_6to4(uint8_t symbol)
{
    uint8_t i;

    // Linear search :-( Could have a 64 byte reverse lookup table?
    for (i = 0; i < 16; i++)
        if (symbol == symbols[i])
            return i;

    return 0; // Not found
}

// Called 8 times per bit period
// Phase locked loop tries to synchronise with the transmitter so that bit 
// transitions occur at about the time vw_rx_pll_ramp is 0;
// Then the average is computed over each bit period to deduce the bit value

void vw_pll(void)
{
    // Integrate each sample
    if (vw_rx_sample)
        vw_rx_integrator++;

    if (vw_rx_sample != vw_rx_last_sample)
    {
        // Transition, advance if ramp > 80, retard if < 80
        vw_rx_pll_ramp += ((vw_rx_pll_ramp < VW_RAMP_TRANSITION)
                ? VW_RAMP_INC_RETARD
                : VW_RAMP_INC_ADVANCE);
        vw_rx_last_sample = vw_rx_sample;
    }
    else
    {
        // No transition
        // Advance ramp by standard 20 (== 160/8 samples)
        vw_rx_pll_ramp += VW_RAMP_INC;
    }
    if (vw_rx_pll_ramp >= VW_RX_RAMP_LEN)
    {
        // Add this to the 12th bit of vw_rx_bits, LSB first
        // The last 12 bits are kept
        vw_rx_bits >>= 1;

        // Check the integrator to see how many samples in this cycle were high.
        // If < 5 out of 8, then its declared a 0 bit, else a 1;
        if (vw_rx_integrator >= 5)
            vw_rx_bits |= 0x800;

        vw_rx_pll_ramp -= VW_RX_RAMP_LEN;
        vw_rx_integrator = 0; // Clear the integral for the next cycle

        if (vw_rx_active)
        {
            // We have the start symbol and now we are collecting message bits,
            // 6 per symbol, each which has to be decoded to 4 bits
            if (++vw_rx_bit_count >= 12)
            {
                // Have 12 bits of encoded message == 1 byte encoded
                // Decode as 2 lots of 6 bits into 2 lots of 4 bits
                // The 6 lsbits are the high nybble
                uint8_t this_byte =
                        (vw_symbol_6to4(vw_rx_bits & 0x3f)) << 4
                        | vw_symbol_6to4(vw_rx_bits >> 6);

                // The first decoded byte is the byte count of the following message
                // the count includes the byte count and the 2 trailing FCS bytes
                // REVISIT: may also include the ACK flag at 0x40
                if (vw_rx_len == 0)
                {
                    // The first byte is the byte count
                    // Check it for sensibility. It cant be less than 4, since it
                    // includes the bytes count itself and the 2 byte FCS
                    vw_rx_count = this_byte;
                    if (vw_rx_count < 4 || vw_rx_count > VW_MAX_MESSAGE_LEN)
                    {
                        // Stupid message length, drop the whole thing
                        vw_rx_active = 0;
                        return;
                    }
                }
                vw_tx_buf[vw_rx_len++] = this_byte;

                if (vw_rx_len >= vw_rx_count)
                {
                    // Got all the bytes now
                    vw_rx_active = 0;
                    vw_rx_done = 1; // Better come get it before the next one starts
                }
                vw_rx_bit_count = 0;
            }
        }
            // Not in a message, see if we have a start symbol
        else if (vw_rx_bits == 0xb38)
        {
            // Have start symbol, start collecting message
            vw_rx_active = 1;
            vw_rx_bit_count = 0;
            vw_rx_len = 0;
            vw_rx_done = 0; // Too bad if you missed the last message
        }
    }
}

bit vw_have_message(void)
{
    return vw_rx_done;
}

bit vw_recv(uint8_t *buf, uint8_t *len)
{
    uint8_t rxlen;

    // Message available?
    if (!vw_rx_done)
        return 0;

    // Wait until vw_rx_done is set before reading vw_rx_len
    // then remove bytecount and FCS
    rxlen = vw_rx_len - 3;

    // Copy message (good or bad)
    if (*len > rxlen)
        *len = rxlen;
    memcpy(buf, vw_tx_buf + 1, *len);

    vw_rx_done = 0; // OK, got that message thanks

    // Check the FCS, return goodness
    return (vw_crc(vw_tx_buf, vw_rx_len) == 0xf0b8); // FCS OK?
}

void vw_rx_stop(void)
{
    vw_rx_enabled = 0;
    vw_rx_enabled_last = 0;
}

void vw_rx_start(void)
{
    vw_rx_enabled = 1;
    vw_rx_enabled_last = 1;
}

void vw_isr_tmr0(void)
{
    TMR0 += -vw_tmr0_value + 4;
    TMR0IF = 0;

    if (vw_rx_enabled && !vw_tx_enabled)
        vw_rx_sample = RxData;

    // Do transmitter stuff first to reduce transmitter bit jitter due
    // to variable receiver processing
    if (vw_tx_enabled && vw_tx_sample++ == 0)
    {
        if (vw_tx_header)
        {
            TxData = (vw_tx_buf_header[vw_tx_index] & (1 << vw_tx_bit++)) ? 1 : 0;
            if (vw_tx_bit >= 6)
            {
                vw_tx_bit = 0;
                vw_tx_index++;
            }

            if (vw_tx_index >= 8)
            {
                // end of header, now switch to the else part
                vw_tx_header = 0;
                vw_tx_index = 0;
            }
        }
        else
        {
            // Send next bit
            // Symbols are sent LSB first
            // Finished sending the whole message? (after waiting one bit period
            // since the last bit)
            if (vw_tx_index >= vw_tx_len)
            {
                vw_tx_stop(); // stop tx
                vw_rx_enabled = vw_rx_enabled_last; // re-enable rx if already active
            }
            else
            {
                TxData = (vw_tx_buf[vw_tx_index] & (1 << vw_tx_bit++)) ? 1 : 0;
                if (vw_tx_bit >= 6)
                {
                    vw_tx_bit = 0;
                    vw_tx_index++;
                }
            }
        }

    }

    if (vw_tx_sample > 7)
        vw_tx_sample = 0;


    // Do receiver
    if (vw_rx_enabled && !vw_tx_enabled)
        vw_pll();
}

