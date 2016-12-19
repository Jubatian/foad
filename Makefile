###############################################################################
# Makefile for the project SpriteDemo
###############################################################################

## General Flags
PROJECT = foaddemo
GAME    = foaddemo
INFO    = gameinfo.properties
MCU     = atmega644
TARGET  = $(GAME).elf
CC      = avr-gcc
OUTDIR  = _bin_
OBJDIR  = _obj_
DEPDIR  = _dep_
DIRS    = $(OUTDIR) $(OBJDIR) $(DEPDIR)

## Kernel settings
KERNEL_DIR = kernelm74
KERNEL_OPTIONS  = -DVIDEO_MODE=74
KERNEL_OPTIONS += -DINTRO_LOGO=1
#KERNEL_OPTIONS += -DINTRO_LOGO=0
KERNEL_OPTIONS += -DSOUND_MIXER=MIXER_TYPE_INLINE
KERNEL_OPTIONS += -DSOUND_CHANNEL_5_ENABLE=0
KERNEL_OPTIONS += -DCONTROLLERS_VSYNC_READ=0
KERNEL_OPTIONS += -DP2_DISABLE=1

## Packrom (.uze)
PACKROM_DIR = ../packrom


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS  = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=28636360UL -O2 -fsigned-char -ffunction-sections -fno-toplevel-reorder
CFLAGS += -MD -MP -MT $(*F).o -MF $(DEPDIR)/$(@F).d
CFLAGS += $(KERNEL_OPTIONS)


## Assembly specific flags
ASMFLAGS  = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS  = $(COMMON)
LDFLAGS += -Wl,-Map=$(OUTDIR)/$(GAME).map
LDFLAGS += -Wl,-gc-sections

## Setup for Mode 74 stuff
CFLAGS  += -include m74cfg.h
LDFLAGS += -Wl,--section-start=.res=0x7800

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS  = $(OBJDIR)/uzeboxVideoEngineCore.o $(OBJDIR)/uzeboxCore.o $(OBJDIR)/uzeboxSoundEngine.o $(OBJDIR)/uzeboxSoundEngineCore.o $(OBJDIR)/uzeboxVideoEngine.o
OBJECTS += $(OBJDIR)/$(GAME).o
OBJECTS += $(OBJDIR)/res.o
OBJECTS += $(OBJDIR)/sprite.o
OBJECTS += $(OBJDIR)/level.o
OBJECTS += $(OBJDIR)/levelcor.o
OBJECTS += $(OBJDIR)/levelscr.o
OBJECTS += $(OBJDIR)/physics.o
OBJECTS += $(OBJDIR)/passable.o
OBJECTS += $(OBJDIR)/dragon.o
OBJECTS += $(OBJDIR)/spriteid.o
OBJECTS += $(OBJDIR)/actordim.o
OBJECTS += $(OBJDIR)/spritelv.o
OBJECTS += $(OBJDIR)/mapact.o
OBJECTS += $(OBJDIR)/pal.o
OBJECTS += $(OBJDIR)/random.o
OBJECTS += $(OBJDIR)/parallax.o
OBJECTS += $(OBJDIR)/mapobjp.o
OBJECTS += $(OBJDIR)/map.o
OBJECTS += $(OBJDIR)/fireball.o
OBJECTS += $(OBJDIR)/firebals.o
OBJECTS += $(OBJDIR)/eproj.o
OBJECTS += $(OBJDIR)/status.o
OBJECTS += $(OBJDIR)/text.o
OBJECTS += $(OBJDIR)/display.o
OBJECTS += $(OBJDIR)/global.o
OBJECTS += $(OBJDIR)/gstat.o
OBJECTS += $(OBJDIR)/torch.o
OBJECTS += $(OBJDIR)/acsupp.o
OBJECTS += $(OBJDIR)/acarcher.o
OBJECTS += $(OBJDIR)/acdoor.o
OBJECTS += $(OBJDIR)/acbomb.o
OBJECTS += $(OBJDIR)/acprison.o
OBJECTS += $(OBJDIR)/acsupply.o
OBJECTS += $(OBJDIR)/aceol.o
OBJECTS += $(OBJDIR)/sound.o
OBJECTS += $(OBJDIR)/eeprom.o
OBJECTS += $(OBJDIR)/seq.o
OBJECTS += $(OBJDIR)/story.o
OBJECTS += $(OBJDIR)/game.o
OBJECTS += $(OBJDIR)/death.o
OBJECTS += $(OBJDIR)/intro.o
OBJECTS += $(OBJDIR)/hiscore.o
OBJECTS += $(OBJDIR)/music.o

## Include Directories
INCLUDES = -I"$(KERNEL_DIR)"

## Build
all: $(OUTDIR)/$(TARGET) $(OUTDIR)/$(GAME).hex $(OUTDIR)/$(GAME).eep $(OUTDIR)/$(GAME).lss $(OUTDIR)/$(GAME).uze size

## Directories
$(OBJDIR):
	mkdir $(OBJDIR)

$(OUTDIR):
	mkdir $(OUTDIR)

$(DEPDIR):
	mkdir $(DEPDIR)

## Compile Kernel files
$(OBJDIR)/uzeboxVideoEngineCore.o: $(KERNEL_DIR)/uzeboxVideoEngineCore.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/uzeboxSoundEngineCore.o: $(KERNEL_DIR)/uzeboxSoundEngineCore.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/uzeboxCore.o: $(KERNEL_DIR)/uzeboxCore.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/uzeboxSoundEngine.o: $(KERNEL_DIR)/uzeboxSoundEngine.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/uzeboxVideoEngine.o: $(KERNEL_DIR)/uzeboxVideoEngine.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

## Compile game sources
$(OBJDIR)/$(GAME).o: main.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/res.o: res.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/sprite.o: sprite.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/level.o: level.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/levelcor.o: levelcor.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/levelscr.o: levelscr.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/physics.o: physics.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/passable.o: passable.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/dragon.o: dragon.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/spriteid.o: spriteid.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/actordim.o: actordim.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/spritelv.o: spritelv.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/mapact.o: mapact.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/pal.o: pal.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/random.o: random.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/parallax.o: parallax.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/mapobjp.o: mapobjp.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/map.o: map.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/fireball.o: fireball.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/firebals.o: firebals.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/eproj.o: eproj.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/status.o: status.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/text.o: text.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/display.o: display.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/global.o: global.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/gstat.o: gstat.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/torch.o: torch.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/acsupp.o: acsupp.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/acarcher.o: acarcher.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/acdoor.o: acdoor.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/acbomb.o: acbomb.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/acprison.o: acprison.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/acsupply.o: acsupply.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/aceol.o: aceol.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/sound.o: sound.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/eeprom.o: eeprom.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

$(OBJDIR)/seq.o: seq.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/story.o: story.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/game.o: game.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/death.o: death.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/intro.o: intro.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/hiscore.o: hiscore.c $(DIRS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJDIR)/music.o: music.s $(DIRS)
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< -o $@

##Link
$(OUTDIR)/$(TARGET): $(OBJECTS) $(DIRS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LIBDIRS) $(LIBS) -o $(OUTDIR)/$(TARGET)

$(OUTDIR)/%.hex: $(OUTDIR)/$(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS) $< $@

$(OUTDIR)/%.eep: $(OUTDIR)/$(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

$(OUTDIR)/%.lss: $(OUTDIR)/$(TARGET)
	avr-objdump -h -S $< > $@

$(OUTDIR)/%.uze: $(OUTDIR)/$(TARGET)
	-$(PACKROM_DIR)/packrom $(OUTDIR)/$(GAME).hex $@ $(INFO)

UNAME := $(shell sh -c 'uname -s 2>/dev/null || echo not')
AVRSIZEFLAGS := -A $(OUTDIR)/${TARGET}
ifneq (,$(findstring MINGW,$(UNAME)))
AVRSIZEFLAGS := -C --mcu=${MCU} $(OUTDIR)/${TARGET}
endif

size: $(OUTDIR)/${TARGET}
	@echo
	@avr-size ${AVRSIZEFLAGS}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(DIRS)

