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

/** Send a packet
 *
 * @param data the 16 bit value to send.
 */
void sendPacket(uint16_t data) {
  }

/** Receive a packet
 *
 * @param pData pointer to the 16 bit integer to receive the packet.
 *
 * @return true if a valid packet was received, false otherwise.
 */
bool recvPacket(uint16_t *pData) {
  return false;
  }
