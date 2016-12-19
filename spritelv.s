/*
 *  Dragon - Sprite to Level blitting
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
** Blits the given sprite. The level (levelscr) has to be set up properly as
** it blits by the position included.
** frame is the sprite image to blit (by spriteid_conv())
** flg is the Mode 74 flags to use for the blit (as required by sprite_blit())
** rep is the color replacement table to use, for REC_FLAT_0 it will select
** the bright variant if the sprite is in cave.
**
** r25:r24: sprite (physics) structure, only X and Y positions are used
**     r22: frame
**     r20: Mode 74 flags
**     r18: Replacement table to use
*/
.global spritelv_blit
spritelv_blit:

	push  r16

	mov   r16,     r18
	mov   r18,     r20
	mov   r20,     r22

	movw  XL,      r24
	ld    r24,     X+
	ld    r25,     X+      ; xpos
	ld    r22,     X+
	ld    r23,     X+      ; ypos

	rcall spritelv_blit_xy

	pop   r16
	ret



/*
** Like spritelv_blit() but with positions instead of sprite structure.
**
** r25:r24: X position
** r23:r22: Y position
**     r20: frame
**     r18: Mode 74 flags
**     r16: Replacement table to use
*/
.global spritelv_blit_xy
spritelv_blit_xy:

	; Save original X & Y positions

	movw  XL,      r24
	movw  ZL,      r22

	; Get screen relative X & Y positions

	lds   r21,     levelscr_x + 0
	lds   r19,     levelscr_x + 1
	sub   r24,     r21
	sbc   r25,     r19
	lds   r21,     levelscr_y + 0
	lds   r19,     levelscr_y + 1
	subi  r21,     128     ; Required Y adjustment by 128
	sbci  r19,     0
	andi  r21,     0xF8    ; Tile boundary
	sub   r22,     r21
	sbc   r23,     r19

	; Check whether sprite is on-screen

	mov   r19,     r24
	subi  r19,     0xE0
	mov   r19,     r25
	sbci  r19,     0xFF    ; Add 32
	cpi   r19,     0x00    ; If X high is not zero, then not on screen
	breq  .+2
	ret
	mov   r19,     r22
	subi  r19,     0xE0
	mov   r19,     r23
	sbci  r19,     0xFF    ; Add 32
	cpi   r19,     0x00    ; If Y high is not zero, then not on screen
	breq  .+2
	ret

	; Check for cave replacement

	push  r16

	cpi   r16,     REC_FLAT_0
	brne  bxy_nc

	; Cave replacement: check for cave tile

	push  r25
	push  r24
	push  r23
	push  r22
	push  r20
	push  r18

	movw  r24,     XL      ; Restore original sprite X
	movw  r22,     ZL      ; Restore original sprite Y
	lsr   r25
	ror   r24
	lsr   r25
	ror   r24
	lsr   r25
	ror   r24
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22              ; Convert to tile offset

	call  level_gettile

	mov   ZL,      r24
	ldi   ZH,      hi8(RES_TILEFLG_OFF)
	lpm   r24,     Z
	sbrc  r24,     7       ; Bit 7 set: Cave tile
	ldi   r16,     REC_FLAT_1

	pop   r18
	pop   r20
	pop   r22
	pop   r23
	pop   r24
	pop   r25

bxy_nc:

	; Convert sprite ID (frame)

	mov   r19,     r20
	movw  r20,     r22     ; Y pos into r21:r20 for sprite_blit
	movw  r22,     r24     ; X pos into r23:r22 for sprite_blit
	mov   r24,     r19
	call  spriteid_conv    ; Get sprite address

	; Blit and done

	call  sprite_blit

	pop   r16
	ret
