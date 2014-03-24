/*--------------------------------------------------------------------------*
* PWM output control
*---------------------------------------------------------------------------*
* 24-Mar-2014 ShaneG
*
* Control the PWM output pin.
*--------------------------------------------------------------------------*/
#ifndef __DIGITAL_H
#define __DIGITAL_H

/** Pin driving the LED */
#define LED_PIN PINB1

/** Pin driving the PWM light output */
#define PWM_PIN PINB4

/** Initialise the LED output
 */
void ledInit();

/** Change the LED state
 *
 * @param state true if the LED should be on, false if not
 */
void ledState(bool state);

/** Initialise the PWM output
 */
void pwmInit();

/** Set the PWM output duty cycle
 *
 * @param duty the duty cycle. From 0 (always off) to 255 (always on).
 */
void pwmOut(uint8_t duty);

#endif /* __DIGITAL_H */

