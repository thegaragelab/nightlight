#!/usr/bin/env python
#----------------------------------------------------------------------------
# 26-Mar-2014 ShaneG
#
# Interface to the Night Light gadget.
#----------------------------------------------------------------------------
import serial
from time import sleep

# Default port to use
DEFAULT_PORT = "/dev/ttyUSB0"

# Protocol definitions
CHAR_START    = '!'
CHAR_END      = '\n'
BODY_LENGTH   = 5
PACKET_LENGTH = BODY_LENGTH + 2
RETRY_COUNT   = 10

# Configuration entries
CONFIG_FIRMWARE    = 0
CONFIG_TRIGGER     = 1
CONFIG_LOW_POWER   = 2
CONFIG_LIGHT_ON    = 3
CONFIG_LIGHT_START = 4
CONFIG_LIGHT_STEP  = 5
CONFIG_LED_ON      = 6
CONFIG_LED_OFF     = 7
CONFIG_LED_ON_LOW  = 8
CONFIG_LED_OFF_LOW = 9
CONFIG_MAX         = 10

# State entries
STATE_POWER  = CONFIG_MAX
STATE_MOTION = 11
STATE_LIGHT  = 12
STATE_MAX    = 13

class NightLight:
  """ This class wraps the Night Light protocol
  """

  def __init__(self, port = DEFAULT_PORT):
    """ Constructor

      Initialise the class with the name of the port to talk to
    """
    self.port = port
    self.serial = None

  #--------------------------------------------------------------------------
  # Helper methods
  #--------------------------------------------------------------------------

  def __checksum(self, value):
    """ Calculate a single byte checksum for the 16 bit data value.
    """
    shift = 12
    result = 0
    for i in range(4):
      result = (result + ((value >> shift) & 0x000F)) & 0x0f
      shift = shift - 4
    return result

  def __send(self, value):
    """ Send a value as a packet
    """
    if self.serial is None:
      raise Exception("Attempting to send on an unopen port")
    # Retry until we get a response
    retries = RETRY_COUNT
    while retries <> 0:
      packet = "%c%04X%1X%c" % (
        CHAR_START,
        value,
        self.__checksum(value),
        CHAR_END
        )
      self.serial.write(packet)
      # Read back what we just sent (side effect of the half-duplex UART)
      self.serial.read(PACKET_LENGTH)
      # Read the return value and convert it
      response = self.serial.read(PACKET_LENGTH)
      if len(response) == PACKET_LENGTH:
        value = int(response[1:5], 16)
        return value
      # Wait and try again
      sleep(0.1)
      retries = retries - 1
    # Failed, raise an exception
    raise Exception("No response from device.")

  #--------------------------------------------------------------------------
  # Public API
  #--------------------------------------------------------------------------

  def connect(self):
    """ Connect to the device
    """
    if self.serial is not None:
      return # Already connected
    self.serial = serial.Serial(
      port = self.port,
      baudrate = 57600,
      timeout = 0.5
      )

  def disconnect(self):
    """ Disconnect from the device
    """
    if self.serial is None:
      return # Not connected
    self.serial.close()
    self.serial = None

  def read(self, register):
    """ Read the current value of a register
    """
    response = self.__send(0x1000 | ((register << 8) & 0X0F00))
    if (response & 0xF000) == 0x1000:
      return response & 0x00FF
    return response

  def write(self, register, value):
    """ Write the value of a register
    """
    response = self.__send(0x2000 | ((register << 8) & 0X0F00) | (value & 0x00FF))
    if (response & 0xF000) == 0x1000:
      return response & 0x00FF
    return response

  def save(self):
    """ Save the current values to EEPROM
    """
    response = self.__send(0x3000)
    return response

  def getConfig(self):
    """ Get all the config registers
    """
    result = list()
    for reg in range(CONFIG_FIRMWARE, CONFIG_MAX):
      result.append(self.read(reg))
    return result

  def getState(self):
    """ Get all the state registers
    """
    result = list()
    for reg in range(STATE_POWER, STATE_MAX):
      result.append(self.read(reg))
    return result

