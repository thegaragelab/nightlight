#!/usr/bin/env python
#----------------------------------------------------------------------------
# Simple configuration tool for the Night Light project
#----------------------------------------------------------------------------
# 23-Mar-2014 ShaneG
#
# Initial version, command line interface only.
#----------------------------------------------------------------------------
from time import sleep
from nightlight import NightLight

def prettyList(values):
  """ Print the contents of a list in a more friendly format
  """
  return ",".join([ "%s" % str(x) for x in values ])

# Main program
if __name__ == "__main__":
  device = NightLight()
  device.connect()
  print "Configuration:"
  print prettyList(device.getConfig())
  print "State:"
  while True:
    print prettyList(device.getState())
    sleep(1)
