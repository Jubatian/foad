/*
 *  Dragon - Display setup
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



.section .text



/*
** Sets up a display mode along with a reset address, and halts execution (so
** after generating a frame with the new mode, it returns onto that reset
** address). The reset address must be specified as a progmem address
** (converting a function pointer to int is OK for this).
**
**     r24: Mode to set up (see define values in header)
** r23:r22: Reset address
*/
.global display_set
display_set:

	; Common initialization

	push  r24

	sts   m74_reset + 0, r22
	sts   m74_reset + 1, r23

	ldi   r24,     34      ; Display start offset at line 34
	ldi   r22,     196     ; Display height is 196 pixels
	call  SetRenderingParameters

	ldi   r22,     lo8(LOC_TDESC_OFF)
	ldi   r23,     hi8(LOC_TDESC_OFF)
	sts   m74_tdesc + 0, r22
	sts   m74_tdesc + 1, r23

	ldi   r24,     56      ; RAM tiles used for sprites
	sts   m74_rtmax, r24

	ldi   XL,      lo8(M74_ROWS_OFF)
	ldi   XH,      hi8(M74_ROWS_OFF)
	ldi   r24,     0xFF
	ldi   r25,     27 * 3
	st    X+,      r24
	dec   r25
	brne  .-6              ; Clear all 27 row selectors

	pop   r24

	; Branch off to mode specific inits

	cpi   r24,     1
	brcs  mode_game        ; 0: DISPLAY_GAME
	breq  mode_story       ; 1: DISPLAY_STORY
	                       ; 2: DISPLAY_INTRO

	; Intro screens are thinner

	ldi   r24,     34  + 24
	ldi   r22,     196 - 48
	call  SetRenderingParameters
	ldi   r22,     lo8(LOC_TDESC_OFF)
	ldi   r23,     hi8(LOC_TDESC_OFF)

	; Set up palettes

	ldi   r21,     0x00    ; Black & White top palette, default sky

	; Initialize row selectors

	ldi   XL,      lo8(M74_ROWS_OFF)
	ldi   XH,      hi8(M74_ROWS_OFF)

	ldi   r25,     0
	ldi   r24,     20 * 8
	st    X+,      r24
	st    X+,      r25     ; 10 text rows beginning with row 20
	ldi   ZL,      80
	ldi   r24,     248 + 4
	st    X+,      ZL
	st    X+,      r24
	st    X+,      r25     ; Separator with palette replace on end
	ldi   ZL,      84
	st    X+,      ZL
	st    X+,      r25
	st    X+,      r25     ; 8 tile rows

	; The rest is same as story mode

	rjmp  mode_story_com

mode_game:

	; Set up palettes

	ldi   r21,     0x10    ; Yellow & Red top palette, default sky

	; Initialize row selectors

	ldi   XL,      lo8(M74_ROWS_OFF)
	ldi   XH,      hi8(M74_ROWS_OFF)
	ldi   r25,     0
	ldi   r24,     48 + 192 + 9
	st    X+,      r24
	st    X+,      r25     ; Top padding (2 separators)
	ldi   r24,     2
	st    X+,      r24
	ldi   r24,     48 + 192
	st    X+,      r24
	st    X+,      r25     ; Status row
	ldi   r24,     10
	st    X+,      r24
	ldi   r24,     48 + 192 + 15
	st    X+,      r24
	st    X+,      r25     ; Single separator with palette change
	ldi   r24,     11
	st    X+,      r24
	ldi   r24,     48
	st    X+,      r24
	st    X+,      r25     ; Initial graphics screen

	; Set up tile row descriptor source

	ldi   ZL,      lo8(RES_SCR_GAME_OFF)
	ldi   ZH,      hi8(RES_SCR_GAME_OFF)

	; Set up tile row address source

	ldi   r24,     lo8(RES_TID_GAME_OFF)
	ldi   r25,     hi8(RES_TID_GAME_OFF)

	rjmp  mode_end

mode_story:

	; Set up palettes

	ldi   r21,     0x10    ; Yellow & Red top palette, default sky

	; Initialize row selectors

	ldi   XL,      lo8(M74_ROWS_OFF)
	ldi   XH,      hi8(M74_ROWS_OFF)
	ldi   r25,     0
	ldi   r24,     248 + 1
	st    X+,      r24
	st    X+,      r25     ; Top padding (2 separators)
	ldi   r24,     2
	st    X+,      r24
	ldi   r24,     (12 + 16) * 8
	st    X+,      r24
	st    X+,      r25     ; Top frame row
	ldi   ZL,      10
	ldi   r24,     (12 + 18) * 8
	st    X+,      ZL
	st    X+,      r24
	st    X+,      r25
	subi  ZL,      0xF8
	cpi   ZL,      34
	brne  .-12             ; 3 empty rows
	ldi   r24,     (12     ) * 8
	st    X+,      ZL
	st    X+,      r24
	st    X+,      r25     ; 16 text rows
	ldi   ZL,      162
	ldi   r24,     (12 + 18) * 8
	st    X+,      ZL
	st    X+,      r24
	st    X+,      r25
	subi  ZL,      0xF8
	cpi   ZL,      186
	brne  .-12             ; 3 empty rows
	st    X+,      ZL
	ldi   r24,     (12 + 17) * 8
	st    X+,      r24
	st    X+,      r25     ; Bottom frame row
	ldi   r24,     194
	st    X+,      r24
	ldi   r24,     248 + 1
	st    X+,      r24
	st    X+,      r25     ; Bottom padding (2 separators)

mode_story_com:

	; Set up tile row descriptor source

	ldi   ZL,      lo8(RES_SCR_STORY_OFF)
	ldi   ZH,      hi8(RES_SCR_STORY_OFF)

	; Set up tile row address source

	ldi   r24,     lo8(RES_TID_STORY_OFF)
	ldi   r25,     hi8(RES_TID_STORY_OFF)

mode_end:

	; Write palette select

	sts   global_palsel, r21

	; Write tile row address

	sts   m74_tidx + 0, r24
	sts   m74_tidx + 1, r25

	; Write tile row descriptors

	movw  XL,      r22     ; LOC_TDESC_OFF
	ldi   r25,     32
	lpm   r24,     Z+
	st    X+,      r24
	dec   r25
	brne  .-8

	; Finalize: enable M74, halt to wait for reset

	ldi   r24,     0x9A    ; 0x08: M74_CFG_RAM_PALETTE
	                       ; 0x02: M74_CFG_RAM_TDESC
	                       ; 0x10: M74_CFG_COL0_RELOAD
	                       ; 0x80: M74_CFG_ENABLE
	sts   m74_config, r24

	rjmp  .-2              ; Wait for M74 reset



/*
** Clears VRAM for a given display mode
**
**     r24: Mode to clear for (see define values in header)
*/
.global display_clear
display_clear:

	ldi   XL,      lo8(M74_VRAM_OFF)
	ldi   XH,      hi8(M74_VRAM_OFF)
	ldi   r23,     0xAF    ; Blank tile (black)

	cpi   r24,     1
	brcs  cl_game          ; 0: DISPLAY_GAME
	breq  cl_story         ; 1: DISPLAY_STORY
	                       ; 2: DISPLAY_INTRO

	ldi   r24,     lo8(M74_VRAM_OFF + ( 8 * 25))
	ldi   r25,     hi8(M74_VRAM_OFF + ( 8 * 25))

	rcall cl_crut

	ldi   XL,      lo8(LOC_INTXTVRAM_OFF)
	ldi   XH,      hi8(LOC_INTXTVRAM_OFF)
	ldi   r24,     lo8(LOC_INTXTVRAM_OFF + (10 * 32))
	ldi   r25,     hi8(LOC_INTXTVRAM_OFF + (10 * 32))
	ldi   r23,     0x60    ; Blank tile (space)

	rjmp  cl_crut

cl_game:

	ldi   r24,     lo8(M74_VRAM_OFF + (24 * 25))
	ldi   r25,     hi8(M74_VRAM_OFF + (24 * 25))

	rcall cl_crut

	ldi   r24,     lo8(M74_VRAM_OFF + (24 * 25) + 32)
	ldi   r25,     hi8(M74_VRAM_OFF + (24 * 25) + 32)
	ldi   r23,     0x20    ; Blank tile (space)

cl_crut:

	st    X+,      r23     ; Small clear routine
	cp    XL,      r24
	cpc   XH,      r25
	brne  .-8
	ret

cl_story:

	ldi   r24,     lo8(M74_VRAM_OFF + (19 * 32))
	ldi   r25,     hi8(M74_VRAM_OFF + (19 * 32))
	ldi   r23,     0x60    ; Blank tile (space)

	rcall cl_crut

	; Add story borders

	ldi   XL,      lo8(M74_VRAM_OFF + (16 * 32))
	ldi   XH,      hi8(M74_VRAM_OFF + (16 * 32))
	ldi   r24,     lo8(M74_VRAM_OFF + (16 * 32) + 32)
	ldi   r25,     hi8(M74_VRAM_OFF + (16 * 32) + 32)
	ldi   r23,     0x41

	rcall cl_crut

	ldi   r24,     lo8(M74_VRAM_OFF + (17 * 32) + 32)
	ldi   r25,     hi8(M74_VRAM_OFF + (17 * 32) + 32)
	ldi   r23,     0x46

	rcall cl_crut

	ldi   r23,     0x40
	sts   (M74_VRAM_OFF + (16 * 32) +  0), r23
	ldi   r23,     0x42
	sts   (M74_VRAM_OFF + (16 * 32) + 31), r23
	ldi   r23,     0x45
	sts   (M74_VRAM_OFF + (17 * 32) +  0), r23
	ldi   r23,     0x47
	sts   (M74_VRAM_OFF + (17 * 32) + 31), r23

	ldi   XL,      lo8(M74_VRAM_OFF +  0)
	ldi   XH,      hi8(M74_VRAM_OFF +  0)
	ldi   r24,     lo8(M74_VRAM_OFF +  0 + (16 * 32))
	ldi   r25,     hi8(M74_VRAM_OFF +  0 + (16 * 32))
	ldi   r23,     0x43
	sts   (M74_VRAM_OFF + (18 * 32) +  0), r23

	rcall cl_crutvb

	ldi   XL,      lo8(M74_VRAM_OFF + 31)
	ldi   XH,      hi8(M74_VRAM_OFF + 31)
	ldi   r24,     lo8(M74_VRAM_OFF + 31 + (16 * 32))
	ldi   r25,     hi8(M74_VRAM_OFF + 31 + (16 * 32))
	ldi   r23,     0x44
	sts   (M74_VRAM_OFF + (18 * 32) + 31), r23

cl_crutvb:

	st    X+,      r23     ; Small routine for vertical borders
	adiw  XL,      31
	cp    XL,      r24
	cpc   XH,      r25
	brne  .-10
	ret
