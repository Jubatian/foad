/*
 *  Dragon - Map actor dimensions
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
** Fills in sprite physics structure's xbs and ybs members by actor type (bits
** 0 - 5). If bit 7 of type is set, alternate xbs & ybs are returned (can be
** used for the dragon & others).
**
**     r24: typ
** r23:r22: spr
*/
.global actordim_get
actordim_get:

	movw  XL,      r22
	adiw  XL,      4       ; Location of 'xbs' in structure
	ldi   ZH,      hi8(RES_SPRIDSIZE_OFF)
	mov   ZL,      r24
	bst   ZL,      7
	bld   ZL,      6
	ori   ZL,      0x80    ; Converted 'typ' input to load address
	lpm   r22,     Z
	mov   r23,     r22
	andi  r22,     0x0F    ; X size
	lsl   r22
	st    X+,      r22     ; xbs written
	andi  r23,     0xF0    ; Y size
	lsr   r23
	lsr   r23
	st    X+,      r23     ; ybs written
	ret
