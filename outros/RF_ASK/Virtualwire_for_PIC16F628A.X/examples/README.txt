The examples folder contains two example programs.
You'll need MPLAB and a PICC compiler to rebuild the examples.

The transmitter example "demo_tx" needs these files:
  demo_tx.c
  ..\common.h
  ..\crc16.h
  ..\examples
  ..\readme.txt
  ..\stdint.h
  ..\string.h
  ..\virtualwire.c
  ..\virtualwire.h

The receiver example "demo_rx" needs these files:
  demo_rx.c
  rs232.h
  rs232.c
  ..\common.h
  ..\crc16.h
  ..\examples
  ..\readme.txt
  ..\stdint.h
  ..\string.h
  ..\virtualwire.c
  ..\virtualwire.h

The output of the receiver demo is sent to the PIC serial port at 9600 bps.