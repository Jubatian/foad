/*
 *  Dragon - Simple 16 bit full sequence PRNG
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


.section .bss

.equ	v_num,         MEM_LOC_A  ; Current PRNG state

.section .text



/*
** Gets next number of sequence. For asm use, r22 and r23 are clobbered.
**
** Returns in r25:r24.
*/
.global random_get
random_get:

	lds   r24,     v_num + 0
	lds   r25,     v_num + 1
	lsl   r24
	rol   r25
	ldi   r22,     0x25
	ldi   r23,     0x7F
	adc   r24,     r22
	adc   r25,     r23
	ldi   r22,     0xDB
	eor   r24,     r22
	sts   v_num + 0, r24
	sts   v_num + 1, r25
	ret
