/*---------------------------------------------------------------------------*
* Implement a simple tick counter for timing purposes.
*----------------------------------------------------------------------------*
* 18-Mar-2014 ShaneG
*
* This implements a simple tick counter for timing purposes. This code uses
* timer 1 on the ATtiny to generate the interrupts.
*---------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ticker.h"

//---------------------------------------------------------------------------
// Interrupt handling
//---------------------------------------------------------------------------

// The global tick count
static volatile uint16_t g_ticks = 0;
static volatile uint16_t g_seconds = 0;

/** Interrupt handler for timer 1
 */
ISR(TIMER1_COMPA_vect) {
  g_ticks++;
  if((g_ticks % 10)==0)
    g_seconds++;
  }

//---------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------

/** Initialise the tick counter
 */
void tickInit() {
  cli();
  TCCR1 = 0x8F; // Set prescaler to 16384
  GTCCR = 0x00;
  TCNT1 = 0x00;
  OCR1A = 49; // Approx 1/10th of a second
  // Enable timer compare interrupt
  TIMSK |= (1 << OCIE1A);
  sei();//allow interrupts
  }

/** Get the current tick count
 *
 * @return the current tick counter value.
 */
uint16_t ticks() {
  return g_ticks;
  }

/** Calculate the duration between now and a previously saved tick count.
 *
 * This function returns the amount of elapsed time (in ticks) since the
 * measured value. The maximum allowed time is 65535 ticks (about 1 hour and
 * 50 minutes).
 *
 * @param ticks the tick sample to test against.
 *
 * @return the number of elapsed ticks
 */
uint16_t ticksElapsed(uint16_t ticks) {
  uint16_t now = g_ticks;
  if(now<ticks)
    return (MAX_TICKS - ticks) + now;
  return now - ticks;
  }

/** Get the current time in seconds
 *
 * @return the current time in seconds
 */
uint16_t seconds() {
  return g_seconds;
  }

/** Calculate the duration between now and a previously saved seconds count.
 *
 * This function returns the amount of elapsed time (in seconds) since the
 * measured value. The maximum allowed time is 65535 seconds (a little over
 * 18 hourse).
 *
 * @param seconds the time sample to test against.
 *
 * @return the number of elapsed seconds
 */
uint16_t secondsElapsed(uint16_t seconds) {
  uint16_t now = g_seconds;
  if(now<seconds)
    return (MAX_TICKS - seconds) + now;
  return now - seconds;
  }

