# Night Light - By [The Garage Lab](http://thegaragelab.com)

This project is a simple presence sensing night light for doors, stairs and
other areas which could be dangerous or difficult to navigate in low lighting
conditions. It is built around an ATtiny85 microcontroller and uses the head
from a cheap LED torch as the lighting element. The ambient lighting is detected
with a simple [LDR](http://www.jaycar.com.au/productView.asp?ID=RD3480) and
presence with a [PIR motion sensor](http://www.instructables.com/id/PIR-Motion-Sensor-Tutorial/?ALLSTEPS).

The project is released under a
[Creative Commons Attribution-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-sa/4.0/).

## Firmware

The firmware directory contains the source code for the firmware to be loaded
on the ATtiny85. A Makefile is provided that assumes you have the *gcc-avr*
compiler suite installed. It has only been tested on Ubuntu but should work
on any modern Linux system. Windows and OS/X support is left as an exercise
for the reader.

The firmware is around 1.5K of compiled code and the Makefile could be easily
modified for use on the ATtiny45 as well.

## Software

This directory contains the configuration utility - it allows you to change
various parameters for the unit through a serial connection. The utility is
written in Python and requires *PyGTK* and *PySerial* modules. It has
only been tested on Ubuntu systems but should work on any platform that supports
Python 2.6 or better.

## Hardware

This directory contains the schematic for the electrical components in
[Fritzing](http://fritzing.org/home/) format as well as the
[OpenSCAD](http://www.openscad.org/) files needed to generate the 3D models for
the printable casing.
