/*--------------------------------------------------------------------------*
* ADC sampling control
*---------------------------------------------------------------------------*
* 24-Mar-2014 ShaneG
*
* Control the analog input pins.
*--------------------------------------------------------------------------*/
#ifndef __ANALOG_H
#define __ANALOG_H

/** Voltage channel (PB3/ADC3) */
#define CHAN_VOLTAGE ((1 << MUX0) | (1 << MUX1))

/** Motion channel (PB4/ADC2) */
#define CHAN_MOTION (1 << MUX1)

/** Initialise the ADC channels
 */
void adcInit();

/** Read the given ADC channel
 *
 * @return the current reading of the voltage monitoring ADC
 */
uint8_t adcRead(uint8_t channel);

/** Helper macro to read the battery voltage */
#define adcVoltage() adcRead(CHAN_VOLTAGE)

/** Helper macro to read the motion sensor */
#define adcMotion() adcRead(CHAN_MOTION)

#endif /* __ANALOG_H */

