/*--------------------------------------------------------------------------*
* Configuration management
*---------------------------------------------------------------------------*
* 20-Mar-2014 ShaneG
*
* Provides a table of configuration (and active state) values. The
* configuration values are backed by the EEPROM and will persist during
* power cycles.
*--------------------------------------------------------------------------*/
#ifndef __NIGHTLIGHT_H
#define __NIGHTLIGHT_H

/** Firmware version
 *
 * This is accessed as a system configuration property. It needs to be
 * incremented if any changes are made to the order or number of system
 * properties.
 */
#define FIRMWARE 0x01

/** System configuration entries.
 *
 * These entries reference configuration values that are backed by EEPROM
 * storage.
 */
typedef enum {
  CONFIG_FIRMWARE    = 0, //! Firmware version (read only)
  CONFIG_TRIGGER,         //! Trigger value for sensing
  CONFIG_LOW_POWER,       //! Low power detection level
  CONFIG_LIGHT_ON,        //! Time to keep the light on (in seconds)
  CONFIG_LIGHT_START,     //! Starting PWM value for turning on light
  CONFIG_LIGHT_STEP,      //! Step value for turning on/off light
  CONFIG_LED_ON,          //! Duration (in 1/10th sec) to keep LED on
  CONFIG_LED_OFF,         //! Duration (in 1/10th sec) to keep LED off
  CONFIG_LED_ON_LOW,      //! Duration (in 1/10th sec) to keep LED on (LP)
  CONFIG_LED_OFF_LOW,     //! Duration (in 1/10th sec) to keep LED off (LP)
  CONFIG_MAX              //! Maximum configuration value
  } CONFIG;

/** System state entries
 *
 * These entries represent active system state and are not backed by EEPROM.
 */
typedef enum {
  STATE_POWER = CONFIG_MAX, //! Current battery power reading
  STATE_MOTION,             //! Current location/brightness reading
  STATE_LIGHT,              //! Seconds remaining for light
  STATE_MAX                 //! Maximum state value
  } STATE;

/** Initialise the configuration
 *
 * Read the current configuration values from EEPROM, if there are no values
 * stored then use a set of defaults.
 */
void configInit();

/** Read a single configuration or state byte
 *
 * @param index the index of the value to read.
 *
 * @return the byte value associated with the parameter. If the index is out
 *         of range then 0 will be returned.
 */
uint8_t configRead(uint8_t index);

/** Write a single configuration or state byte
 *
 * @param index the index of the value to write.
 * @param value the value to store at the index.
 */
void configWrite(uint8_t index, uint8_t value);

/** Save the current configuration to EEPROM
 *
 * This function will save the current set of configuration values to the
 * EEPROM for use after the next power cycle.
 */
void configSave();

#endif /* __NIGHTLIGHT_H */

