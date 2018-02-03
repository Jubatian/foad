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


/*
** Shared data between various screens (which don't coexist), words. The space
** is shared with global_shared, this is provided for storing 16 bit values
** directly, which the C compiler is terrible at splitting up and merging into
** 8 bit values. Don't alias between the two (Undefined in C), use only to
** store and retrieve 16 bit values.
*/
.global global_shared_word
.equ    global_shared_word, global_shared


/*
** Credit count (number of lives). Setting Bit 7 disables controller reads,
** this may be used by JAMMA to count coins (requiring processing P2 input).
*/
.global global_credits
.equ    global_credits, MEM_LOC_P



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



; Include from the kernel for the joypad defines to get the appropriate P2
; inputs for the coin slots.
#include <defines.h>

/*
** Checks coin count for JAMMA and sets credit counter accordingly. Bit 7 of
** the credit counter is set when it waits for coins (0 credits). Takes the
** start playing (0: game may start), bit 7 set if there was a coin slot
** activity.
**
** Inputs:
**     r24: JAMMA soft dip-switches
** Outputs:
**     r24: Coins required to play
*/
.global global_jammacount
global_jammacount:

	; Does the player have any credits?

	lds   r20,     global_credits
	cpi   r20,     0
	brne  .+2
	ldi   r20,     0x80    ; 0 credits: Start waiting for coins
	sbrs  r20,     7
	ldi   r24,     0       ; Return: Coins required to play
	sbrs  r20,     7
	ret                    ; If the player has credits, no need to do anything

	; Read P2 controller data for the coin slots

	ldi   XL,      0
	ldi   XH,      0
	movw  r18,     XL
	ldi   r23,     16

	; Latch controller

	sbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_LATCH_PIN
	rcall gjc_wait
	cbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_LATCH_PIN

	; Read controller data

gjc_loop:
	rcall gjc_wait
	cbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_CLOCK_PIN
	rcall gjc_wait
	clc
	sbis  _SFR_IO_ADDR(JOYPAD_IN_PORT), JOYPAD_DATA1_PIN
	sec
	ror   r19
	ror   r18
	sbis  _SFR_IO_ADDR(JOYPAD_IN_PORT), JOYPAD_DATA2_PIN
	sec
	ror   XH
	ror   XL
	rcall gjc_wait
	sbi   _SFR_IO_ADDR(JOYPAD_OUT_PORT), JOYPAD_CLOCK_PIN
	dec   r23
	brne  gjc_loop
	sts   joypad1_status_lo + 0, r18 ; Player 1 controller
	sts   joypad1_status_lo + 1, r19

	; If there is a rising edge on either coin counter, add a coin.

	clt                    ; T: No coin slot activity
	sbrc  r20,     6
	rjmp  gjc_cchigh
#if (((BTN_SL & 0xFF) != 0U) || ((BTN_SR & 0xFF) != 0U))
	andi  XL,      lo8(BTN_SL | BTN_SR)
	breq  gjc_ccend
#endif
#if (((BTN_SL  >>  8) != 0U) || ((BTN_SR  >>  8) != 0U))
	andi  XH,      hi8(BTN_SL | BTN_SR)
	breq  gjc_ccend
#endif
	inc   r20              ; Add a coin
	ori   r20,     0x40    ; Rising edge
	set                    ; T: Coin slot activity
	rjmp  gjc_ccend
gjc_cchigh:
#if (((BTN_SL & 0xFF) != 0U) || ((BTN_SR & 0xFF) != 0U))
	andi  XL,      lo8(BTN_SL | BTN_SR)
	brne  gjc_ccend
#endif
#if (((BTN_SL  >>  8) != 0U) || ((BTN_SR  >>  8) != 0U))
	andi  XH,      hi8(BTN_SL | BTN_SR)
	brne  gjc_ccend
#endif
	andi  r20,     0xBF    ; Falling edge
gjc_ccend:

	; Check soft dip-switches against the coin counter, and set credit
	; counter when appropriate.

	andi  r24,     0x0E    ; Mask off coin/credit settings on the JAMMA soft dip-switches
	ldi   r25,     0
	movw  ZL,      r24
	subi  ZL,      lo8(-(gjc_table))
	sbci  ZH,      hi8(-(gjc_table))
	lpm   r23,     Z+      ; Coins required
	mov   r24,     r20
	andi  r24,     0x1F    ; Mask off coin count
	sub   r24,     r23
	brcs  .+4
	lpm   r20,     Z+      ; Credits given
	ldi   r24,     0       ; No more coins needed

	; Check results and save credit counter

	neg   r24              ; If still not enough coins, this is the required count
	bld   r24,     7       ; Add coin slot activity
	sts   global_credits, r20

	; Done

	ret

gjc_wait:
	ldi   r22,     32
	dec   r22
	brne  .-4
	ret

;
; Soft dip-switch decoding table: coins required / credits given
;
gjc_table:
	.byte 0x01, 0x01       ; 1 coin  / 1 credit
	.byte 0x01, 0x02       ; 1 coin  / 2 credits
	.byte 0x01, 0x03       ; 1 coin  / 3 credits
	.byte 0x02, 0x01       ; 2 coins / 1 credit
	.byte 0x03, 0x02       ; 3 coins / 2 credits
	.byte 0x03, 0x04       ; 3 coins / 4 credits
	.byte 0x04, 0x03       ; 4 coins / 3 credits
	.byte 0x00, 0x01       ; 0 coin  / 1 credit  (Free play)
