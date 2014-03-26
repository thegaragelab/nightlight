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

if __name__ == "__main__":
  device = NightLight()
  device.connect()
  print "Configuration:"
  print device.getConfig()
  print "State:"
  while True:
    print device.getState()
    sleep(1)
