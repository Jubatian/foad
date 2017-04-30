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



#include "m74cfg.h"
#include <avr/io.h>
; (Needed only for register names for X, Y and Z)



.section .bss

/*
** Frame counter, increments 60 times a second. Can be used for various
** timings. Should not be written.
*/
.global global_framectr
	global_framectr: .space 1


/*
** Current fade target color. Should be used for palette generation if a fade
** is in progress.
*/
.global global_fadecolor
	global_fadecolor: .space 1


/*
** Fade counter. 0: No fading, 255: Fully faded out. Normally should not be
** altered (the palette control drives it).
*/
.global global_fadectr
	global_fadectr: .space 1


/*
** Palette control. This controls how the automatic palette & fade processing
** (global_genpal()) should operate. GLOBAL_FADE_ON controls direction: If
** set, fades out (increments fadectr), otherwise fades in (decrements).
** GLOBAL_FADE_FLASH requests a two phase fade, first according to
** GLOBAL_FADE_ON, then to its inverse. GLOBAL_FADE_2X requests 2x speed
** processing.
*/
.global global_palctr
	global_palctr: .space 1


/*
** Palette selects. This selects the sky (bottom 4 bits) and the top (top 4
** bits) palettes to use for generation.
*/
.global global_palsel
	global_palsel: .space 1


/*
** Shared data between various screens (which don't coexist). If the button
** press detector is used, the first 2 entries belong to it.
*/
.global global_shared
	global_shared: .space 8


.section .text



/*
** Processes globals. Should be called on the beginning of all frames.
*/
.global global_process
global_process:

	; Frame counter

	lds   r24,     global_framectr
	inc   r24
	sts   global_framectr, r24

	; Fades

	lds   r23,     global_fadectr
	lds   r25,     global_palctr
	sbrs  r25,     0       ; GLOBAL_FADE_INC
	rjmp  prc_dec

	ldi   r22,     0x08
	sbrc  r25,     5       ; GLOBAL_FADE_2X
	ldi   r22,     0x10
	add   r23,     r22
	brcc  prc_ide
	ldi   r23,     0xFF
	sbrs  r25,     4       ; GLOBAL_FADE_FLASH
	rjmp  prc_ide
	andi  r25,     0xEF    ; Clear GLOBAL_FADE_FLASH
	andi  r25,     0xFE    ; Clear GLOBAL_FADE_INC
	rjmp  prc_ide
prc_dec:
	ldi   r22,     0x08
	sbrc  r25,     5       ; GLOBAL_FADE_2X
	ldi   r22,     0x10
	sub   r23,     r22
	brcc  prc_ide
	ldi   r23,     0x00
	sbrs  r25,     4       ; GLOBAL_FADE_FLASH
	rjmp  prc_ide
	andi  r25,     0xEF    ; Clear GLOBAL_FADE_FLASH
	ori   r25,     0x01    ; Set GLOBAL_FADE_INC
prc_ide:

	sts   global_palctr, r25
	sts   global_fadectr, r23

	jmp   music_process    ; Go on with processing sound & music



/*
** Generates game palettes according to the fade configuration.
*/
.global global_genpal
global_genpal:

	lds   r22,     global_fadecolor
	lds   r20,     global_fadectr
	lds   r25,     global_palctr
	sbrs  r25,     2       ; GLOBAL_FADE_SKY
	ldi   r20,     0x00
	lds   r24,     global_palsel
	andi  r24,     0x0F
	call  pal_sky_render_fade

	lds   r22,     global_fadecolor
	lds   r20,     global_fadectr
	lds   r25,     global_palctr
	sbrs  r25,     3       ; GLOBAL_FADE_TOP
	ldi   r20,     0x00
	lds   r24,     global_palsel
	swap  r24
	andi  r24,     0x0F
	call  pal_top_render_fade

	lds   r24,     global_fadecolor
	lds   r22,     global_fadectr
	lds   r25,     global_palctr
	sbrs  r25,     1       ; GLOBAL_FADE_PF
	ldi   r22,     0x00
	call  pal_pf_render_fade

	lds   r24,     global_fadectr
	lds   r25,     global_palctr
	com   r24
	sbrs  r25,     7       ; GLOBAL_FADE_VOL
	ldi   r24,     0xFF
	call  SetMasterVolume

	ret



/*
** Inits button press detector (use when you only need to advance screens)
*/
.global global_initpress
global_initpress:

	ldi   r24,     0xFF
	sts   global_shared + 0, r24
	sts   global_shared + 1, r24
	ret



/*
** Returns nonzero if a button had a rising edge (use when you only need to
** advance screens).
**
** Returns:
** r25:r24: Nonzero (1) if button was pressed.
*/
.global global_ispress
global_ispress:

	ldi   r24,     0
	call  ReadJoypad
	ldi   XL,      lo8(global_shared)
	ldi   XH,      hi8(global_shared)
	ld    r22,     X
	st    X+,      r24
	ld    r23,     X
	st    X+,      r25
	com   r22
	com   r23
	and   r22,     r24
	and   r23,     r25
	clr   r24
	clr   r25
	or    r22,     r23
	breq  .+2
	ldi   r24,     1
	ret



/*
** Sets X shift map for simple screens (this is always required before
** blitting any sprite since the frame render destroys it)
**
**     r24: Value to set
*/
.global global_setxshift
global_setxshift:

	ldi   XL,      lo8(M74_XSHIFT_OFF)
	ldi   XH,      hi8(M74_XSHIFT_OFF)
	ldi   r25,     32
	st    X+,      r24
	dec   r25
	brne  .-6
	ret



/*
** Makes sure nothing is visible by applying maximal fading. This may be used
** in transitions. It sets global_fadectr to max and calls global_genpal().
*/
.global global_hide
global_hide:

	ldi   r24,     0xFF
	sts   global_fadectr, r24
	rjmp  global_genpal
