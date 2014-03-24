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
#include "pwm.h"

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
  // Update sensor values
  configWrite(STATE_POWER, adcVoltage());
  configWrite(STATE_MOTION, adcMotion());
  // Adjust motion sensor value match battery level
  if(configRead(STATE_POWER)<BASE_LEVEL) {
    uint8_t delta = BASE_LEVEL - configRead(STATE_POWER);
    configWrite(STATE_MOTION, configRead(STATE_MOTION) + delta);
    }
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
  };

/** The 'GET' verb - used to
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
      light_changed = seconds();
      }
    else {
      uint16_t remaining = secondsElapsed(light_changed);
      if(remaining>configRead(STATE_LIGHT))
        configWrite(STATE_LIGHT, 0);
      else
        configWrite(STATE_LIGHT, configRead(STATE_LIGHT) - remaining);
      }
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
    else
      ledState(led_on);
    }
  return 0;
  }

