/*--------------------------------------------------------------------------*
* ADC sampling control
*---------------------------------------------------------------------------*
* 24-Mar-2014 ShaneG
*
* Control the analog input pins.
*--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "analog.h"

/** Channel mask */
#define CHAN_MASK ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))

/** Initialise the ADC channels
 */
void adcInit() {
  ADMUX = (1 << ADLAR) | CHAN_VOLTAGE;
  ADCSRA = (1 << ADEN);
  DIDR0 = (1 << ADC2D) | (1 << ADC3D);
  }

/** Read the given ADC channel
 *
 * @return the current reading of the voltage monitoring ADC
 */
uint8_t adcRead(uint8_t channel) {
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

