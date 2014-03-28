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

/** Number of samples to average */
#define SAMPLE_COUNT 4

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
  uint16_t total = 0;
  // Select the channel
  ADMUX = (ADMUX & ~CHAN_MASK) | channel;
  // Take samples and sum them
  for(uint8_t i=0; i<SAMPLE_COUNT; i++) {
    ADCSRA |= (1 << ADSC);
    // Wait for the sample to complete
    while(!(ADCSRA&(1 << ADIF)));
    // Skip the first sample (allow settling)
    if(i>0)
      total = total + ADCH;
    // Clear the completion flag
    ADCSRA |= (1 << ADIF);
    }
  // All done
  return (uint8_t)((total / (SAMPLE_COUNT - 1)) & 0x00FF);
  }

