/*--------------------------------------------------------------------------*
* Simple word oriented configuration protocol.
*---------------------------------------------------------------------------*
* 18-Mar-2014 ShaneG
*
* Implements a simple word (16 bit) based protocol for basic configuration
* and monitoring operations.
*--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "softuart.h"

/** Send a single byte with the appropriate delays.
 */
#define TxByte(C) TxTimedByte(C , TXDELAY)

/** Receive a single byte with the appropriate delays.
 */
#define RxByte() RxTimedByte((RXDELAY*1.5)-2.5, RXDELAY)

//---------------------------------------------------------------------------
// Helper functions
//---------------------------------------------------------------------------

/** Calculate the checksum for a 16 bit value.
 *
 * This function calculates a 4 bit checksum from the four 4 bit nybbles in
 * the data word.
 *
 * @param data the data to calculate the checksum for
 *
 * @return the checksum value (in the lowest 4 bits)
 */
static uint8_t checksum(uint16_t data) {
  int shift = 12;
  uint8_t sum = 0;
  for(int i=0; i<4; i++, shift-=4)
    sum = (sum + ((data >> shift) & 0x000f)) & 0x0f;
  return sum;
  }

//---------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------

/** Initialise the UART hardware
 */
void uartInit() {
  }

/** Send a packet
 *
 * @param data the 16 bit value to send.
 */
void uartSend(uint16_t data) {
  }

/** Receive a packet
 *
 * @param pData pointer to the 16 bit integer to receive the packet.
 *
 * @return true if a valid packet was received, false otherwise.
 */
bool uartRecv(uint16_t *pData) {
  return false;
  }
