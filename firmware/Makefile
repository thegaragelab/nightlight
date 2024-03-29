# Makefile for the TinyGB firmware
#----------------------------------------------------------------------------
# 22-Dec-2013 ShaneG
#
# You will need avr-gcc for this.
#----------------------------------------------------------------------------

# CPU specific options
MCU       = attiny85
F_CPU     = 8000000
FUSE_LOW  = 0xE2
FUSE_HIGH = 0xDF
FUSE_EXT  = 0xFF

# Define tools and options
TARGET  := nightlight
SIZE    := avr-size
CXX     := avr-gcc
CFLAGS  := -std=c99 -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS := -mmcu=$(MCU)
OBJCOPY := avr-objcopy
AVRDUDE := avrdude

# Main source
SOURCES = \
  nightlight.c \
  protocol.c \
  ticker.c \
  digital.c \
  analog.c \
  sysconfig.c \
  softuart.S

# Derive object file names
OBJECTS = $(filter-out $(SOURCES), $(patsubst %.c,%.o,$(SOURCES)) $(patsubst %.S,%.o,$(SOURCES)))

.PHONY: all clean

all: $(TARGET).hex

clean:
	@rm -f $(OBJECTS) $(TARGET).hex $(TARGET).elf

flash: $(TARGET).hex
	@$(AVRDUDE) -c usbasp -p t85 -U flash:w:$(TARGET).hex -u

fuses:
	@$(AVRDUDE) -c usbasp -p t85 -u -U lfuse:w:$(FUSE_LOW):m -U hfuse:w:$(FUSE_HIGH):m -U efuse:w:$(FUSE_EXT):m

$(TARGET).elf: $(OBJECTS)
	@echo Linking
	@$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)
	@$(SIZE) --mcu=$(MCU) --format=avr $@

$(TARGET).hex: $(TARGET).elf
	@echo Creating Hex File
	@$(OBJCOPY) -j .text -j .data -O ihex $< $@

# How to compile
%.o: %.c
	@echo Compiling $<
	@$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.S
	@echo Assembling $<
	@$(CXX) $(CFLAGS) -c $< -o $@
