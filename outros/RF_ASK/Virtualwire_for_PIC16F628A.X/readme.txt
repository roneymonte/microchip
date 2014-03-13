Simplified Virtual Wire compatible library for PIC microcontrollers.

Tested on PIC 16F628.

It is based on the original VirtualWire 1.20 available for Arduino at
http://www.airspayce.com/mikem/arduino/

--//--

This example was re-compiled by me (Roney Monte @ 13-mar-2014) using
now a PIC18F4550 and XC8 compiler, working well.

The TX module (connected to PIC18F4550) is a FS1000A.
The RX module is connected to a Arduino running its default Sketch for
VirtualWire reception.
The baud rate tested was 600 bps set in both sides (PIC and Arduino)

Here is the source of the library for Microchip PIC:
http://www.enide.net/webcms/?page=virtualwire-for-pic