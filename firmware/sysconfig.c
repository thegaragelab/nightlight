/*--------------------------------------------------------------------------*
* Configuration management
*---------------------------------------------------------------------------*
* 20-Mar-2014 ShaneG
*
* Provides a table of configuration (and active state) values. The
* configuration values are backed by the EEPROM and will persist during
* power cycles.
*--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "nightlight.h"

/** System configuration and state values
 */
static uint8_t g_sysinfo[STATE_MAX];

//---------------------------------------------------------------------------
// EEPROM access
//---------------------------------------------------------------------------

/** Write a byte to the EEPROM
 *
 * @param address the address to write to.
 * @param data the byte to store at the address
 */
static void eepromWrite(uint8_t address, uint8_t data) {
  cli(); // Disable interrupts
  while(EECR & (1<<EEPE)); // Wait for any previous write to complete
  // Set up state and data
  EECR = (0<<EEPM1)|(0<<EEPM0);
  EEAR = address;
  EEDR = data;
  // Trigger the erase/write cycle
  EECR |= (1<<EEMPE);
  EECR |= (1<<EEPE);
  while(EECR & (1<<EEPE)); // Wait for the write to complete
  sei(); // Enable interrupts
  }

/** Read a byte from the EEPROM
 *
 * @param address the address to read from
 *
 * @return the byte value that is stored at the EEPROM address.
 */
static uint8_t eepromRead(uint8_t address) {
  cli(); // Disable interrupts
  while(EECR & (1<<EEPE)); // Wait for any previous write to complete
  EEAR = address;
  EECR |= (1<<EERE);
  uint8_t result = EEDR;
  sei(); // Enable interrupts
  return result;
  }

/** Calculate a 1 byte checksum to verify stored EEPROM data
 *
 * This calculates the checksum over the entire block of configuration
 * values.
 *
 * @return the calculated checksum
 */
static uint8_t checksum() {
  uint8_t result = 0x55; // Seed value
  for(uint8_t index=1; index<CONFIG_MAX; index++)
    result ^= g_sysinfo[index];
  return result;
  }

//---------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------

/** Initialise the configuration
 *
 * Read the current configuration values from EEPROM, if there are no values
 * stored then use a set of defaults.
 */
void configInit() {
  // Set the firmware value first
  g_sysinfo[CONFIG_FIRMWARE] = FIRMWARE;
  // Read values from EEPROM
  for(uint8_t index = 1; index<CONFIG_MAX; index++)
    g_sysinfo[index] = eepromRead(index - 1);
  // Verify the checksum
  if(eepromRead(CONFIG_MAX - 1)==checksum())
    return; // Data was valid, we are done
  // Set up with suitable defaults
  g_sysinfo[CONFIG_TRIGGER]     = 180; // Trigger value for sensing
  g_sysinfo[CONFIG_LIGHT_START] = 120; // Starting PWM value for turning on light
  g_sysinfo[CONFIG_LIGHT_STEP]  = 7;   // Step value for turning on/off light
  g_sysinfo[CONFIG_LOW_POWER]   = 173; // Low power detection level
  g_sysinfo[CONFIG_LED_ON]      = 10;  // Duration (in 1/10th sec) to keep LED on
  g_sysinfo[CONFIG_LED_OFF]     = 50;  // Duration (in 1/10th sec) to keep LED off
  g_sysinfo[CONFIG_LED_ON_LOW]  = 5;   // Duration (in 1/10th sec) to keep LED on (LP)
  g_sysinfo[CONFIG_LED_OFF_LOW] = 5;   // Duration (in 1/10th sec) to keep LED off (LP)
  }

/** Read a single configuration or state byte
 *
 * @param index the index of the value to read.
 *
 * @return the byte value associated with the parameter. If the index is out
 *         of range then 0 will be returned.
 */
uint8_t configRead(uint8_t index) {
  if(index<STATE_MAX)
    return g_sysinfo[index];
  // No such value
  return 0;
  }

/** Write a single configuration or state byte
 *
 * @param index the index of the value to write.
 * @param value the value to store at the index.
 */
void configWrite(uint8_t index, uint8_t value) {
  if((index<STATE_MAX)&&(index!=CONFIG_FIRMWARE))
    g_sysinfo[index] = value;
  }

/** Save the current configuration to EEPROM
 *
 * This function will save the current set of configuration values to the
 * EEPROM for use after the next power cycle.
 */
void configSave() {
  // Write values to EEPROM
  for(uint8_t index = 1; index<CONFIG_MAX; index++)
    eepromWrite(index - 1, g_sysinfo[index]);
  // Add the checksum
  eepromWrite(CONFIG_MAX - 1, checksum());
  }

