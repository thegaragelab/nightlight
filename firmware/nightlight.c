/*--------------------------------------------------------------------------*
* Main firmware for night light controller.
*---------------------------------------------------------------------------*
* 17-Mar-2014 ShaneG
*--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "nightlight.h"
#include "softuart.h"
#include "ticker.h"

//---------------------------------------------------------------------------
// Status LED control
//---------------------------------------------------------------------------

#define LED_PIN PINB1

/** Initialise the LED
 */
static void ledInit() {
  PORTB |= (1 << LED_PIN);
  DDRB |= (1 << LED_PIN);
  }

/** Change the LED state
 *
 * @param state true if the LED should be on, false if not
 */
static void ledState(bool state) {
  if(state)
    PORTB |= (1 << LED_PIN);
  else
    PORTB &= ~(1 << LED_PIN);
  }

//---------------------------------------------------------------------------
// Main program
//---------------------------------------------------------------------------

/** Program entry point
 */
int main() {
  uint16_t now;
  // Initialise libraries
  configInit();
  ledInit();
  tickInit();
  uartInit();
  // Enter main loop
  while(true) {
    now = ticks();
    while(tickElapsed(now)<10);
    ledState(false);
    now = ticks();
    while(tickElapsed(now)<10);
    ledState(true);
    }
  return 0;
  }

