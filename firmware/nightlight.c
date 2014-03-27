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
#include "analog.h"
#include "digital.h"

//---------------------------------------------------------------------------
// Helper functions
//---------------------------------------------------------------------------

/** Base value for full voltage
 *
 * This is the value expected for a fully charged 6V battery pack.
 */
#define BASE_LEVEL 0xE8

/** Read the analog inputs
 *
 * This function reads the analog inputs (battery voltage and motion sensor).
 * Because the PIR and LDR are being driven directly from the batter we adjust
 * that input to the current battery voltage (so readings remain consistant).
 */
static void readSensors() {
  uint8_t power = adcVoltage();
  uint8_t motion = adcMotion();
  // Adjust motion value according to current power level
  if(power<BASE_LEVEL)
    motion = motion + (BASE_LEVEL - power);
  else if(power>BASE_LEVEL)
    motion = motion - (power - BASE_LEVEL);
  // Update sensor values
  configWrite(STATE_POWER, power);
  configWrite(STATE_MOTION, motion);
  }

/** Mask to extract command verb */
#define MASK_COMMAND 0xF000

/** Available commands and responses
 */
typedef enum {
  CMD_GET    = 0x1000, //! Get the value of a configuration/state register
  CMD_SET    = 0x2000, //! Set the value of a configuration/state register
  CMD_SAVE   = 0x3000, //! Save configuration registers to EEPROM
  STATUS_OK  = 0x0000, //! The 'OK' state
  STATUS_INF = 0x1000, //! 'OK' state with information
  STATUS_ERR = 0xFFFF, //! The 'ERR' state
  } PROTOCOL_VALUES;

/** Process an incoming command
 *
 * @param command the incoming command to process
 *
 * @return the result to return for the command.
 */
static uint16_t processCommand(uint16_t command) {
  uint16_t result = STATUS_ERR;
  uint16_t cmd = command & MASK_COMMAND;
  if(cmd==CMD_GET) {
    // Fetch the value of a register
    result = STATUS_INF | (command & 0x0F00) | (configRead((command >> 8) & 0x0F));
    }
  else if(cmd==CMD_SET) {
    // Set the value of a register
    configWrite((command >> 8) & 0x0F, command & 0x00FF);
    result = STATUS_INF | (command & 0x0F00) | (configRead((command >> 8) & 0x0F));
    }
  else if(cmd==CMD_SAVE) {
    // Save registers to EEPROM
    configSave();
    result = STATUS_OK;
    }
  return result;
  }

//---------------------------------------------------------------------------
// Main program
//---------------------------------------------------------------------------

/** Program entry point
 */
int main() {
  uint16_t led_changed, light_changed, command;
  bool led_on = true;
  // Initialise libraries
  configInit();
  adcInit();
  ledInit();
  pwmInit();
  tickInit();
  uartInit();
  // Initialise values
  led_changed = 0;
  light_changed = 0;
  // Enter main loop
  while(true) {
    // Update sensor values
    readSensors();
    // Check for any commands and respond to them
    if(uartRecv(&command))
      uartSend(processCommand(command));
    // Update light state
    if(configRead(STATE_MOTION) >= configRead(CONFIG_TRIGGER)) {
      configWrite(STATE_LIGHT, configRead(CONFIG_LIGHT_ON));
      }
    else {
      uint16_t remaining = secondsElapsed(light_changed);
      if(remaining>configRead(STATE_LIGHT))
        configWrite(STATE_LIGHT, 0);
      else
        configWrite(STATE_LIGHT, configRead(STATE_LIGHT) - remaining);
      }
    light_changed = seconds();
    // Update LED state
    if(configRead(STATE_POWER) <= configRead(CONFIG_LOW_POWER)) {
      // Use low power values
      if(led_on&&(ticksElapsed(led_changed)>=configRead(CONFIG_LED_ON_LOW))) {
        led_on = false;
        led_changed = ticks();
        }
      else if((!led_on)&&(ticksElapsed(led_changed)>=configRead(CONFIG_LED_OFF_LOW))) {
        led_on = true;
        led_changed = ticks();
        }
      }
    else {
      // Use normal power levels
      if(led_on&&(ticksElapsed(led_changed)>=configRead(CONFIG_LED_ON))) {
        led_on = false;
        led_changed = ticks();
        }
      else if((!led_on)&&(ticksElapsed(led_changed)>=configRead(CONFIG_LED_OFF))) {
        led_on = true;
        led_changed = ticks();
        }
      }
    // Now update the hardware outputs
    if(configRead(STATE_LIGHT)>0) {
      pwmOut(255);
      ledState(false); // Don't turn on led if light is on
      }
    else {
      pwmOut(0);
      ledState(led_on);
      }
    }
  return 0;
  }

