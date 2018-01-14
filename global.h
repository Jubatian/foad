/*
 *  Dragon - Globals
 *  Copyright (C) 2016 Sandor Zsuga (Jubatian)
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
*/


/*
** Globals governing the overall game.
*/


#ifndef GLOBAL_H
#define GLOBAL_H


#include "types.h"



/* For the palette control variable */
#define GLOBAL_FADE_TOP   0x08U
#define GLOBAL_FADE_SKY   0x04U
#define GLOBAL_FADE_PF    0x02U
#define GLOBAL_FADE_VOL   0x80U
#define GLOBAL_FADE_ALL   (GLOBAL_FADE_TOP | GLOBAL_FADE_SKY | GLOBAL_FADE_PF)
#define GLOBAL_FADE_GAME  (GLOBAL_FADE_SKY | GLOBAL_FADE_PF)
#define GLOBAL_FADE_ALLV  (GLOBAL_FADE_ALL | GLOBAL_FADE_VOL)
#define GLOBAL_FADE_INC   0x01U
#define GLOBAL_FADE_FLASH 0x10U
#define GLOBAL_FADE_2X    0x20U



/*
** Frame counter, increments 60 times a second. Can be used for various
** timings. Should not be written.
*/
extern auint  global_framectr;


/*
** Current fade target color. Should be used for palette generation if a fade
** is in progress.
*/
extern auint  global_fadecolor;


/*
** Fade counter. 0: No fading, 255: Fully faded out. Normally should not be
** altered (the palette control drives it).
*/
extern auint  global_fadectr;


/*
** Palette control. This controls how the automatic palette & fade processing
** (global_genpal()) should operate. GLOBAL_FADE_INC controls direction: If
** set, fades out (increments fadectr), otherwise fades in (decrements).
** GLOBAL_FADE_FLASH requests a two phase fade, first according to
** GLOBAL_FADE_INC, then to its inverse. GLOBAL_FADE_2X requests 2x speed
** processing.
*/
extern auint  global_palctr;


/*
** Palette selects. This selects the sky (bottom 4 bits) and the top (top 4
** bits) palettes to use for generation.
*/
extern auint  global_palsel;


/*
** Shared data between various screens (which don't coexist). If the button
** press detector is used, the first 2 entries belong to it.
*/
extern auint  global_shared[8];


/*
** Shared data between various screens (which don't coexist), words. The space
** is shared with global_shared, this is provided for storing 16 bit values
** directly, which the C compiler is terrible at splitting up and merging into
** 8 bit values. Don't alias between the two (Undefined in C), use only to
** store and retrieve 16 bit values.
*/
extern uint16 global_shared_word[4];


/*
** Credit count (number of lives). Setting Bit 7 disables controller reads,
** this may be used by JAMMA to count coins (requiring processing P2 input).
*/
extern auint  global_credits;


/*
** Processes globals. Should be called on the beginning of all frames.
*/
void global_process(void);


/*
** Generates game palettes according to the fade configuration.
*/
void global_genpal(void);


/*
** Inits button press detector (use when you only need to advance screens)
*/
void global_initpress(void);


/*
** Returns nonzero if a button had a rising edge (use when you only need to
** advance screens).
*/
auint global_ispress(void);


/*
** Sets X shift map for simple screens (this is always required before
** blitting any sprite since the frame render destroys it)
*/
void  global_setxshift(auint pos);


/*
** Makes sure nothing is visible by applying maximal fading. This may be used
** in transitions. It sets global_fadectr to max and calls global_genpal().
*/
void  global_hide(void);


/*
** Checks coin count for JAMMA and sets credit counter accordingly. Bit 7 of
** the credit counter is set when it waits for coins (0 credits). Takes the
** state of the JAMMA soft dip-switches as argument. Returns coins required to
** start playing (0: game may start), bit 7 set if there was a coin slot
** activity.
*/
auint global_jammacount(auint sdips);


#endif
