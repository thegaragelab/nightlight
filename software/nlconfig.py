#!/usr/bin/env python
#----------------------------------------------------------------------------
# Night Light Configuration Tool
#----------------------------------------------------------------------------
# 30-Mar-2014 ShaneG
#
# A graphical configuration tool for the Night Light device. Requires GTK and
# Glade modules.
#----------------------------------------------------------------------------
import sys
import nightlight
from time import sleep

# Make sure we have GTK and Glade
try:
  import pygtk
  pygtk.require("2.0")
except:
  pass

try:
  import gtk, gobject
except:
  print "Error! Unable to import required modules."
  sys.exit(1)

class Configuration:
  """ Main application class.
  """

  def __init__(self):
    # Set up internal state
    self.config = None
    self.state = None
    self.monitoring = False
    self.device = nightlight.NightLight()
    # Initialise the UI
    self.builder = gtk.Builder()
    self.builder.add_from_file("nlconfig.glade")
    self.builder.connect_signals(self)
    self.updateButtons()
    # Set up a timer for monitoring
    gobject.timeout_add_seconds(1, self.monitor)

  #--------------------------------------------------------------------------
  # Helper methods
  #--------------------------------------------------------------------------

  def monitor(self):
    """ Called every second to update values when monitoring
    """
    if self.monitoring and self.device.connected():
      # Update state values
      self.state = self.device.getState()
      self.updateState()
    # Always return true so we get triggered again
    return True

  def finishApp(self):
    if self.device.connected():
      self.device.disconnect()
    gtk.main_quit()

  def updateConfig(self):
    """ Update configuration values
    """
    for reg in range(0, nightlight.CONFIG_MAX):
      ctl = self.builder.get_object("txtRegister_%-i" % reg)
      ctl.set_value(self.config[reg])

  def updateState(self):
    """ Update the state values
    """
    for reg in range(nightlight.CONFIG_MAX, nightlight.STATE_MAX):
      ctl = self.builder.get_object("txtRegister_%-i" % reg)
      ctl.set_text(str(self.state[reg - nightlight.CONFIG_MAX]))

  def updateButtons(self):
    """ Enable or disable buttons based on state
    """
    self.builder.get_object("btnReset").set_sensitive(self.device.connected() and not self.monitoring)
    self.builder.get_object("btnSet").set_sensitive(self.device.connected() and not self.monitoring)
    self.builder.get_object("btnSave").set_sensitive(self.device.connected() and not self.monitoring)
    self.builder.get_object("btnUpdate").set_sensitive(self.device.connected() and not self.monitoring)
    self.builder.get_object("btnMonitor").set_sensitive(self.device.connected())
    self.builder.get_object("btnConnect").set_sensitive(True)
    self.builder.get_object("btnQuit").set_sensitive(True)

  def applyChanges(self):
    """ Send any changed values to the device
    """
    for reg in range(0, nightlight.CONFIG_MAX):
      ctl = self.builder.get_object("txtRegister_%-i" % reg)
      val = ctl.get_value_as_int()
      if val <> self.config[reg]:
        self.device.write(reg, val)
    # Read back the configuration
    self.config = self.device.getConfig()
    self.updateConfig()

  #--------------------------------------------------------------------------
  # Event handlers
  #--------------------------------------------------------------------------

  def on_btnConnect_clicked(self, widget):
    if self.device.connected():
      self.device.disconnect()
      widget.set_label("Connect")
    else:
      self.monitoring = False
      self.device.connect()
      widget.set_label("Disconnect")
      self.initial = self.device.getConfig()
      self.config = self.device.getConfig()
      self.state = self.device.getState()
      # Update the value
      self.updateConfig()
      self.updateState()
    self.updateButtons()

  def on_btnUpdate_clicked(self, widget):
    if self.monitoring or not self.device.connected():
      return # Nothing to do
    self.state = self.device.getState()
    self.updateState()
    self.updateButtons()

  def on_btnMonitor_clicked(self, widget):
    self.monitoring = not self.monitoring
    self.updateButtons()

  def on_btnSet_clicked(self, widget):
    if not self.device.connected():
      return
    self.applyChanges()

  def on_btnReset_clicked(self, widget):
    if not self.device.connected():
      return
    # Restore the initial values
    for reg in range(0, nightlight.CONFIG_MAX):
      self.config[reg] = self.initial[reg]
      ctl = self.builder.get_object("txtRegister_%-i" % reg)
      ctl.set_value(self.config[reg])

  def on_btnSave_clicked(self, widget):
    if not self.device.connected():
      return
    # Apply any pending changes
    self.applyChanges()
    # Save the current set of values to EEPROM
    self.device.save()

  def on_MainWindow_destroy(self, widget):
    self.finishApp()

  def on_btnQuit_clicked(self, widget):
    self.finishApp()

#--- Main program
if __name__ == "__main__":
  app = Configuration()
  gtk.main()

