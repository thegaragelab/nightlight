/*--------------------------------------------------------------------------*
* PWM output control
*---------------------------------------------------------------------------*
* 24-Mar-2014 ShaneG
*
* Control the PWM output pin.
*--------------------------------------------------------------------------*/
#ifndef __PWM_H
#define __PWM_H

/** Initialise the PWM output
 */
void pwmInit();

/** Set the PWM output duty cycle
 *
 * @param duty the duty cycle. From 0 (always off) to 255 (always on).
 */
void pwmOut(uint8_t duty);

#endif /* __PWM_H */

