/*--------------------------------------------------------------------------*
* PWM output control
*---------------------------------------------------------------------------*
* 24-Mar-2014 ShaneG
*
* Control the PWM output pin.
*--------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include "digital.h"

/** Initialise the LED output
 */
void ledInit() {
  PORTB |= (1 << LED_PIN);
  DDRB |= (1 << LED_PIN);
  }

/** Change the LED state
 *
 * @param state true if the LED should be on, false if not
 */
void ledState(bool state) {
  if(state)
    PORTB |= (1 << LED_PIN);
  else
    PORTB &= ~(1 << LED_PIN);
  }

/** Initialise the PWM output
 */
void pwmInit() {
  PORTB &= ~(1 << PWM_PIN);
  DDRB |= (1 << PWM_PIN);
  }

/** Set the PWM output duty cycle
 *
 * @param duty the duty cycle. From 0 (always off) to 255 (always on).
 */
void pwmOut(uint8_t duty) {
  if(duty!=0)
    PORTB |= (1 << PWM_PIN);
  else
    PORTB &= ~(1 << PWM_PIN);
  }

