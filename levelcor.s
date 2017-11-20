/*
 *  Dragon - Level (map) coordinate encoding & decoding
 *  Copyright (C) 2017 Sandor Zsuga (Jubatian)
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



#include <avr/io.h>
; (Needed only for register names for X, Y and Z)



.section .text



/*
** Encodes coordinates into dst (3 bytes).
** For assembly calls, ZL, r1:r0 and r19:r18 are not clobbered.
** For assembly calls, XH:XL upon return is set dst + 3.
**
** r25:r24: dst (3 bytes)
** r23:r22: x
** r21:r20: y
*/
.global levelcor_encode
levelcor_encode:

	; Note: Above map Y = 0, there are 128 negative lines where sprites
	; may end up (top parallax region). These need to be encoded as well.
	; This is done by a 256 line clearance, which in the decoder is turned
	; into 2's complement negatives.

	movw  XL,      r24
	st    X+,      r20     ; Byte 0: Y low

	; X pos is at most 8191 (256 L0 blocks)
	; Y range is 2048 lines (At most 56 L0 blocks)

	andi  r21,     0x07    ; 3 bits of Y high will be stored
	lsl   r23
	lsl   r23
	lsl   r23              ; 5 bits of X high will be stored

	or    r21,     r23
	st    X+,      r21     ; Byte 1: Y & X mix
	st    X+,      r22     ; Byte 2: X low
	ret



/*
** Decodes coordinates from src (3 bytes).
** For assembly calls, r1:r0 and r19:r18 are not clobbered.
** For assembly calls, XH:XL upon return is set src + 3.
**
** r25:r24: src (3 bytes)
** r23:r22: x (pointer to uint16)
** r21:r20: y (pointer to uint16)
*/
.global levelcor_decode
levelcor_decode:

	movw  XL,      r24
	movw  ZL,      r20
	ld    r20,     X+      ; Byte 0: Y low
	st    Z+,      r20
	ld    r20,     X+      ; Byte 1: Y & X mix
	mov   r21,     r20

	; X pos is at most 8191 (256 L0 blocks)
	; Y range is 2048 lines (At most 56 L0 blocks)

	andi  r20,     0x07    ; Y high
	cpi   r20,     0x07
	brne  .+2
	ldi   r20,     0xFF    ; 1792 - 2047: Convert to 2's compl. negative
	lsr   r21
	lsr   r21
	lsr   r21              ; X high

	st    Z+,      r20
	movw  ZL,      r22
	ld    r20,     X+      ; Byte 2: X low
	st    Z+,      r20
	st    Z+,      r21
	ret




/*
** Decodes coordinates from src (3 bytes), assembly routine
**
** Inputs:
** XH:XL:   src (3 bytes)
**
** Outputs:
** r23:r22: X
** r21:r20: Y
** XH:XL:   src + 3
*/
.global levelcor_decode_asm
levelcor_decode_asm:

	ld    r20,     X+      ; Byte 0: Y low
	ld    r21,     X+      ; Byte 1: Y & X mix
	mov   r23,     r21

	; X pos is at most 8191 (256 L0 blocks)
	; Y range is 2048 lines (At most 56 L0 blocks)

	andi  r21,     0x07    ; Y high
	cpi   r21,     0x07
	brne  .+2
	ldi   r21,     0xFF    ; 1792 - 2047: Convert to 2's compl. negative
	lsr   r23
	lsr   r23
	lsr   r23              ; X high
	ld    r22,     X+      ; Byte 2: X low
	ret
