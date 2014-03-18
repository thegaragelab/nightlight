/*--------------------------------------------------------------------------*
* Configuration for the soft UART implementation
*---------------------------------------------------------------------------*
* 18-Mar-2014 ShaneG
*
* Configuration for the UART (IO pin, speed, etc) has been pulled out into
* this file so it can be shared by the assembly and C source files.
*--------------------------------------------------------------------------*/
#ifndef __UARTCONF_H
#define __UARTCONF_H

/** Set the baud rate */
#define BAUD_RATE 57600

/** Define the hardware port and pin */
#define UART_PORT PORTB
#define UART_PIN  PINB5

#endif /* __UARTCONF_H */
