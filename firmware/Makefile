# Makefile for the TinyGB firmware
#----------------------------------------------------------------------------
# 22-Dec-2013 ShaneG
#
# You will need avr-gcc for this.
#----------------------------------------------------------------------------

# CPU specific options
MCU       = attiny85
F_CPU     = 8000000
FUSE_LOW  = E2
FUSE_HIGH = DF
FUSE_EXT  = 01

# Define tools and options
TARGET  := nightlight
SIZE    := avr-size
CXX     := avr-gcc
CFLAGS  := -std=c99 -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS := -mmcu=$(MCU)
OBJCOPY := avr-objcopy

# Main source
SOURCES = \
  nightlight.c \
  protocol.c \
  ticker.c \
  softuart.S

# Derive object file names
OBJECTS = $(filter-out $(SOURCES), $(patsubst %.c,%.o,$(SOURCES)) $(patsubst %.S,%.o,$(SOURCES)))

.PHONY: all clean

all: $(TARGET).hex

clean:
	@rm -f $(OBJECTS) $(TARGET).hex $(TARGET).elf

$(TARGET).elf: $(OBJECTS)
	@echo Linking
	@$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)
	@$(SIZE) $@

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