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

#define CHAR_START    '!'
#define CHAR_END      '\n'
#define BODY_LENGTH   5
#define PACKET_LENGTH (BODY_LENGTH + 2)

//---------------------------------------------------------------------------
// Delay constants and simple send/recv byte macros
//---------------------------------------------------------------------------

#ifdef F_CPU
  /* account for integer truncation by adding 3/2 = 1.5 */
  #define TXDELAY (((F_CPU/BAUD_RATE)-7 +1.5)/3)
  #define RXDELAY (((F_CPU/BAUD_RATE)-5 +1.5)/3)
  #define RXROUNDED (((F_CPU/BAUD_RATE)-5 +2)/3)
  #if RXROUNDED > 127
    #error low baud rates unsupported - use higher BAUD_RATE
  #endif
#else
  #error CPU frequency F_CPU undefined
#endif

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

/** Wait for the next character
 *
 * This function will attempt to read characters for up to 'wait' character
 * lengths for a valid character.
 *
 * @param wait the number of character periods to wait
 *
 * @return the valid character read or 0xFF if it timed out.
 */
static uint8_t waitChar(int delay) {
  delay = delay * 2; // Each RxByte() call waits up to 5 bits anyway
  uint8_t result = RxByte();
  while((result==BAD_CHAR)&&delay) {
    delay--;
    result = RxByte();
    }
  return result;
  }

/** Convert the lowest 4 bits of a value to a hex digit
 *
 * @param value the value to convert
 *
 * @return the hex digit representing the value
 */
static uint8_t toHex(int value) {
  value = value & 0x000F;
  if(value<10)
    return (uint8_t)('0' + value);
  return (uint8_t)('A' + value - 10);
  }

/** Convert an ASCII hex digit to binary
 *
 * @param hex the hex character to convert
 *
 * @return the value represented by the character or -1 if the character is
 *         not a valid hex digit.
 */
static int fromHex(uint8_t hex) {
  if((hex>='0')&&(hex<='9'))
    return (int)(hex - '0');
  if((hex>='a')&&(hex<='f'))
    return (int)(hex - 'a');
  if((hex>='A')&&(hex<='F'))
    return (int)(hex - 'A');
  // Bad hex digit
  return -1;
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
  TxByte(CHAR_START);
  for(int i=0; i<4; i++)
    TxByte(toHex((data >> ((3 - i) * 4))));
  TxByte(toHex(checksum(data)));
  TxByte(CHAR_END);
  }

/** Receive a packet
 *
 * @param pData pointer to the 16 bit integer to receive the packet.
 *
 * @return true if a valid packet was received, false otherwise.
 */
bool uartRecv(uint16_t *pData) {
  static char buffer[BODY_LENGTH];
  // Check for the start character
  if(RxByte()!=CHAR_START)
    return false;
  // Read the body
  int index = 0;
  while(index<BODY_LENGTH) {
    buffer[index] = waitChar(2);
    if(buffer[index]==BAD_CHAR)
      return false;
    index++;
    }
  // Next byte must be the terminator
  if(waitChar(2)!=CHAR_END)
    return false;
  // Convert the packet to a 16 bit value
  uint16_t result = 0;
  int nybble;
  for(index=0; index<(BODY_LENGTH - 1); index++) {
    if((nybble=fromHex(buffer[index]))<0)
      return false;
    result = ((result << 4) & 0xFFF0) | (nybble & 0x000F);
    }
  // Verify the checksum
  if(fromHex(buffer[BODY_LENGTH - 1])!=checksum(result))
    return false;
  // Valid packet
  *pData = result;
  return true;
  }
