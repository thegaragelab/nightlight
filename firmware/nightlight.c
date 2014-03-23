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
// ADC Sampling
//---------------------------------------------------------------------------

/** Voltage channel (PB3/ADC3) */
#define CHAN_VOLTAGE ((1 << MUX0) | (1 << MUX1))

/** Motion channel (PB4/ADC2) */
#define CHAN_MOTION (1 << MUX1)

/** Channel mask */
#define CHAN_MASK ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))

/** Initialise the ADC channels
 */
static void adcInit() {
  ADMUX = (1 << ADLAR) | CHAN_VOLTAGE;
  ADCSRA = (1 << ADEN);
  DIDR0 = (1 << ADC2D) | (1 << ADC3D);
  }

/** Read the given ADC channel
 *
 * @return the current reading of the voltage monitoring ADC
 */
static uint8_t adcRead(uint8_t channel) {
  // Select the channel and start the sampling
  ADMUX = (ADMUX & ~CHAN_MASK) | channel;
  ADCSRA |= (1 << ADSC);
  // Wait for the sample to complete
  while(!(ADCSRA&(1 << ADIF)));
  // Grab the result and clear the completion flag
  uint8_t result = ADCH;
  ADCSRA |= (1 << ADIF);
  // All done
  return result;
  }

/** Helper macro to read the voltage */
#define adcVoltage() adcRead(CHAN_VOLTAGE)

/** Helper macro to read the motion sensor */
#define adcMotion() adcRead(CHAN_MOTION)

//---------------------------------------------------------------------------
// Main program
//---------------------------------------------------------------------------

/** Program entry point
 */
int main() {
  uint16_t now;
  // Initialise libraries
  configInit();
  adcInit();
  ledInit();
  tickInit();
  uartInit();
  // Enter main loop
  while(true) {
    // Get current time stamp
    now = ticks();
    // Update sensor values
    configWrite(STATE_POWER, adcVoltage());
    configWrite(STATE_MOTION, adcMotion());
    while(tickElapsed(now)<10);
    ledState(false);
    while(tickElapsed(now)<20);
    ledState(true);
    }
  return 0;
  }

