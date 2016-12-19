/*
 *  Dragon - Passability check
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
** Checks if a given pixel location on the map is passable.
** Returns nonzero if it is so. Depends on level.
**
** Inputs:
** r25:r24: x
** r23:r22: y
**
** Outputs:
** r25:r24: 1 or 0
*/
.global passable
passable:

	; Note: dimension checks are unnecessary as level_gettile will handle
	; these proper (inpassable for leaving the map on X, passable for
	; leaving the map on Y, allowing jumping up into parallax clouds).

	; Prepare tile & pixel offsets

	mov   XL,      r24     ; Will hold X pixel offset
	mov   XH,      r22     ; Will hold Y pixel offset
	lsr   r25
	ror   r24
	lsr   r25
	ror   r24
	lsr   r25
	ror   r24              ; r25:r24: X tile offset
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22
	lsr   r23
	ror   r22              ; r23:r22: Y tile offset

	; Get passability block

	call  level_gettile
	mov   ZL,      r24
	ldi   ZH,      hi8(RES_TILEFLG_OFF)
	lpm   r24,     Z
	sbrc  XH,      2
	rjmp  .+4
	lsr   r24
	lsr   r24              ; Upper half is on bits 2-3
	andi  r24,     0x03

	; Check passabilities

	cpi   r24,     1
	brne  .+6              ; 1: Passable
ispr1:
	ldi   r25,     0
	ldi   r24,     1
	ret
	brcc  .+6              ; 0: Impassable
ispr0:
	ldi   r25,     0
	ldi   r24,     0
	ret

	; Slopes. Prepare pixel coordinates

	andi  XH,      0x03
	lsr   XL
	andi  XL,      0x03

	; Check passabilities

	sbrc  r24,     0
	rjmp  .+4
	neg   XL
	subi  XL,      0xFD   ; XL = 3 - XL (X pixel offset inversion)
	cp    XL,      XH
	brcc  ispr1
	rjmp  ispr0
