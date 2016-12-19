/*
 *  Uzebox(tm) Global defines and build options
 *  Copyright (C) 2008-2009 Alec Bourque
 *  Optimized and trimmed to the game by Sandor Zsuga (Jubatian), 2016
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Uzebox is a reserved trade mark
*/

/** 
 * ==============================================================================
 *
 * This file contains default compilation setting that sets the video mode and 
 * enables certains options. Note that these options affects the
 * memory consomption (RAM & FLASH).
 *
 * ===============================================================================
 */
#ifndef __DEFINES_H_
	#define __DEFINES_H_
 	#include <avr/io.h>

	//For some reason the Atmega1284P io.h does not include the old "PA0" defines
	#ifndef PA0
		#define PA0 PORTA0
		#define PA1 PORTA1 
		#define PA2 PORTA2 
		#define PA3 PORTA3 
		#define PA4 PORTA4 
		#define PA5 PORTA5 
		#define PA6 PORTA6 
		#define PA7 PORTA7 
	#endif
	#ifndef PB0
		#define PB0 PORTB0
		#define PB1 PORTB1 
		#define PB2 PORTB2 
		#define PB3 PORTB3 
		#define PB4 PORTB4 
		#define PB5 PORTB5 
		#define PB6 PORTB6 
		#define PB7 PORTB7 
	#endif
	#ifndef PC0
		#define PC0 PORTC0
		#define PC1 PORTC1 
		#define PC2 PORTC2 
		#define PC3 PORTC3 
		#define PC4 PORTC4 
		#define PC5 PORTC5 
		#define PC6 PORTC6 
		#define PC7 PORTC7 
	#endif
	#ifndef PD0
		#define PD0 PORTD0
		#define PD1 PORTD1 
		#define PD2 PORTD2 
		#define PD3 PORTD3 
		#define PD4 PORTD4 
		#define PD5 PORTD5 
		#define PD6 PORTD6 
		#define PD7 PORTD7 
	#endif


	//Generic define
	#define DISABLED 0
	#define ENABLED  1
	
	#define MIXER_TYPE_VSYNC 	0
	#define MIXER_TYPE_INLINE	1

	#define MIDI 0
	#define MOD	 1

 	/*
	 * Defines the video mode to use. 
	 *
	 * 0 = Reserved
	 * 1 = 40x28 Tile-only. 'Unlimited' tiles per frame (16 bit VRAM). 6x8 tiles.
	 * 2 = 22x26 Tiles+Sprites, full-screen scrolling with split screens. Max 256 background tiles. 6x8 tiles & sprites.
	 * 3 = 30x28 Tiles+Sprites (currently no scrolling). Max 256 background tiles. 8x8 tiles. 
	 */	
	#ifndef VIDEO_MODE 
		#define VIDEO_MODE 1
	#endif

	/*
	 * Enable horizontal scrolling for video mode 2.
	 * 
	 * Note: This option needs 9K of flash due to unrolled loops.
	 * 
	 * 0 = no
	 * 1 = yes
	 */	
	#ifndef MODE2_HORIZONTAL_SCROLLING
		#define MODE2_HORIZONTAL_SCROLLING 1
	#endif


	/*
	 * Display the Uzebox logo when the console is reset
	 * 0 = none
	 * 1 = with "bling" sound
	 * 2 = with "uzebox" synth voice (PCM required, MIXER_CHAN4_TYPE must == 1)
	 */
	#ifndef INTRO_LOGO
		#define INTRO_LOGO 1
	#endif

	/*
	 * Joystick type used on the board.
	 * Note: Will be read from EEPROM in a future release. 
	 *
	 * 0 = SNES
	 * 1 = NES
	 */
	#ifndef JOYSTICK
		#define JOYSTICK 0
	#endif

	/*
	 * Disable second player's controller.
	 * This saves some ROM and RAM for single player games
	 */
	#ifndef P2_DISABLE
		#define P2_DISABLE 0
	#endif

	/*
	 * Adds support to generate a true random seed upon
	 * startup. This uses the watchdog timer RC oscillator
	 * entropy to generate a non-deterministic seed that can
	 * be used in rand().
	 *
	 * 0 = no support
	 * 1 = support enabled
	 */
	#ifndef TRUE_RANDOM_GEN
		#define TRUE_RANDOM_GEN 0
	#endif


	/*
	 * Completely remove all the sound mixer & repalyer code.
	 * The ring buffer (vsync mixer only) & basic sound functionality is preserved for 
	 * the emulator and application that fills themselves the buffer.
	 */
	#ifndef ENABLE_MIXER
		#define ENABLE_MIXER 1
	#endif
		
	/*
	 * Define the ammount of memory to allocate
	 * for ramtiles 
	 * (used in video modes that supports ramtiles)
	 */
	 
	#ifndef RAM_TILES_COUNT
		#define RAM_TILES_COUNT 0
	#endif

	/*
	 * Defines the numbers of sprites to alloacate ressources for.
	 * (used in video modes that supports sprites)
	 */
	#ifndef MAX_SPRITES
		#define MAX_SPRITES 32
	#endif

	/*
	 * Number of screen sections to allocate memory for
	 * Min=1, Max=SCREEN_TILES_V*TILE_HEIGHT
	 * (used in video modes that supports screen sections)
	 */
	#ifndef SCREEN_SECTIONS_COUNT
		#define SCREEN_SECTIONS_COUNT 1
	#endif

	/*
	 * Kernel Internal settings, do not modify
	 */

	//Pin used to enable the AD723
	#define VIDEOCE_PIN PB4

	//sound player master volume
	#define DEFAULT_MASTER_VOL	0x6f

	//Joypad standard buttons mappings.
	//Applies to both NES & SNES gamepads.
	#define TYPE_SNES 0
	#define TYPE_NES 1

	#if JOYSTICK == TYPE_SNES
		#define BTN_SR	   2048
		#define BTN_SL	   1024
		#define BTN_X	   512
		#define BTN_A	   256
		#define BTN_RIGHT  128
		#define BTN_LEFT   64
		#define BTN_DOWN   32
		#define BTN_UP     16
		#define BTN_START  8
		#define BTN_SELECT 4
		#define BTN_Y      2
		#define BTN_B      1
	#elif JOYSTICK == TYPE_NES
		#define BTN_SR	   2048 //unused
		#define BTN_SL	   1024 //unused		
		#define BTN_X	   512 //unused
		#define BTN_Y      256 //unused

		#define BTN_RIGHT  128
		#define BTN_LEFT   64
		#define BTN_DOWN   32
		#define BTN_UP     16
		#define BTN_START  8
		#define BTN_SELECT 4
		#define BTN_B      2
		#define BTN_A      1
	#endif 


	//Screen sections flags
	#define SCT_PRIORITY_BG  0
	#define SCT_PRIORITY_SPR 1

	//Sprites Options
	#define SPR_OVERFLOW_CLIP   0
	#define SPR_OVERFLOW_ROTATE 1
	#define SPR_FLIP_X 1
	#define SPR_FLIP_Y 2


	//Patch commands
	#define PC_ENV_SPEED        0
	#define PC_NOISE_PARAMS     1
	#define PC_WAVE             2
	#define PC_NOTE_UP          3
	#define PC_NOTE_DOWN        4
	#define PC_NOTE_CUT         5
	#define PC_NOTE_HOLD        6
	#define PC_ENV_VOL          7
	#define PC_PITCH            8
	#define PC_TREMOLO_LEVEL    9
	#define PC_TREMOLO_RATE     10
	#define PC_SLIDE            11
	#define PC_SLIDE_SPEED      12
	#define PC_TREMOLO_POS      13
	#define PC_TREMOLO_WAVE     14
	#define PATCH_END           0xff


	// Mixer
	#define WAVE_CHANNELS  5
	#define NOISE_CHANNELS 0
	#define PCM_CHANNELS   0
	#define CHANNELS       (WAVE_CHANNELS + NOISE_CHANNELS + PCM_CHANNELS)
	#define AUDIO_OUT_HSYNC_CYCLES (142 + ((CHANNELS - 3) * 36))


	#define SWEEP_UP   0x80
	#define SWEEP_DOWN 0x00


	//Line rate timer delay: 15.73426 kHz*2 = 1820/2 = 910
	//2x is to account for vsync equalization & serration pulse that are at 2x line rate
	#define HDRIVE_CL 1819
	#define HDRIVE_CL_TWICE 909
	#define SYNC_HSYNC_PULSES 253

	#define SYNC_PRE_EQ_PULSES 6
	#define SYNC_EQ_PULSES 6
	#define SYNC_POST_EQ_PULSES 6

	#define SYNC_FLAG_VSYNC			1
	#define SYNC_FLAG_FIELD			2

	#define SYNC_PIN PB0
	#define SYNC_PORT PORTB
	#define DATA_PORT PORTC

	#define VIDEO_PORT _SFR_IO_ADDR(DATA_PORT)

	#define MIX_BANK_SIZE (SYNC_HSYNC_PULSES + ((SYNC_PRE_EQ_PULSES+SYNC_EQ_PULSES+SYNC_POST_EQ_PULSES)/2))
	#define MIX_BUF_SIZE MIX_BANK_SIZE*2
	//#define MIDI_RX_BUF_SIZE 128

	#define JOYPAD_OUT_PORT PORTA
	#define JOYPAD_IN_PORT PINA
	#define JOYPAD_CLOCK_PIN PA3
	#define JOYPAD_LATCH_PIN PA2
	#define JOYPAD_DATA1_PIN PA0
	#define JOYPAD_DATA2_PIN PA1

	#define EEPROM_BLOCK_SIZE 32
	#define EEPROM_HEADER_SIZE 1
	#define EEPROM_SIGNATURE 0x555A
	#define EEPROM_SIGNATURE2 0x555B
	#define EEPROM_FREE_BLOCK 0xffff
	#define EEPROM_ERROR_INVALID_BLOCK 0x1
	#define EEPROM_ERROR_FULL 0x2
	#define EEPROM_ERROR_BLOCK_NOT_FOUND 0x3
	#define EEPROM_ERROR_NOT_FORMATTED 0x4

	#if VIDEO_MODE == 74
		#include "videoMode74/videoMode74.def.h"
	#else
		#error Invalid video mode defined with VIDEO_MODE
	#endif

	#ifdef HSYNC_USABLE_CYCLES 
		#if HSYNC_USABLE_CYCLES - AUDIO_OUT_HSYNC_CYCLES <0
			#error There is not enough CPU cycles to support the build options. Disable the UART (-DUART=0), audio channel 5 (-DSOUND_CHANNEL_5_ENABLE=0) or the inline mixer (-DSOUND_MIXER=0).
		#endif 
	#endif

#endif
