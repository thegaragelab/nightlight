/*--------------------------------------------------------------------------*
* Software only UART implementation and packet protocol.
*---------------------------------------------------------------------------*
* 18-Mar-2014 ShaneG
*
* Based on code by Ralph Doncaster for a half duplex software UART that uses
* a single IO pin. Original sources can be found at:
*   http://nerdralph.blogspot.ca/2014/01/avr-half-duplex-software-uart.html
*
* This version provides a simple data transfer protocol that allows the
* exchange of 16 bit values at 57.6KBaud - enough to do basic configuration
* and debugging.
*---------------------------------------------------------------------------*/
#ifndef __SOFTUART_H
#define __SOFTUART_H

// Get common configuration (IO, speed, etc)
#include "uartconf.h"

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Low level communications routines
//---------------------------------------------------------------------------

/** Send a single byte
 *
 * @param data the byte value to send.
 * @param delay the delay to use for each bit.
 */
void TxTimedByte(uint8_t data, uint8_t delay);

/** Receive a single character
 *
 * @param start the delay to use for the start bit
 * @param delay the delay to use for each bit.
 *
 * @return the byte read from the serial port or BAD_CHAR if the read timed
 *         out.
 */
uint8_t RxTimedByte(uint8_t start, uint8_t delay);

//---------------------------------------------------------------------------
// Protocol level operations
//---------------------------------------------------------------------------

/** Initialise the UART hardware
 */
void uartInit();

/** Send a packet
 *
 * @param data the 16 bit value to send.
 */
void uartSend(uint16_t data);

/** Receive a packet
 *
 * @param pData pointer to the 16 bit integer to receive the packet.
 *
 * @return true if a valid packet was received, false otherwise.
 */
bool uartRecv(uint16_t *pData);

#ifdef __cplusplus
}
#endif


#endif /* __SOFTUART_H */

