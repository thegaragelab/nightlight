#!/usr/bin/env python
#----------------------------------------------------------------------------
# Monitoring tool for the Night Light device
#----------------------------------------------------------------------------
# 28-Mar-2014 ShaneG
#
# This is a simple tool that collects information from the device and emits
# it in a comma-delimited format that can easily be used in a spreadsheet for
# analysis.
#----------------------------------------------------------------------------
import nightlight
from time import sleep

def prettyList(values):
  """ Print the contents of a list in a more friendly format
  """
  return ",".join([ "%s" % str(x) for x in values ])

# Main program
if __name__ == "__main__":
  device = nightlight.NightLight()
  device.connect()
  # Grab the configuration values and print them
  config = device.getConfig()
  print prettyList([
    config[nightlight.CONFIG_LOW_POWER],
    config[nightlight.CONFIG_TRIGGER],
    config[nightlight.CONFIG_LIGHT_ON],
    config[nightlight.CONFIG_COUNT],
    ])
  # Now just monitor the state values
  while True:
    print prettyList(device.getState())
    sleep(1)

