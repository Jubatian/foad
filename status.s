/*
 *  Dragon - Status bar
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



; Remapping from a 4 bit nybble to a status bar column (5 px tall + empty)
nyb_remap:
	.byte 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02
	.byte 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05

; Column to begin bar 0 map (leftmost bar)
remap_beg0:
	.byte 0x40, 0x40, 0x41, 0x42, 0x43, 0x43
; Column to begin bar 1 map
remap_beg1:
	.byte 0x43, 0x44, 0x45, 0x46, 0x47, 0x48
; Column to begin bar 2 map (rightmost partial elements)
remap_beg2:
	.byte 0x48, 0x49, 0x49, 0x49, 0x49, 0x4A
; Column to middle bar 0 map (leftmost bar)
remap_mid0:
	.byte 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50
; Column to middle bar 1 map
remap_mid1:
	.byte 0x50, 0x51, 0x52, 0x53, 0x54, 0x55
; Column to middle bar 2 map (rightmost partial elements)
remap_mid2:
	.byte 0x55, 0x56, 0x56, 0x56, 0x56, 0x57
; Column to end bar 0 map (leftmost bar)
remap_end0:
	.byte 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D
; Column to end bar 1 map
remap_end1:
	.byte 0x5D, 0x5E, 0x5F, 0x5F, 0x60, 0x61



/*
** Renders a single bar by a 8 bit input value and properties. Every 16
** increments of a value represents a full bar.
**
** Inputs:
**       X: Memory address where the bar begins
**     r24: The value to display
**     r23: Total width of bar in tiles
**     r22: Disabled width (must be less than total width)
** Outputs:
**       X: The bar's total width added
** Clobbers:
** Z, r24, r23, r22
*/
status_genbar:

	; Check for low value, if so, flashing display using the frame counter

	cpi   r24,     24
	brcc  gb_ren
	lds   ZL,      global_framectr
	sbrs  ZL,      4
	rjmp  gb_ren
	ldi   r24,     0x20
gb_cll:
	st    X+,      r24
	dec   r23
	brne  gb_cll
	ret

gb_ren:

	; Begin tile

	clr   ZH
	mov   ZL,      r24
	subi  r24,     16
	brcc  gb_b0
	clr   r24
	rcall gb_nybremap
	subi  ZL,      lo8(-(remap_beg0))
	sbci  ZH,      hi8(-(remap_beg0))
	rjmp  gb_bd
gb_b0:
	mov   ZL,      r24
	subi  r24,     16
	brcc  gb_b1
	clr   r24
	rcall gb_nybremap
	subi  ZL,      lo8(-(remap_beg1))
	sbci  ZH,      hi8(-(remap_beg1))
	rjmp  gb_bd
gb_b1:
	rcall gb_nybremap_sattozl
	subi  ZL,      lo8(-(remap_beg2))
	sbci  ZH,      hi8(-(remap_beg2))
gb_bd:

	; Mid tiles (also outputs begin tile)

gb_mlp:
	lpm   ZL,      Z
	st    X+,      ZL
	dec   r23
	cp    r23,     r22
	breq  gb_mle           ; Reached disabled amount, so end
	cpi   r23,     1
	breq  gb_et            ; Last tile is special (shorter)
	clr   ZH
	mov   ZL,      r24
	subi  r24,     16
	brcc  gb_m0
	clr   r24
	rcall gb_nybremap
	subi  ZL,      lo8(-(remap_mid0))
	sbci  ZH,      hi8(-(remap_mid0))
	rjmp  gb_mlp
gb_m0:
	mov   ZL,      r24
	subi  r24,     16
	brcc  gb_m1
	clr   r24
	rcall gb_nybremap
	subi  ZL,      lo8(-(remap_mid1))
	sbci  ZH,      hi8(-(remap_mid1))
	rjmp  gb_mlp
gb_m1:
	rcall gb_nybremap_sattozl
	subi  ZL,      lo8(-(remap_mid2))
	sbci  ZH,      hi8(-(remap_mid2))
	rjmp  gb_mlp

	; Disabled tiles

gb_dlp:
	ldi   ZL,      0x63
	st    X+,      ZL
	dec   r23
gb_mle:
	cpi   r23,     1
	brne  gb_dlp           ; Last disabled tile (shorter)
	ldi   ZL,      0x62
	st    X+,      ZL
	ret

	; Last shorter tile

gb_et:
	clr   ZH
	mov   ZL,      r24
	subi  r24,     16
	brcc  gb_e0
	clr   r24
	rcall gb_nybremap
	subi  ZL,      lo8(-(remap_end0))
	sbci  ZH,      hi8(-(remap_end0))
	rjmp  gb_ed
gb_e0:
	rcall gb_nybremap_sattozl
	subi  ZL,      lo8(-(remap_end1))
	sbci  ZH,      hi8(-(remap_end1))
gb_ed:
	lpm   ZL,      Z
	st    X+,      ZL
	ret

	; Mini functions for code size trimming

gb_nybremap_sattozl:
	mov   ZL,      r24
	cpi   ZL,      15
	brcs  .+2
	ldi   ZL,      15
gb_nybremap:
	subi  ZL,      lo8(-(nyb_remap))
	sbci  ZH,      hi8(-(nyb_remap))
	lpm   ZL,      Z
	clr   ZH
	ret



/*
** Renders the status bar according to the dragon's current state (including
** score and remaining time).
*/
.global status_render
status_render:

	; Get dragon status

	ldi   XL,      lo8(dragon_stat)
	ldi   XH,      hi8(dragon_stat)
	ld    r18,     X+      ; Health
	ld    r19,     X+      ; Energy
	ld    r20,     X+      ; Fire reserve
	ld    r21,     X+      ; Capacities

	; Produce initial fill

	ldi   XL,      lo8(LOC_STATVRAM_OFF)
	ldi   XH,      hi8(LOC_STATVRAM_OFF)
	ldi   r24,     0x08
	st    X+,      r24
	ldi   r24,     0x0A
	st    X+,      r24
	adiw  XL,      4
	ldi   r24,     0x0B
	st    X+,      r24
	adiw  XL,      4
	ldi   r24,     0x0C
	st    X+,      r24
	adiw  XL,      4
	ldi   r24,     0x20
	st    X+,      r24
	adiw  XL,      5
	st    X+,      r24
	ldi   r24,     0x0D
	st    X+,      r24
	adiw  XL,      7
	ldi   r24,     0x09
	st    X+,      r24

	; Dragon status

	ldi   r23,     4
	mov   r22,     r21
	andi  r22,     0x03    ; Health capacity
	neg   r22
	subi  r22,     0xFD
	mov   r24,     r18     ; Health
	lsr   r24
	ldi   XL,      lo8(LOC_STATVRAM_OFF + 2)
	ldi   XH,      hi8(LOC_STATVRAM_OFF + 2)
	rcall status_genbar

	ldi   r23,     4
	mov   r22,     r21
	andi  r22,     0x30    ; Fire capacity
	swap  r22
	neg   r22
	subi  r22,     0xFD
	mov   r24,     r20     ; Fire reserve
	lsr   r24
	ldi   XL,      lo8(LOC_STATVRAM_OFF + 7)
	ldi   XH,      hi8(LOC_STATVRAM_OFF + 7)
	rcall status_genbar

	ldi   r23,     4
	mov   r22,     r21
	andi  r22,     0x0C    ; Energy capacity
	lsr   r22
	lsr   r22
	neg   r22
	subi  r22,     0xFD
	mov   r24,     r19     ; Energy
	lsr   r24
	ldi   XL,      lo8(LOC_STATVRAM_OFF + 12)
	ldi   XH,      hi8(LOC_STATVRAM_OFF + 12)
	rcall status_genbar

	; Remaining time

	ldi   r23,     7
	ldi   r22,     0
	lds   r24,     gstat_time
	ldi   XL,      lo8(LOC_STATVRAM_OFF + 24)
	ldi   XH,      hi8(LOC_STATVRAM_OFF + 24)
	rcall status_genbar

	; Score

	lds   r22,     gstat_score + 0
	lds   r23,     gstat_score + 1
	ldi   r20,     0x30
	ldi   XL,      lo8(LOC_STATVRAM_OFF + 17)
	ldi   XH,      hi8(LOC_STATVRAM_OFF + 17)
	jmp   gstat_score_disp_x
