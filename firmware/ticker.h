/*---------------------------------------------------------------------------*
* Implements a simple tick counter for timing purposes.
*----------------------------------------------------------------------------*
* 18-Mar-2014 ShaneG
*
* Defines an interface to a simple timing system.
*---------------------------------------------------------------------------*/
#ifndef __TICKER_H
#define __TICKER_H

#define MAX_TICKS 65535

/** Initialise the tick counter
 */
void initTicks();

/** Get the current tick count
 *
 * @return the current tick counter value.
 */
uint16_t getTicks();

/** Calculate the duration between now and a previously managed tick count.
 *
 * This function returns the amount of elapsed time (in ticks) since the
 * measured value. The maximum allowed time is 65535 ticks (about 1 hour and
 * 50 minutes).
 *
 * @param ticks the tick sample to test against.
 *
 * @return the number of elapsed ticks (0 if out of range)
 */
uint16_t elapsedTime(uint16_t ticks);

#endif /* __TICKER_H */

